cmake_minimum_required(VERSION 3.31)
project(mpd_info_screen2 CXX)

set(mpd_info_screen2_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/main.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/args.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/mpd_client.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/constants.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/helpers.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/signal_handler.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/mpd_display.cc
)

set(unittest_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/test.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/args.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/mpd_client.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/constants.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/helpers.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/signal_handler.cc
)

set(mpd_info_screen2_HEADERS
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/args.h
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/constants.h
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/helpers.h
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/mpd_client.h
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/signal_handler.h
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/mpd_display.h
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/print_helper.h
)

add_executable(mpd_info_screen2 ${mpd_info_screen2_SOURCES})
target_compile_features(mpd_info_screen2 PUBLIC cxx_std_23)

add_executable(unittests ${unittest_SOURCES})
target_compile_features(unittests PUBLIC cxx_std_23)

if(DEFINED CROSS_CC AND NOT DEFINED CROSS_AR)
    message(FATAL_ERROR "CROSS_CC defined but not CROSS_AR!")
elseif(NOT DEFINED CROSS_CC AND NOT DEFINED CROSS_AR)
    set(CROSS_AR "/usr/bin/ar")
endif()
if(DEFINED CROSS_CC AND NOT DEFINED CROSS_RANLIB)
    message(FATAL_ERROR "CROSS_CC defined but not CROSS_RANLIB!")
elseif(NOT DEFINED CROSS_CC AND NOT DEFINED CROSS_RANLIB)
    set(CROSS_RANLIB "/usr/bin/ranlib")
endif()
if(DEFINED CROSS_CC AND NOT DEFINED CROSS_SYSTEM)
    message(FATAL_ERROR "CROSS_CC defined but not CROSS_SYSTEM! (e.g. \"linux\")")
endif()
if(DEFINED CROSS_CC AND NOT DEFINED CROSS_CPU)
    message(FATAL_ERROR "CROSS_CC defined but not CROSS_CPU! (e.g. \"aarch64\")")
endif()
if(DEFINED CROSS_CC AND NOT DEFINED CROSS_HOST)
    message(FATAL_ERROR "CROSS_CC defined but not CROSS_HOST! (e.g. \"aarch64-linux-gnu\")")
endif()
if(NOT DEFINED CROSS_ENDIAN)
    if(DEFINED CROSS_CC)
        message(WARNING "\"CROSS_ENDIAN\" not specified, defaulting to \"little\"")
    endif()
    set(CROSS_ENDIAN "little")
endif()
if(NOT DEFINED CROSS_CC)
    set(CROSS_CC "/usr/bin/gcc")
else()
    set(CROSS_COMP_ENABLED ON)
endif()
if(NOT DEFINED CROSS_CXX)
    set(CROSS_CXX "/usr/bin/g++")
endif()
if(DEFINED CROSS_HOST)
    set(CROSS_HOST_FLAG "--host=${CROSS_HOST}")
endif()

if(CROSS_COMP_ENABLED)
    add_custom_command(OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/third_party/meson_cross.txt"
        COMMAND mkdir -p "${CMAKE_CURRENT_BINARY_DIR}/third_party"
        COMMAND echo "[binaries]" > "${CMAKE_CURRENT_BINARY_DIR}/third_party/meson_cross.txt"
        COMMAND echo "c = '${CROSS_CC}'" >> "${CMAKE_CURRENT_BINARY_DIR}/third_party/meson_cross.txt"
        COMMAND echo "cpp = '${CROSS_CXX}'" >> "${CMAKE_CURRENT_BINARY_DIR}/third_party/meson_cross.txt"
        COMMAND echo "ar = '${CROSS_AR}'" >> "${CMAKE_CURRENT_BINARY_DIR}/third_party/meson_cross.txt"
        COMMAND echo "ranlib = '${CROSS_RANLIB}'" >> "${CMAKE_CURRENT_BINARY_DIR}/third_party/meson_cross.txt"
        COMMAND echo "pkg-config = 'pkg-config'" >> "${CMAKE_CURRENT_BINARY_DIR}/third_party/meson_cross.txt"
        COMMAND echo "[host_machine]" >> "${CMAKE_CURRENT_BINARY_DIR}/third_party/meson_cross.txt"
        COMMAND echo "system = '${CROSS_SYSTEM}'" >> "${CMAKE_CURRENT_BINARY_DIR}/third_party/meson_cross.txt"
        COMMAND echo "cpu_family = '${CROSS_CPU}'" >> "${CMAKE_CURRENT_BINARY_DIR}/third_party/meson_cross.txt"
        COMMAND echo "cpu = '${CROSS_CPU}'" >> "${CMAKE_CURRENT_BINARY_DIR}/third_party/meson_cross.txt"
        COMMAND echo "endian = '${CROSS_ENDIAN}'" >> "${CMAKE_CURRENT_BINARY_DIR}/third_party/meson_cross.txt"
        COMMAND echo "[properties]" >> "${CMAKE_CURRENT_BINARY_DIR}/third_party/meson_cross.txt"
        COMMAND echo "pkg_config_libdir = '${CMAKE_CURRENT_BINARY_DIR}/third_party/PREFIX_OUT/lib/pkgconfig'" >> "${CMAKE_CURRENT_BINARY_DIR}/third_party/meson_cross.txt"
        VERBATIM)

    add_custom_target(MESON_CROSS DEPENDS "${CMAKE_CURRENT_BINARY_DIR}/third_party/meson_cross.txt")

    set(MESON_CROSS_FILE "${CMAKE_CURRENT_BINARY_DIR}/third_party/meson_cross.txt")
else()
    set(MESON_CROSS_FILE "")
endif()

add_custom_command(OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/third_party/native/PREFIX_OUT/lib64/libffi.a"
    COMMAND mkdir -p "${CMAKE_CURRENT_BINARY_DIR}/third_party/native"
    COMMAND test -e "${CMAKE_CURRENT_SOURCE_DIR}/../third_party/libffi-3.5.2.tar.gz" || curl -L -o "${CMAKE_CURRENT_SOURCE_DIR}/../third_party/libffi-3.5.2.tar.gz" https://github.com/libffi/libffi/archive/refs/tags/v3.5.2.tar.gz
    COMMAND sha256sum "${CMAKE_CURRENT_SOURCE_DIR}/../third_party/libffi-3.5.2.tar.gz" | grep dd19253d3007f366319a51d248a40c9e5fcace4498cbea990b566291844e4e30
    COMMAND tar -xf "${CMAKE_CURRENT_SOURCE_DIR}/../third_party/libffi-3.5.2.tar.gz" -C "${CMAKE_CURRENT_BINARY_DIR}/third_party/native"
    COMMAND cd "${CMAKE_CURRENT_BINARY_DIR}/third_party/native/libffi-3.5.2" && ./autogen.sh --enable-static=yes --enable-shared=no "--prefix=${CMAKE_CURRENT_BINARY_DIR}/third_party/native/PREFIX_OUT" && ./configure --enable-static=yes --enable-shared=no "--prefix=${CMAKE_CURRENT_BINARY_DIR}/third_party/native/PREFIX_OUT" --disable-docs
    COMMAND make -C "${CMAKE_CURRENT_BINARY_DIR}/third_party/native/libffi-3.5.2" install
    COMMAND test -e "${CMAKE_CURRENT_BINARY_DIR}/third_party/native/PREFIX_OUT/lib64" || ln -sr "${CMAKE_CURRENT_BINARY_DIR}/third_party/native/PREFIX_OUT/lib" "${CMAKE_CURRENT_BINARY_DIR}/third_party/native/PREFIX_OUT/lib64"
    VERBATIM)

add_custom_target(LIBFFINATIVE DEPENDS "${CMAKE_CURRENT_BINARY_DIR}/third_party/native/PREFIX_OUT/lib64/libffi.a")
set_target_properties(LIBFFINATIVE PROPERTIES ADDITIONAL_CLEAN_FILES "${CMAKE_CURRENT_BINARY_DIR}/third_party/native/libffi-3.5.2;${CMAKE_CURRENT_BINARY_DIR}/third_party/native/PREFIX_OUT")

add_custom_command(OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/third_party/PREFIX_OUT/lib64/libffi.a"
    COMMAND mkdir -p "${CMAKE_CURRENT_BINARY_DIR}/third_party"
    COMMAND test -e "${CMAKE_CURRENT_SOURCE_DIR}/../third_party/libffi-3.5.2.tar.gz" || curl -L -o "${CMAKE_CURRENT_SOURCE_DIR}/../third_party/libffi-3.5.2.tar.gz" https://github.com/libffi/libffi/archive/refs/tags/v3.5.2.tar.gz
    COMMAND sha256sum "${CMAKE_CURRENT_SOURCE_DIR}/../third_party/libffi-3.5.2.tar.gz" | grep dd19253d3007f366319a51d248a40c9e5fcace4498cbea990b566291844e4e30
    COMMAND tar -xf "${CMAKE_CURRENT_SOURCE_DIR}/../third_party/libffi-3.5.2.tar.gz" -C "${CMAKE_CURRENT_BINARY_DIR}/third_party"
    COMMAND cd "${CMAKE_CURRENT_BINARY_DIR}/third_party/libffi-3.5.2" && CC=${CROSS_CC} CXX=${CROSS_CXX} AR=${CROSS_AR} RANLIB=${CROSS_RANLIB} ./autogen.sh --enable-static=yes --enable-shared=no ${CROSS_HOST_FLAG} "--prefix=${CMAKE_CURRENT_BINARY_DIR}/third_party/PREFIX_OUT" && CC=${CROSS_CC} CXX=${CROSS_CXX} AR=${CROSS_AR} RANLIB=${CROSS_RANLIB} ./configure --enable-static=yes --enable-shared=no ${CROSS_HOST_FLAG} "--prefix=${CMAKE_CURRENT_BINARY_DIR}/third_party/PREFIX_OUT" --disable-docs
    COMMAND make -C "${CMAKE_CURRENT_BINARY_DIR}/third_party/libffi-3.5.2" install
    COMMAND test -e "${CMAKE_CURRENT_BINARY_DIR}/third_party/PREFIX_OUT/lib64" || ln -sr "${CMAKE_CURRENT_BINARY_DIR}/third_party/PREFIX_OUT/lib" "${CMAKE_CURRENT_BINARY_DIR}/third_party/PREFIX_OUT/lib64"
    VERBATIM)

add_custom_target(LIBFFI DEPENDS "${CMAKE_CURRENT_BINARY_DIR}/third_party/PREFIX_OUT/lib64/libffi.a")
set_target_properties(LIBFFI PROPERTIES ADDITIONAL_CLEAN_FILES "${CMAKE_CURRENT_BINARY_DIR}/third_party/libffi-3.5.2;${CMAKE_CURRENT_BINARY_DIR}/third_party/PREFIX_OUT")

add_custom_command(OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/third_party/native/PREFIX_OUT/bin/wayland-scanner"
    COMMAND mkdir -p "${CMAKE_CURRENT_BINARY_DIR}/third_party/native"
    COMMAND mkdir -p "${CMAKE_CURRENT_BINARY_DIR}/third_party/PREFIX_OUT/lib/pkgconfig"
    COMMAND test -e "${CMAKE_CURRENT_SOURCE_DIR}/../third_party/wayland-1.25.0.tar.xz" || curl -L -o "${CMAKE_CURRENT_SOURCE_DIR}/../third_party/wayland-1.25.0.tar.xz" "https://gitlab.freedesktop.org/wayland/wayland/-/releases/1.25.0/downloads/wayland-1.25.0.tar.xz"
    COMMAND sha256sum "${CMAKE_CURRENT_SOURCE_DIR}/../third_party/wayland-1.25.0.tar.xz" | grep c065f040afdff3177680600f249727e41a1afc22fccf27222f15f5306faa1f03
    COMMAND tar -xf "${CMAKE_CURRENT_SOURCE_DIR}/../third_party/wayland-1.25.0.tar.xz" -C "${CMAKE_CURRENT_BINARY_DIR}/third_party/native"
    COMMAND env "PKG_CONFIG_PATH=${CMAKE_CURRENT_BINARY_DIR}/third_party/native/PREFIX_OUT/lib/pkgconfig" meson setup --prefix "${CMAKE_CURRENT_BINARY_DIR}/third_party/native/PREFIX_OUT" --buildtype release -D documentation=false -D dtd_validation=false --prefer-static --default-library static --default-both-libraries static "${CMAKE_CURRENT_BINARY_DIR}/third_party/native/wayland-1.25.0/BUILD" "${CMAKE_CURRENT_BINARY_DIR}/third_party/native/wayland-1.25.0"
    COMMAND ninja -C "${CMAKE_CURRENT_BINARY_DIR}/third_party/native/wayland-1.25.0/BUILD" install
    COMMAND find "${CMAKE_CURRENT_BINARY_DIR}/third_party/native/PREFIX_OUT" -name wayland-scanner.pc -execdir cp "{}" "${CMAKE_CURRENT_BINARY_DIR}/third_party/PREFIX_OUT/lib/pkgconfig/" "\;"
    COMMAND_EXPAND_LISTS
    VERBATIM)

