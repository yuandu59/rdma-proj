project(RDMATest LANGUAGES C)

cmake_minimum_required(VERSION 3.10)

add_executable(server src/server.c)
add_executable(client src/client.c)

# 链接RDMA库
find_library(RDMACM_LIB rdmacm)
find_library(IBVERBS_LIB ibverbs)

if(NOT RDMACM_LIB OR NOT IBVERBS_LIB)
  message(FATAL_ERROR "RDMA libraries not found")
endif()

target_link_libraries(server
  ${RDMACM_LIB}
  ${IBVERBS_LIB}
)

target_link_libraries(client
  ${RDMACM_LIB}
  ${IBVERBS_LIB}
)