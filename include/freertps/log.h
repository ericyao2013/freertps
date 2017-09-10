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

#ifndef FREERTPS_LOG_H
#define FREERTPS_LOG_H

#ifndef USE_CC3200
#include <stdio.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

#ifdef USE_CC3200
extern void Error(char *format,...);
extern int Report(const char *format, ...);

#define LOG_REPORT(...) do { Report(__VA_ARGS__); } while (0)
#define LOG_ERROR(...) do { Error(__VA_ARGS__); } while (0)
#else
#define LOG_REPORT(...) printf(__VA_ARGS__)
#define LOG_ERROR(...) printf(__VA_ARGS__)
#endif

// maybe make this smarter someday
#ifdef DEBUG
#define FREERTPS_DEBUG(...) \
  do { LOG_REPORT("[%d] [freertps DEBUG] ", fr_time_now().fraction); LOG_REPORT(__VA_ARGS__); } while (0)
#else
#define FREERTPS_DEBUG(...) ((void)0)
#endif

#define FREERTPS_INFO(...) \
  do { LOG_REPORT("[%d] [freertps INFO] ", fr_time_now().fraction); LOG_REPORT(__VA_ARGS__); } while (0)
#define FREERTPS_WARNING(...) \
  do { LOG_REPORT("[%d] [freertps WARNING] ", fr_time_now().fraction); LOG_REPORT(__VA_ARGS__); } while (0)
#define FREERTPS_ERROR(...) \
  do { LOG_ERROR("[%d] [freertps ERROR] ", fr_time_now().fraction); LOG_ERROR(__VA_ARGS__); } while (0)
#define FREERTPS_FATAL(...) \
  do { LOG_ERROR("[%d] [freertps FATAL] ", fr_time_now().fraction); LOG_ERROR(__VA_ARGS__); } while (0)


#ifdef __cplusplus
}
#endif
#endif // FREERTPS_LOG_H
