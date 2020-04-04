# Install script for directory: C:/Users/mbirch/Downloads/cfit-3.47/cfitsio-3.47

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "C:/Program Files (x86)/CFITSIO")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Debug")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
   "C:/Program Files (x86)/CFITSIO/lib/cfitsio.lib")
  if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
file(INSTALL DESTINATION "C:/Program Files (x86)/CFITSIO/lib" TYPE STATIC_LIBRARY OPTIONAL FILES "C:/Users/mbirch/Downloads/cfit-3.47/cfitsio.build/cfitsio.lib")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
   "C:/Program Files (x86)/CFITSIO/lib/cfitsio.dll")
  if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
file(INSTALL DESTINATION "C:/Program Files (x86)/CFITSIO/lib" TYPE SHARED_LIBRARY FILES "C:/Users/mbirch/Downloads/cfit-3.47/cfitsio.build/cfitsio.dll")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY OPTIONAL FILES "C:/Users/mbirch/Downloads/cfit-3.47/cfitsio.build/cfitsio.lib")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE SHARED_LIBRARY FILES "C:/Users/mbirch/Downloads/cfit-3.47/cfitsio.build/cfitsio.dll")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xDevelx" OR NOT CMAKE_INSTALL_COMPONENT)
  list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
   "C:/Program Files (x86)/CFITSIO/include/cfortran.h;C:/Program Files (x86)/CFITSIO/include/drvrgsiftp.h;C:/Program Files (x86)/CFITSIO/include/drvrsmem.h;C:/Program Files (x86)/CFITSIO/include/eval_defs.h;C:/Program Files (x86)/CFITSIO/include/eval_tab.h;C:/Program Files (x86)/CFITSIO/include/f77_wrap.h;C:/Program Files (x86)/CFITSIO/include/fitsio.h;C:/Program Files (x86)/CFITSIO/include/fitsio2.h;C:/Program Files (x86)/CFITSIO/include/fpack.h;C:/Program Files (x86)/CFITSIO/include/group.h;C:/Program Files (x86)/CFITSIO/include/grparser.h;C:/Program Files (x86)/CFITSIO/include/longnam.h;C:/Program Files (x86)/CFITSIO/include/region.h;C:/Program Files (x86)/CFITSIO/include/simplerng.h;C:/Program Files (x86)/CFITSIO/include/unistd.h")
  if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
file(INSTALL DESTINATION "C:/Program Files (x86)/CFITSIO/include" TYPE FILE FILES
    "C:/Users/mbirch/Downloads/cfit-3.47/cfitsio-3.47/cfortran.h"
    "C:/Users/mbirch/Downloads/cfit-3.47/cfitsio-3.47/drvrgsiftp.h"
    "C:/Users/mbirch/Downloads/cfit-3.47/cfitsio-3.47/drvrsmem.h"
    "C:/Users/mbirch/Downloads/cfit-3.47/cfitsio-3.47/eval_defs.h"
    "C:/Users/mbirch/Downloads/cfit-3.47/cfitsio-3.47/eval_tab.h"
    "C:/Users/mbirch/Downloads/cfit-3.47/cfitsio-3.47/f77_wrap.h"
    "C:/Users/mbirch/Downloads/cfit-3.47/cfitsio-3.47/fitsio.h"
    "C:/Users/mbirch/Downloads/cfit-3.47/cfitsio-3.47/fitsio2.h"
    "C:/Users/mbirch/Downloads/cfit-3.47/cfitsio-3.47/fpack.h"
    "C:/Users/mbirch/Downloads/cfit-3.47/cfitsio-3.47/group.h"
    "C:/Users/mbirch/Downloads/cfit-3.47/cfitsio-3.47/grparser.h"
    "C:/Users/mbirch/Downloads/cfit-3.47/cfitsio-3.47/longnam.h"
    "C:/Users/mbirch/Downloads/cfit-3.47/cfitsio-3.47/region.h"
    "C:/Users/mbirch/Downloads/cfit-3.47/cfitsio-3.47/simplerng.h"
    "C:/Users/mbirch/Downloads/cfit-3.47/cfitsio-3.47/unistd.h"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/pkgconfig" TYPE FILE FILES "C:/Users/mbirch/Downloads/cfit-3.47/cfitsio.build/cfitsio.pc")
endif()

if(CMAKE_INSTALL_COMPONENT)
  set(CMAKE_INSTALL_MANIFEST "install_manifest_${CMAKE_INSTALL_COMPONENT}.txt")
else()
  set(CMAKE_INSTALL_MANIFEST "install_manifest.txt")
endif()

string(REPLACE ";" "\n" CMAKE_INSTALL_MANIFEST_CONTENT
       "${CMAKE_INSTALL_MANIFEST_FILES}")
file(WRITE "C:/Users/mbirch/Downloads/cfit-3.47/cfitsio.build/${CMAKE_INSTALL_MANIFEST}"
     "${CMAKE_INSTALL_MANIFEST_CONTENT}")
