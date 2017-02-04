// Copyright 2016 Mickael Gaillard <mick.gaillard@gmail.com>.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include "freertps/freertps.h"
#include "freertps/bswap.h"
#include "freertps/udp.h"

#include "metal/enet.h"
#include "metal/enet_config.h"

// simplelink includes
#include "simplelink.h"
#include "wlan.h"
#include "network_if.h"

#include "common.h"
#include "uart_if.h"

// Application specific status/error codes
typedef enum{
    // Choosing -0x7D0 to avoid overlap w/ host-driver's error codes
    UCP_CLIENT_FAILED = -0x7D0,
    UCP_SERVER_FAILED = UCP_CLIENT_FAILED - 1,
    DEVICE_NOT_IN_STATION_MODE = UCP_SERVER_FAILED - 1,

    STATUS_CODE_MAX = -0xBB8
}e_AppStatusCodes;

#define ALIGN4 __attribute__((aligned(4)));

#define ENET_RXPOOL_LEN     FRUDP_BUFFER_SIZE
#define ENET_RXPOOL_NPTR      64
#define ENET_RXPOOL_OFFSET     2

static volatile uint8_t  g_enet_rxpool[ENET_RXPOOL_LEN] ALIGN4;
static volatile uint16_t g_enet_rxpool_wpos = ENET_RXPOOL_OFFSET;
static volatile uint8_t *g_enet_rxpool_start[ENET_RXPOOL_NPTR] ALIGN4;
static volatile uint16_t g_enet_rxpool_len[ENET_RXPOOL_NPTR] ALIGN4;
static volatile uint16_t g_enet_rxpool_ptrs_wpos;
static volatile uint16_t g_enet_rxpool_ptrs_rpos;

static uint16_t g_enet_allowed_udp_ports[ENET_MAX_ALLOWED_UDP_PORTS];
static uint16_t g_enet_allowed_udp_ports_wpos;

static uint16_t g_enet_allowed_udp_sockets[ENET_MAX_ALLOWED_UDP_PORTS];
static uint16_t g_enet_allowed_udp_sockets_wpos;

//static uint8_t  g_enet_udpbuf[BUF_SIZE] __attribute__((aligned(8)));

///////////////////////////////////////////////////////////////////////////
// local functions
///////////////////////////////////////////////////////////////////////////

//static bool enet_dispatch_udp(const uint8_t *data, const uint16_t len);
static bool udp_send(const uint32_t dst_addr,
                     const uint16_t dst_port,
                     const uint8_t *tx_data,
                     const uint16_t tx_len);
int udp_multicast_listener(const uint32_t group_addr, const uint16_t dst_port);
int udp_unicast_listener(const uint16_t dst_port);

void enet_init(void)
{
  FREERTPS_DEBUG("enet_init()\r\n");
  // set up the RAM pool for reception
  for (int i = 0; i < ENET_RXPOOL_NPTR; i++)
    g_enet_rxpool_start[i] = &g_enet_rxpool[ENET_RXPOOL_OFFSET];
  // chain into the MAC-provided initialization
  enet_mac_init();
}

enet_link_status_t enet_get_link_status(void)
{
  uint16_t status = enet_read_phy_reg(0x01);
  //FREERTPS_INFO("PHY status = 0x%02x\r\n", status);
  if (status & 0x04)
    return ENET_LINK_UP;
  return ENET_LINK_DOWN;
}


bool enet_send_udp_mcast(const uint32_t mcast_ip, const uint16_t mcast_port,
                         const uint8_t *payload, const uint16_t payload_len)
{
  uint8_t dest_mac[6] = { 0x01, 0x00, 0x5e,
                          (uint8_t)((mcast_ip & 0xff0000) >> 16),
                          (uint8_t)((mcast_ip & 0x00ff00) >>  8),
                          (uint8_t) (mcast_ip & 0x0000ff) };

  uint32_t ip = getIp();
  return enet_send_udp_ucast(dest_mac,
                      mcast_ip,
                      mcast_port,
                      ip,
                      mcast_port,
                      payload,
                      payload_len);
}

bool enet_send_udp_ucast(const uint8_t *dest_mac,
                         const uint32_t dest_ip,
                         const uint16_t dest_port,
                         const uint32_t source_ip,
                         const uint16_t source_port,
                         const uint8_t *payload,
                         const uint16_t payload_len)
{
  return udp_send(dest_ip, dest_port, payload, payload_len);
}