add_custom_target(WAYLAND_SCANNER DEPENDS "${CMAKE_CURRENT_BINARY_DIR}/third_party/native/PREFIX_OUT/bin/wayland-scanner")
set_target_properties(WAYLAND_SCANNER PROPERTIES ADDITIONAL_CLEAN_FILES "${CMAKE_CURRENT_BINARY_DIR}/third_party/native")

add_dependencies(WAYLAND_SCANNER LIBFFINATIVE)

add_custom_command(OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/third_party/PREFIX_OUT/lib/libwayland-server.a"
    COMMAND mkdir -p "${CMAKE_CURRENT_BINARY_DIR}/third_party"
    COMMAND mkdir -p "${CMAKE_CURRENT_BINARY_DIR}/third_party/PREFIX_OUT/lib/pkgconfig"
    COMMAND test -e "${CMAKE_CURRENT_SOURCE_DIR}/../third_party/wayland-1.25.0.tar.xz" || curl -L -o "${CMAKE_CURRENT_SOURCE_DIR}/../third_party/wayland-1.25.0.tar.xz" "https://gitlab.freedesktop.org/wayland/wayland/-/releases/1.25.0/downloads/wayland-1.25.0.tar.xz"
    COMMAND sha256sum "${CMAKE_CURRENT_SOURCE_DIR}/../third_party/wayland-1.25.0.tar.xz" | grep c065f040afdff3177680600f249727e41a1afc22fccf27222f15f5306faa1f03
    COMMAND tar -xf "${CMAKE_CURRENT_SOURCE_DIR}/../third_party/wayland-1.25.0.tar.xz" -C "${CMAKE_CURRENT_BINARY_DIR}/third_party"
    COMMAND env "PKG_CONFIG_PATH=${CMAKE_CURRENT_BINARY_DIR}/third_party/PREFIX_OUT/lib/pkgconfig" meson setup --prefix "${CMAKE_CURRENT_BINARY_DIR}/third_party/PREFIX_OUT" --buildtype release -D documentation=false -D dtd_validation=false -D scanner=false --prefer-static --default-library static --default-both-libraries static "$<IF:$<STREQUAL:\"${MESON_CROSS_FILE}\",\"\">,,--native-file>" "$<IF:$<STREQUAL:\"${MESON_CROSS_FILE}\",\"\">,,${MESON_CROSS_FILE}>" "$<IF:$<STREQUAL:\"${MESON_CROSS_FILE}\",\"\">,,--cross-file>" "$<IF:$<STREQUAL:\"${MESON_CROSS_FILE}\",\"\">,,${MESON_CROSS_FILE}>" "${CMAKE_CURRENT_BINARY_DIR}/third_party/wayland-1.25.0/BUILD" "${CMAKE_CURRENT_BINARY_DIR}/third_party/wayland-1.25.0"
    COMMAND ninja -C "${CMAKE_CURRENT_BINARY_DIR}/third_party/wayland-1.25.0/BUILD" install
    COMMAND find "${CMAKE_CURRENT_BINARY_DIR}/third_party/wayland-1.25.0/BUILD/src" -name libwayland-server.a -execdir cp "{}" "${CMAKE_CURRENT_BINARY_DIR}/third_party/PREFIX_OUT/lib/" "\;"
    COMMAND find "${CMAKE_CURRENT_BINARY_DIR}/third_party/wayland-1.25.0/BUILD/src" -name libwayland-client.a -execdir cp "{}" "${CMAKE_CURRENT_BINARY_DIR}/third_party/PREFIX_OUT/lib/" "\;"
    COMMAND find "${CMAKE_CURRENT_BINARY_DIR}/third_party/wayland-1.25.0/BUILD/src" -name libwayland-cursor.a -execdir cp "{}" "${CMAKE_CURRENT_BINARY_DIR}/third_party/PREFIX_OUT/lib/" "\;"
    COMMAND find "${CMAKE_CURRENT_BINARY_DIR}/third_party/wayland-1.25.0/BUILD/src" -name libwayland-egl.a -execdir cp "{}" "${CMAKE_CURRENT_BINARY_DIR}/third_party/PREFIX_OUT/lib/" "\;"
    COMMAND find "${CMAKE_CURRENT_BINARY_DIR}/third_party/wayland-1.25.0/BUILD" -name wayland-client.pc -execdir cp "{}" "${CMAKE_CURRENT_BINARY_DIR}/third_party/PREFIX_OUT/lib/pkgconfig/" "\;"
    COMMAND find "${CMAKE_CURRENT_BINARY_DIR}/third_party/wayland-1.25.0/BUILD" -name wayland-cursor.pc -execdir cp "{}" "${CMAKE_CURRENT_BINARY_DIR}/third_party/PREFIX_OUT/lib/pkgconfig/" "\;"
    COMMAND find "${CMAKE_CURRENT_BINARY_DIR}/third_party/wayland-1.25.0/BUILD" -name wayland-egl-backend.pc -execdir cp "{}" "${CMAKE_CURRENT_BINARY_DIR}/third_party/PREFIX_OUT/lib/pkgconfig/" "\;"
    COMMAND find "${CMAKE_CURRENT_BINARY_DIR}/third_party/wayland-1.25.0/BUILD" -name wayland-server.pc -execdir cp "{}" "${CMAKE_CURRENT_BINARY_DIR}/third_party/PREFIX_OUT/lib/pkgconfig/" "\;"
    COMMAND_EXPAND_LISTS
    VERBATIM)

add_custom_target(LIBWAYLAND DEPENDS "${CMAKE_CURRENT_BINARY_DIR}/third_party/PREFIX_OUT/lib/libwayland-server.a")
set_target_properties(LIBWAYLAND PROPERTIES ADDITIONAL_CLEAN_FILES "${CMAKE_CURRENT_BINARY_DIR}/third_party/wayland-1.25.0;${CMAKE_CURRENT_BINARY_DIR}/third_party/PREFIX_OUT")

add_dependencies(LIBWAYLAND WAYLAND_SCANNER)
add_dependencies(LIBWAYLAND LIBFFI)
if(CROSS_COMP_ENABLED)
    add_dependencies(LIBWAYLAND MESON_CROSS)
endif()

add_custom_command(OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/third_party/PREFIX_OUT/include/wayland-protocols"
    COMMAND mkdir -p "${CMAKE_CURRENT_BINARY_DIR}/third_party"
    COMMAND test -e "${CMAKE_CURRENT_SOURCE_DIR}/../third_party/wayland-protocols-1.47.tar.xz" || curl -L -o "${CMAKE_CURRENT_SOURCE_DIR}/../third_party/wayland-protocols-1.47.tar.xz" https://gitlab.freedesktop.org/wayland/wayland-protocols/-/releases/1.47/downloads/wayland-protocols-1.47.tar.xz
    COMMAND sha256sum "${CMAKE_CURRENT_SOURCE_DIR}/../third_party/wayland-protocols-1.47.tar.xz" | grep 5fd4349bcbc9bab9a46f8cf77d1f434296a7a052c87440a094f63fcf62a58e20
    COMMAND tar -xf "${CMAKE_CURRENT_SOURCE_DIR}/../third_party/wayland-protocols-1.47.tar.xz" -C "${CMAKE_CURRENT_BINARY_DIR}/third_party"
    COMMAND env "PKG_CONFIG_PATH=${CMAKE_CURRENT_BINARY_DIR}/third_party/PREFIX_OUT/lib/pkgconfig" meson setup --prefix "${CMAKE_CURRENT_BINARY_DIR}/third_party/PREFIX_OUT" --pkg-config-path "${CMAKE_CURRENT_BINARY_DIR}/third_party/PREFIX_OUT/lib/pkgconfig" --buildtype release -D tests=false --prefer-static --default-library static --default-both-libraries static "$<IF:$<STREQUAL:\"${MESON_CROSS_FILE}\",\"\">,,--native-file>" "$<IF:$<STREQUAL:\"${MESON_CROSS_FILE}\",\"\">,,${MESON_CROSS_FILE}>" "$<IF:$<STREQUAL:\"${MESON_CROSS_FILE}\",\"\">,,--cross-file>" "$<IF:$<STREQUAL:\"${MESON_CROSS_FILE}\",\"\">,,${MESON_CROSS_FILE}>" "${CMAKE_CURRENT_BINARY_DIR}/third_party/wayland-protocols-1.47/BUILD" "${CMAKE_CURRENT_BINARY_DIR}/third_party/wayland-protocols-1.47"
    COMMAND ninja -C "${CMAKE_CURRENT_BINARY_DIR}/third_party/wayland-protocols-1.47/BUILD" install
    COMMAND_EXPAND_LISTS
    VERBATIM)

add_custom_target(LIBWAYLAND_PROTOCOLS DEPENDS "${CMAKE_CURRENT_BINARY_DIR}/third_party/PREFIX_OUT/include/wayland-protocols")
set_target_properties(LIBWAYLAND_PROTOCOLS PROPERTIES ADDITIONAL_CLEAN_FILES "${CMAKE_CURRENT_BINARY_DIR}/third_party/wayland-protocols-1.47;${CMAKE_CURRENT_BINARY_DIR}/third_party/PREFIX_OUT")

add_dependencies(LIBWAYLAND_PROTOCOLS LIBWAYLAND)

add_custom_command(OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/third_party/PREFIX_OUT/lib/libxml2.a"
    COMMAND mkdir -p "${CMAKE_CURRENT_BINARY_DIR}/third_party"
    COMMAND test -e "${CMAKE_CURRENT_SOURCE_DIR}/../third_party/libxml2-2.15.2.tar.gz" || curl -L -o "${CMAKE_CURRENT_SOURCE_DIR}/../third_party/libxml2-2.15.2.tar.gz" https://github.com/GNOME/libxml2/archive/refs/tags/v2.15.2.tar.gz
    COMMAND sha256sum "${CMAKE_CURRENT_SOURCE_DIR}/../third_party/libxml2-2.15.2.tar.gz" | grep 2769234c4fe2fab9b0b043e891c2af0f1ae51c8d4b94799472981e676ed8009e
    COMMAND tar -xf "${CMAKE_CURRENT_SOURCE_DIR}/../third_party/libxml2-2.15.2.tar.gz" -C "${CMAKE_CURRENT_BINARY_DIR}/third_party"
    COMMAND cd "${CMAKE_CURRENT_BINARY_DIR}/third_party/libxml2-2.15.2" && CC=${CROSS_CC} CXX=${CROSS_CXX} AR=${CROSS_AR} RANLIB=${CROSS_RANLIB} ./autogen.sh ${CROSS_HOST_FLAG} "--prefix=${CMAKE_CURRENT_BINARY_DIR}/third_party/PREFIX_OUT" --enable-static=yes --enable-shared=no
    COMMAND make -C "${CMAKE_CURRENT_BINARY_DIR}/third_party/libxml2-2.15.2" install
    VERBATIM)

add_custom_target(LIBXML2 DEPENDS "${CMAKE_CURRENT_BINARY_DIR}/third_party/PREFIX_OUT/lib/libxml2.a")
set_target_properties(LIBXML2 PROPERTIES ADDITIONAL_CLEAN_FILES "${CMAKE_CURRENT_BINARY_DIR}/third_party/libxml2-2.15.2;${CMAKE_CURRENT_BINARY_DIR}/third_party/PREFIX_OUT")

