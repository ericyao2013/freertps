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

#ifndef FREERTPS_LED_H
#define FREERTPS_LED_H

#ifdef __cplusplus
extern "C" {
#endif

// todo: extend this for multiple LEDs as well, maybe by using plurals, like
// leds_on(uint32_t mask), etc. etc.

void led_init(void);
void led_on(void);
void led_off(void);
void led_toggle(void);

#ifdef __cplusplus
}
#endif
#endif // FREERTPS_LED_H