uint_fast8_t enet_process_rx_ring(void)
{
  uint_fast8_t num_pkts_rx = 0;

  int iStatus;
  SlSocklen_t iAddrSize = sizeof(SlSockAddrIn_t);
  int sTestBufLen = FRUDP_BUFFER_SIZE;

  for (int i = 0; i < g_enet_allowed_udp_sockets_wpos; ++i) {
    SlSockAddrIn_t  sAddr;
    unsigned char g_cBsdBuf[sTestBufLen];
    short iSockID = g_enet_allowed_udp_sockets[i];

    iStatus = sl_RecvFrom(iSockID, g_cBsdBuf, sTestBufLen, 0, ( SlSockAddr_t *)&sAddr, (SlSocklen_t*)&iAddrSize );
    if( iStatus < 0 && iStatus != SL_EAGAIN ) { sl_Close(iSockID);
        ASSERT_ON_ERROR(UCP_SERVER_FAILED); }

//    SlSockAddrIn_t sin;
//    if (getsockname(iSockID, (struct sockaddr *)&sin, &iAddrSize) == -1)
//        perror("getsockname");
//    else
//        printf("port number %d\n", ntohs(sin.sin_port));

    if ( iStatus != SL_EAGAIN ) { // Or try again
      //FREERTPS_INFO("eth dispatch @ %8u\r\n", (unsigned)SYSTIME);
      const uint32_t src_addr = freertps_htonl(sAddr.sin_addr.s_addr);
      const uint16_t src_port = freertps_htons(sAddr.sin_port);
      const uint16_t dst_port = g_enet_allowed_udp_ports[i];

      uint32_t dst_addr = getIp();
      if (frudp_mcast_builtin_port() == dst_port || frudp_mcast_user_port() == dst_port)
      {
        dst_addr = FRUDP_DEFAULT_MCAST_GROUP;
      }

      frudp_rx(src_addr, src_port, dst_addr, dst_port, g_cBsdBuf, iStatus);
      num_pkts_rx++;
    }
  }

  //FREERTPS_INFO("leaving enet_process_rx_ring()\r\n");
  return num_pkts_rx;
}

void enet_rx_raw(const uint8_t *pkt, const uint16_t pkt_len)
{
  // this function just places the incoming packet in a circular buffer
  // we want the ethernet payload 32-bit aligned, so we'll offset the
  // writes into the rxpool buffer by 2 bytes (ETH_RXPOOL_OFFSET)
  // see if this packet will run off the end of the buffer. if so, wrap.
  if (g_enet_rxpool_wpos + pkt_len + ENET_RXPOOL_OFFSET >= ENET_RXPOOL_LEN)
    g_enet_rxpool_wpos = ENET_RXPOOL_OFFSET;
  const uint16_t wp = g_enet_rxpool_ptrs_wpos;
  g_enet_rxpool_start[wp] = &g_enet_rxpool[g_enet_rxpool_wpos];
  g_enet_rxpool_len[wp] = pkt_len;
  memcpy((void *)g_enet_rxpool_start[wp], pkt, pkt_len);
  //FREERTPS_INFO("ethernet rx %d into rxpool at 0x%08x\r\n",
  //       rxn, (unsigned)g_enet_rxpool_start[wp]);
  g_enet_rxpool_ptrs_wpos++;
  if (g_enet_rxpool_ptrs_wpos >= ENET_RXPOOL_NPTR)
    g_enet_rxpool_ptrs_wpos = 0;

  // make sure we end up with the rxpool write pointer on a 2-byte offset
  // address (to keep the ethernet payloads 4-byte aligned) by incrementing
  // the pointer by a multiple of 4
  g_enet_rxpool_wpos += ((pkt_len + 3) & ~0x3);
}

//static void enet_add_ip_header_checksum(enet_ip_header_t *ip)
//{
//  ip->checksum = 0;
//  uint32_t sum = 0;
//  for (int word_idx = 0; word_idx < 10; word_idx++)
//  {
//    uint16_t word = *((uint16_t *)ip + sizeof(enet_eth_header_t)/2 + word_idx);
//    word = freertps_htons(word);
//    sum += word;
//    //FREERTPS_INFO("word %d: 0x%02x\r\n", word_idx, word);
//  }
//  sum += (sum >> 16);
//  sum &= 0xffff;
//  ip->checksum = (uint16_t)freertps_htons(~sum);
//  //FREERTPS_INFO("ip header checksum: 0x%04x\r\n", ip->ip_checksum);
//}