add_custom_command(OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/third_party/native/PREFIX_OUT/bin/bison"
    COMMAND mkdir -p "${CMAKE_CURRENT_BINARY_DIR}/third_party/native"
    COMMAND test -e "${CMAKE_CURRENT_SOURCE_DIR}/../third_party/bison-3.8.2.tar.xz" || curl -L -o "${CMAKE_CURRENT_SOURCE_DIR}/../third_party/bison-3.8.2.tar.xz" https://mirrors.ocf.berkeley.edu/gnu/bison/bison-3.8.2.tar.xz
    COMMAND sha256sum "${CMAKE_CURRENT_SOURCE_DIR}/../third_party/bison-3.8.2.tar.xz" | grep 9bba0214ccf7f1079c5d59210045227bcf619519840ebfa80cd3849cff5a5bf2
    COMMAND tar -xf "${CMAKE_CURRENT_SOURCE_DIR}/../third_party/bison-3.8.2.tar.xz" -C "${CMAKE_CURRENT_BINARY_DIR}/third_party/native"
    COMMAND cd "${CMAKE_CURRENT_BINARY_DIR}/third_party/native/bison-3.8.2" && ./configure --prefix "${CMAKE_CURRENT_BINARY_DIR}/third_party/native/PREFIX_OUT"
    COMMAND make -C "${CMAKE_CURRENT_BINARY_DIR}/third_party/native/bison-3.8.2" install
    VERBATIM)

add_custom_target(BISON_NATIVE DEPENDS "${CMAKE_CURRENT_BINARY_DIR}/third_party/native/PREFIX_OUT/bin/bison")
set_target_properties(BISON_NATIVE PROPERTIES ADDITIONAL_CLEAN_FILES "${CMAKE_CURRENT_BINARY_DIR}/third_party/native/bison-3.8.2;${CMAKE_CURRENT_BINARY_DIR}/third_party/native/PREFIX_OUT")

add_custom_command(OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/third_party/PREFIX_OUT/lib/libxkbcommon.a"
    COMMAND mkdir -p "${CMAKE_CURRENT_BINARY_DIR}/third_party"
    COMMAND test -e "${CMAKE_CURRENT_SOURCE_DIR}/../third_party/xkbcommon-1.13.1.tar.gz" || curl -L -o "${CMAKE_CURRENT_SOURCE_DIR}/../third_party/xkbcommon-1.13.1.tar.gz" https://github.com/xkbcommon/libxkbcommon/archive/refs/tags/xkbcommon-1.13.1.tar.gz
    COMMAND sha256sum "${CMAKE_CURRENT_SOURCE_DIR}/../third_party/xkbcommon-1.13.1.tar.gz" | grep aeb951964c2f7ecc08174cb5517962d157595e9e3f38fc4a130b91dc2f9fec18
    COMMAND tar -xf "${CMAKE_CURRENT_SOURCE_DIR}/../third_party/xkbcommon-1.13.1.tar.gz" -C "${CMAKE_CURRENT_BINARY_DIR}/third_party"
    COMMAND env "PATH=$ENV{PATH}:${CMAKE_CURRENT_BINARY_DIR}/third_party/native/PREFIX_OUT/bin" "PKG_CONFIG_PATH=${CMAKE_CURRENT_BINARY_DIR}/third_party/PREFIX_OUT/lib/pkgconfig" meson setup --prefix "${CMAKE_CURRENT_BINARY_DIR}/third_party/PREFIX_OUT" --pkg-config-path "${CMAKE_CURRENT_BINARY_DIR}/third_party/PREFIX_OUT/lib/pkgconfig:${CMAKE_CURRENT_BINARY_DIR}/third_party/PREFIX_OUT/share/pkgconfig" --buildtype release --prefer-static --default-library static --default-both-libraries static -D enable-x11=false -D enable-bash-completion=false "$<IF:$<STREQUAL:\"${MESON_CROSS_FILE}\",\"\">,,--native-file>" "$<IF:$<STREQUAL:\"${MESON_CROSS_FILE}\",\"\">,,${MESON_CROSS_FILE}>" "$<IF:$<STREQUAL:\"${MESON_CROSS_FILE}\",\"\">,,--cross-file>" "$<IF:$<STREQUAL:\"${MESON_CROSS_FILE}\",\"\">,,${MESON_CROSS_FILE}>" "${CMAKE_CURRENT_BINARY_DIR}/third_party/libxkbcommon-xkbcommon-1.13.1/BUILD" "${CMAKE_CURRENT_BINARY_DIR}/third_party/libxkbcommon-xkbcommon-1.13.1"
    COMMAND env "PATH=$ENV{PATH}:${CMAKE_CURRENT_BINARY_DIR}/third_party/native/PREFIX_OUT/bin" ninja -C "${CMAKE_CURRENT_BINARY_DIR}/third_party/libxkbcommon-xkbcommon-1.13.1/BUILD" install
    COMMAND_EXPAND_LISTS
    VERBATIM)

add_custom_target(LIBXKBCOMMON DEPENDS "${CMAKE_CURRENT_BINARY_DIR}/third_party/PREFIX_OUT/lib/libxkbcommon.a")
set_target_properties(LIBXKBCOMMON PROPERTIES ADDITIONAL_CLEAN_FILES "${CMAKE_CURRENT_BINARY_DIR}/third_party/libxkbcommon-xkbcommon-1.13.1;${CMAKE_CURRENT_BINARY_DIR}/third_party/PREFIX_OUT")

add_dependencies(LIBXKBCOMMON LIBWAYLAND)
add_dependencies(LIBXKBCOMMON LIBWAYLAND_PROTOCOLS)
add_dependencies(LIBXKBCOMMON LIBXML2)
add_dependencies(LIBXKBCOMMON BISON_NATIVE)

add_custom_command(OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/third_party/PREFIX_OUT/share/aclocal/xorg-macros.m4"
    COMMAND mkdir -p "${CMAKE_CURRENT_BINARY_DIR}/third_party"
    COMMAND test -e "${CMAKE_CURRENT_SOURCE_DIR}/../third_party/xorg-util-macros-1.20.2.tar.xz" || curl -L -o "${CMAKE_CURRENT_SOURCE_DIR}/../third_party/xorg-util-macros-1.20.2.tar.xz" https://www.x.org/releases/individual/util/util-macros-1.20.2.tar.xz
    COMMAND sha256sum "${CMAKE_CURRENT_SOURCE_DIR}/../third_party/xorg-util-macros-1.20.2.tar.xz" | grep 9ac269eba24f672d7d7b3574e4be5f333d13f04a7712303b1821b2a51ac82e8e
    COMMAND tar -xf "${CMAKE_CURRENT_SOURCE_DIR}/../third_party/xorg-util-macros-1.20.2.tar.xz" -C "${CMAKE_CURRENT_BINARY_DIR}/third_party"
    COMMAND cd "${CMAKE_CURRENT_BINARY_DIR}/third_party/util-macros-1.20.2" && CC=${CROSS_CC} CXX=${CROSS_CXX} AR=${CROSS_AR} RANLIB=${CROSS_RANLIB} ./configure ${CROSS_HOST_FLAG} "--prefix=${CMAKE_CURRENT_BINARY_DIR}/third_party/PREFIX_OUT"
    COMMAND make -C "${CMAKE_CURRENT_BINARY_DIR}/third_party/util-macros-1.20.2" install
    VERBATIM)

add_custom_target(XORGUTILMACROS DEPENDS "${CMAKE_CURRENT_BINARY_DIR}/third_party/PREFIX_OUT/share/aclocal/xorg-macros.m4")
set_target_properties(XORGUTILMACROS PROPERTIES ADDITIONAL_CLEAN_FILES "${CMAKE_CURRENT_BINARY_DIR}/third_party/util-macros-1.20.2;${CMAKE_CURRENT_BINARY_DIR}/third_party/PREFIX_OUT")

add_custom_command(OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/third_party/PREFIX_OUT/include/X11/Xtrans/Xtrans.h"
    COMMAND mkdir -p "${CMAKE_CURRENT_BINARY_DIR}/third_party"
    COMMAND test -e "${CMAKE_CURRENT_SOURCE_DIR}/../third_party/xtrans-1.6.0.tar.xz" || curl -L -o "${CMAKE_CURRENT_SOURCE_DIR}/../third_party/xtrans-1.6.0.tar.xz" https://www.x.org/releases/individual/lib/xtrans-1.6.0.tar.xz
    COMMAND sha256sum "${CMAKE_CURRENT_SOURCE_DIR}/../third_party/xtrans-1.6.0.tar.xz" | grep faafea166bf2451a173d9d593352940ec6404145c5d1da5c213423ce4d359e92
    COMMAND tar -xf "${CMAKE_CURRENT_SOURCE_DIR}/../third_party/xtrans-1.6.0.tar.xz" -C "${CMAKE_CURRENT_BINARY_DIR}/third_party"
    COMMAND cd "${CMAKE_CURRENT_BINARY_DIR}/third_party/xtrans-1.6.0" && CC=${CROSS_CC} CXX=${CROSS_CXX} AR=${CROSS_AR} RANLIB=${CROSS_RANLIB} ./configure ${CROSS_HOST_FLAG} "--prefix=${CMAKE_CURRENT_BINARY_DIR}/third_party/PREFIX_OUT"
    COMMAND make -C "${CMAKE_CURRENT_BINARY_DIR}/third_party/xtrans-1.6.0" install
    VERBATIM)

add_custom_target(XTRANSFILES DEPENDS "${CMAKE_CURRENT_BINARY_DIR}/third_party/PREFIX_OUT/include/X11/Xtrans/Xtrans.h")
set_target_properties(XTRANSFILES PROPERTIES ADDITIONAL_CLEAN_FILES "${CMAKE_CURRENT_BINARY_DIR}/third_party/xtrans-1.6.0;${CMAKE_CURRENT_BINARY_DIR}/third_party/PREFIX_OUT")

add_custom_command(OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/third_party/PREFIX_OUT/include/X11/Xproto.h"
    COMMAND mkdir -p "${CMAKE_CURRENT_BINARY_DIR}/third_party"
    COMMAND test -e "${CMAKE_CURRENT_SOURCE_DIR}/../third_party/xorgproto-2025.1.tar.xz" || curl -L -o "${CMAKE_CURRENT_SOURCE_DIR}/../third_party/xorgproto-2025.1.tar.xz" https://www.x.org/releases/individual/proto/xorgproto-2025.1.tar.xz
    COMMAND sha256sum "${CMAKE_CURRENT_SOURCE_DIR}/../third_party/xorgproto-2025.1.tar.xz" | grep 56898c716c0578df8a2d828c9c3e5c528277705c0484381a81960fe1a67668e8
    COMMAND tar -xf "${CMAKE_CURRENT_SOURCE_DIR}/../third_party/xorgproto-2025.1.tar.xz" -C "${CMAKE_CURRENT_BINARY_DIR}/third_party"
    COMMAND cd "${CMAKE_CURRENT_BINARY_DIR}/third_party/xorgproto-2025.1" && CC=${CROSS_CC} CXX=${CROSS_CXX} AR=${CROSS_AR} RANLIB=${CROSS_RANLIB} ./configure ${CROSS_HOST_FLAG} "--prefix=${CMAKE_CURRENT_BINARY_DIR}/third_party/PREFIX_OUT"
    COMMAND make -C "${CMAKE_CURRENT_BINARY_DIR}/third_party/xorgproto-2025.1" install
    VERBATIM)

add_custom_target(XORGPROTO DEPENDS "${CMAKE_CURRENT_BINARY_DIR}/third_party/PREFIX_OUT/include/X11/Xproto.h")
set_target_properties(XORGPROTO PROPERTIES ADDITIONAL_CLEAN_FILES "${CMAKE_CURRENT_BINARY_DIR}/third_party/xorgproto-2025.1;${CMAKE_CURRENT_BINARY_DIR}/third_party/PREFIX_OUT")

