# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles\\LABA7_QT5_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\LABA7_QT5_autogen.dir\\ParseCache.txt"
  "LABA7_QT5_autogen"
  )
endif()
