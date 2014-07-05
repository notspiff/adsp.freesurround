project(adsp.freesurround)

cmake_minimum_required(VERSION 2.6)

list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR})

enable_language(CXX)

find_package(xbmc REQUIRED)
find_package(xbmcplatform REQUIRED)

include_directories(${xbmcplatform_INCLUDE_DIRS}
                    ${XBMC_INCLUDE_DIR})

set(FREESURROUND_SOURCES src/client.cpp
                         src/GUIDialogFreeSurround.cpp
                         src/kissfft/kiss_fft.cpp
                         src/kissfft/kiss_fftr.cpp
                         src/DSPProcessFreeSurround.cpp
                         src/FreeSurroundSettings.cpp
                         src/ChannelMaps.cpp
                         src/FreeSurroundDecoder.cpp)

set(DEPLIBS ${xbmcplatform_LIBRARIES})

build_addon(adsp.freesurround FREESURROUND DEPLIBS)

include(CPack)