add_custom_command(OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/third_party/PREFIX_OUT/share/xcb/xproto.xml"
    COMMAND mkdir -p "${CMAKE_CURRENT_BINARY_DIR}/third_party"
    COMMAND test -e "${CMAKE_CURRENT_SOURCE_DIR}/../third_party/xcb-proto-1.17.0.tar.xz" || curl -L -o "${CMAKE_CURRENT_SOURCE_DIR}/../third_party/xcb-proto-1.17.0.tar.xz" https://www.x.org/releases/individual/proto/xcb-proto-1.17.0.tar.xz
    COMMAND sha256sum "${CMAKE_CURRENT_SOURCE_DIR}/../third_party/xcb-proto-1.17.0.tar.xz" | grep 2c1bacd2110f4799f74de6ebb714b94cf6f80fb112316b1219480fd22562148c
    COMMAND tar -xf "${CMAKE_CURRENT_SOURCE_DIR}/../third_party/xcb-proto-1.17.0.tar.xz" -C "${CMAKE_CURRENT_BINARY_DIR}/third_party"
    COMMAND cd "${CMAKE_CURRENT_BINARY_DIR}/third_party/xcb-proto-1.17.0" && CC=${CROSS_CC} CXX=${CROSS_CXX} AR=${CROSS_AR} RANLIB=${CROSS_RANLIB} ./configure ${CROSS_HOST_FLAG} "--prefix=${CMAKE_CURRENT_BINARY_DIR}/third_party/PREFIX_OUT"
    COMMAND make -C "${CMAKE_CURRENT_BINARY_DIR}/third_party/xcb-proto-1.17.0" install
    VERBATIM)

add_custom_target(XCBPROTO DEPENDS "${CMAKE_CURRENT_BINARY_DIR}/third_party/PREFIX_OUT/share/xcb/xproto.xml")
set_target_properties(XCBPROTO PROPERTIES ADDITIONAL_CLEAN_FILES "${CMAKE_CURRENT_BINARY_DIR}/third_party/xcb-proto-1.17.0;${CMAKE_CURRENT_BIANRY_DIR}/third_party/PREFIX_OUT")

add_custom_command(OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/third_party/PREFIX_OUT/lib/libXau.a"
    COMMAND mkdir -p "${CMAKE_CURRENT_BINARY_DIR}/third_party"
    COMMAND test -e "${CMAKE_CURRENT_SOURCE_DIR}/../third_party/libXau-1.0.12.tar.xz" || curl -L -o "${CMAKE_CURRENT_SOURCE_DIR}/../third_party/libXau-1.0.12.tar.xz" https://www.x.org/releases/individual/lib/libXau-1.0.12.tar.xz
    COMMAND sha256sum "${CMAKE_CURRENT_SOURCE_DIR}/../third_party/libXau-1.0.12.tar.xz" | grep 74d0e4dfa3d39ad8939e99bda37f5967aba528211076828464d2777d477fc0fb
    COMMAND tar -xf "${CMAKE_CURRENT_SOURCE_DIR}/../third_party/libXau-1.0.12.tar.xz" -C "${CMAKE_CURRENT_BINARY_DIR}/third_party"
    COMMAND cd "${CMAKE_CURRENT_BINARY_DIR}/third_party/libXau-1.0.12" && env "PKG_CONFIG_PATH=${CMAKE_CURRENT_BINARY_DIR}/third_party/PREFIX_OUT/share/pkgconfig" CC=${CROSS_CC} CXX=${CROSS_CXX} AR=${CROSS_AR} RANLIB=${CROSS_RANLIB} ./configure ${CROSS_HOST_FLAG} "--prefix=${CMAKE_CURRENT_BINARY_DIR}/third_party/PREFIX_OUT" --enable-static=yes --enable-shared=no
    COMMAND make -C "${CMAKE_CURRENT_BINARY_DIR}/third_party/libXau-1.0.12" install
    VERBATIM)

add_custom_target(LIBXAU DEPENDS "${CMAKE_CURRENT_BINARY_DIR}/third_party/PREFIX_OUT/lib/libXau.a")
set_target_properties(LIBXAU PROPERTIES ADDITIONAL_CLEAN_FILES "${CMAKE_CURRENT_BINARY_DIR}/third_party/libXau-1.0.12;${CMAKE_CURRENT_BINARY_DIR}/third_party/PREFIX_OUT")

add_dependencies(LIBXAU XORGPROTO)

add_custom_command(OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/third_party/PREFIX_OUT/lib/libxcb-xkb.a"
    COMMAND mkdir -p "${CMAKE_CURRENT_BINARY_DIR}/third_party"
    COMMAND test -e "${CMAKE_CURRENT_SOURCE_DIR}/../third_party/libxcb-1.17.0.tar.xz" || curl -L -o "${CMAKE_CURRENT_SOURCE_DIR}/../third_party/libxcb-1.17.0.tar.xz" https://www.x.org/releases/individual/lib/libxcb-1.17.0.tar.xz
    COMMAND sha256sum "${CMAKE_CURRENT_SOURCE_DIR}/../third_party/libxcb-1.17.0.tar.xz" | grep 599ebf9996710fea71622e6e184f3a8ad5b43d0e5fa8c4e407123c88a59a6d55
    COMMAND tar -xf "${CMAKE_CURRENT_SOURCE_DIR}/../third_party/libxcb-1.17.0.tar.xz" -C "${CMAKE_CURRENT_BINARY_DIR}/third_party"
    COMMAND cd "${CMAKE_CURRENT_BINARY_DIR}/third_party/libxcb-1.17.0" && env "PKG_CONFIG_PATH=${CMAKE_CURRENT_BINARY_DIR}/third_party/PREFIX_OUT/share/pkgconfig:${CMAKE_CURRENT_BINARY_DIR}/third_party/PREFIX_OUT/lib/pkgconfig" CC=${CROSS_CC} CXX=${CROSS_CXX} AR=${CROSS_AR} RANLIB=${CROSS_RANLIB} ./configure ${CROSS_HOST_FLAG} "--prefix=${CMAKE_CURRENT_BINARY_DIR}/third_party/PREFIX_OUT" --enable-static=yes --enable-shared=no
    COMMAND make -C "${CMAKE_CURRENT_BINARY_DIR}/third_party/libxcb-1.17.0" install
    VERBATIM)

add_custom_target(LIBXCB DEPENDS "${CMAKE_CURRENT_BINARY_DIR}/third_party/PREFIX_OUT/lib/libxcb-xkb.a")
set_target_properties(LIBXCB PROPERTIES ADDITIONAL_CLEAN_FILES "${CMAKE_CURRENT_BINARY_DIR}/third_party/libxcb-1.17.0;${CMAKE_CURRENT_BINARY_DIR}/third_party/PREFIX_OUT")

add_dependencies(LIBXCB LIBXAU)
add_dependencies(LIBXCB XCBPROTO)

add_custom_command(OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/third_party/PREFIX_OUT/lib/libX11.a"
    COMMAND mkdir -p "${CMAKE_CURRENT_BINARY_DIR}/third_party"
    COMMAND test -e "${CMAKE_CURRENT_SOURCE_DIR}/../third_party/libx11-1.8.13.tar.gz" || curl -L -o "${CMAKE_CURRENT_SOURCE_DIR}/../third_party/libx11-1.8.13.tar.gz" https://www.x.org/releases/individual/lib/libX11-1.8.13.tar.gz
    COMMAND sha256sum "${CMAKE_CURRENT_SOURCE_DIR}/../third_party/libx11-1.8.13.tar.gz" | grep acf0e7cd7541110e6330ecb539441a2d53061f386ec7be6906dfde0de2598470
    COMMAND tar -xf "${CMAKE_CURRENT_SOURCE_DIR}/../third_party/libx11-1.8.13.tar.gz" -C "${CMAKE_CURRENT_BINARY_DIR}/third_party"
    COMMAND cd "${CMAKE_CURRENT_BINARY_DIR}/third_party/libX11-1.8.13" && env "ACLOCAL_PATH=${CMAKE_CURRENT_BINARY_DIR}/third_party/PREFIX_OUT/share/aclocal" "PKG_CONFIG_PATH=${CMAKE_CURRENT_BINARY_DIR}/third_party/PREFIX_OUT/share/pkgconfig:${CMAKE_CURRENT_BINARY_DIR}/third_party/PREFIX_OUT/lib/pkgconfig" CC=${CROSS_CC} CXX=${CROSS_CXX} AR=${CROSS_AR} RANLIB=${CROSS_RANLIB} ./configure ${CROSS_HOST_FLAG} "--prefix=${CMAKE_CURRENT_BINARY_DIR}/third_party/PREFIX_OUT" --enable-static=yes --enable-shared=no
    COMMAND make -C "${CMAKE_CURRENT_BINARY_DIR}/third_party/libX11-1.8.13" install
    VERBATIM)

add_custom_target(LIBX11 DEPENDS "${CMAKE_CURRENT_BINARY_DIR}/third_party/PREFIX_OUT/lib/libX11.a")
set_target_properties(LIBX11 PROPERTIES ADDITIONAL_CLEAN_FILES "${CMAKE_CURRENT_BINARY_DIR}/third_party/libX11-1.8.13;${CMAKE_CURRENT_BINARY_DIR}/third_party/PREFIX_OUT")

add_dependencies(LIBX11 XORGUTILMACROS)
add_dependencies(LIBX11 XTRANSFILES)
add_dependencies(LIBX11 XORGPROTO)
add_dependencies(LIBX11 XCBPROTO)
add_dependencies(LIBX11 LIBXAU)
add_dependencies(LIBX11 LIBXCB)

add_custom_command(OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/third_party/PREFIX_OUT/lib/libXext.a"
    COMMAND mkdir -p "${CMAKE_CURRENT_BINARY_DIR}/third_party"
    COMMAND test -e "${CMAKE_CURRENT_SOURCE_DIR}/../third_party/libXext-1.3.7.tar.xz" || curl -L -o "${CMAKE_CURRENT_SOURCE_DIR}/../third_party/libXext-1.3.7.tar.xz" https://www.x.org/releases/individual/lib/libXext-1.3.7.tar.xz
    COMMAND sha256sum "${CMAKE_CURRENT_SOURCE_DIR}/../third_party/libXext-1.3.7.tar.xz" | grep 6c643c7035cdacf67afd68f25d01b90ef889d546c9fcd7c0adf7c2cf91e3a32d
    COMMAND tar -xf "${CMAKE_CURRENT_SOURCE_DIR}/../third_party/libXext-1.3.7.tar.xz" -C "${CMAKE_CURRENT_BINARY_DIR}/third_party"
    COMMAND cd "${CMAKE_CURRENT_BINARY_DIR}/third_party/libXext-1.3.7" && env "PKG_CONFIG_PATH=${CMAKE_CURRENT_BINARY_DIR}/third_party/PREFIX_OUT/lib/pkgconfig:${CMAKE_CURRENT_BINARY_DIR}/third_party/PREFIX_OUT/share/pkgconfig" CC=${CROSS_CC} CXX=${CROSS_CXX} AR=${CROSS_AR} RANLIB=${CROSS_RANLIB} ./configure ${CROSS_HOST_FLAG} "--prefix=${CMAKE_CURRENT_BINARY_DIR}/third_party/PREFIX_OUT" --enable-static=yes --enable-shared=no
    COMMAND make -C "${CMAKE_CURRENT_BINARY_DIR}/third_party/libXext-1.3.7" install
    VERBATIM)

add_custom_target(LIBXEXT DEPENDS "${CMAKE_CURRENT_BINARY_DIR}/third_party/PREFIX_OUT/lib/libXext.a")
set_target_properties(LIBXEXT PROPERTIES ADDITIONAL_CLEAN_FILES "${CMAKE_CURRENT_BINARY_DIR}/third_party/libXext-1.3.7;${CMAKE_CURRENT_BINARY_DIR}/third_party/PREFIX_OUT")

add_dependencies(LIBXEXT LIBX11)

