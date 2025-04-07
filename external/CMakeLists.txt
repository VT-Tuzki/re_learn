# External dependencies - just add subdirectories without forcing all apps to use them






# Check which apps need which libraries
set(NEED_CMOCKERY FALSE)
set(NEED_VTUZKICC FALSE)

# Loop through apps that will be built
foreach(APP ${APPS_TO_BUILD})
  # Read the app's dependencies
  if(EXISTS "${CMAKE_SOURCE_DIR}/apps/${APP}/dependencies.cmake")
    include("${CMAKE_SOURCE_DIR}/apps/${APP}/dependencies.cmake")
  endif()
endforeach()

# Only build libraries that are needed
if(NEED_CMOCKERY)
  message(STATUS "Building cmockery library")
  add_subdirectory(cmockery)
endif()

if(NEED_VTUZKICC)
  message(STATUS "Building vtuzkicc library")
  add_subdirectory(vtuzkicc)
endif()
# No global "external_dependencies" target - let each app choose what it needs