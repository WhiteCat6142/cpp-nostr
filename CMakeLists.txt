cmake_minimum_required(VERSION 3.20)

set(CMAKE_TOOLCHAIN_FILE ${CMAKE_CURRENT_SOURCE_DIR}/vcpkg/scripts/buildsystems/vcpkg.cmake CACHE STRING "Vcpkg toolchain file")

project(cpp_nostr)

if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
  set(CPP_NOSTR_IS_TOPLEVEL_PROJECT ON)
else()
  set(CPP_NOSTR_IS_IS_TOPLEVEL_PROJECT OFF)
endif()

option(CPP_NOSTR_IS_BUILD_EXAMPLE "Build ${PROJECT_NAME} example" ${CPP_NOSTR_IS_TOPLEVEL_PROJECT})

set(CMAKE_CPP_STANDARD 20)
set(CMAKE_CXX_STANDARD 20)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

find_package(OpenSSL 3.0.0 REQUIRED COMPONENTS SSL Crypto)

find_package(fmt CONFIG REQUIRED)
find_package(nameof CONFIG REQUIRED)
find_package(yyjson CONFIG REQUIRED)
find_package(unofficial-secp256k1 CONFIG REQUIRED)
find_package(libhv CONFIG REQUIRED)

find_package(cpp_yyjson CONFIG REQUIRED)
find_package(libbech32 CONFIG REQUIRED)

add_executable(main src/main.cpp)

target_compile_options(main PUBLIC -O2 -Wall)

target_include_directories(main
    PUBLIC
        $<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        ${OPENSSL_INCLUDE_DIR}
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_link_libraries(main PUBLIC Threads::Threads)
target_link_libraries(main PUBLIC ${OPENSSL_LIBRARIES})
target_link_libraries(main PUBLIC fmt::fmt)
target_link_libraries(main PUBLIC nameof::nameof)
target_link_libraries(main PUBLIC yyjson::yyjson)
target_link_libraries(main PUBLIC unofficial::secp256k1 unofficial::secp256k1_precomputed)
target_link_libraries(main PUBLIC hv_static)
target_link_libraries(main PUBLIC cpp_yyjson::cpp_yyjson)
target_link_libraries(main PUBLIC bech32::bech32)
