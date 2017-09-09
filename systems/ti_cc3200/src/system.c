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
#include "freertps/psm/system.h"
#include "freertps/psm/udp.h"
#include "freertps/rtps/type/config.h"

void freertps_system_init(void)
{
  FREERTPS_DEBUG("freertps_system_init()\r\n");
  frudp_init();
}

bool freertps_system_ok(void)
{
  return true;
}

bool frudp_init_participant_id(void)
{
  FREERTPS_DEBUG("frudp_init_participant_id()\r\n");
  g_frudp_config.participant_id = 0;
  return true;
}
