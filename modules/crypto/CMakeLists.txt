# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# shard :: modules :: crypto :: CMakeLists.txt
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

set(MODULE_NAME crypto)

set(MODULE_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/modules/crypto/include)
set(MODULE_SRC_DIR ${PROJECT_SOURCE_DIR}/modules/crypto/src)

set(MODULE_SOURCES
    ${MODULE_SRC_DIR}/base64.cpp
    ${MODULE_SRC_DIR}/sha.cpp
    )

shard_add_static_library(${MODULE_NAME}
                         SOURCES ${MODULE_SOURCES}
                         INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include
                         )

find_package(OpenSSL REQUIRED)

target_link_libraries(${MODULE_NAME} PUBLIC
                      shard::memory
                      # third party
                      OpenSSL::Crypto
                      )