add_custom_command(OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/third_party/PREFIX_OUT/lib/libXrender.a"
    COMMAND mkdir -p "${CMAKE_CURRENT_BINARY_DIR}/third_party"
    COMMAND test -e "${CMAKE_CURRENT_SOURCE_DIR}/../third_party/libXrender-0.9.12.tar.xz" || curl -L -o "${CMAKE_CURRENT_SOURCE_DIR}/../third_party/libXrender-0.9.12.tar.xz" https://www.x.org/releases/individual/lib/libXrender-0.9.12.tar.xz
    COMMAND sha256sum "${CMAKE_CURRENT_SOURCE_DIR}/../third_party/libXrender-0.9.12.tar.xz" | grep b832128da48b39c8d608224481743403ad1691bf4e554e4be9c174df171d1b97
    COMMAND tar -xf "${CMAKE_CURRENT_SOURCE_DIR}/../third_party/libXrender-0.9.12.tar.xz" -C "${CMAKE_CURRENT_BINARY_DIR}/third_party"
    COMMAND cd "${CMAKE_CURRENT_BINARY_DIR}/third_party/libXrender-0.9.12" && env "PKG_CONFIG_PATH=${CMAKE_CURRENT_BINARY_DIR}/third_party/PREFIX_OUT/lib/pkgconfig:${CMAKE_CURRENT_BINARY_DIR}/third_party/PREFIX_OUT/share/pkgconfig" CC=${CROSS_CC} CXX=${CROSS_CXX} AR=${CROSS_AR} RANLIB=${CROSS_RANLIB} ./configure ${CROSS_HOST_FLAG} "--prefix=${CMAKE_CURRENT_BINARY_DIR}/third_party/PREFIX_OUT" --enable-static=yes --enable-shared=no
    COMMAND make -C "${CMAKE_CURRENT_BINARY_DIR}/third_party/libXrender-0.9.12" install
    VERBATIM)

add_custom_target(LIBXRENDER DEPENDS "${CMAKE_CURRENT_BINARY_DIR}/third_party/PREFIX_OUT/lib/libXrender.a")
set_target_properties(LIBXRENDER PROPERTIES ADDITIONAL_CLEAN_FILES "${CMAKE_CURRENT_BINARY_DIR}/third_party/libXrender-0.9.12;${CMAKE_CURRENT_BINARY_DIR}/third_party/PREFIX_OUT")

add_dependencies(LIBXRENDER LIBX11)

add_custom_command(OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/third_party/PREFIX_OUT/lib/libXrandr.a"
    COMMAND mkdir -p "${CMAKE_CURRENT_BINARY_DIR}/third_party"
    COMMAND test -e "${CMAKE_CURRENT_SOURCE_DIR}/../third_party/libXrandr-1.5.5.tar.xz" || curl -L -o "${CMAKE_CURRENT_SOURCE_DIR}/../third_party/libXrandr-1.5.5.tar.xz" https://www.x.org/releases/individual/lib/libXrandr-1.5.5.tar.xz
    COMMAND sha256sum "${CMAKE_CURRENT_SOURCE_DIR}/../third_party/libXrandr-1.5.5.tar.xz" | grep 72b922c2e765434e9e9f0960148070bd4504b288263e2868a4ccce1b7cf2767a
    COMMAND tar -xf "${CMAKE_CURRENT_SOURCE_DIR}/../third_party/libXrandr-1.5.5.tar.xz" -C "${CMAKE_CURRENT_BINARY_DIR}/third_party"
    COMMAND cd "${CMAKE_CURRENT_BINARY_DIR}/third_party/libXrandr-1.5.5" && env "PKG_CONFIG_PATH=${CMAKE_CURRENT_BINARY_DIR}/third_party/PREFIX_OUT/lib/pkgconfig:${CMAKE_CURRENT_BINARY_DIR}/third_party/PREFIX_OUT/share/pkgconfig" CC=${CROSS_CC} CXX=${CROSS_CXX} AR=${CROSS_AR} RANLIB=${CROSS_RANLIB} ./configure ${CROSS_HOST_FLAG} "--prefix=${CMAKE_CURRENT_BINARY_DIR}/third_party/PREFIX_OUT" --enable-static=yes --enable-shared=no
    COMMAND make -C "${CMAKE_CURRENT_BINARY_DIR}/third_party/libXrandr-1.5.5" install
    VERBATIM)

add_custom_target(LIBXRANDR DEPENDS "${CMAKE_CURRENT_BINARY_DIR}/third_party/PREFIX_OUT/lib/libXrandr.a")
set_target_properties(LIBXRANDR PROPERTIES ADDITIONAL_CLEAN_FILES "${CMAKE_CURRENT_BINARY_DIR}/third_party/libXrandr-1.5.5;${CMAKE_CURRENT_BINARY_DIR}/third_party/PREFIX_OUT")

add_dependencies(LIBXRANDR LIBXEXT)
add_dependencies(LIBXRANDR LIBXRENDER)

add_custom_command(OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/third_party/PREFIX_OUT/lib/libXinerama.a"
    COMMAND mkdir -p "${CMAKE_CURRENT_BINARY_DIR}/third_party"
    COMMAND test -e "${CMAKE_CURRENT_SOURCE_DIR}/../third_party/libXinerama-1.1.6.tar.xz" || curl -L -o "${CMAKE_CURRENT_SOURCE_DIR}/../third_party/libXinerama-1.1.6.tar.xz" https://www.x.org/releases/individual/lib/libXinerama-1.1.6.tar.xz
    COMMAND sha256sum "${CMAKE_CURRENT_SOURCE_DIR}/../third_party/libXinerama-1.1.6.tar.xz" | grep d00fc1599c303dc5cbc122b8068bdc7405d6fcb19060f4597fc51bd3a8be51d7
    COMMAND tar -xf "${CMAKE_CURRENT_SOURCE_DIR}/../third_party/libXinerama-1.1.6.tar.xz" -C "${CMAKE_CURRENT_BINARY_DIR}/third_party"
    COMMAND cd "${CMAKE_CURRENT_BINARY_DIR}/third_party/libXinerama-1.1.6" && env "PKG_CONFIG_PATH=${CMAKE_CURRENT_BINARY_DIR}/third_party/PREFIX_OUT/lib/pkgconfig:${CMAKE_CURRENT_BINARY_DIR}/third_party/PREFIX_OUT/share/pkgconfig" CC=${CROSS_CC} CXX=${CROSS_CXX} AR=${CROSS_AR} RANLIB=${CROSS_RANLIB} ./configure ${CROSS_HOST_FLAG} "--prefix=${CMAKE_CURRENT_BINARY_DIR}/third_party/PREFIX_OUT" --enable-static=yes --enable-shared=no
    COMMAND make -C "${CMAKE_CURRENT_BINARY_DIR}/third_party/libXinerama-1.1.6" install
    VERBATIM)

add_custom_target(LIBXINERAMA DEPENDS "${CMAKE_CURRENT_BINARY_DIR}/third_party/PREFIX_OUT/lib/libXinerama.a")
set_target_properties(LIBXINERAMA PROPERTIES ADDITIONAL_CLEAN_FILES "${CMAKE_CURRENT_BINARY_DIR}/third_party/libXinerama-1.1.6;${CMAKE_CURRENT_BINARY_DIR}/third_party/PREFIX_OUT")

add_dependencies(LIBXINERAMA LIBX11)
add_dependencies(LIBXINERAMA LIBXEXT)

add_custom_command(OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/third_party/PREFIX_OUT/lib/libXfixes.a"
    COMMAND mkdir -p "${CMAKE_CURRENT_BINARY_DIR}/third_party"
    COMMAND test -e "${CMAKE_CURRENT_SOURCE_DIR}/../third_party/libXfixes-6.0.2.tar.xz" || curl -L -o "${CMAKE_CURRENT_SOURCE_DIR}/../third_party/libXfixes-6.0.2.tar.xz" https://www.x.org/releases/individual/lib/libXfixes-6.0.2.tar.xz
    COMMAND sha256sum "${CMAKE_CURRENT_SOURCE_DIR}/../third_party/libXfixes-6.0.2.tar.xz" | grep 39f115d72d9c5f8111e4684164d3d68cc1fd21f9b27ff2401b08fddfc0f409ba
    COMMAND tar -xf "${CMAKE_CURRENT_SOURCE_DIR}/../third_party/libXfixes-6.0.2.tar.xz" -C "${CMAKE_CURRENT_BINARY_DIR}/third_party"
    COMMAND cd "${CMAKE_CURRENT_BINARY_DIR}/third_party/libXfixes-6.0.2" && env "PKG_CONFIG_PATH=${CMAKE_CURRENT_BINARY_DIR}/third_party/PREFIX_OUT/lib/pkgconfig:${CMAKE_CURRENT_BINARY_DIR}/third_party/PREFIX_OUT/share/pkgconfig" CC=${CROSS_CC} CXX=${CROSS_CXX} AR=${CROSS_AR} RANLIB=${CROSS_RANLIB} ./configure ${CROSS_HOST_FLAG} "--prefix=${CMAKE_CURRENT_BINARY_DIR}/third_party/PREFIX_OUT" --enable-static=yes --enable-shared=no
    COMMAND make -C "${CMAKE_CURRENT_BINARY_DIR}/third_party/libXfixes-6.0.2" install
    VERBATIM)

add_custom_target(LIBXFIXES DEPENDS "${CMAKE_CURRENT_BINARY_DIR}/third_party/PREFIX_OUT/lib/libXfixes.a")
set_target_properties(LIBXFIXES PROPERTIES ADDITIONAL_CLEAN_FILES "${CMAKE_CURRENT_BINARY_DIR}/third_party/libXfixes-6.0.2;${CMAKE_CURRENT_BINARY_DIR}/third_party/PREFIX_OUT")

add_dependencies(LIBXFIXES LIBX11)

add_custom_command(OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/third_party/PREFIX_OUT/lib/libXcursor.a"
    COMMAND mkdir -p "${CMAKE_CURRENT_BINARY_DIR}/third_party"
    COMMAND test -e "${CMAKE_CURRENT_SOURCE_DIR}/../third_party/libXcursor-1.2.3.tar.xz" || curl -L -o "${CMAKE_CURRENT_SOURCE_DIR}/../third_party/libXcursor-1.2.3.tar.xz" https://www.x.org/releases/individual/lib/libXcursor-1.2.3.tar.xz
    COMMAND sha256sum "${CMAKE_CURRENT_SOURCE_DIR}/../third_party/libXcursor-1.2.3.tar.xz" | grep fde9402dd4cfe79da71e2d96bb980afc5e6ff4f8a7d74c159e1966afb2b2c2c0
    COMMAND tar -xf "${CMAKE_CURRENT_SOURCE_DIR}/../third_party/libXcursor-1.2.3.tar.xz" -C "${CMAKE_CURRENT_BINARY_DIR}/third_party"
    COMMAND cd "${CMAKE_CURRENT_BINARY_DIR}/third_party/libXcursor-1.2.3" && env "PKG_CONFIG_PATH=${CMAKE_CURRENT_BINARY_DIR}/third_party/PREFIX_OUT/lib/pkgconfig:${CMAKE_CURRENT_BINARY_DIR}/third_party/PREFIX_OUT/share/pkgconfig" CC=${CROSS_CC} CXX=${CROSS_CXX} AR=${CROSS_AR} RANLIB=${CROSS_RANLIB} ./configure ${CROSS_HOST_FLAG} "--prefix=${CMAKE_CURRENT_BINARY_DIR}/third_party/PREFIX_OUT" --enable-static=yes --enable-shared=no
    COMMAND make -C "${CMAKE_CURRENT_BINARY_DIR}/third_party/libXcursor-1.2.3" install
    VERBATIM)

add_custom_target(LIBXCURSOR DEPENDS "${CMAKE_CURRENT_BINARY_DIR}/third_party/PREFIX_OUT/lib/libXcursor.a")
set_target_properties(LIBXCURSOR PROPERTIES ADDITIONAL_CLEAN_FILES "${CMAKE_CURRENT_BINARY_DIR}/third_party/libXcursor-1.2.3;${CMAKE_CURRENT_BINARY_DIR}/third_party/PREFIX_OUT")

add_dependencies(LIBXCURSOR LIBXFIXES)
add_dependencies(LIBXCURSOR LIBXRENDER)

