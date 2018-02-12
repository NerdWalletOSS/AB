# Install script for directory: /home/i/AB/mysql-connector-c-6.1.6-src

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/usr/local/mysql")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "RelWithDebInfo")
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

# Install shared libraries without execute permission?
if(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  set(CMAKE_INSTALL_SO_NO_EXE "1")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Readme")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/." TYPE FILE OPTIONAL FILES
    "/home/i/AB/mysql-connector-c-6.1.6-src/COPYING"
    "/home/i/AB/mysql-connector-c-6.1.6-src/LICENSE.mysql"
    )
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Readme")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/." TYPE FILE FILES "/home/i/AB/mysql-connector-c-6.1.6-src/README")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Documentation")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/docs" TYPE FILE FILES
    "/home/i/AB/mysql-connector-c-6.1.6-src/Docs/INFO_SRC"
    "/home/i/AB/mysql-connector-c-6.1.6-src/Docs/INFO_BIN"
    "/home/i/AB/mysql-connector-c-6.1.6-src/Docs/ChangeLog"
    )
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for each subdirectory.
  include("/home/i/AB/mysql-connector-c-6.1.6-src/zlib/cmake_install.cmake")
  include("/home/i/AB/mysql-connector-c-6.1.6-src/extra/yassl/cmake_install.cmake")
  include("/home/i/AB/mysql-connector-c-6.1.6-src/extra/yassl/taocrypt/cmake_install.cmake")
  include("/home/i/AB/mysql-connector-c-6.1.6-src/include/cmake_install.cmake")
  include("/home/i/AB/mysql-connector-c-6.1.6-src/dbug/cmake_install.cmake")
  include("/home/i/AB/mysql-connector-c-6.1.6-src/strings/cmake_install.cmake")
  include("/home/i/AB/mysql-connector-c-6.1.6-src/vio/cmake_install.cmake")
  include("/home/i/AB/mysql-connector-c-6.1.6-src/regex/cmake_install.cmake")
  include("/home/i/AB/mysql-connector-c-6.1.6-src/mysys/cmake_install.cmake")
  include("/home/i/AB/mysql-connector-c-6.1.6-src/mysys_ssl/cmake_install.cmake")
  include("/home/i/AB/mysql-connector-c-6.1.6-src/libmysql/cmake_install.cmake")
  include("/home/i/AB/mysql-connector-c-6.1.6-src/extra/cmake_install.cmake")
  include("/home/i/AB/mysql-connector-c-6.1.6-src/scripts/cmake_install.cmake")
  include("/home/i/AB/mysql-connector-c-6.1.6-src/tests/cmake_install.cmake")
  include("/home/i/AB/mysql-connector-c-6.1.6-src/support-files/cmake_install.cmake")
  include("/home/i/AB/mysql-connector-c-6.1.6-src/packaging/WiX/cmake_install.cmake")

endif()

if(CMAKE_INSTALL_COMPONENT)
  set(CMAKE_INSTALL_MANIFEST "install_manifest_${CMAKE_INSTALL_COMPONENT}.txt")
else()
  set(CMAKE_INSTALL_MANIFEST "install_manifest.txt")
endif()

string(REPLACE ";" "\n" CMAKE_INSTALL_MANIFEST_CONTENT
       "${CMAKE_INSTALL_MANIFEST_FILES}")
file(WRITE "/home/i/AB/mysql-connector-c-6.1.6-src/${CMAKE_INSTALL_MANIFEST}"
     "${CMAKE_INSTALL_MANIFEST_CONTENT}")
