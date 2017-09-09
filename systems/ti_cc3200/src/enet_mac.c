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
#include "freertps/log.h"

#include <stdint.h>

void enet_mac_init(void)
{
  FREERTPS_DEBUG("enet_mac_init()\r\n");
}

void enet_mac_tx_raw(const uint8_t *pkt, uint16_t pkt_len)
{

}

uint16_t enet_read_phy_reg(const uint8_t reg_idx)
{
    return 0;
}