add_custom_command(OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/third_party/PREFIX_OUT/lib/libXi.a"
    COMMAND mkdir -p "${CMAKE_CURRENT_BINARY_DIR}/third_party"
    COMMAND test -e "${CMAKE_CURRENT_SOURCE_DIR}/../third_party/libXi-1.8.2.tar.xz" || curl -L -o "${CMAKE_CURRENT_SOURCE_DIR}/../third_party/libXi-1.8.2.tar.xz" https://www.x.org/releases/individual/lib/libXi-1.8.2.tar.xz
    COMMAND sha256sum "${CMAKE_CURRENT_SOURCE_DIR}/../third_party/libXi-1.8.2.tar.xz" | grep d0e0555e53d6e2114eabfa44226ba162d2708501a25e18d99cfb35c094c6c104
    COMMAND tar -xf "${CMAKE_CURRENT_SOURCE_DIR}/../third_party/libXi-1.8.2.tar.xz" -C "${CMAKE_CURRENT_BINARY_DIR}/third_party"
    COMMAND cd "${CMAKE_CURRENT_BINARY_DIR}/third_party/libXi-1.8.2" && env "PKG_CONFIG_PATH=${CMAKE_CURRENT_BINARY_DIR}/third_party/PREFIX_OUT/lib/pkgconfig:${CMAKE_CURRENT_BINARY_DIR}/third_party/PREFIX_OUT/share/pkgconfig" CC=${CROSS_CC} CXX=${CROSS_CXX} AR=${CROSS_AR} RANLIB=${CROSS_RANLIB} ./configure ${CROSS_HOST_FLAG} "--prefix=${CMAKE_CURRENT_BINARY_DIR}/third_party/PREFIX_OUT" --enable-static=yes --enable-shared=no --enable-docs=no --disable-malloc0returnsnull
    COMMAND make -C "${CMAKE_CURRENT_BINARY_DIR}/third_party/libXi-1.8.2" install
    VERBATIM)

add_custom_target(LIBXI DEPENDS "${CMAKE_CURRENT_BINARY_DIR}/third_party/PREFIX_OUT/lib/libXi.a")
set_target_properties(LIBXI PROPERTIES ADDITIONAL_CLEAN_FILES "${CMAKE_CURRENT_BINARY_DIR}/third_party/libXi-1.8.2;${CMAKE_CURRENT_BINARY_DIR}/third_party/PREFIX_OUT")

add_dependencies(LIBXI LIBX11)
add_dependencies(LIBXI LIBXEXT)
add_dependencies(LIBXI LIBXFIXES)

add_custom_command(OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/third_party/raylib_BUILD/raylib/libraylib.a"
    COMMAND mkdir -p "${CMAKE_CURRENT_BINARY_DIR}/third_party"
    COMMAND test -e "${CMAKE_CURRENT_SOURCE_DIR}/../third_party/raylib-5.5.tar.gz" || curl -L -o "${CMAKE_CURRENT_SOURCE_DIR}/../third_party/raylib-5.5.tar.gz" https://github.com/raysan5/raylib/archive/refs/tags/5.5.tar.gz
    COMMAND cd "${CMAKE_CURRENT_SOURCE_DIR}/.." && sha256sum -c "${CMAKE_CURRENT_SOURCE_DIR}/../third_party/raylib-5.5_SHA256SUMS.txt"
    COMMAND tar -xf "${CMAKE_CURRENT_SOURCE_DIR}/../third_party/raylib-5.5.tar.gz" -C "${CMAKE_CURRENT_BINARY_DIR}/third_party"
    COMMAND cd "${CMAKE_CURRENT_BINARY_DIR}" && patch -p1 < "${CMAKE_CURRENT_SOURCE_DIR}/../third_party/disable-busy-wait-loop.patch"
    COMMAND cd "${CMAKE_CURRENT_BINARY_DIR}" && patch -p1 < "${CMAKE_CURRENT_SOURCE_DIR}/../third_party/font-atlas-size-conservative.patch"
    COMMAND test -e "${CMAKE_CURRENT_SOURCE_DIR}/../third_party/glfw-3.4.tar.gz" || curl -L -o "${CMAKE_CURRENT_SOURCE_DIR}/../third_party/glfw-3.4.tar.gz" "https://github.com/glfw/glfw/archive/refs/tags/3.4.tar.gz"
    COMMAND sha256sum "${CMAKE_CURRENT_SOURCE_DIR}/../third_party/glfw-3.4.tar.gz" | grep c038d34200234d071fae9345bc455e4a8f2f544ab60150765d7704e08f3dac01
    COMMAND tar -xf "${CMAKE_CURRENT_SOURCE_DIR}/../third_party/glfw-3.4.tar.gz" -C "${CMAKE_CURRENT_BINARY_DIR}/third_party"
    COMMAND sed -i -e "/^    add_subdirectory(external\\/glfw)/cadd_subdirectory(${CMAKE_CURRENT_BINARY_DIR}/third_party/glfw-3.4 ${CMAKE_CURRENT_BINARY_DIR}/third_party/glfw-3.4/BUILD_DIR)" "${CMAKE_CURRENT_BINARY_DIR}/third_party/raylib-5.5/cmake/GlfwImport.cmake"
    COMMAND env "CMAKE_PREFIX_PATH=${CMAKE_CURRENT_BINARY_DIR}/third_party/PREFIX_OUT" CC=${CROSS_CC} CXX=${CROSS_CXX} AR=${CROSS_AR} RANLIB=${CROSS_RANLIB} cmake -S "${CMAKE_CURRENT_BINARY_DIR}/third_party/raylib-5.5" -B "${CMAKE_CURRENT_BINARY_DIR}/third_party/raylib_BUILD" -DCMAKE_BUILD_TYPE=Release -DGLFW_BUILD_WAYLAND=ON "-DWAYLAND_SCANNER_EXECUTABLE=${CMAKE_CURRENT_BINARY_DIR}/third_party/native/PREFIX_OUT/bin/wayland-scanner" "-DOPENGL_INCLUDE_DIR=${CMAKE_CURRENT_BINARY_DIR}/third_party/PREFIX_OUT/include"
    COMMAND make -C "${CMAKE_CURRENT_BINARY_DIR}/third_party/raylib_BUILD" raylib
    VERBATIM)

add_custom_target(LIBRAYLIB DEPENDS "${CMAKE_CURRENT_BINARY_DIR}/third_party/raylib_BUILD/raylib/libraylib.a")
set_target_properties(LIBRAYLIB PROPERTIES ADDITIONAL_CLEAN_FILES "${CMAKE_CURRENT_BINARY_DIR}/third_party/raylib-5.5;${CMAKE_CURRENT_BINARY_DIR}/third_party/raylib_BUILD;${CMAKE_CURRENT_BINARY_DIR}/third_party/glfw-3.4")

add_dependencies(LIBRAYLIB LIBXKBCOMMON)
add_dependencies(LIBRAYLIB LIBX11)
add_dependencies(LIBRAYLIB LIBXRANDR)
add_dependencies(LIBRAYLIB LIBXINERAMA)
add_dependencies(LIBRAYLIB LIBXCURSOR)
add_dependencies(LIBRAYLIB LIBXI)

add_dependencies(mpd_info_screen2 LIBRAYLIB)
add_dependencies(unittests LIBRAYLIB)

add_custom_command(OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/third_party/expat-2.7.5/lib/.libs/libexpat.a"
    COMMAND mkdir -p "${CMAKE_CURRENT_BINARY_DIR}/third_party"
    COMMAND test -e "${CMAKE_CURRENT_SOURCE_DIR}/../third_party/expat-2.7.5.tar.xz" || curl -o "${CMAKE_CURRENT_SOURCE_DIR}/../third_party/expat-2.7.5.tar.xz" -L "https://github.com/libexpat/libexpat/releases/download/R_2_7_5/expat-2.7.5.tar.xz"
    COMMAND sha256sum "${CMAKE_CURRENT_SOURCE_DIR}/../third_party/expat-2.7.5.tar.xz" | grep 1032dfef4ff17f70464827daa28369b20f6584d108bc36f17ab1676e1edd2f91
    COMMAND tar -xf "${CMAKE_CURRENT_SOURCE_DIR}/../third_party/expat-2.7.5.tar.xz" -C "${CMAKE_CURRENT_BINARY_DIR}/third_party"
    COMMAND cd "${CMAKE_CURRENT_BINARY_DIR}/third_party/expat-2.7.5" && CC=${CROSS_CC} CXX=${CROSS_CXX} AR=${CROSS_AR} RANLIB=${CROSS_RANLIB} ./buildconf.sh && CC=${CROSS_CC} CXX=${CROSS_CXX} AR=${CROSS_AR} RANLIB=${CROSS_RANLIB} ./configure --prefix=/usr --sysconfdir=/etc --mandir=/usr/share/man --enable-static=yes --enable-shared=no ${CROSS_HOST_FLAG}
    COMMAND make -C "${CMAKE_CURRENT_BINARY_DIR}/third_party/expat-2.7.5"
    VERBATIM)

add_custom_target(LIBEXPAT DEPENDS "${CMAKE_CURRENT_BINARY_DIR}/third_party/expat-2.7.5/lib/.libs/libexpat.a")
set_target_properties(LIBEXPAT PROPERTIES ADDITIONAL_CLEAN_FILES "${CMAKE_CURRENT_BINARY_DIR}/third_party/expat-2.7.5")

add_custom_command(OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/third_party/zlib-1.3.2/libz.a"
    COMMAND mkdir -p "${CMAKE_CURRENT_BINARY_DIR}/third_party"
    COMMAND test -e "${CMAKE_CURRENT_SOURCE_DIR}/../third_party/zlib-1.3.2.tar.gz" || curl -o "${CMAKE_CURRENT_SOURCE_DIR}/../third_party/zlib-1.3.2.tar.gz" -L "https://github.com/madler/zlib/releases/download/v1.3.2/zlib-1.3.2.tar.gz"
    COMMAND sha256sum "${CMAKE_CURRENT_SOURCE_DIR}/../third_party/zlib-1.3.2.tar.gz" | grep bb329a0a2cd0274d05519d61c667c062e06990d72e125ee2dfa8de64f0119d16
    COMMAND tar -xf "${CMAKE_CURRENT_SOURCE_DIR}/../third_party/zlib-1.3.2.tar.gz" -C "${CMAKE_CURRENT_BINARY_DIR}/third_party"
    COMMAND cd "${CMAKE_CURRENT_BINARY_DIR}/third_party/zlib-1.3.2" && CC=${CROSS_CC} CXX=${CROSS_CXX} AR=${CROSS_AR} RANLIB=${CROSS_RANLIB} ./configure --prefix=/usr --static
    COMMAND make -C "${CMAKE_CURRENT_BINARY_DIR}/third_party/zlib-1.3.2"
    VERBATIM)

add_custom_target(LIBZLIB DEPENDS "${CMAKE_CURRENT_BINARY_DIR}/third_party/zlib-1.3.2/libz.a")
set_target_properties(LIBZLIB PROPERTIES ADDITIONAL_CLEAN_FILES "${CMAKE_CURRENT_BINARY_DIR}/third_party/zlib-1.3.2")

add_custom_command(OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/third_party/gzip-1.14/lib/libgzip.a"
    COMMAND mkdir -p "${CMAKE_CURRENT_BINARY_DIR}/third_party"
    COMMAND test -e "${CMAKE_CURRENT_SOURCE_DIR}/../third_party/gzip-1.14.tar.gz" || curl -o "${CMAKE_CURRENT_SOURCE_DIR}/../third_party/gzip-1.14.tar.gz" -L "https://mirrors.ocf.berkeley.edu/gnu/gzip/gzip-1.14.tar.gz"
    COMMAND sha256sum "${CMAKE_CURRENT_SOURCE_DIR}/../third_party/gzip-1.14.tar.gz" | grep 613d6ea44f1248d7370c7ccdeee0dd0017a09e6c39de894b3c6f03f981191c6b
    COMMAND tar -xf "${CMAKE_CURRENT_SOURCE_DIR}/../third_party/gzip-1.14.tar.gz" -C "${CMAKE_CURRENT_BINARY_DIR}/third_party"
    COMMAND cd "${CMAKE_CURRENT_BINARY_DIR}/third_party/gzip-1.14" && CC=${CROSS_CC} CXX=${CROSS_CXX} AR=${CROSS_AR} RANLIB=${CROSS_RANLIB} ./configure --prefix=/usr ${CROSS_HOST_FLAG}
    COMMAND make -C "${CMAKE_CURRENT_BINARY_DIR}/third_party/gzip-1.14"
    VERBATIM)

add_custom_target(LIBGZIP DEPENDS "${CMAKE_CURRENT_BINARY_DIR}/third_party/gzip-1.14/lib/libgzip.a")
set_target_properties(LIBGZIP PROPERTIES ADDITIONAL_CLEAN_FILES "${CMAKE_CURRENT_BINARY_DIR}/third_party/gzip-1.14")