//
//static bool enet_dispatch_ip(const uint8_t *data, const uint16_t len)
//{
//  const enet_ip_header_t *ip = (const enet_ip_header_t *)data;
//  if (ip->version != 4) // we only handle ipv4 (for now...)
//    return false;
//  // if it's unicast, verify our IP address, otherwise ignore the packet
//  uint8_t g_enet_mac[] = getMacAddress();
//  if (ip->eth.dest_addr[0] == g_enet_mac[0]) // todo: smarter MAC filter.
//    if (ip->dest_addr != freertps_htonl(getIp()))
//      return false;
//  if (ip->proto == ENET_IP_PROTO_UDP)
//    return enet_dispatch_udp(data, len);
//  return false; // if we get here, we aren't smart enough to handle this packet
//}

//static bool enet_dispatch_udp(const uint8_t *data, const uint16_t len)
//{
//  const enet_udp_header_t *udp = (const enet_udp_header_t *)data;
//  const uint16_t port = freertps_htons(udp->dest_port);
//  const uint16_t payload_len = freertps_htons(udp->len) - 8;
//  const uint8_t *payload = data + sizeof(enet_udp_header_t);
//  //FREERTPS_INFO("  udp len: %d\r\n", udp_payload_len);
//  if (payload_len > len - sizeof(enet_udp_header_t))
//    return false; // ignore fragmented UDP packets.
//  //FREERTPS_INFO("dispatch udp @ %8u\r\n", (unsigned)SYSTIME);
//
//  // todo: more efficient filtering
//  bool port_match = false;
//  for (int i = 0; !port_match && i < g_enet_allowed_udp_ports_wpos; i++)
//    if (port == g_enet_allowed_udp_ports[i])
//      port_match = true;
//  // it would be nicer to have a callback mechanism here. someday.....
//  if (port_match)
//  {
//    frudp_rx(udp->ip.source_addr, freertps_htonl(udp->source_port),
//             udp->ip.dest_addr, freertps_htonl(udp->dest_port),
//             payload, payload_len);
//    return true;
//  }
//
///*
//  if (port == frudp_ucast_builtin_port() ||
//      port == frudp_mcast_builtin_port() ||
//      port == frudp_ucast_user_port()    ||
//      port == frudp_mcast_user_port())
//*/
//  FREERTPS_INFO("unhandled udp: port = %d  payload_len = %d\r\n", port, payload_len);
//  return false;
//}

// todo: be smarter about multicast group choice
// old garbage:  #define MCAST_IP 0xe000008e

bool enet_allow_udp_port(const uint16_t port)
{
  // make sure we aren't already listening to this port
  FREERTPS_DEBUG("enet_allow_udp_port(%d)\r\n", port);

  for (int i = 0; i < ENET_MAX_ALLOWED_UDP_PORTS; i++)
    if (g_enet_allowed_udp_ports[i] == port) {
      FREERTPS_INFO("enet_allow_udp_port(%d) already allowed\r\n", port);
      return true; // it's already allowed
    }

  if (g_enet_allowed_udp_ports_wpos >= ENET_MAX_ALLOWED_UDP_PORTS) {
    FREERTPS_INFO("enet_allow_udp_port(%d) no room\r\n", port);
    return false; // sorry, no room, have a nice day
  }

  FREERTPS_INFO("Allow udp port : %d\r\n", port);
  g_enet_allowed_udp_ports[g_enet_allowed_udp_ports_wpos++] = port;
  /*
  for (int i = 0; i < g_enet_allowed_udp_ports_wpos; i++)
    FREERTPS_DEBUG(" allowed port %d: %d\r\n", i, g_enet_allowed_udp_ports[i]);
  */
  return true;
}

void enet_add_mcast_rx(const uint32_t group, const uint16_t port)
{
  int socketId = udp_multicast_listener(group, port);
  g_enet_allowed_udp_sockets[g_enet_allowed_udp_sockets_wpos++] = socketId;
}
void enet_add_ucast_rx(const uint16_t port)
{
  int socketId = udp_unicast_listener(port);
  g_enet_allowed_udp_sockets[g_enet_allowed_udp_sockets_wpos++] = socketId;
}

