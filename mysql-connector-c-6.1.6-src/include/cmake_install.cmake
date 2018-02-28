# Install script for directory: /home/i/AB/mysql-connector-c-6.1.6-src/include

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

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Development")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include" TYPE FILE FILES
    "/home/i/AB/mysql-connector-c-6.1.6-src/include/mysql.h"
    "/home/i/AB/mysql-connector-c-6.1.6-src/include/mysql_com.h"
    "/home/i/AB/mysql-connector-c-6.1.6-src/include/mysql_time.h"
    "/home/i/AB/mysql-connector-c-6.1.6-src/include/my_list.h"
    "/home/i/AB/mysql-connector-c-6.1.6-src/include/my_alloc.h"
    "/home/i/AB/mysql-connector-c-6.1.6-src/include/typelib.h"
    "/home/i/AB/mysql-connector-c-6.1.6-src/include/my_dbug.h"
    "/home/i/AB/mysql-connector-c-6.1.6-src/include/m_string.h"
    "/home/i/AB/mysql-connector-c-6.1.6-src/include/my_sys.h"
    "/home/i/AB/mysql-connector-c-6.1.6-src/include/my_xml.h"
    "/home/i/AB/mysql-connector-c-6.1.6-src/include/mysql_embed.h"
    "/home/i/AB/mysql-connector-c-6.1.6-src/include/my_pthread.h"
    "/home/i/AB/mysql-connector-c-6.1.6-src/include/decimal.h"
    "/home/i/AB/mysql-connector-c-6.1.6-src/include/errmsg.h"
    "/home/i/AB/mysql-connector-c-6.1.6-src/include/my_global.h"
    "/home/i/AB/mysql-connector-c-6.1.6-src/include/my_getopt.h"
    "/home/i/AB/mysql-connector-c-6.1.6-src/include/sslopt-longopts.h"
    "/home/i/AB/mysql-connector-c-6.1.6-src/include/my_dir.h"
    "/home/i/AB/mysql-connector-c-6.1.6-src/include/sslopt-vars.h"
    "/home/i/AB/mysql-connector-c-6.1.6-src/include/sslopt-case.h"
    "/home/i/AB/mysql-connector-c-6.1.6-src/include/sql_common.h"
    "/home/i/AB/mysql-connector-c-6.1.6-src/include/keycache.h"
    "/home/i/AB/mysql-connector-c-6.1.6-src/include/m_ctype.h"
    "/home/i/AB/mysql-connector-c-6.1.6-src/include/my_compiler.h"
    "/home/i/AB/mysql-connector-c-6.1.6-src/include/mysql_com_server.h"
    "/home/i/AB/mysql-connector-c-6.1.6-src/include/my_byteorder.h"
    "/home/i/AB/mysql-connector-c-6.1.6-src/include/byte_order_generic.h"
    "/home/i/AB/mysql-connector-c-6.1.6-src/include/byte_order_generic_x86.h"
    "/home/i/AB/mysql-connector-c-6.1.6-src/include/little_endian.h"
    "/home/i/AB/mysql-connector-c-6.1.6-src/include/big_endian.h"
    "/home/i/AB/mysql-connector-c-6.1.6-src/include/mysql_version.h"
    "/home/i/AB/mysql-connector-c-6.1.6-src/include/my_config.h"
    "/home/i/AB/mysql-connector-c-6.1.6-src/include/mysqld_ername.h"
    "/home/i/AB/mysql-connector-c-6.1.6-src/include/mysqld_error.h"
    "/home/i/AB/mysql-connector-c-6.1.6-src/include/sql_state.h"
    )
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Development")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/mysql" TYPE DIRECTORY FILES "/home/i/AB/mysql-connector-c-6.1.6-src/include/mysql/" REGEX "/[^/]*\\.h$" REGEX "/psi\\_abi[^/]*$" EXCLUDE)
endif()