add_custom_command(OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/third_party/bzip2-bzip2-1.0.8/libbz2.a"
    COMMAND mkdir -p "${CMAKE_CURRENT_BINARY_DIR}/third_party"
    COMMAND test -e "${CMAKE_CURRENT_SOURCE_DIR}/../third_party/bzip2-bzip2-1.0.8.tar.gz" || curl -o "${CMAKE_CURRENT_SOURCE_DIR}/../third_party/bzip2-bzip2-1.0.8.tar.gz" -L "https://gitlab.com/bzip2/bzip2/-/archive/bzip2-1.0.8/bzip2-bzip2-1.0.8.tar.gz?ref_type=tags"
    COMMAND sha256sum "${CMAKE_CURRENT_SOURCE_DIR}/../third_party/bzip2-bzip2-1.0.8.tar.gz" | grep db106b740252669664fd8f3a1c69fe7f689d5cd4b132f82ba82b9afba27627df
    COMMAND tar -xf "${CMAKE_CURRENT_SOURCE_DIR}/../third_party/bzip2-bzip2-1.0.8.tar.gz" -C "${CMAKE_CURRENT_BINARY_DIR}/third_party"
    COMMAND sed -i -e "/^CC=/cCC=${CROSS_CC}" -e "/^AR=/cAR=${CROSS_AR}" -e "/^RANLIB=/cRANLIB=${CROSS_RANLIB}" "${CMAKE_CURRENT_BINARY_DIR}/third_party/bzip2-bzip2-1.0.8/Makefile"
    COMMAND CC=${CROSS_CC} CXX=${CROSS_CXX} AR=${CROSS_AR} RANLIB=${CROSS_RANLIB} make -C "${CMAKE_CURRENT_BINARY_DIR}/third_party/bzip2-bzip2-1.0.8" libbz2.a
    VERBATIM)

add_custom_target(LIBBZIP2 DEPENDS "${CMAKE_CURRENT_BINARY_DIR}/third_party/bzip2-bzip2-1.0.8/libbz2.a")
set_target_properties(LIBBZIP2 PROPERTIES ADDITIONAL_CLEAN_FILES "${CMAKE_CURRENT_BINARY_DIR}/third_party/bzip2-bzip2-1.0.8")

add_custom_command(
    OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/third_party/brotli-1.2.0/BUILD/libbrotlicommon.a"
           "${CMAKE_CURRENT_BINARY_DIR}/third_party/brotli-1.2.0/BUILD/libbrotlidec.a"
           "${CMAKE_CURRENT_BINARY_DIR}/third_party/brotli-1.2.0/BUILD/libbrotlienc.a"
    COMMAND mkdir -p "${CMAKE_CURRENT_BINARY_DIR}/third_party"
    COMMAND test -e "${CMAKE_CURRENT_SOURCE_DIR}/../third_party/brotli-1.2.0.tar.gz" || curl -o "${CMAKE_CURRENT_SOURCE_DIR}/../third_party/brotli-1.2.0.tar.gz" -L "https://github.com/google/brotli/archive/refs/tags/v1.2.0.tar.gz"
    COMMAND sha256sum "${CMAKE_CURRENT_SOURCE_DIR}/../third_party/brotli-1.2.0.tar.gz" | grep 816c96e8e8f193b40151dad7e8ff37b1221d019dbcb9c35cd3fadbfe6477dfec
    COMMAND tar -xf "${CMAKE_CURRENT_SOURCE_DIR}/../third_party/brotli-1.2.0.tar.gz" -C "${CMAKE_CURRENT_BINARY_DIR}/third_party"
    COMMAND CC=${CROSS_CC} CXX=${CROSS_CXX} AR=${CROSS_AR} RANLIB=${CROSS_RANLIB} cmake -S "${CMAKE_CURRENT_BINARY_DIR}/third_party/brotli-1.2.0" -B "${CMAKE_CURRENT_BINARY_DIR}/third_party/brotli-1.2.0/BUILD" -DCMAKE_BUILD_TYPE=Release -DBUILD_SHARED_LIBS=Off
    COMMAND make -C "${CMAKE_CURRENT_BINARY_DIR}/third_party/brotli-1.2.0/BUILD"
    VERBATIM)

add_custom_target(LIBBROTLI
    DEPENDS "${CMAKE_CURRENT_BINARY_DIR}/third_party/brotli-1.2.0/BUILD/libbrotlicommon.a"
            "${CMAKE_CURRENT_BINARY_DIR}/third_party/brotli-1.2.0/BUILD/libbrotlidec.a"
            "${CMAKE_CURRENT_BINARY_DIR}/third_party/brotli-1.2.0/BUILD/libbrotlienc.a")
set_target_properties(LIBBROTLI PROPERTIES ADDITIONAL_CLEAN_FILES "${CMAKE_CURRENT_BINARY_DIR}/third_party/brotli-1.2.0")

add_custom_command(OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/third_party/libpng-1.6.56/.libs/libpng16.a"
    COMMAND mkdir -p "${CMAKE_CURRENT_BINARY_DIR}/third_party"
    COMMAND test -e "${CMAKE_CURRENT_SOURCE_DIR}/../third_party/libpng-1.6.56.tar.xz" || curl -o "${CMAKE_CURRENT_SOURCE_DIR}/../third_party/libpng-1.6.56.tar.xz" -L "http://prdownloads.sourceforge.net/libpng/libpng-1.6.56.tar.xz?download"
    COMMAND sha256sum "${CMAKE_CURRENT_SOURCE_DIR}/../third_party/libpng-1.6.56.tar.xz" | grep f7d8bf1601b7804f583a254ab343a6549ca6cf27d255c302c47af2d9d36a6f18
    COMMAND tar -xf "${CMAKE_CURRENT_SOURCE_DIR}/../third_party/libpng-1.6.56.tar.xz" -C "${CMAKE_CURRENT_BINARY_DIR}/third_party"
    COMMAND cd "${CMAKE_CURRENT_BINARY_DIR}/third_party/libpng-1.6.56" && CC=${CROSS_CC} CXX=${CROSS_CXX} AR=${CROSS_AR} RANLIB=${CROSS_RANLIB} CPPFLAGS=-I${CMAKE_CURRENT_BINARY_DIR}/third_party/zlib-1.3.2 LDFLAGS=-L${CMAKE_CURRENT_BINARY_DIR}/third_party/zlib-1.3.2 ./configure --prefix=/usr --enable-shared=no --enable-static=yes ${CROSS_HOST_FLAG}
    COMMAND make -C "${CMAKE_CURRENT_BINARY_DIR}/third_party/libpng-1.6.56"
    VERBATIM)

add_custom_target(LIBPNG DEPENDS "${CMAKE_CURRENT_BINARY_DIR}/third_party/libpng-1.6.56/.libs/libpng16.a")
set_target_properties(LIBPNG PROPERTIES ADDITIONAL_CLEAN_FILES "${CMAKE_CURRENT_BINARY_DIR}/third_party/libpng-1.6.56")

add_dependencies(LIBPNG LIBZLIB)

add_custom_command(OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/third_party/harfbuzz-14.0.0/BUILD/libharfbuzz.a"
    COMMAND mkdir -p "${CMAKE_CURRENT_BINARY_DIR}/third_party"
    COMMAND test -e "${CMAKE_CURRENT_SOURCE_DIR}/../third_party/harfbuzz-14.0.0.tar.gz" || curl -o "${CMAKE_CURRENT_SOURCE_DIR}/../third_party/harfbuzz-14.0.0.tar.gz" -L "https://github.com/harfbuzz/harfbuzz/archive/refs/tags/14.0.0.tar.gz"
    COMMAND sha256sum "${CMAKE_CURRENT_SOURCE_DIR}/../third_party/harfbuzz-14.0.0.tar.gz" | grep f29db9470e0ca5cef484e04e27baeec233aa428e8fdabe9e51b0f706c0809d24
    COMMAND tar -xf "${CMAKE_CURRENT_SOURCE_DIR}/../third_party/harfbuzz-14.0.0.tar.gz" -C "${CMAKE_CURRENT_BINARY_DIR}/third_party"
    COMMAND CC=${CROSS_CC} CXX=${CROSS_CXX} AR=${CROSS_AR} RANLIB=${CROSS_RANLIB} cmake -S "${CMAKE_CURRENT_BINARY_DIR}/third_party/harfbuzz-14.0.0" -B "${CMAKE_CURRENT_BINARY_DIR}/third_party/harfbuzz-14.0.0/BUILD" -DCMAKE_BUILD_TYPE=Release
    COMMAND make -C "${CMAKE_CURRENT_BINARY_DIR}/third_party/harfbuzz-14.0.0/BUILD" harfbuzz
    VERBATIM)

add_custom_target(LIBHARFBUZZ DEPENDS "${CMAKE_CURRENT_BINARY_DIR}/third_party/harfbuzz-14.0.0/BUILD/libharfbuzz.a")
set_target_properties(LIBHARFBUZZ PROPERTIES ADDITIONAL_CLEAN_FILES "${CMAKE_CURRENT_BINARY_DIR}/third_party/harfbuzz-14.0.0")

add_custom_command(OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/third_party/freetype-2.14.3/objs/.libs/libfreetype.a"
    COMMAND mkdir -p "${CMAKE_CURRENT_BINARY_DIR}/third_party"
    COMMAND test -e "${CMAKE_CURRENT_SOURCE_DIR}/../third_party/freetype-2.14.3.tar.xz" || curl -o "${CMAKE_CURRENT_SOURCE_DIR}/../third_party/freetype-2.14.3.tar.xz" -L "https://download.savannah.gnu.org/releases/freetype/freetype-2.14.3.tar.xz"
    COMMAND sha256sum "${CMAKE_CURRENT_SOURCE_DIR}/../third_party/freetype-2.14.3.tar.xz" | grep 36bc4f1cc413335368ee656c42afca65c5a3987e8768cc28cf11ba775e785a5f
    COMMAND tar -xf "${CMAKE_CURRENT_SOURCE_DIR}/../third_party/freetype-2.14.3.tar.xz" -C "${CMAKE_CURRENT_BINARY_DIR}/third_party"
    COMMAND cd "${CMAKE_CURRENT_BINARY_DIR}/third_party/freetype-2.14.3" && CC=${CROSS_CC} CXX=${CROSS_CXX} AR=${CROSS_AR} RANLIB=${CROSS_RANLIB} ./autogen.sh && env CC=${CROSS_CC} CXX=${CROSS_CXX} AR=${CROSS_AR} RANLIB=${CROSS_RANLIB} CPPFLAGS=-I${CMAKE_CURRENT_BINARY_DIR}/third_party/brotli-1.2.0/c/include\ -I${CMAKE_CURRENT_BINARY_DIR}/third_party/zlib-1.3.2\ -I${CMAKE_CURRENT_BINARY_DIR}/third_party/bzip2-bzip2-1.0.8 ./configure --prefix=/usr --sysconfdir=/etc --mandir=/usr/share/man --enable-static=yes --enable-shared=no ${CROSS_HOST_FLAG}
    COMMAND make -C "${CMAKE_CURRENT_BINARY_DIR}/third_party/freetype-2.14.3"
    VERBATIM)

add_custom_target(LIBFREETYPE2 DEPENDS "${CMAKE_CURRENT_BINARY_DIR}/third_party/freetype-2.14.3/objs/.libs/libfreetype.a")
set_target_properties(LIBFREETYPE2 PROPERTIES ADDITIONAL_CLEAN_FILES "${CMAKE_CURRENT_BINARY_DIR}/third_party/freetype-2.14.3")

add_dependencies(LIBFREETYPE2 LIBZLIB)
add_dependencies(LIBFREETYPE2 LIBGZIP)
add_dependencies(LIBFREETYPE2 LIBBZIP2)
add_dependencies(LIBFREETYPE2 LIBBROTLI)
add_dependencies(LIBFREETYPE2 LIBPNG)

