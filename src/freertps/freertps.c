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

#include "freertps/freertps.h"
#include "freertps/rtps/sub.h"
#include "freertps/rtps/discovery/sedp.h"
#include "freertps/rtps/discovery/spdp.h"
#include "freertps/psm/udp.h"

#include <stdlib.h>
#include <stdbool.h>

bool g_freertps_init_complete = false;

void freertps_perish_if(bool b, const char *msg)
{
  if (b)
  {
    FREERTPS_FATAL("%s\n", msg);
    exit(1);
  }
}

// todo: return something
void freertps_create_sub(const char *topic_name,
                         const char *type_name,
                         freertps_msg_cb_t msg_cb)
{
  // assume for now that we are only using UDP. in the future, this can
  // become smarter to handle when different (or multiple?) physical layer
  // are initialized
  frudp_add_user_sub(topic_name, type_name, msg_cb);
  //g_rtps_psm.create_sub(topic_name, type
}

frudp_pub_t *freertps_create_pub(const char *topic_name,
                                 const char *type_name)
{
  // assume for now that we are only using UDP. in the future, this can
  // become smarter to handle when different (or multiple?) physical layers
  // are initialized
  return frudp_create_user_pub(topic_name, type_name);
}

bool freertps_publish(frudp_pub_t *pub,
                      const uint8_t *msg,
                      const uint32_t msg_len)
{
  // todo: other physical layers...
  return frudp_publish_user_msg(pub, msg, msg_len);
}

void freertps_start(void)
{
  frudp_print_participants_debug();
  frudp_print_sedp_debug();

  // todo: other physical layers...
  frudp_disco_start();
}

void freertps_spinOnce(void)
{
  frudp_listen(500000); // 500 millisecond for receive data.
  frudp_disco_tick();
}

void freertps_spin(void)
{
  while (freertps_system_ok())
  {
    freertps_spinOnce();
  }
}

void freertps_stop(void)
{
  frudp_fini();
}
