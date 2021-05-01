if(NOT PKG_CONFIG_FOUND)
    INCLUDE(FindPkgConfig)
endif()
PKG_CHECK_MODULES(PC_QO100_MODEM qo100_modem)

FIND_PATH(
    QO100_MODEM_INCLUDE_DIRS
    NAMES qo100_modem/api.h
    HINTS $ENV{QO100_MODEM_DIR}/include
        ${PC_QO100_MODEM_INCLUDEDIR}
    PATHS ${CMAKE_INSTALL_PREFIX}/include
          /usr/local/include
          /usr/include
)

FIND_LIBRARY(
    QO100_MODEM_LIBRARIES
    NAMES gnuradio-qo100_modem
    HINTS $ENV{QO100_MODEM_DIR}/lib
        ${PC_QO100_MODEM_LIBDIR}
    PATHS ${CMAKE_INSTALL_PREFIX}/lib
          ${CMAKE_INSTALL_PREFIX}/lib64
          /usr/local/lib
          /usr/local/lib64
          /usr/lib
          /usr/lib64
          )

include("${CMAKE_CURRENT_LIST_DIR}/qo100_modemTarget.cmake")

INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(QO100_MODEM DEFAULT_MSG QO100_MODEM_LIBRARIES QO100_MODEM_INCLUDE_DIRS)
MARK_AS_ADVANCED(QO100_MODEM_LIBRARIES QO100_MODEM_INCLUDE_DIRS)
