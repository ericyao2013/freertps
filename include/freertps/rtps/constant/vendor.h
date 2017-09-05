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

#ifndef FREERTPS_VENDOR_H
#define FREERTPS_VENDOR_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/////////////////////////////////////////////////////////////////////////
// VENDOR ID STUFF
// for now let's pretend that our vendor ID is 11311 in hex
#define FREERTPS_VID_FREERTPS      0x2C2F
#define FREERTPS_VID_RTI_CONNEXT   0x0101 // "RTI Connext";
#define FREERTPS_VID_OPENSPLICE    0x0102 // "PrismTech OpenSplice";
#define FREERTPS_VID_OPENDDS       0x0103 // "OCI OpenDDS";
#define FREERTPS_VID_MILSOFT       0x0104 // "MilSoft";
#define FREERTPS_VID_GALLIUM       0x0105 // "Gallium InterCOM";
#define FREERTPS_VID_TWINOAKS      0x0106 // "TwinOaks CoreDX";
#define FREERTPS_VID_LAKOTA        0x0107 // "Lakota Technical Systems";
#define FREERTPS_VID_ICOUP         0x0108 // "ICOUP Consulting";
#define FREERTPS_VID_ETRI          0x0109 // "ETRI";
#define FREERTPS_VID_RTI_MICRO     0x010a // "RTI Connext Micro";
#define FREERTPS_VID_PVORTEX_CAFE  0x010b // "PrismTech Vortex Cafe";
#define FREERTPS_VID_PVORTEX_GW    0x010c // "PrismTech Vortex Gateway";
#define FREERTPS_VID_PVORTEX_LITE  0x010d // "PrismTech Vortex Lite";
#define FREERTPS_VID_QEO           0x010e // "Technicolor Qeo";
#define FREERTPS_VID_EPROSIMA      0x010f // "eProsima";
#define FREERTPS_VID_PVORTEX_CLOUD 0x0120 // "PrismTech Vortex Cloud";

#define FREERTPS_VID_UNKNOWN       {0,0}

#ifdef __cplusplus
}
#endif
#endif // FREERTPS_VENDOR_H
