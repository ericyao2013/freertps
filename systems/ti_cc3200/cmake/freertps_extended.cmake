include_directories(
	 common
	 systems/${SYSTEM}/cc3200-sdk/simplelink/include
)

if (NOT freertps_standalone)
  file(GLOB msgs_SOURCES "build/msgs/src/*.c")
  
  if(msgs_SOURCES STREQUAL "" AND DEFINED ENV{AMENT_PREFIX_PATH})
    execute_process(COMMAND r2/mega_genmsg.py WORKING_DIRECTORY ${PROJECT_SOURCE_DIR})
    file(GLOB msgs_SOURCES "build/msgs/src/*.c")
  endif()
  
  add_library(msgs ${msgs_SOURCES})
  
  install(
  	TARGETS msgs
  	ARCHIVE DESTINATION lib
  	LIBRARY DESTINATION lib
  	RUNTIME DESTINATION bin
  )

  install(
    DIRECTORY build/msgs/
    DESTINATION include
    FILES_MATCHING PATTERN "*.h"
  )
endif()
