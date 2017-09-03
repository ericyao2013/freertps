// Copyright 2016 Open Source Robotics Foundation, Inc.
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

#ifndef FREERTPS_UDP_H
#define FREERTPS_UDP_H

#include "freertps/log.h"
#include "freertps/rtps/type/guid.h"
#include "freertps/rtps/type/message.h"
#include "freertps/rtps/type/receiver_state.h"
#include "freertps/rtps/type/sub_message.h"
#include "freertps/rtps/type/sequence_number.h"

#include <string.h>
#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

#define FRUDP_FLAGS_ACKNACK_FINAL      0x02

#define FRUDP_SCHEME_CDR_LE            0x0001
#define FRUDP_SCHEME_PL_CDR_LE         0x0003

#define FRUDP_PLIST_ADVANCE(list_item) \
  do { \
    list_item = (frudp_parameter_list_item_t *) \
                (((uint8_t *)list_item) + 4 + list_item->len); \
  } while (0)


/////////////////////////////////////////////////////////////////////
// TYPES
/////////////////////////////////////////////////////////////////////

typedef uint16_t frudp_parameterid_t;
typedef struct
{
  frudp_parameterid_t pid;
  uint16_t len;
  uint8_t value[];
} __attribute__((packed)) frudp_parameter_list_item_t;

typedef struct
{
  uint16_t scheme;
  uint16_t options;
} __attribute__((packed)) frudp_encapsulation_scheme_t;

typedef void (*frudp_rx_data_cb_t)(frudp_receiver_state_t *rcvr,
                                   const frudp_submsg_t *submsg,
                                   const uint16_t scheme,
                                   const uint8_t *data);

typedef struct
{
  int32_t sec;
  uint32_t nanosec;
} frudp_duration_t;

typedef uint32_t frudp_builtin_endpoint_set_t;

typedef struct
{
  uint32_t len;
  uint8_t data[];
} frudp_rtps_string_t;

/////////////////////////////////////////////////////////////////////
// FUNCTIONS
/////////////////////////////////////////////////////////////////////

bool frudp_init(void);
void frudp_fini(void);

bool frudp_generic_init(void);
bool frudp_init_participant_id(void);

bool frudp_add_mcast_rx(const uint32_t group,
                        const uint16_t port); //,
                               //const freertps_udp_rx_callback_t rx_cb);

// todo: elicit desired interface from the user in a sane way
bool frudp_add_ucast_rx(const uint16_t port);

bool frudp_listen(const uint32_t max_usec);

bool frudp_rx(const uint32_t src_addr,
              const uint16_t src_port,
              const uint32_t dst_addr,
              const uint16_t dst_port,
              const uint8_t *rx_data,
              const uint16_t rx_len);

bool frudp_tx(const uint32_t dst_addr,
              const uint16_t dst_port,
              const uint8_t *tx_data,
              const uint16_t tx_len);

void frudp_tx_acknack(const frudp_guid_prefix_t *guid_prefix,
                      const frudp_eid_t *reader_eid,
                      const frudp_guid_t *writer_guid,
                      const frudp_sn_set_t *set);

uint16_t frudp_ucast_builtin_port(void);
uint16_t frudp_mcast_builtin_port(void);
uint16_t frudp_ucast_user_port(void);
uint16_t frudp_mcast_user_port(void);
uint16_t frudp_spdp_port(void);

//const char *frudp_ip4_ntoa(const uint32_t addr);

bool frudp_parse_string(char *buf, uint32_t buf_len, frudp_rtps_string_t *s);
bool frudp_parse_string_prefix(char *buf, uint32_t buf_len, frudp_rtps_string_t *s, char *prefix);

frudp_msg_t *frudp_init_msg(frudp_msg_t *buf);

extern const struct rtps_psm g_rtps_psm_udp;

long getIp();

long getNetwork();

void set_string_alligned(const char *string, frudp_parameter_list_item_t *param );

#ifdef __cplusplus
}
#endif
#endif // FREERTPS_UDP_H
