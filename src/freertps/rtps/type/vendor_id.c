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

#include "freertps/rtps/type/vendor_id.h"
#include "freertps/rtps/constant/vendor.h"

const char *frudp_vendor(const frudp_vid_t vid)
{
  switch (vid)
  {
    case FREERTPS_VID_RTI_CONNEXT:      return "RTI Connext";
    case FREERTPS_VID_OPENSPLICE:       return "PrismTech OpenSplice";
    case FREERTPS_VID_OPENDDS:          return "OCI OpenDDS";
    case FREERTPS_VID_MILSOFT:          return "MilSoft";
    case FREERTPS_VID_GALLIUM:          return "Gallium InterCOM";
    case FREERTPS_VID_TWINOAKS:         return "TwinOaks CoreDX";
    case FREERTPS_VID_LAKOTA:           return "Lakota Technical Systems";
    case FREERTPS_VID_ICOUP:            return "ICOUP Consulting";
    case FREERTPS_VID_ETRI:             return "ETRI";
    case FREERTPS_VID_RTI_MICRO:        return "RTI Connext Micro";
    case FREERTPS_VID_PVORTEX_CAFE:     return "PrismTech Vortex Cafe";
    case FREERTPS_VID_PVORTEX_GW:       return "PrismTech Vortex Gateway";
    case FREERTPS_VID_PVORTEX_LITE:     return "PrismTech Vortex Lite";
    case FREERTPS_VID_QEO:              return "Technicolor Qeo";
    case FREERTPS_VID_EPROSIMA:         return "eProsima";
    case FREERTPS_VID_PVORTEX_CLOUD:    return "PrismTech Vortex Cloud";
    case FREERTPS_VID_FREERTPS:         return "freertps";
    default:                            return "unknown";
  }
}
