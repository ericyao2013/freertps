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

#ifndef FREERTPS_DISCO_H
#define FREERTPS_DISCO_H

// this has a bunch of discovery-related debris


#include "freertps/rtps/type/part.h"

#ifdef __cplusplus
extern "C" {
#endif

/** Initialize of discovery stack. */
void frudp_disco_init(void);

/** Release of discovery stack. */
void frudp_disco_fini(void);


/** Start of discovery stack. */
void frudp_disco_start(void); /// must be called to kick off discovery

/** Internal Tick of discovery stack. */
void frudp_disco_tick(void);  /// must be called periodically to broadcast SPDP

/** Stop of discovery stack. */
void frudp_disco_stop(void);

//// Global discovery stack ////
// Buffer
extern uint8_t g_frudp_disco_tx_buf[FRUDP_DISCO_TX_BUFLEN];
extern uint16_t g_frudp_disco_tx_buf_wpos;

// Participants
extern frudp_part_t g_frudp_disco_parts[FRUDP_DISCO_MAX_PARTS];
extern int g_frudp_disco_num_parts;

#ifdef __cplusplus
}
#endif
#endif // FREERTPS_DISCO_H
