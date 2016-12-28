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

#include "freertps/disco.h"
#include "freertps/spdp.h"
#include "freertps/sedp.h"

uint8_t g_frudp_disco_tx_buf[FRUDP_DISCO_TX_BUFLEN];
uint16_t g_frudp_disco_tx_buf_wpos;

frudp_part_t g_frudp_disco_parts[FRUDP_DISCO_MAX_PARTS];
int g_frudp_disco_num_parts = 0;

////////////////////////////////////////////////////////////////

void frudp_disco_init(void)
{
  FREERTPS_INFO("discovery init...\r\n");
  frudp_spdp_init();
  frudp_sedp_init();
}

void frudp_disco_start(void)
{
  FREERTPS_INFO("discovery start...\r\n");
  frudp_spdp_start();
  frudp_sedp_start();
}

void frudp_disco_fini(void)
{
  FREERTPS_INFO("discovery fini !\r\n");
  frudp_spdp_fini();
  frudp_sedp_fini();
}

void frudp_disco_tick(void)
{
//  FREERTPS_DEBUG("discovery tick...\r\n");
  frudp_spdp_tick();
  frudp_sedp_tick();
}