int udp_multicast_listener(const uint32_t group_addr, const uint16_t dst_port)
{
    SlSockAddrIn_t  sLocalAddr;
    SlSockIpMreq    mReq;
    int             iAddrSize;
    int             iSockID;
    int             iStatus;

    FREERTPS_DEBUG("udp_multicast_listener(%d)\r\n", dst_port);

    //filling the UDP server socket address
    memset(&sLocalAddr, 0, sizeof(sLocalAddr));
    sLocalAddr.sin_family = SL_AF_INET;
    sLocalAddr.sin_port = sl_Htons((unsigned short)dst_port);
    sLocalAddr.sin_addr.s_addr = 0;

    memset(&mReq, 0, sizeof(SlSockIpMreq));
    mReq.imr_interface.s_addr = sl_Htons(SL_INADDR_ANY);
    mReq.imr_multiaddr.s_addr = sl_Htonl(group_addr);

    iAddrSize = sizeof(SlSockAddrIn_t);

    // creating a UDP socket.
    iSockID = sl_Socket(SL_AF_INET, SL_SOCK_DGRAM, SL_IPPROTO_UDP);
    if( iSockID < 0 ) { ASSERT_ON_ERROR(UCP_SERVER_FAILED); }

    // Join MultiCast group.
    if ((sl_SetSockOpt(iSockID, SL_IPPROTO_IP, SL_IP_ADD_MEMBERSHIP, &mReq, sizeof(SlSockIpMreq)) == -1) &&
        (sl_SetSockOpt(iSockID, SL_IPPROTO_IP, SL_IP_MULTICAST_TTL, (_u8 *)2, sizeof(int)) == -1))
    { sl_Close(iSockID); ASSERT_ON_ERROR(UCP_SERVER_FAILED); }

    // Non Blocking mode.
    SlSockNonblocking_t enableOption;
    enableOption.NonblockingEnabled = 1;
    if ((sl_SetSockOpt(iSockID, SL_SOL_SOCKET, SL_SO_NONBLOCKING, &enableOption, sizeof(enableOption))) == -1)
    { sl_Close(iSockID); ASSERT_ON_ERROR(UCP_SERVER_FAILED); }

    // binding the UDP socket to the UDP server address
    iStatus = sl_Bind(iSockID, (SlSockAddr_t *)&sLocalAddr, iAddrSize);
    if( iStatus < 0 ) { sl_Close(iSockID); ASSERT_ON_ERROR(UCP_SERVER_FAILED); }

    return iSockID;
}

int udp_unicast_listener(const uint16_t dst_port)
{
    SlSockAddrIn_t  sLocalAddr;
    int             iAddrSize;
    int             iSockID;
    int             iStatus;

    FREERTPS_DEBUG("udp_unicast_listener(%d)\r\n", dst_port);

    //filling the UDP server socket address
    memset(&sLocalAddr, 0, sizeof(sLocalAddr));
    sLocalAddr.sin_family = SL_AF_INET;
    sLocalAddr.sin_port = sl_Htons((unsigned short)dst_port);
    sLocalAddr.sin_addr.s_addr = 0;

    iAddrSize = sizeof(SlSockAddrIn_t);

    // creating a UDP socket
    iSockID = sl_Socket(SL_AF_INET,SL_SOCK_DGRAM, SL_IPPROTO_UDP);
    if( iSockID < 0 ) { ASSERT_ON_ERROR(UCP_SERVER_FAILED); }

    // Non Blocking mode.
    SlSockNonblocking_t enableOption;
    enableOption.NonblockingEnabled = 1;
    if ((sl_SetSockOpt(iSockID, SL_SOL_SOCKET, SL_SO_NONBLOCKING, &enableOption, sizeof(enableOption))) == -1)
    { sl_Close(iSockID); ASSERT_ON_ERROR(UCP_SERVER_FAILED); }

    // binding the UDP socket to the UDP server address
    iStatus = sl_Bind(iSockID, (SlSockAddr_t *)&sLocalAddr, iAddrSize);
    if( iStatus < 0 ) { sl_Close(iSockID); ASSERT_ON_ERROR(UCP_SERVER_FAILED); }

    return iSockID;
}

static bool udp_send(const uint32_t dst_addr,
                     const uint16_t dst_port,
                     const uint8_t *tx_data,
                     const uint16_t tx_len) {
  SlSockAddrIn_t  sAddr;
  int             iAddrSize;
  int             iSockID;
  int             iStatus;

  //filling the UDP server socket address
  memset(&sAddr, 0, sizeof(sAddr));
  sAddr.sin_family = SL_AF_INET;
  sAddr.sin_port = sl_Htons((unsigned short)dst_port);
  sAddr.sin_addr.s_addr = sl_Htonl(dst_addr);

  iAddrSize = sizeof(SlSockAddrIn_t);

  // creating a UDP socket
  iSockID = sl_Socket(SL_AF_INET, SL_SOCK_DGRAM, SL_IPPROTO_UDP);
  if( iSockID < 0 ) { ASSERT_ON_ERROR(UCP_CLIENT_FAILED); }

  // sending packet
  iStatus = sl_SendTo(iSockID, tx_data, tx_len, 0, (const SlSockAddr_t *)&sAddr, iAddrSize);
  if( iStatus <= 0 ) { ASSERT_ON_ERROR(UCP_CLIENT_FAILED); }

  sl_Close(iSockID);
  return true;
}
