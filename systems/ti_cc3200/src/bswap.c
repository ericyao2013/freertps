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

#include "freertps/psm/bswap.h"
#include "simplelink.h"

uint32_t freertps_htonl(uint32_t u) { return sl_Htonl(u); }
uint32_t freertps_ntohl(uint32_t u) { return sl_Htonl(u); }

uint16_t freertps_htons(uint16_t u) { return sl_Htons(u) ; }
uint16_t freertps_ntohs(uint16_t u) { return sl_Htons(u) ; }