add_custom_command(OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/third_party/fontconfig-2.17.1/src/.libs/libfontconfig.a"
    COMMAND mkdir -p "${CMAKE_CURRENT_BINARY_DIR}/third_party"
    COMMAND test -e "${CMAKE_CURRENT_SOURCE_DIR}/../third_party/fontconfig-2.17.1.tar.gz" || curl -o "${CMAKE_CURRENT_SOURCE_DIR}/../third_party/fontconfig-2.17.1.tar.gz" -L "https://gitlab.freedesktop.org/fontconfig/fontconfig/-/archive/2.17.1/fontconfig-2.17.1.tar.gz"
    COMMAND sha256sum "${CMAKE_CURRENT_SOURCE_DIR}/../third_party/fontconfig-2.17.1.tar.gz" | grep 82e73b26adad651b236e5f5d4b3074daf8ff0910188808496326bd3449e5261d
    COMMAND tar -xf "${CMAKE_CURRENT_SOURCE_DIR}/../third_party/fontconfig-2.17.1.tar.gz" -C "${CMAKE_CURRENT_BINARY_DIR}/third_party"
    COMMAND cd "${CMAKE_CURRENT_BINARY_DIR}" && patch -p1 < "${CMAKE_CURRENT_SOURCE_DIR}/../third_party/fontconfig_autogen.patch"
    COMMAND cd "${CMAKE_CURRENT_BINARY_DIR}/third_party/fontconfig-2.17.1" && env CC=${CROSS_CC} CXX=${CROSS_CXX} AR=${CROSS_AR} RANLIB=${CROSS_RANLIB} EXPAT_CFLAGS=-I${CMAKE_CURRENT_BINARY_DIR}/third_party/expat-2.7.5/lib EXPAT_LIBS=-L${CMAKE_CURRENT_BINARY_DIR}/third_party/expat-2.7.5/lib/.libs\ -lexpat FREETYPE_CFLAGS=-I${CMAKE_CURRENT_BINARY_DIR}/third_party/freetype-2.14.3/include FREETYPE_LIBS=-L${CMAKE_CURRENT_BINARY_DIR}/third_party/freetype-2.14.3/objs/.libs\ -lfreetype LDFLAGS=-L${CMAKE_CURRENT_BINARY_DIR}/third_party/zlib-1.3.2\ -L${CMAKE_CURRENT_BINARY_DIR}/third_party/bzip2-bzip2-1.0.8\ -L${CMAKE_CURRENT_BINARY_DIR}/third_party/libpng-1.6.56/.libs\ -L${CMAKE_CURRENT_BINARY_DIR}/third_party/brotli-1.2.0/BUILD\ -L${CMAKE_CURRENT_BINARY_DIR}/third_party/harfbuzz-14.0.0/BUILD ./autogen.sh --sysconfdir=/etc --prefix=/usr --mandir=/usr/share/man --enable-static=yes --enable-shared=no ${CROSS_HOST_FLAG}
    COMMAND make -C "${CMAKE_CURRENT_BINARY_DIR}/third_party/fontconfig-2.17.1/fc-lang"
    COMMAND make -C "${CMAKE_CURRENT_BINARY_DIR}/third_party/fontconfig-2.17.1/fc-case"
    COMMAND make -C "${CMAKE_CURRENT_BINARY_DIR}/third_party/fontconfig-2.17.1/src" fcftalias.h
    COMMAND make -C "${CMAKE_CURRENT_BINARY_DIR}/third_party/fontconfig-2.17.1/src" fcobjshash.h
    COMMAND make -C "${CMAKE_CURRENT_BINARY_DIR}/third_party/fontconfig-2.17.1/src" libfontconfig.la
    VERBATIM)

add_custom_target(LIBFONTCONFIG DEPENDS "${CMAKE_CURRENT_BINARY_DIR}/third_party/fontconfig-2.17.1/src/.libs/libfontconfig.a")
set_target_properties(LIBFONTCONFIG PROPERTIES ADDITIONAL_CLEAN_FILES "${CMAKE_CURRENT_BINARY_DIR}/third_party/fontconfig-2.17.1")

add_dependencies(LIBFONTCONFIG LIBEXPAT)
add_dependencies(LIBFONTCONFIG LIBFREETYPE2)
add_dependencies(LIBFONTCONFIG LIBHARFBUZZ)

add_dependencies(mpd_info_screen2 LIBFONTCONFIG)
add_dependencies(unittests LIBFONTCONFIG)

if ("${CMAKE_BUILD_TYPE}" STREQUAL "")
    message(STATUS "CMAKE_BUILD_TYPE not specified, defaulting to \"Debug\".")
    set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "Build type" FORCE)
else()
    message(STATUS "CMAKE_BUILD_TYPE is set to \"${CMAKE_BUILD_TYPE}\".")
endif()

if(DEFINED FORCE_DEBUG_FLAG AND FORCE_DEBUG_FLAG)
    message(STATUS "FORCE_DEBUG_FLAG Enabled")
else()
    message(STATUS "FORCE_DEBUG_FLAG Disabled")
    unset(FORCE_DEBUG_FLAG)
endif()

target_compile_options(mpd_info_screen2 PRIVATE -I${CMAKE_CURRENT_BINARY_DIR}/third_party/raylib-5.5/src -I${CMAKE_CURRENT_BINARY_DIR}/third_party/fontconfig-2.17.1
    PRIVATE -Wall -Wformat -Wformat=2 -Wconversion -Wimplicit-fallthrough  -Werror=format-security  -U_FORTIFY_SOURCE -D_FORTIFY_SOURCE=3  -D_GLIBCXX_ASSERTIONS  -fstrict-flex-arrays=3  -fstack-clash-protection -fstack-protector-strong  -Wl,-z,nodlopen -Wl,-z,noexecstack  -Wl,-z,relro -Wl,-z,now  -Wl,--as-needed -Wl,--no-copy-dt-needed-entriesa -fPIE -pie $<$<STREQUAL:"Release","${CMAKE_BUILD_TYPE}">:-O2 -DNDEBUG -fno-delete-null-pointer-checks -fno-strict-overflow -fno-strict-aliasing -ftrivial-auto-var-init=zero> $<$<STREQUAL:"Debug","${CMAKE_BUILD_TYPE}">:-Werror -Og -g> $<$<BOOL:${FORCE_DEBUG_FLAG}>:-g>
)
target_link_libraries(mpd_info_screen2 "${CMAKE_CURRENT_BINARY_DIR}/third_party/raylib_BUILD/raylib/libraylib.a" "${CMAKE_CURRENT_BINARY_DIR}/third_party/fontconfig-2.17.1/src/.libs/libfontconfig.a" "${CMAKE_CURRENT_BINARY_DIR}/third_party/expat-2.7.5/lib/.libs/libexpat.a" "${CMAKE_CURRENT_BINARY_DIR}/third_party/freetype-2.14.3/objs/.libs/libfreetype.a" "${CMAKE_CURRENT_BINARY_DIR}/third_party/bzip2-bzip2-1.0.8/libbz2.a" "${CMAKE_CURRENT_BINARY_DIR}/third_party/brotli-1.2.0/BUILD/libbrotlidec.a" "${CMAKE_CURRENT_BINARY_DIR}/third_party/brotli-1.2.0/BUILD/libbrotlienc.a" "${CMAKE_CURRENT_BINARY_DIR}/third_party/brotli-1.2.0/BUILD/libbrotlicommon.a" "${CMAKE_CURRENT_BINARY_DIR}/third_party/libpng-1.6.56/.libs/libpng16.a" "${CMAKE_CURRENT_BINARY_DIR}/third_party/zlib-1.3.2/libz.a" "${CMAKE_CURRENT_BINARY_DIR}/third_party/gzip-1.14/lib/libgzip.a" "${CMAKE_CURRENT_BINARY_DIR}/third_party/harfbuzz-14.0.0/BUILD/libharfbuzz.a" ${EXTERNAL_GLFW_LINKER_LIBS})
target_compile_options(unittests PRIVATE -I${CMAKE_CURRENT_BINARY_DIR}/third_party/raylib-5.5/src -I${CMAKE_CURRENT_BINARY_DIR}/third_party/fontconfig-2.17.1
    PRIVATE -Wall -Wformat -Wformat=2 -Wconversion -Wimplicit-fallthrough  -Werror=format-security  -U_FORTIFY_SOURCE -D_FORTIFY_SOURCE=3  -D_GLIBCXX_ASSERTIONS  -fstrict-flex-arrays=3  -fstack-clash-protection -fstack-protector-strong  -Wl,-z,nodlopen -Wl,-z,noexecstack  -Wl,-z,relro -Wl,-z,now  -Wl,--as-needed -Wl,--no-copy-dt-needed-entriesa -fPIE -pie $<$<STREQUAL:"Release","${CMAKE_BUILD_TYPE}">:-O2 -DNDEBUG -fno-delete-null-pointer-checks -fno-strict-overflow -fno-strict-aliasing -ftrivial-auto-var-init=zero> $<$<STREQUAL:"Debug","${CMAKE_BUILD_TYPE}">:-Werror -Og -g> $<$<BOOL:${FORCE_DEBUG_FLAG}>:-g>
)
target_link_libraries(unittests "${CMAKE_CURRENT_BINARY_DIR}/third_party/raylib_BUILD/raylib/libraylib.a" "${CMAKE_CURRENT_BINARY_DIR}/third_party/fontconfig-2.17.1/src/.libs/libfontconfig.a" "${CMAKE_CURRENT_BINARY_DIR}/third_party/expat-2.7.5/lib/.libs/libexpat.a" "${CMAKE_CURRENT_BINARY_DIR}/third_party/freetype-2.14.3/objs/.libs/libfreetype.a" "${CMAKE_CURRENT_BINARY_DIR}/third_party/bzip2-bzip2-1.0.8/libbz2.a" "${CMAKE_CURRENT_BINARY_DIR}/third_party/brotli-1.2.0/BUILD/libbrotlidec.a" "${CMAKE_CURRENT_BINARY_DIR}/third_party/brotli-1.2.0/BUILD/libbrotlienc.a" "${CMAKE_CURRENT_BINARY_DIR}/third_party/brotli-1.2.0/BUILD/libbrotlicommon.a" "${CMAKE_CURRENT_BINARY_DIR}/third_party/libpng-1.6.56/.libs/libpng16.a" "${CMAKE_CURRENT_BINARY_DIR}/third_party/zlib-1.3.2/libz.a" "${CMAKE_CURRENT_BINARY_DIR}/third_party/gzip-1.14/lib/libgzip.a" "${CMAKE_CURRENT_BINARY_DIR}/third_party/harfbuzz-14.0.0/BUILD/libharfbuzz.a" ${EXTERNAL_GLFW_LINKER_LIBS} atomic)

if(EXISTS "/usr/bin/clang-format")
    add_custom_target(CLANG_FORMAT COMMAND /usr/bin/clang-format -i --style=file ${mpd_info_screen2_SOURCES} ${unittests_SOURCES} ${mpd_info_screen2_HEADERS}
        VERBATIM)
    add_dependencies(mpd_info_screen2 CLANG_FORMAT)
    add_dependencies(unittests CLANG_FORMAT)
endif()

if(DEFINED MPD_INFO_SCREEN_2_VERSION)
    message(STATUS "MPD_INFO_SCREEN_2_VERSION is defined, using version str: ${MPD_INFO_SCREEN_2_VERSION}")
    target_compile_options(mpd_info_screen2 PRIVATE "-DMPD_INFO_SCREEN_2_VERSION=\"${MPD_INFO_SCREEN_2_VERSION}\"")
else()
    message(STATUS "MPD_INFO_SCREEN_2_VERSION is NOT defined, using 'git describe ...' for version str.")
    add_custom_target(MPD_INFO_SCREEN2_VER_STR
        COMMAND cd "${CMAKE_CURRENT_SOURCE_DIR}" && git describe --long --tags | xargs -I verstring echo -e "#define MPD_INFO_SCREEN_2_VERSION \"verstring\"" > "${CMAKE_CURRENT_BINARY_DIR}/MPD_INFO_SCREEN2_VER_STR.h"
        VERBATIM)
    add_dependencies(mpd_info_screen2 MPD_INFO_SCREEN2_VER_STR)
    set_source_files_properties(${CMAKE_CURRENT_SOURCE_DIR}/../src/main.cc PROPERTIES COMPILE_OPTIONS "-include;${CMAKE_CURRENT_BINARY_DIR}/MPD_INFO_SCREEN2_VER_STR.h")
endif()
