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

#include "freertps/freertps.h"
#include "freertps/rtps/constant/vendor.h"
#include "freertps/rtps/discovery/disco.h"
#include "freertps/utility.h"
#include "freertps/psm/udp.h"
#include "freertps/psm/bswap.h"

#include <stdint.h>
#include <string.h>
#include <stdlib.h>

#include "metal/enet_config.h"
//#include "metal/enet.h"
#include "metal/systime.h"

#include "simplelink.h"

uint8_t *getMacAddress()
{
  // TODO can use CC3200 network_if.c::SimpleLinkNetAppEventHandler or Network_IF_MacConfigGet

  static uint8_t macAddressVal[SL_MAC_ADDR_LEN];
  uint8_t macAddressLen = SL_MAC_ADDR_LEN;
  sl_NetCfgGet(SL_MAC_ADDRESS_GET, NULL, &macAddressLen, (uint8_t *)macAddressVal);

  return macAddressVal;
}

extern void enet_init();
extern void enet_process_rx_ring();
extern void enet_send_udp_mcast(const uint32_t mcast_ip, const uint16_t mcast_port,
        const uint8_t *payload, const uint16_t payload_len);
extern bool enet_send_udp_ucast(const uint8_t *dest_mac,
        const uint32_t dest_ip,
        const uint16_t dest_port,
        const uint32_t source_ip,
        const uint16_t source_port,
        const uint8_t *payload,
        const uint16_t payload_len);
extern void enet_add_ucast_rx(const uint16_t port);
extern bool enet_allow_udp_port(const uint16_t port);
extern void enet_add_mcast_rx(const uint32_t group, const uint16_t port);

bool frudp_init(void)
{
  FREERTPS_DEBUG("udp_init()\r\n");
  enet_init();
  uint32_t ip = getIp();
  uint8_t *enet_mac_tmp = getMacAddress();
  FREERTPS_INFO("using address %s for unicast\r\n", frudp_print_ip(ip));
  g_frudp_config.unicast_addr = freertps_htonl(ip);
  g_frudp_config.guid_prefix.prefix[0] = FREERTPS_VID_FREERTPS >> 8;
  g_frudp_config.guid_prefix.prefix[1] = FREERTPS_VID_FREERTPS & 0xff;
  memcpy(&g_frudp_config.guid_prefix.prefix[2], enet_mac_tmp, 6);
  frudp_generic_init();
  // not sure about endianness here.
  // 4 bytes left. let's use the system time in microseconds since power-up
  // todo: init ethernet PHY. after PHY link is up,
  // store system time in the guid_prefix.
  //memcpy(&g_frudp_config.guid_prefix.prefix[8], &pid, 4);
  //frudp_disco_init();
  return true;
}

void frudp_fini(void)
{
  frudp_disco_fini();
  FREERTPS_DEBUG("udp fini\r\n");
}

bool frudp_listen(const uint32_t max_usec)
{
  // just busy-wait here for the requested amount of time
  volatile uint32_t t_start = systime_usecs();
  while (1)
  {
    enet_process_rx_ring();
    volatile uint32_t t = systime_usecs();
    if (t - t_start >= max_usec)
      break;
  }
  return true;
}

bool frudp_tx(const uint32_t dst_addr,
              const uint16_t dst_port,
              const uint8_t *tx_data,
              const uint16_t tx_len)
{
#ifdef EXCESSIVELY_VERBOSE_MSG_RX
  FREERTPS_DEBUG(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\r\n");
  FREERTPS_DEBUG("Sending data %d bytes to %s:%d\r\n",
                tx_len, frudp_print_ip(dst_addr), dst_port);
  FREERTPS_DEBUG(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\r\n");
#endif

  if ((dst_addr & 0xe0000000) == 0xe0000000)
  {
    // ipv4 multicast
    enet_send_udp_mcast(dst_addr, dst_port, tx_data, tx_len);
  }
  else
  {
    uint32_t ip = getIp();
    uint8_t *dst_mac = getMacAddress(); // todo: not this
    enet_send_udp_ucast(dst_mac,
                        dst_addr,
                        dst_port,
                        ip,
                        dst_port,
                        tx_data, tx_len);
  }
  return true;
}

bool frudp_add_mcast_rx(uint32_t group, uint16_t port)
{
  FREERTPS_DEBUG("frudp_add_mcast_rx(%s:%d)\r\n",
                 frudp_print_ip(group),
                 port);

  bool result = enet_allow_udp_port(port);
  if (result) {
    enet_add_mcast_rx(group, port);
  }
  return result;
}

bool frudp_add_ucast_rx(const uint16_t port)
{
  FREERTPS_DEBUG("frudp_add_ucast_rx(%d)\r\n", port);

  bool result = enet_allow_udp_port(port);
  if (result) {
    enet_add_ucast_rx(port);
  }
  return result;
}
