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

#ifndef FREERTPS_BUILTIN_H
#define FREERTPS_BUILTIN_H

#ifdef __cplusplus
extern "C" {
#endif

#define FRUDP_BUILTIN_EP_PARTICIPANT_ANNOUNCER           0x00000001
#define FRUDP_BUILTIN_EP_PARTICIPANT_DETECTOR            0x00000002
#define FRUDP_BUILTIN_EP_PUBLICATION_ANNOUNCER           0x00000004
#define FRUDP_BUILTIN_EP_PUBLICATION_DETECTOR            0x00000008
#define FRUDP_BUILTIN_EP_SUBSCRIPTION_ANNOUNCER          0x00000010
#define FRUDP_BUILTIN_EP_SUBSCRIPTION_DETECTOR           0x00000020
#define FRUDP_BUILTIN_EP_PARTICIPANT_PROXY_ANNOUNCER     0x00000040
#define FRUDP_BUILTIN_EP_PARTICIPANT_PROXY_DETECTOR      0x00000080
#define FRUDP_BUILTIN_EP_PARTICIPANT_STATE_ANNOUNCER     0x00000100
#define FRUDP_BUILTIN_EP_PARTICIPANT_STATE_DETECTOR      0x00000200
#define FRUDP_BUILTIN_EP_PARTICIPANT_MESSAGE_DATA_WRITER 0x00000400
#define FRUDP_BUILTIN_EP_PARTICIPANT_MESSAGE_DATA_READER 0x00000800

#ifdef __cplusplus
}
#endif
#endif // FREERTPS_BUILTIN_H
