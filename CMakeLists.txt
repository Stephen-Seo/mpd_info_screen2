cmake_minimum_required(VERSION 3.31)
project(mpd_info_screen2 CXX)

set(mpd_info_screen2_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/src/args.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/src/mpd_client.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/src/constants.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/src/helpers.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/src/signal_handler.cc
)

set(unittest_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/test.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/src/args.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/src/mpd_client.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/src/constants.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/src/helpers.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/src/signal_handler.cc
)

add_executable(mpd_info_screen2 ${mpd_info_screen2_SOURCES})
target_compile_features(mpd_info_screen2 PUBLIC cxx_std_23)

add_executable(unittests ${unittest_SOURCES})
target_compile_features(unittests PUBLIC cxx_std_23)

add_custom_command(OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/third_party/lib/libraylib.a
    COMMAND test -e "${CMAKE_CURRENT_SOURCE_DIR}/third_party/raylib-5.5.tar.gz" || curl -L -o "${CMAKE_CURRENT_SOURCE_DIR}/third_party/raylib-5.5.tar.gz" https://github.com/raysan5/raylib/archive/refs/tags/5.5.tar.gz
    COMMAND cd ${CMAKE_CURRENT_SOURCE_DIR} && sha256sum -c ${CMAKE_CURRENT_SOURCE_DIR}/third_party/raylib-5.5_SHA256SUMS.txt
    COMMAND tar -xf ${CMAKE_CURRENT_SOURCE_DIR}/third_party/raylib-5.5.tar.gz -C ${CMAKE_CURRENT_SOURCE_DIR}/third_party
    COMMAND cmake -S ${CMAKE_CURRENT_SOURCE_DIR}/third_party/raylib-5.5 -B ${CMAKE_CURRENT_SOURCE_DIR}/third_party/raylib_BUILD -DCMAKE_BUILD_TYPE=Release
    COMMAND make -C ${CMAKE_CURRENT_SOURCE_DIR}/third_party/raylib_BUILD raylib
    COMMAND install -D -m444 ${CMAKE_CURRENT_SOURCE_DIR}/third_party/raylib_BUILD/raylib/libraylib.a ${CMAKE_CURRENT_SOURCE_DIR}/third_party/lib/libraylib.a
    VERBATIM)

add_custom_target(LIBRAYLIB DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/third_party/lib/libraylib.a)

add_dependencies(mpd_info_screen2 LIBRAYLIB)
add_dependencies(unittests LIBRAYLIB)

if ("${CMAKE_BUILD_TYPE}" STREQUAL "")
    message(STATUS "CMAKE_BUILD_TYPE not specified, defaulting to \"Debug\".")
    set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "Build type" FORCE)
else()
    message(STATUS "CMAKE_BUILD_TYPE is set to \"${CMAKE_BUILD_TYPE}\".")
endif()

target_compile_options(mpd_info_screen2 PRIVATE -I${CMAKE_CURRENT_SOURCE_DIR}/third_party/raylib-5.5/src
PRIVATE -Wall -Wformat -Wformat=2 -Wconversion -Wimplicit-fallthrough  -Werror=format-security  -U_FORTIFY_SOURCE -D_FORTIFY_SOURCE=3  -D_GLIBCXX_ASSERTIONS  -fstrict-flex-arrays=3  -fstack-clash-protection -fstack-protector-strong  -Wl,-z,nodlopen -Wl,-z,noexecstack  -Wl,-z,relro -Wl,-z,now  -Wl,--as-needed -Wl,--no-copy-dt-needed-entriesa -fPIE -pie $<IF:$<STREQUAL:"Release","${CMAKE_BUILD_TYPE}">,-O2 -DNDEBUG -fno-delete-null-pointer-checks -fno-strict-overflow -fno-strict-aliasing -ftrivial-auto-var-init=zero,-Werror -Og -g>
)
target_link_libraries(mpd_info_screen2 ${CMAKE_CURRENT_SOURCE_DIR}/third_party/lib/libraylib.a)
target_compile_options(unittests PRIVATE -I${CMAKE_CURRENT_SOURCE_DIR}/third_party/raylib-5.5/src
PRIVATE -Wall -Wformat -Wformat=2 -Wconversion -Wimplicit-fallthrough  -Werror=format-security  -U_FORTIFY_SOURCE -D_FORTIFY_SOURCE=3  -D_GLIBCXX_ASSERTIONS  -fstrict-flex-arrays=3  -fstack-clash-protection -fstack-protector-strong  -Wl,-z,nodlopen -Wl,-z,noexecstack  -Wl,-z,relro -Wl,-z,now  -Wl,--as-needed -Wl,--no-copy-dt-needed-entriesa -fPIE -pie $<IF:$<STREQUAL:"Release","${CMAKE_BUILD_TYPE}">,-O2 -DNDEBUG -fno-delete-null-pointer-checks -fno-strict-overflow -fno-strict-aliasing -ftrivial-auto-var-init=zero,-Werror -Og -g>
)
target_link_libraries(unittests ${CMAKE_CURRENT_SOURCE_DIR}/third_party/lib/libraylib.a)

set(mpd_info_screen2_clean_files
    "${CMAKE_CURRENT_SOURCE_DIR}/third_party/lib"
    "${CMAKE_CURRENT_SOURCE_DIR}/third_party/raylib-5.5"
    "${CMAKE_CURRENT_SOURCE_DIR}/third_party/raylib_BUILD"
)
set_property(TARGET mpd_info_screen2 PROPERTY ADDITIONAL_CLEAN_FILES ${mpd_info_screen2_clean_files})
