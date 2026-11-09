cmake_minimum_required(VERSION 3.31)
project(mpd_info_screen2 CXX)

set(LIBFFI_VERSION "3.8.0")
set(LIBFFI_DOWNLOAD_URL "https://github.com/libffi/libffi/archive/refs/tags/v${LIBFFI_VERSION}.tar.gz")
set(LIBFFI_SHA256SUM "bf40d752d8f5fd4505bcd1c7d4208ea87fd12c91f087e359651c776748352dc0")

set(WAYLAND_VERSION "1.26.0")
set(WAYLAND_DOWNLOAD_URL "https://gitlab.freedesktop.org/wayland/wayland/-/releases/${WAYLAND_VERSION}/downloads/wayland-${WAYLAND_VERSION}.tar.xz")
set(WAYLAND_SHA256SUM "64176eaa46e4969903e286f8e5ef8331affc17fdf03ac9b58381d2b23162b7a3")

set(WAYLAND_PROTOCOLS_VERSION "1.49")
set(WAYLAND_PROTOCOLS_DOWNLOAD_URL "https://gitlab.freedesktop.org/wayland/wayland-protocols/-/releases/${WAYLAND_PROTOCOLS_VERSION}/downloads/wayland-protocols-${WAYLAND_PROTOCOLS_VERSION}.tar.xz")
set(WAYLAND_PROTOCOLS_SHA256SUM "ec4c8f74942d6dff7ace8b4ce4764f0ef9ff618a935d974ea77edee2ad240b14")

set(LIBXML2_VERSION "2.15.4")
set(LIBXML2_DOWNLOAD_URL "https://github.com/GNOME/libxml2/archive/refs/tags/v${LIBXML2_VERSION}.tar.gz")
set(LIBXML2_SHA256SUM "bb01bd9ac9a7403f9706816fd6d3e7888041d32b0211075feb20e755d2a9f29d")

set(BISON_VERSION "3.8.2")
set(BISON_DOWNLOAD_URL "https://mirrors.ocf.berkeley.edu/gnu/bison/bison-${BISON_VERSION}.tar.xz")
set(BISON_SHA256SUM "9bba0214ccf7f1079c5d59210045227bcf619519840ebfa80cd3849cff5a5bf2")

set(LIBXKBCOMMON_VERSION "1.13.2")
set(LIBXKBCOMMON_DOWNLOAD_URL "https://github.com/xkbcommon/libxkbcommon/archive/refs/tags/xkbcommon-${LIBXKBCOMMON_VERSION}.tar.gz")
set(LIBXKBCOMMON_SHA256SUM "acc4d5f7c3cbba5f9f8d08d8bdbeede84ecede46792f47929aa9321873385528")

set(XORG_UTIL_MACROS_VERSION "1.20.2")
set(XORG_UTIL_MACROS_DOWNLOAD_URL "https://www.x.org/releases/individual/util/util-macros-${XORG_UTIL_MACROS_VERSION}.tar.xz")
set(XORG_UTIL_MACROS_SHA256SUM "9ac269eba24f672d7d7b3574e4be5f333d13f04a7712303b1821b2a51ac82e8e")

set(XTRANS_VERSION "1.6.0")
set(XTRANS_DOWNLOAD_URL "https://www.x.org/releases/individual/lib/xtrans-${XTRANS_VERSION}.tar.xz")
set(XTRANS_SHA256SUM "faafea166bf2451a173d9d593352940ec6404145c5d1da5c213423ce4d359e92")

set(XORGPROTO_VERSION "2026.1")
set(XORGPROTO_DOWNLOAD_URL "https://xorg.freedesktop.org/archive/individual/proto/xorgproto-${XORGPROTO_VERSION}.tar.xz")
set(XORGPROTO_SHA256SUM "f9bfe4a9ed8c8ab9d2a3b0d49797f046052dadd06b7a8b45dbffaffb137e8290")

set(XCB_PROTO_VERSION "1.17.0")
set(XCB_PROTO_DOWNLOAD_URL "https://www.x.org/releases/individual/proto/xcb-proto-${XCB_PROTO_VERSION}.tar.xz")
set(XCB_PROTO_SHA256SUM "2c1bacd2110f4799f74de6ebb714b94cf6f80fb112316b1219480fd22562148c")

set(LIBXAU_VERSION "1.0.12")
set(LIBXAU_DOWNLOAD_URL "https://www.x.org/releases/individual/lib/libXau-${LIBXAU_VERSION}.tar.xz")
set(LIBXAU_SHA256SUM "74d0e4dfa3d39ad8939e99bda37f5967aba528211076828464d2777d477fc0fb")

set(LIBXDMCP_VERSION "1.1.5")
set(LIBXDMCP_DOWNLOAD_URL "https://www.x.org/releases/individual/lib/libXdmcp-${LIBXDMCP_VERSION}.tar.xz")
set(LIBXDMCP_SHA256SUM "d8a5222828c3adab70adf69a5583f1d32eb5ece04304f7f8392b6a353aa2228c")

set(LIBXCB_VERSION "1.17.0")
set(LIBXCB_DOWNLOAD_URL "https://www.x.org/releases/individual/lib/libxcb-${LIBXCB_VERSION}.tar.xz")
set(LIBXCB_SHA256SUM "599ebf9996710fea71622e6e184f3a8ad5b43d0e5fa8c4e407123c88a59a6d55")

set(LIBX11_VERSION "1.8.14")
set(LIBX11_DOWNLOAD_URL "https://xorg.freedesktop.org/archive/individual/lib/libX11-${LIBX11_VERSION}.tar.gz")
set(LIBX11_SHA256SUM "ebb8a58549ddde50aa2e742b3b256557f451053d173be8dbfc2e8ac078eee1d6")

set(LIBXEXT_VERSION "1.3.7")
set(LIBXEXT_DOWNLOAD_URL "https://www.x.org/releases/individual/lib/libXext-${LIBXEXT_VERSION}.tar.xz")
set(LIBXEXT_SHA256SUM "6c643c7035cdacf67afd68f25d01b90ef889d546c9fcd7c0adf7c2cf91e3a32d")

set(LIBXRENDER_VERSION "0.9.13")
set(LIBXRENDER_DOWNLOAD_URL "https://xorg.freedesktop.org/archive/individual/lib/libXrender-${LIBXRENDER_VERSION}.tar.xz")
set(LIBXRENDER_SHA256SUM "3fe88c609df6782e1e7af2e4de36b2563d70cd70ff7dcb1cda561ad64912bb64")

set(LIBXRANDR_VERSION "1.5.5")
set(LIBXRANDR_DOWNLOAD_URL "https://www.x.org/releases/individual/lib/libXrandr-${LIBXRANDR_VERSION}.tar.xz")
set(LIBXRANDR_SHA256SUM "72b922c2e765434e9e9f0960148070bd4504b288263e2868a4ccce1b7cf2767a")

set(LIBXINERAMA_VERSION "1.1.6")
set(LIBXINERAMA_DOWNLOAD_URL "https://www.x.org/releases/individual/lib/libXinerama-${LIBXINERAMA_VERSION}.tar.xz")
set(LIBXINERAMA_SHA256SUM "d00fc1599c303dc5cbc122b8068bdc7405d6fcb19060f4597fc51bd3a8be51d7")

set(LIBXFIXES_VERSION "6.0.2")
set(LIBXFIXES_DOWNLOAD_URL "https://www.x.org/releases/individual/lib/libXfixes-${LIBXFIXES_VERSION}.tar.xz")
set(LIBXFIXES_SHA256SUM "39f115d72d9c5f8111e4684164d3d68cc1fd21f9b27ff2401b08fddfc0f409ba")

set(LIBXCURSOR_VERSION "1.2.3")
set(LIBXCURSOR_DOWNLOAD_URL "https://www.x.org/releases/individual/lib/libXcursor-${LIBXCURSOR_VERSION}.tar.xz")
set(LIBXCURSOR_SHA256SUM "fde9402dd4cfe79da71e2d96bb980afc5e6ff4f8a7d74c159e1966afb2b2c2c0")

set(LIBXI_VERSION "1.8.4")
set(LIBXI_DOWNLOAD_URL "https://xorg.freedesktop.org/archive/individual/lib/libXi-${LIBXI_VERSION}.tar.xz")
set(LIBXI_SHA256SUM "ddd6057b89a91903bc2f884d55c8467440906945e482c36547260dff28661afc")

set(RAYLIB_VERSION "6.2")
set(RAYLIB_DOWNLOAD_URL "https://github.com/raysan5/raylib/archive/refs/tags/${RAYLIB_VERSION}.tar.gz")
set(RAYLIB_SHA256SUM "df7a13129b1bec9e73a6962efff170beba310ee6eab8f94cd000b7316b4db5ee")

set(GLFW_VERSION "3.5.1")
set(GLFW_DOWNLOAD_URL "https://github.com/glfw/glfw/archive/refs/tags/${GLFW_VERSION}.tar.gz")
set(GLFW_SHA256SUM "5234f4f29473e9a06bc7847d8371858dd135d38466eeeaa652fdc9f8f9ff0c20")

set(EXPAT_VERSION "2.10.0")
set(EXPAT_TAG "R_2_10_0")
set(EXPAT_DOWNLOAD_URL "https://github.com/libexpat/libexpat/releases/download/${EXPAT_TAG}/expat-${EXPAT_VERSION}.tar.lz")
set(EXPAT_SHA256SUM "30d081f01e803c96be5d09e6ed4345d37a28f3514963459ef116093b7bde44af")

set(ZLIB_VERSION "1.4")
set(ZLIB_DOWNLOAD_URL "https://github.com/madler/zlib/releases/download/v${ZLIB_VERSION}/zlib-${ZLIB_VERSION}.tar.xz")
set(ZLIB_SHA256SUM "4f11c512053d7bab8d9e9b89bde3575add3e4c50a383b42ef6cb837035fc5a7c")

set(GZIP_VERSION "1.15")
set(GZIP_DOWNLOAD_URL "https://mirrors.ocf.berkeley.edu/gnu/gzip/gzip-${GZIP_VERSION}.tar.xz")
set(GZIP_SHA256SUM "9aa0cc780dec156b8282844833b342ab7cb08c25d2cd9a1869cdd0df31deff48")

set(BZIP2_VERSION "1.0.8")
set(BZIP2_DOWNLOAD_URL "https://gitlab.com/bzip2/bzip2/-/archive/bzip2-${BZIP2_VERSION}/bzip2-bzip2-${BZIP2_VERSION}.tar.gz?ref_type=tags")
set(BZIP2_SHA256SUM "db106b740252669664fd8f3a1c69fe7f689d5cd4b132f82ba82b9afba27627df")

set(BROTLI_VERSION "1.2.1")
set(BROTLI_DOWNLOAD_URL "https://github.com/google/brotli/archive/refs/tags/v${BROTLI_VERSION}.tar.gz")
set(BROTLI_SHA256SUM "98d35d11a78dc453aebb7d10c3748f3f73ff78c8267c0dc697b8631f67bf7e8f")

set(LIBPNG_VERSION "1.6.59")
set(LIBPNG_DOWNLOAD_URL "http://prdownloads.sourceforge.net/libpng/libpng-${LIBPNG_VERSION}.tar.xz?download")
set(LIBPNG_SHA256SUM "d80dd2a38a37f803cb9b6ac7b14bd6e74ddc3b654780a8380bdf93523fdb4389")

set(HARFBUZZ_VERSION "15.1.0")
set(HARFBUZZ_DOWNLOAD_URL "https://github.com/harfbuzz/harfbuzz/releases/download/${HARFBUZZ_VERSION}/harfbuzz-${HARFBUZZ_VERSION}.tar.xz")
set(HARFBUZZ_SHA256SUM "0fc7c565443a5f4be7da65ac5ee5bd737d9b158dc908708b7689a0a5796b0c8d")

set(FREETYPE_TAG "VER-2-15-0")
set(FREETYPE_SHA256SUM "0e0e5635e47cb38b446265262b53a5a5f457958ae55da206ad621020f6d34290")

set(FONTCONFIG_VERSION "2.18.3")
set(FONTCONFIG_DOWNLOAD_URL "https://gitlab.freedesktop.org/fontconfig/fontconfig/-/archive/${FONTCONFIG_VERSION}/fontconfig-${FONTCONFIG_VERSION}.tar.bz2?ref_type=tags")
set(FONTCONFIG_SHA256SUM "8d28f79d2017cbe1fbb7da84b2502c86421b4f45860234d2f4ab5b35564c8d01")

set(mpd_info_screen2_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/main.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/args.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/mpd_client.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/constants.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/helpers.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/signal_handler.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/mpd_display.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/host_prompt.cc
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
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/host_prompt.h
)

add_executable(mpd_info_screen2 ${mpd_info_screen2_SOURCES})
target_compile_features(mpd_info_screen2 PUBLIC cxx_std_23)

add_executable(unittests ${unittest_SOURCES})
target_compile_features(unittests PUBLIC cxx_std_23)

if(DEFINED CROSS_CC AND NOT DEFINED CROSS_AR)
    message(FATAL_ERROR "CROSS_CC defined but not CROSS_AR! (e.g. \"/usr/bin/aarch64-linux-gnu-ar\")")
elseif(NOT DEFINED CROSS_CC AND NOT DEFINED CROSS_AR)
    set(CROSS_AR "/usr/bin/ar")
endif()
if(DEFINED CROSS_CC AND NOT DEFINED CROSS_RANLIB)
    message(FATAL_ERROR "CROSS_CC defined but not CROSS_RANLIB! (e.g. \"/usr/bin/aarch64-linux-gnu-ranlib\")")
elseif(NOT DEFINED CROSS_CC AND NOT DEFINED CROSS_RANLIB)
    set(CROSS_RANLIB "/usr/bin/ranlib")
endif()
if(DEFINED CROSS_CC AND NOT DEFINED CROSS_STRIP)
    message(FATAL_ERROR "CROSS_CC defined but not CROSS_STRIP! (e.g. \"/usr/bin/aarch64-linux-gnu-strip\")")
elseif(NOT DEFINED CROSS_CC AND NOT DEFINED CROSS_STRIP)
    set(CROSS_STRIP "/usr/bin/strip")
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
if(CROSS_VA_COPY_ENABLED)
    message(STATUS "NOTICE: set ac_cv_va_copy=yes for fontconfig")
    set(CROSS_VA_COPY_ENV "ac_cv_va_copy=yes")
elseif(DEFINED CROSS_VA_COPY_ENABLED AND NOT CROSS_VA_COPY_ENABLED)
    message(STATUS "NOTICE: set ac_cv_va_copy=no for fontconfig")
    set(CROSS_VA_COPY_ENV "ac_cv_va_copy=no")
else()
    message(STATUS "NOTICE: When compiling fontconfig, setting ac_cv_va_copy=yes")
    set(CROSS_VA_COPY_ENV "ac_cv_va_copy=yes")
endif()

if(CROSS_COMP_ENABLED)
    add_custom_command(OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/third_party/meson_cross.txt"
        COMMAND mkdir -p "${CMAKE_CURRENT_BINARY_DIR}/third_party"
        COMMAND echo "[binaries]" > "${CMAKE_CURRENT_BINARY_DIR}/third_party/meson_cross.txt"
        COMMAND echo "c = '${CROSS_CC}'" >> "${CMAKE_CURRENT_BINARY_DIR}/third_party/meson_cross.txt"
        COMMAND echo "cpp = '${CROSS_CXX}'" >> "${CMAKE_CURRENT_BINARY_DIR}/third_party/meson_cross.txt"
        COMMAND echo "ar = '${CROSS_AR}'" >> "${CMAKE_CURRENT_BINARY_DIR}/third_party/meson_cross.txt"
        COMMAND echo "ranlib = '${CROSS_RANLIB}'" >> "${CMAKE_CURRENT_BINARY_DIR}/third_party/meson_cross.txt"
        COMMAND echo "strip = '${CROSS_STRIP}'" >> "${CMAKE_CURRENT_BINARY_DIR}/third_party/meson_cross.txt"
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
    COMMAND test -e "${CMAKE_CURRENT_SOURCE_DIR}/../third_party/libffi-${LIBFFI_VERSION}.tar.gz" || curl -L -o "${CMAKE_CURRENT_SOURCE_DIR}/../third_party/libffi-${LIBFFI_VERSION}.tar.gz" "${LIBFFI_DOWNLOAD_URL}"
    COMMAND echo "Checking third_party/libffi-${LIBFFI_VERSION}.tar.gz sha256sum..."
    COMMAND sha256sum "${CMAKE_CURRENT_SOURCE_DIR}/../third_party/libffi-${LIBFFI_VERSION}.tar.gz" | grep "${LIBFFI_SHA256SUM}"
    COMMAND tar -xf "${CMAKE_CURRENT_SOURCE_DIR}/../third_party/libffi-${LIBFFI_VERSION}.tar.gz" -C "${CMAKE_CURRENT_BINARY_DIR}/third_party/native"
    COMMAND cd "${CMAKE_CURRENT_BINARY_DIR}/third_party/native/libffi-${LIBFFI_VERSION}" && ./autogen.sh --enable-static=yes --enable-shared=no "--prefix=${CMAKE_CURRENT_BINARY_DIR}/third_party/native/PREFIX_OUT" && ./configure --enable-static=yes --enable-shared=no "--prefix=${CMAKE_CURRENT_BINARY_DIR}/third_party/native/PREFIX_OUT" --disable-docs
    COMMAND make -C "${CMAKE_CURRENT_BINARY_DIR}/third_party/native/libffi-${LIBFFI_VERSION}" install
    COMMAND test -e "${CMAKE_CURRENT_BINARY_DIR}/third_party/native/PREFIX_OUT/lib64" || ln -sr "${CMAKE_CURRENT_BINARY_DIR}/third_party/native/PREFIX_OUT/lib" "${CMAKE_CURRENT_BINARY_DIR}/third_party/native/PREFIX_OUT/lib64"
    VERBATIM)

add_custom_target(LIBFFINATIVE DEPENDS "${CMAKE_CURRENT_BINARY_DIR}/third_party/native/PREFIX_OUT/lib64/libffi.a")
set_target_properties(LIBFFINATIVE PROPERTIES ADDITIONAL_CLEAN_FILES "${CMAKE_CURRENT_BINARY_DIR}/third_party/native/libffi-${LIBFFI_VERSION};${CMAKE_CURRENT_BINARY_DIR}/third_party/native/PREFIX_OUT")

add_custom_command(OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/third_party/PREFIX_OUT/lib64/libffi.a"
    COMMAND mkdir -p "${CMAKE_CURRENT_BINARY_DIR}/third_party"
    COMMAND test -e "${CMAKE_CURRENT_SOURCE_DIR}/../third_party/libffi-${LIBFFI_VERSION}.tar.gz" || curl -L -o "${CMAKE_CURRENT_SOURCE_DIR}/../third_party/libffi-${LIBFFI_VERSION}.tar.gz" "${LIBFFI_DOWNLOAD_URL}"
    COMMAND echo "Checking third_party/libffi-${LIBFFI_VERSION}.tar.gz sha256sum..."
    COMMAND sha256sum "${CMAKE_CURRENT_SOURCE_DIR}/../third_party/libffi-${LIBFFI_VERSION}.tar.gz" | grep "${LIBFFI_SHA256SUM}"
    COMMAND tar -xf "${CMAKE_CURRENT_SOURCE_DIR}/../third_party/libffi-${LIBFFI_VERSION}.tar.gz" -C "${CMAKE_CURRENT_BINARY_DIR}/third_party"
    COMMAND cd "${CMAKE_CURRENT_BINARY_DIR}/third_party/libffi-${LIBFFI_VERSION}" && CC=${CROSS_CC} CXX=${CROSS_CXX} AR=${CROSS_AR} RANLIB=${CROSS_RANLIB} ./autogen.sh --enable-static=yes --enable-shared=no ${CROSS_HOST_FLAG} "--prefix=${CMAKE_CURRENT_BINARY_DIR}/third_party/PREFIX_OUT" && CC=${CROSS_CC} CXX=${CROSS_CXX} AR=${CROSS_AR} RANLIB=${CROSS_RANLIB} ./configure --enable-static=yes --enable-shared=no ${CROSS_HOST_FLAG} "--prefix=${CMAKE_CURRENT_BINARY_DIR}/third_party/PREFIX_OUT" --disable-docs
    COMMAND make -C "${CMAKE_CURRENT_BINARY_DIR}/third_party/libffi-${LIBFFI_VERSION}" install
    COMMAND test -e "${CMAKE_CURRENT_BINARY_DIR}/third_party/PREFIX_OUT/lib64" || ln -sr "${CMAKE_CURRENT_BINARY_DIR}/third_party/PREFIX_OUT/lib" "${CMAKE_CURRENT_BINARY_DIR}/third_party/PREFIX_OUT/lib64"
    VERBATIM)

add_custom_target(LIBFFI DEPENDS "${CMAKE_CURRENT_BINARY_DIR}/third_party/PREFIX_OUT/lib64/libffi.a")
set_target_properties(LIBFFI PROPERTIES ADDITIONAL_CLEAN_FILES "${CMAKE_CURRENT_BINARY_DIR}/third_party/libffi-${LIBFFI_VERSION};${CMAKE_CURRENT_BINARY_DIR}/third_party/PREFIX_OUT")

add_custom_command(OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/third_party/native/PREFIX_OUT/bin/wayland-scanner"
    COMMAND mkdir -p "${CMAKE_CURRENT_BINARY_DIR}/third_party/native"
    COMMAND mkdir -p "${CMAKE_CURRENT_BINARY_DIR}/third_party/PREFIX_OUT/lib/pkgconfig"
    COMMAND test -e "${CMAKE_CURRENT_SOURCE_DIR}/../third_party/wayland-${WAYLAND_VERSION}.tar.xz" || curl -L -o "${CMAKE_CURRENT_SOURCE_DIR}/../third_party/wayland-${WAYLAND_VERSION}.tar.xz" "${WAYLAND_DOWNLOAD_URL}"
    COMMAND echo "Checking third_party/wayland-${WAYLAND_VERSION}.tar.xz sha256sum..."
    COMMAND sha256sum "${CMAKE_CURRENT_SOURCE_DIR}/../third_party/wayland-${WAYLAND_VERSION}.tar.xz" | grep "${WAYLAND_SHA256SUM}"
    COMMAND tar -xf "${CMAKE_CURRENT_SOURCE_DIR}/../third_party/wayland-${WAYLAND_VERSION}.tar.xz" -C "${CMAKE_CURRENT_BINARY_DIR}/third_party/native"
    COMMAND env "PKG_CONFIG_PATH=${CMAKE_CURRENT_BINARY_DIR}/third_party/native/PREFIX_OUT/lib/pkgconfig" meson setup --prefix "${CMAKE_CURRENT_BINARY_DIR}/third_party/native/PREFIX_OUT" --buildtype release -D documentation=false -D dtd_validation=false --prefer-static --default-library static --default-both-libraries static "${CMAKE_CURRENT_BINARY_DIR}/third_party/native/wayland-${WAYLAND_VERSION}/BUILD" "${CMAKE_CURRENT_BINARY_DIR}/third_party/native/wayland-${WAYLAND_VERSION}"
    COMMAND ninja -C "${CMAKE_CURRENT_BINARY_DIR}/third_party/native/wayland-${WAYLAND_VERSION}/BUILD" install
    COMMAND find "${CMAKE_CURRENT_BINARY_DIR}/third_party/native/PREFIX_OUT" -name wayland-scanner.pc -execdir cp "{}" "${CMAKE_CURRENT_BINARY_DIR}/third_party/PREFIX_OUT/lib/pkgconfig/" "\;"
    COMMAND_EXPAND_LISTS
    VERBATIM)

add_custom_target(WAYLAND_SCANNER DEPENDS "${CMAKE_CURRENT_BINARY_DIR}/third_party/native/PREFIX_OUT/bin/wayland-scanner")
set_target_properties(WAYLAND_SCANNER PROPERTIES ADDITIONAL_CLEAN_FILES "${CMAKE_CURRENT_BINARY_DIR}/third_party/native")

add_dependencies(WAYLAND_SCANNER LIBFFINATIVE)

add_custom_command(OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/third_party/PREFIX_OUT/lib/libwayland-server.a"
    COMMAND mkdir -p "${CMAKE_CURRENT_BINARY_DIR}/third_party"
    COMMAND mkdir -p "${CMAKE_CURRENT_BINARY_DIR}/third_party/PREFIX_OUT/lib/pkgconfig"
    COMMAND test -e "${CMAKE_CURRENT_SOURCE_DIR}/../third_party/wayland-${WAYLAND_VERSION}.tar.xz" || curl -L -o "${CMAKE_CURRENT_SOURCE_DIR}/../third_party/wayland-${WAYLAND_VERSION}.tar.xz" "${WAYLAND_DOWNLOAD_URL}"
    COMMAND echo "Checking third_party/wayland-${WAYLAND_VERSION}.tar.xz sha256sum..."
    COMMAND sha256sum "${CMAKE_CURRENT_SOURCE_DIR}/../third_party/wayland-${WAYLAND_VERSION}.tar.xz" | grep "${WAYLAND_SHA256SUM}"
    COMMAND tar -xf "${CMAKE_CURRENT_SOURCE_DIR}/../third_party/wayland-${WAYLAND_VERSION}.tar.xz" -C "${CMAKE_CURRENT_BINARY_DIR}/third_party"
    COMMAND env "PKG_CONFIG_PATH=${CMAKE_CURRENT_BINARY_DIR}/third_party/PREFIX_OUT/lib/pkgconfig" meson setup --prefix "${CMAKE_CURRENT_BINARY_DIR}/third_party/PREFIX_OUT" --buildtype release -D documentation=false -D dtd_validation=false -D scanner=false --prefer-static --default-library static --default-both-libraries static "$<IF:$<STREQUAL:\"${MESON_CROSS_FILE}\",\"\">,,--native-file>" "$<IF:$<STREQUAL:\"${MESON_CROSS_FILE}\",\"\">,,${MESON_CROSS_FILE}>" "$<IF:$<STREQUAL:\"${MESON_CROSS_FILE}\",\"\">,,--cross-file>" "$<IF:$<STREQUAL:\"${MESON_CROSS_FILE}\",\"\">,,${MESON_CROSS_FILE}>" "${CMAKE_CURRENT_BINARY_DIR}/third_party/wayland-${WAYLAND_VERSION}/BUILD" "${CMAKE_CURRENT_BINARY_DIR}/third_party/wayland-${WAYLAND_VERSION}"
    COMMAND ninja -C "${CMAKE_CURRENT_BINARY_DIR}/third_party/wayland-${WAYLAND_VERSION}/BUILD" install
    COMMAND find "${CMAKE_CURRENT_BINARY_DIR}/third_party/wayland-${WAYLAND_VERSION}/BUILD/src" -name libwayland-server.a -execdir cp "{}" "${CMAKE_CURRENT_BINARY_DIR}/third_party/PREFIX_OUT/lib/" "\;"
    COMMAND find "${CMAKE_CURRENT_BINARY_DIR}/third_party/wayland-${WAYLAND_VERSION}/BUILD/src" -name libwayland-client.a -execdir cp "{}" "${CMAKE_CURRENT_BINARY_DIR}/third_party/PREFIX_OUT/lib/" "\;"
    COMMAND find "${CMAKE_CURRENT_BINARY_DIR}/third_party/wayland-${WAYLAND_VERSION}/BUILD/src" -name libwayland-cursor.a -execdir cp "{}" "${CMAKE_CURRENT_BINARY_DIR}/third_party/PREFIX_OUT/lib/" "\;"
    COMMAND find "${CMAKE_CURRENT_BINARY_DIR}/third_party/wayland-${WAYLAND_VERSION}/BUILD/src" -name libwayland-egl.a -execdir cp "{}" "${CMAKE_CURRENT_BINARY_DIR}/third_party/PREFIX_OUT/lib/" "\;"
    COMMAND find "${CMAKE_CURRENT_BINARY_DIR}/third_party/wayland-${WAYLAND_VERSION}/BUILD" -name wayland-client.pc -execdir cp "{}" "${CMAKE_CURRENT_BINARY_DIR}/third_party/PREFIX_OUT/lib/pkgconfig/" "\;"
    COMMAND find "${CMAKE_CURRENT_BINARY_DIR}/third_party/wayland-${WAYLAND_VERSION}/BUILD" -name wayland-cursor.pc -execdir cp "{}" "${CMAKE_CURRENT_BINARY_DIR}/third_party/PREFIX_OUT/lib/pkgconfig/" "\;"
    COMMAND find "${CMAKE_CURRENT_BINARY_DIR}/third_party/wayland-${WAYLAND_VERSION}/BUILD" -name wayland-egl-backend.pc -execdir cp "{}" "${CMAKE_CURRENT_BINARY_DIR}/third_party/PREFIX_OUT/lib/pkgconfig/" "\;"
    COMMAND find "${CMAKE_CURRENT_BINARY_DIR}/third_party/wayland-${WAYLAND_VERSION}/BUILD" -name wayland-server.pc -execdir cp "{}" "${CMAKE_CURRENT_BINARY_DIR}/third_party/PREFIX_OUT/lib/pkgconfig/" "\;"
    COMMAND_EXPAND_LISTS
    VERBATIM)

add_custom_target(LIBWAYLAND DEPENDS "${CMAKE_CURRENT_BINARY_DIR}/third_party/PREFIX_OUT/lib/libwayland-server.a")
set_target_properties(LIBWAYLAND PROPERTIES ADDITIONAL_CLEAN_FILES "${CMAKE_CURRENT_BINARY_DIR}/third_party/wayland-${WAYLAND_VERSION};${CMAKE_CURRENT_BINARY_DIR}/third_party/PREFIX_OUT")

add_dependencies(LIBWAYLAND WAYLAND_SCANNER)
add_dependencies(LIBWAYLAND LIBFFI)
if(CROSS_COMP_ENABLED)
    add_dependencies(LIBWAYLAND MESON_CROSS)
endif()

add_custom_command(OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/third_party/PREFIX_OUT/include/wayland-protocols"
    COMMAND mkdir -p "${CMAKE_CURRENT_BINARY_DIR}/third_party"
    COMMAND test -e "${CMAKE_CURRENT_SOURCE_DIR}/../third_party/wayland-protocols-${WAYLAND_PROTOCOLS_VERSION}.tar.xz" || curl -L -o "${CMAKE_CURRENT_SOURCE_DIR}/../third_party/wayland-protocols-${WAYLAND_PROTOCOLS_VERSION}.tar.xz" "${WAYLAND_PROTOCOLS_DOWNLOAD_URL}"
    COMMAND echo "Checking third_party/wayland-protocols-${WAYLAND_PROTOCOLS_VERSION}.tar.xz sha256sum..."
    COMMAND sha256sum "${CMAKE_CURRENT_SOURCE_DIR}/../third_party/wayland-protocols-${WAYLAND_PROTOCOLS_VERSION}.tar.xz" | grep "${WAYLAND_PROTOCOLS_SHA256SUM}"
    COMMAND tar -xf "${CMAKE_CURRENT_SOURCE_DIR}/../third_party/wayland-protocols-${WAYLAND_PROTOCOLS_VERSION}.tar.xz" -C "${CMAKE_CURRENT_BINARY_DIR}/third_party"
    COMMAND env "PKG_CONFIG_PATH=${CMAKE_CURRENT_BINARY_DIR}/third_party/PREFIX_OUT/lib/pkgconfig" meson setup --prefix "${CMAKE_CURRENT_BINARY_DIR}/third_party/PREFIX_OUT" --pkg-config-path "${CMAKE_CURRENT_BINARY_DIR}/third_party/PREFIX_OUT/lib/pkgconfig" --buildtype release -D tests=false --prefer-static --default-library static --default-both-libraries static "$<IF:$<STREQUAL:\"${MESON_CROSS_FILE}\",\"\">,,--native-file>" "$<IF:$<STREQUAL:\"${MESON_CROSS_FILE}\",\"\">,,${MESON_CROSS_FILE}>" "$<IF:$<STREQUAL:\"${MESON_CROSS_FILE}\",\"\">,,--cross-file>" "$<IF:$<STREQUAL:\"${MESON_CROSS_FILE}\",\"\">,,${MESON_CROSS_FILE}>" "${CMAKE_CURRENT_BINARY_DIR}/third_party/wayland-protocols-${WAYLAND_PROTOCOLS_VERSION}/BUILD" "${CMAKE_CURRENT_BINARY_DIR}/third_party/wayland-protocols-${WAYLAND_PROTOCOLS_VERSION}"
    COMMAND ninja -C "${CMAKE_CURRENT_BINARY_DIR}/third_party/wayland-protocols-${WAYLAND_PROTOCOLS_VERSION}/BUILD" install
    COMMAND_EXPAND_LISTS
    VERBATIM)

add_custom_target(LIBWAYLAND_PROTOCOLS DEPENDS "${CMAKE_CURRENT_BINARY_DIR}/third_party/PREFIX_OUT/include/wayland-protocols")
set_target_properties(LIBWAYLAND_PROTOCOLS PROPERTIES ADDITIONAL_CLEAN_FILES "${CMAKE_CURRENT_BINARY_DIR}/third_party/wayland-protocols-${WAYLAND_PROTOCOLS_VERSION};${CMAKE_CURRENT_BINARY_DIR}/third_party/PREFIX_OUT")

add_dependencies(LIBWAYLAND_PROTOCOLS LIBWAYLAND)

add_custom_command(OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/third_party/PREFIX_OUT/lib/libxml2.a"
    COMMAND mkdir -p "${CMAKE_CURRENT_BINARY_DIR}/third_party"
    COMMAND test -e "${CMAKE_CURRENT_SOURCE_DIR}/../third_party/libxml2-${LIBXML2_VERSION}.tar.gz" || curl -L -o "${CMAKE_CURRENT_SOURCE_DIR}/../third_party/libxml2-${LIBXML2_VERSION}.tar.gz" "${LIBXML2_DOWNLOAD_URL}"
    COMMAND echo "Checking third_party/libxml2-${LIBXML2_VERSION}.tar.gz sha256sum..."
    COMMAND sha256sum "${CMAKE_CURRENT_SOURCE_DIR}/../third_party/libxml2-${LIBXML2_VERSION}.tar.gz" | grep "${LIBXML2_SHA256SUM}"
    COMMAND tar -xf "${CMAKE_CURRENT_SOURCE_DIR}/../third_party/libxml2-${LIBXML2_VERSION}.tar.gz" -C "${CMAKE_CURRENT_BINARY_DIR}/third_party"
    COMMAND cd "${CMAKE_CURRENT_BINARY_DIR}/third_party/libxml2-${LIBXML2_VERSION}" && CC=${CROSS_CC} CXX=${CROSS_CXX} AR=${CROSS_AR} RANLIB=${CROSS_RANLIB} ./autogen.sh ${CROSS_HOST_FLAG} "--prefix=${CMAKE_CURRENT_BINARY_DIR}/third_party/PREFIX_OUT" --enable-static=yes --enable-shared=no
    COMMAND make -C "${CMAKE_CURRENT_BINARY_DIR}/third_party/libxml2-${LIBXML2_VERSION}" install
    VERBATIM)

add_custom_target(LIBXML2 DEPENDS "${CMAKE_CURRENT_BINARY_DIR}/third_party/PREFIX_OUT/lib/libxml2.a")
set_target_properties(LIBXML2 PROPERTIES ADDITIONAL_CLEAN_FILES "${CMAKE_CURRENT_BINARY_DIR}/third_party/libxml2-${LIBXML2_VERSION};${CMAKE_CURRENT_BINARY_DIR}/third_party/PREFIX_OUT")

add_custom_command(OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/third_party/native/PREFIX_OUT/bin/bison"
    COMMAND mkdir -p "${CMAKE_CURRENT_BINARY_DIR}/third_party/native"
    COMMAND test -e "${CMAKE_CURRENT_SOURCE_DIR}/../third_party/bison-${BISON_VERSION}.tar.xz" || curl -L -o "${CMAKE_CURRENT_SOURCE_DIR}/../third_party/bison-${BISON_VERSION}.tar.xz" "${BISON_DOWNLOAD_URL}"
    COMMAND echo "Checking third_party/bison-${BISON_VERSION}.tar.xz sha256sum..."
    COMMAND sha256sum "${CMAKE_CURRENT_SOURCE_DIR}/../third_party/bison-${BISON_VERSION}.tar.xz" | grep "${BISON_SHA256SUM}"
    COMMAND tar -xf "${CMAKE_CURRENT_SOURCE_DIR}/../third_party/bison-${BISON_VERSION}.tar.xz" -C "${CMAKE_CURRENT_BINARY_DIR}/third_party/native"
    COMMAND cd "${CMAKE_CURRENT_BINARY_DIR}/third_party/native/bison-${BISON_VERSION}" && ./configure --prefix "${CMAKE_CURRENT_BINARY_DIR}/third_party/native/PREFIX_OUT"
    COMMAND make -C "${CMAKE_CURRENT_BINARY_DIR}/third_party/native/bison-${BISON_VERSION}" install
    VERBATIM)

add_custom_target(BISON_NATIVE DEPENDS "${CMAKE_CURRENT_BINARY_DIR}/third_party/native/PREFIX_OUT/bin/bison")
set_target_properties(BISON_NATIVE PROPERTIES ADDITIONAL_CLEAN_FILES "${CMAKE_CURRENT_BINARY_DIR}/third_party/native/bison-${BISON_VERSION};${CMAKE_CURRENT_BINARY_DIR}/third_party/native/PREFIX_OUT")

add_custom_command(OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/third_party/PREFIX_OUT/lib/libxkbcommon.a"
    COMMAND mkdir -p "${CMAKE_CURRENT_BINARY_DIR}/third_party"
    COMMAND test -e "${CMAKE_CURRENT_SOURCE_DIR}/../third_party/xkbcommon-${LIBXKBCOMMON_VERSION}.tar.gz" || curl -L -o "${CMAKE_CURRENT_SOURCE_DIR}/../third_party/xkbcommon-${LIBXKBCOMMON_VERSION}.tar.gz" "${LIBXKBCOMMON_DOWNLOAD_URL}"
    COMMAND echo "Checking third_party/xkbcommon-${LIBXKBCOMMON_VERSION}.tar.gz sha256sum..."
    COMMAND sha256sum "${CMAKE_CURRENT_SOURCE_DIR}/../third_party/xkbcommon-${LIBXKBCOMMON_VERSION}.tar.gz" | grep "${LIBXKBCOMMON_SHA256SUM}"
    COMMAND tar -xf "${CMAKE_CURRENT_SOURCE_DIR}/../third_party/xkbcommon-${LIBXKBCOMMON_VERSION}.tar.gz" -C "${CMAKE_CURRENT_BINARY_DIR}/third_party"
    COMMAND env "PATH=$ENV{PATH}:${CMAKE_CURRENT_BINARY_DIR}/third_party/native/PREFIX_OUT/bin" "PKG_CONFIG_PATH=${CMAKE_CURRENT_BINARY_DIR}/third_party/PREFIX_OUT/lib/pkgconfig" meson setup --prefix "${CMAKE_CURRENT_BINARY_DIR}/third_party/PREFIX_OUT" --pkg-config-path "${CMAKE_CURRENT_BINARY_DIR}/third_party/PREFIX_OUT/lib/pkgconfig:${CMAKE_CURRENT_BINARY_DIR}/third_party/PREFIX_OUT/share/pkgconfig" --buildtype release --prefer-static --default-library static --default-both-libraries static -D enable-x11=false -D enable-bash-completion=false -D enable-tools=false "$<IF:$<STREQUAL:\"${MESON_CROSS_FILE}\",\"\">,,--native-file>" "$<IF:$<STREQUAL:\"${MESON_CROSS_FILE}\",\"\">,,${MESON_CROSS_FILE}>" "$<IF:$<STREQUAL:\"${MESON_CROSS_FILE}\",\"\">,,--cross-file>" "$<IF:$<STREQUAL:\"${MESON_CROSS_FILE}\",\"\">,,${MESON_CROSS_FILE}>" "${CMAKE_CURRENT_BINARY_DIR}/third_party/libxkbcommon-xkbcommon-${LIBXKBCOMMON_VERSION}/BUILD" "${CMAKE_CURRENT_BINARY_DIR}/third_party/libxkbcommon-xkbcommon-${LIBXKBCOMMON_VERSION}"
    COMMAND env "PATH=$ENV{PATH}:${CMAKE_CURRENT_BINARY_DIR}/third_party/native/PREFIX_OUT/bin" ninja -C "${CMAKE_CURRENT_BINARY_DIR}/third_party/libxkbcommon-xkbcommon-${LIBXKBCOMMON_VERSION}/BUILD" install
    COMMAND_EXPAND_LISTS
    VERBATIM)

add_custom_target(LIBXKBCOMMON DEPENDS "${CMAKE_CURRENT_BINARY_DIR}/third_party/PREFIX_OUT/lib/libxkbcommon.a")
set_target_properties(LIBXKBCOMMON PROPERTIES ADDITIONAL_CLEAN_FILES "${CMAKE_CURRENT_BINARY_DIR}/third_party/libxkbcommon-xkbcommon-${LIBXKBCOMMON_VERSION};${CMAKE_CURRENT_BINARY_DIR}/third_party/PREFIX_OUT")

add_dependencies(LIBXKBCOMMON LIBWAYLAND)
add_dependencies(LIBXKBCOMMON LIBWAYLAND_PROTOCOLS)
add_dependencies(LIBXKBCOMMON LIBXML2)
add_dependencies(LIBXKBCOMMON BISON_NATIVE)

add_custom_command(OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/third_party/PREFIX_OUT/share/aclocal/xorg-macros.m4"
    COMMAND mkdir -p "${CMAKE_CURRENT_BINARY_DIR}/third_party"
    COMMAND test -e "${CMAKE_CURRENT_SOURCE_DIR}/../third_party/xorg-util-macros-${XORG_UTIL_MACROS_VERSION}.tar.xz" || curl -L -o "${CMAKE_CURRENT_SOURCE_DIR}/../third_party/xorg-util-macros-${XORG_UTIL_MACROS_VERSION}.tar.xz" "${XORG_UTIL_MACROS_DOWNLOAD_URL}"
    COMMAND echo "Checking third_party/xorg-util-macros-${XORG_UTIL_MACROS_VERSION}.tar.xz sha256sum..."
    COMMAND sha256sum "${CMAKE_CURRENT_SOURCE_DIR}/../third_party/xorg-util-macros-${XORG_UTIL_MACROS_VERSION}.tar.xz" | grep "${XORG_UTIL_MACROS_SHA256SUM}"
    COMMAND tar -xf "${CMAKE_CURRENT_SOURCE_DIR}/../third_party/xorg-util-macros-${XORG_UTIL_MACROS_VERSION}.tar.xz" -C "${CMAKE_CURRENT_BINARY_DIR}/third_party"
    COMMAND cd "${CMAKE_CURRENT_BINARY_DIR}/third_party/util-macros-${XORG_UTIL_MACROS_VERSION}" && CC=${CROSS_CC} CXX=${CROSS_CXX} AR=${CROSS_AR} RANLIB=${CROSS_RANLIB} ./configure ${CROSS_HOST_FLAG} "--prefix=${CMAKE_CURRENT_BINARY_DIR}/third_party/PREFIX_OUT"
    COMMAND make -C "${CMAKE_CURRENT_BINARY_DIR}/third_party/util-macros-${XORG_UTIL_MACROS_VERSION}" install
    VERBATIM)

add_custom_target(XORGUTILMACROS DEPENDS "${CMAKE_CURRENT_BINARY_DIR}/third_party/PREFIX_OUT/share/aclocal/xorg-macros.m4")
set_target_properties(XORGUTILMACROS PROPERTIES ADDITIONAL_CLEAN_FILES "${CMAKE_CURRENT_BINARY_DIR}/third_party/util-macros-${XORG_UTIL_MACROS_VERSION};${CMAKE_CURRENT_BINARY_DIR}/third_party/PREFIX_OUT")

add_custom_command(OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/third_party/PREFIX_OUT/include/X11/Xtrans/Xtrans.h"
    COMMAND mkdir -p "${CMAKE_CURRENT_BINARY_DIR}/third_party"
    COMMAND test -e "${CMAKE_CURRENT_SOURCE_DIR}/../third_party/xtrans-${XTRANS_VERSION}.tar.xz" || curl -L -o "${CMAKE_CURRENT_SOURCE_DIR}/../third_party/xtrans-${XTRANS_VERSION}.tar.xz" "${XTRANS_DOWNLOAD_URL}"
    COMMAND echo "Checking third_party/xtrans-${XTRANS_VERSION}.tar.xz sha256sum..."
    COMMAND sha256sum "${CMAKE_CURRENT_SOURCE_DIR}/../third_party/xtrans-${XTRANS_VERSION}.tar.xz" | grep "${XTRANS_SHA256SUM}"
    COMMAND tar -xf "${CMAKE_CURRENT_SOURCE_DIR}/../third_party/xtrans-${XTRANS_VERSION}.tar.xz" -C "${CMAKE_CURRENT_BINARY_DIR}/third_party"
    COMMAND cd "${CMAKE_CURRENT_BINARY_DIR}/third_party/xtrans-${XTRANS_VERSION}" && CC=${CROSS_CC} CXX=${CROSS_CXX} AR=${CROSS_AR} RANLIB=${CROSS_RANLIB} ./configure ${CROSS_HOST_FLAG} "--prefix=${CMAKE_CURRENT_BINARY_DIR}/third_party/PREFIX_OUT"
    COMMAND make -C "${CMAKE_CURRENT_BINARY_DIR}/third_party/xtrans-${XTRANS_VERSION}" install
    VERBATIM)

add_custom_target(XTRANSFILES DEPENDS "${CMAKE_CURRENT_BINARY_DIR}/third_party/PREFIX_OUT/include/X11/Xtrans/Xtrans.h")
set_target_properties(XTRANSFILES PROPERTIES ADDITIONAL_CLEAN_FILES "${CMAKE_CURRENT_BINARY_DIR}/third_party/xtrans-${XTRANS_VERSION};${CMAKE_CURRENT_BINARY_DIR}/third_party/PREFIX_OUT")

add_custom_command(OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/third_party/PREFIX_OUT/include/X11/Xproto.h"
    COMMAND mkdir -p "${CMAKE_CURRENT_BINARY_DIR}/third_party"
    COMMAND test -e "${CMAKE_CURRENT_SOURCE_DIR}/../third_party/xorgproto-${XORGPROTO_VERSION}.tar.xz" || curl -L -o "${CMAKE_CURRENT_SOURCE_DIR}/../third_party/xorgproto-${XORGPROTO_VERSION}.tar.xz" "${XORGPROTO_DOWNLOAD_URL}"
    COMMAND echo "Checking third_party/xorgproto-${XORGPROTO_VERSION}.tar.xz sha256sum..."
    COMMAND sha256sum "${CMAKE_CURRENT_SOURCE_DIR}/../third_party/xorgproto-${XORGPROTO_VERSION}.tar.xz" | grep "${XORGPROTO_SHA256SUM}"
    COMMAND tar -xf "${CMAKE_CURRENT_SOURCE_DIR}/../third_party/xorgproto-${XORGPROTO_VERSION}.tar.xz" -C "${CMAKE_CURRENT_BINARY_DIR}/third_party"
    COMMAND cd "${CMAKE_CURRENT_BINARY_DIR}/third_party/xorgproto-${XORGPROTO_VERSION}" && CC=${CROSS_CC} CXX=${CROSS_CXX} AR=${CROSS_AR} RANLIB=${CROSS_RANLIB} ./configure ${CROSS_HOST_FLAG} "--prefix=${CMAKE_CURRENT_BINARY_DIR}/third_party/PREFIX_OUT"
    COMMAND make -C "${CMAKE_CURRENT_BINARY_DIR}/third_party/xorgproto-${XORGPROTO_VERSION}" install
    VERBATIM)

add_custom_target(XORGPROTO DEPENDS "${CMAKE_CURRENT_BINARY_DIR}/third_party/PREFIX_OUT/include/X11/Xproto.h")
set_target_properties(XORGPROTO PROPERTIES ADDITIONAL_CLEAN_FILES "${CMAKE_CURRENT_BINARY_DIR}/third_party/xorgproto-${XORGPROTO_VERSION};${CMAKE_CURRENT_BINARY_DIR}/third_party/PREFIX_OUT")

add_custom_command(OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/third_party/PREFIX_OUT/share/xcb/xproto.xml"
    COMMAND mkdir -p "${CMAKE_CURRENT_BINARY_DIR}/third_party"
    COMMAND test -e "${CMAKE_CURRENT_SOURCE_DIR}/../third_party/xcb-proto-${XCB_PROTO_VERSION}.tar.xz" || curl -L -o "${CMAKE_CURRENT_SOURCE_DIR}/../third_party/xcb-proto-${XCB_PROTO_VERSION}.tar.xz" "${XCB_PROTO_DOWNLOAD_URL}"
    COMMAND echo "Checking third_party/xcb-proto-${XCB_PROTO_VERSION}.tar.xz sha256sum..."
    COMMAND sha256sum "${CMAKE_CURRENT_SOURCE_DIR}/../third_party/xcb-proto-${XCB_PROTO_VERSION}.tar.xz" | grep "${XCB_PROTO_SHA256SUM}"
    COMMAND tar -xf "${CMAKE_CURRENT_SOURCE_DIR}/../third_party/xcb-proto-${XCB_PROTO_VERSION}.tar.xz" -C "${CMAKE_CURRENT_BINARY_DIR}/third_party"
    COMMAND cd "${CMAKE_CURRENT_BINARY_DIR}/third_party/xcb-proto-${XCB_PROTO_VERSION}" && CC=${CROSS_CC} CXX=${CROSS_CXX} AR=${CROSS_AR} RANLIB=${CROSS_RANLIB} ./configure ${CROSS_HOST_FLAG} "--prefix=${CMAKE_CURRENT_BINARY_DIR}/third_party/PREFIX_OUT"
    COMMAND make -C "${CMAKE_CURRENT_BINARY_DIR}/third_party/xcb-proto-${XCB_PROTO_VERSION}" install
    VERBATIM)

add_custom_target(XCBPROTO DEPENDS "${CMAKE_CURRENT_BINARY_DIR}/third_party/PREFIX_OUT/share/xcb/xproto.xml")
set_target_properties(XCBPROTO PROPERTIES ADDITIONAL_CLEAN_FILES "${CMAKE_CURRENT_BINARY_DIR}/third_party/xcb-proto-${XCB_PROTO_VERSION};${CMAKE_CURRENT_BIANRY_DIR}/third_party/PREFIX_OUT")

add_custom_command(OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/third_party/PREFIX_OUT/lib/libXau.a"
    COMMAND mkdir -p "${CMAKE_CURRENT_BINARY_DIR}/third_party"
    COMMAND test -e "${CMAKE_CURRENT_SOURCE_DIR}/../third_party/libXau-${LIBXAU_VERSION}.tar.xz" || curl -L -o "${CMAKE_CURRENT_SOURCE_DIR}/../third_party/libXau-${LIBXAU_VERSION}.tar.xz" "${LIBXAU_DOWNLOAD_URL}"
    COMMAND echo "Checking third_party/libXau-${LIBXAU_VERSION}.tar.xz sha256sum..."
    COMMAND sha256sum "${CMAKE_CURRENT_SOURCE_DIR}/../third_party/libXau-${LIBXAU_VERSION}.tar.xz" | grep "${LIBXAU_SHA256SUM}"
    COMMAND tar -xf "${CMAKE_CURRENT_SOURCE_DIR}/../third_party/libXau-${LIBXAU_VERSION}.tar.xz" -C "${CMAKE_CURRENT_BINARY_DIR}/third_party"
    COMMAND cd "${CMAKE_CURRENT_BINARY_DIR}/third_party/libXau-${LIBXAU_VERSION}" && env "PKG_CONFIG_PATH=${CMAKE_CURRENT_BINARY_DIR}/third_party/PREFIX_OUT/share/pkgconfig" CC=${CROSS_CC} CXX=${CROSS_CXX} AR=${CROSS_AR} RANLIB=${CROSS_RANLIB} ./configure ${CROSS_HOST_FLAG} "--prefix=${CMAKE_CURRENT_BINARY_DIR}/third_party/PREFIX_OUT" --enable-static=yes --enable-shared=no
    COMMAND make -C "${CMAKE_CURRENT_BINARY_DIR}/third_party/libXau-${LIBXAU_VERSION}" install
    VERBATIM)

add_custom_target(LIBXAU DEPENDS "${CMAKE_CURRENT_BINARY_DIR}/third_party/PREFIX_OUT/lib/libXau.a")
set_target_properties(LIBXAU PROPERTIES ADDITIONAL_CLEAN_FILES "${CMAKE_CURRENT_BINARY_DIR}/third_party/libXau-${LIBXAU_VERSION};${CMAKE_CURRENT_BINARY_DIR}/third_party/PREFIX_OUT")

add_dependencies(LIBXAU XORGPROTO)

add_custom_command(OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/third_party/PREFIX_OUT/lib/libXdmcp.a"
    COMMAND mkdir -p "${CMAKE_CURRENT_BINARY_DIR}/third_party"
    COMMAND test -e "${CMAKE_CURRENT_SOURCE_DIR}/../third_party/libXdmcp-${LIBXDMCP_VERSION}.tar.xz" || curl -L -o "${CMAKE_CURRENT_SOURCE_DIR}/../third_party/libXdmcp-${LIBXDMCP_VERSION}.tar.xz" "${LIBXDMCP_DOWNLOAD_URL}"
    COMMAND echo "Checking third_party/libXdmcp-${LIBXDMCP_VERSION}.tar.xz sha256sum..."
    COMMAND sha256sum "${CMAKE_CURRENT_SOURCE_DIR}/../third_party/libXdmcp-${LIBXDMCP_VERSION}.tar.xz" | grep "${LIBXDMCP_SHA256SUM}"
    COMMAND tar -xf "${CMAKE_CURRENT_SOURCE_DIR}/../third_party/libXdmcp-${LIBXDMCP_VERSION}.tar.xz" -C "${CMAKE_CURRENT_BINARY_DIR}/third_party"
    COMMAND cd "${CMAKE_CURRENT_BINARY_DIR}/third_party/libXdmcp-${LIBXDMCP_VERSION}" && env "PKG_CONFIG_PATH=${CMAKE_CURRENT_BINARY_DIR}/third_party/PREFIX_OUT/share/pkgconfig:${CMAKE_CURRENT_BINARY_DIR}/third_party/PREFIX_OUT/lib/pkgconfig" CC=${CROSS_CC} CXX=${CROSS_CXX} AR=${CROSS_AR} RANLIB=${CROSS_RANLIB} ./configure ${CROSS_HOST_FLAG} "--prefix=${CMAKE_CURRENT_BINARY_DIR}/third_party/PREFIX_OUT" --enable-static=yes --enable-shared=no
    COMMAND make -C "${CMAKE_CURRENT_BINARY_DIR}/third_party/libXdmcp-${LIBXDMCP_VERSION}" install
    VERBATIM)

add_custom_target(LIBXDMCP DEPENDS "${CMAKE_CURRENT_BINARY_DIR}/third_party/PREFIX_OUT/lib/libXdmcp.a")
set_target_properties(LIBXDMCP PROPERTIES ADDITIONAL_CLEAN_FILES "${CMAKE_CURRENT_BINARY_DIR}/third_party/libXdmcp-${LIBXDMCP_VERSION};${CMAKE_CURRENT_BINARY_DIR}/third_party/PREFIX_OUT")

add_dependencies(LIBXDMCP XORGPROTO)

add_custom_command(OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/third_party/PREFIX_OUT/lib/libxcb-xkb.a"
    COMMAND mkdir -p "${CMAKE_CURRENT_BINARY_DIR}/third_party"
    COMMAND test -e "${CMAKE_CURRENT_SOURCE_DIR}/../third_party/libxcb-${LIBXCB_VERSION}.tar.xz" || curl -L -o "${CMAKE_CURRENT_SOURCE_DIR}/../third_party/libxcb-${LIBXCB_VERSION}.tar.xz" "${LIBXCB_DOWNLOAD_URL}"
    COMMAND echo "Checking third_party/libxcb-${LIBXCB_VERSION}.tar.xz sha256sum..."
    COMMAND sha256sum "${CMAKE_CURRENT_SOURCE_DIR}/../third_party/libxcb-${LIBXCB_VERSION}.tar.xz" | grep "${LIBXCB_SHA256SUM}"
    COMMAND tar -xf "${CMAKE_CURRENT_SOURCE_DIR}/../third_party/libxcb-${LIBXCB_VERSION}.tar.xz" -C "${CMAKE_CURRENT_BINARY_DIR}/third_party"
    COMMAND cd "${CMAKE_CURRENT_BINARY_DIR}/third_party/libxcb-${LIBXCB_VERSION}" && env "PKG_CONFIG_PATH=${CMAKE_CURRENT_BINARY_DIR}/third_party/PREFIX_OUT/share/pkgconfig:${CMAKE_CURRENT_BINARY_DIR}/third_party/PREFIX_OUT/lib/pkgconfig" CC=${CROSS_CC} CXX=${CROSS_CXX} AR=${CROSS_AR} RANLIB=${CROSS_RANLIB} ./configure ${CROSS_HOST_FLAG} "--prefix=${CMAKE_CURRENT_BINARY_DIR}/third_party/PREFIX_OUT" --enable-static=yes --enable-shared=no
    COMMAND make -C "${CMAKE_CURRENT_BINARY_DIR}/third_party/libxcb-${LIBXCB_VERSION}" install
    VERBATIM)

add_custom_target(LIBXCB DEPENDS "${CMAKE_CURRENT_BINARY_DIR}/third_party/PREFIX_OUT/lib/libxcb-xkb.a")
set_target_properties(LIBXCB PROPERTIES ADDITIONAL_CLEAN_FILES "${CMAKE_CURRENT_BINARY_DIR}/third_party/libxcb-${LIBXCB_VERSION};${CMAKE_CURRENT_BINARY_DIR}/third_party/PREFIX_OUT")

add_dependencies(LIBXCB LIBXAU)
add_dependencies(LIBXCB XCBPROTO)

add_custom_command(OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/third_party/PREFIX_OUT/lib/libX11.a"
    COMMAND mkdir -p "${CMAKE_CURRENT_BINARY_DIR}/third_party"
    COMMAND test -e "${CMAKE_CURRENT_SOURCE_DIR}/../third_party/libx11-${LIBX11_VERSION}.tar.gz" || curl -L -o "${CMAKE_CURRENT_SOURCE_DIR}/../third_party/libx11-${LIBX11_VERSION}.tar.gz" "${LIBX11_DOWNLOAD_URL}"
    COMMAND echo "Checking third_party/libx11-${LIBX11_VERSION}.tar.gz sha256sum..."
    COMMAND sha256sum "${CMAKE_CURRENT_SOURCE_DIR}/../third_party/libx11-${LIBX11_VERSION}.tar.gz" | grep "${LIBX11_SHA256SUM}"
    COMMAND tar -xf "${CMAKE_CURRENT_SOURCE_DIR}/../third_party/libx11-${LIBX11_VERSION}.tar.gz" -C "${CMAKE_CURRENT_BINARY_DIR}/third_party"
    COMMAND cd "${CMAKE_CURRENT_BINARY_DIR}/third_party/libX11-${LIBX11_VERSION}" && env "ACLOCAL_PATH=${CMAKE_CURRENT_BINARY_DIR}/third_party/PREFIX_OUT/share/aclocal" "PKG_CONFIG_PATH=${CMAKE_CURRENT_BINARY_DIR}/third_party/PREFIX_OUT/share/pkgconfig:${CMAKE_CURRENT_BINARY_DIR}/third_party/PREFIX_OUT/lib/pkgconfig" CC=${CROSS_CC} CXX=${CROSS_CXX} AR=${CROSS_AR} RANLIB=${CROSS_RANLIB} ./configure ${CROSS_HOST_FLAG} "--prefix=${CMAKE_CURRENT_BINARY_DIR}/third_party/PREFIX_OUT" --enable-static=yes --enable-shared=no
    COMMAND make -C "${CMAKE_CURRENT_BINARY_DIR}/third_party/libX11-${LIBX11_VERSION}" install
    VERBATIM)

add_custom_target(LIBX11 DEPENDS "${CMAKE_CURRENT_BINARY_DIR}/third_party/PREFIX_OUT/lib/libX11.a")
set_target_properties(LIBX11 PROPERTIES ADDITIONAL_CLEAN_FILES "${CMAKE_CURRENT_BINARY_DIR}/third_party/libX11-${LIBX11_VERSION};${CMAKE_CURRENT_BINARY_DIR}/third_party/PREFIX_OUT")

add_dependencies(LIBX11 XORGUTILMACROS)
add_dependencies(LIBX11 XTRANSFILES)
add_dependencies(LIBX11 XORGPROTO)
add_dependencies(LIBX11 XCBPROTO)
add_dependencies(LIBX11 LIBXAU)
add_dependencies(LIBX11 LIBXCB)

add_custom_command(OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/third_party/PREFIX_OUT/lib/libXext.a"
    COMMAND mkdir -p "${CMAKE_CURRENT_BINARY_DIR}/third_party"
    COMMAND test -e "${CMAKE_CURRENT_SOURCE_DIR}/../third_party/libXext-${LIBXEXT_VERSION}.tar.xz" || curl -L -o "${CMAKE_CURRENT_SOURCE_DIR}/../third_party/libXext-${LIBXEXT_VERSION}.tar.xz" "${LIBXEXT_DOWNLOAD_URL}"
    COMMAND echo "Checking third_party/libXext-${LIBXEXT_VERSION}.tar.xz sha256sum..."
    COMMAND sha256sum "${CMAKE_CURRENT_SOURCE_DIR}/../third_party/libXext-${LIBXEXT_VERSION}.tar.xz" | grep "${LIBXEXT_SHA256SUM}"
    COMMAND tar -xf "${CMAKE_CURRENT_SOURCE_DIR}/../third_party/libXext-${LIBXEXT_VERSION}.tar.xz" -C "${CMAKE_CURRENT_BINARY_DIR}/third_party"
    COMMAND cd "${CMAKE_CURRENT_BINARY_DIR}/third_party/libXext-${LIBXEXT_VERSION}" && env "PKG_CONFIG_PATH=${CMAKE_CURRENT_BINARY_DIR}/third_party/PREFIX_OUT/lib/pkgconfig:${CMAKE_CURRENT_BINARY_DIR}/third_party/PREFIX_OUT/share/pkgconfig" CC=${CROSS_CC} CXX=${CROSS_CXX} AR=${CROSS_AR} RANLIB=${CROSS_RANLIB} ./configure ${CROSS_HOST_FLAG} "--prefix=${CMAKE_CURRENT_BINARY_DIR}/third_party/PREFIX_OUT" --enable-static=yes --enable-shared=no
    COMMAND make -C "${CMAKE_CURRENT_BINARY_DIR}/third_party/libXext-${LIBXEXT_VERSION}" install
    VERBATIM)

add_custom_target(LIBXEXT DEPENDS "${CMAKE_CURRENT_BINARY_DIR}/third_party/PREFIX_OUT/lib/libXext.a")
set_target_properties(LIBXEXT PROPERTIES ADDITIONAL_CLEAN_FILES "${CMAKE_CURRENT_BINARY_DIR}/third_party/libXext-${LIBXEXT_VERSION};${CMAKE_CURRENT_BINARY_DIR}/third_party/PREFIX_OUT")

add_dependencies(LIBXEXT LIBX11)

add_custom_command(OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/third_party/PREFIX_OUT/lib/libXrender.a"
    COMMAND mkdir -p "${CMAKE_CURRENT_BINARY_DIR}/third_party"
    COMMAND test -e "${CMAKE_CURRENT_SOURCE_DIR}/../third_party/libXrender-${LIBXRENDER_VERSION}.tar.xz" || curl -L -o "${CMAKE_CURRENT_SOURCE_DIR}/../third_party/libXrender-${LIBXRENDER_VERSION}.tar.xz" "${LIBXRENDER_DOWNLOAD_URL}"
    COMMAND echo "Checking third_party/libXrender-${LIBXRENDER_VERSION}.tar.xz sha256sum..."
    COMMAND sha256sum "${CMAKE_CURRENT_SOURCE_DIR}/../third_party/libXrender-${LIBXRENDER_VERSION}.tar.xz" | grep "${LIBXRENDER_SHA256SUM}"
    COMMAND tar -xf "${CMAKE_CURRENT_SOURCE_DIR}/../third_party/libXrender-${LIBXRENDER_VERSION}.tar.xz" -C "${CMAKE_CURRENT_BINARY_DIR}/third_party"
    COMMAND cd "${CMAKE_CURRENT_BINARY_DIR}/third_party/libXrender-${LIBXRENDER_VERSION}" && env "PKG_CONFIG_PATH=${CMAKE_CURRENT_BINARY_DIR}/third_party/PREFIX_OUT/lib/pkgconfig:${CMAKE_CURRENT_BINARY_DIR}/third_party/PREFIX_OUT/share/pkgconfig" CC=${CROSS_CC} CXX=${CROSS_CXX} AR=${CROSS_AR} RANLIB=${CROSS_RANLIB} ./configure ${CROSS_HOST_FLAG} "--prefix=${CMAKE_CURRENT_BINARY_DIR}/third_party/PREFIX_OUT" --enable-static=yes --enable-shared=no
    COMMAND make -C "${CMAKE_CURRENT_BINARY_DIR}/third_party/libXrender-${LIBXRENDER_VERSION}" install
    VERBATIM)

add_custom_target(LIBXRENDER DEPENDS "${CMAKE_CURRENT_BINARY_DIR}/third_party/PREFIX_OUT/lib/libXrender.a")
set_target_properties(LIBXRENDER PROPERTIES ADDITIONAL_CLEAN_FILES "${CMAKE_CURRENT_BINARY_DIR}/third_party/libXrender-${LIBXRENDER_VERSION};${CMAKE_CURRENT_BINARY_DIR}/third_party/PREFIX_OUT")

add_dependencies(LIBXRENDER LIBX11)

add_custom_command(OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/third_party/PREFIX_OUT/lib/libXrandr.a"
    COMMAND mkdir -p "${CMAKE_CURRENT_BINARY_DIR}/third_party"
    COMMAND test -e "${CMAKE_CURRENT_SOURCE_DIR}/../third_party/libXrandr-${LIBXRANDR_VERSION}.tar.xz" || curl -L -o "${CMAKE_CURRENT_SOURCE_DIR}/../third_party/libXrandr-${LIBXRANDR_VERSION}.tar.xz" "${LIBXRANDR_DOWNLOAD_URL}"
    COMMAND echo "Checking third_party/libXrandr-${LIBXRANDR_VERSION}.tar.xz sha256sum..."
    COMMAND sha256sum "${CMAKE_CURRENT_SOURCE_DIR}/../third_party/libXrandr-${LIBXRANDR_VERSION}.tar.xz" | grep "${LIBXRANDR_SHA256SUM}"
    COMMAND tar -xf "${CMAKE_CURRENT_SOURCE_DIR}/../third_party/libXrandr-${LIBXRANDR_VERSION}.tar.xz" -C "${CMAKE_CURRENT_BINARY_DIR}/third_party"
    COMMAND cd "${CMAKE_CURRENT_BINARY_DIR}/third_party/libXrandr-${LIBXRANDR_VERSION}" && env "PKG_CONFIG_PATH=${CMAKE_CURRENT_BINARY_DIR}/third_party/PREFIX_OUT/lib/pkgconfig:${CMAKE_CURRENT_BINARY_DIR}/third_party/PREFIX_OUT/share/pkgconfig" CC=${CROSS_CC} CXX=${CROSS_CXX} AR=${CROSS_AR} RANLIB=${CROSS_RANLIB} ./configure ${CROSS_HOST_FLAG} "--prefix=${CMAKE_CURRENT_BINARY_DIR}/third_party/PREFIX_OUT" --enable-static=yes --enable-shared=no
    COMMAND make -C "${CMAKE_CURRENT_BINARY_DIR}/third_party/libXrandr-${LIBXRANDR_VERSION}" install
    VERBATIM)

add_custom_target(LIBXRANDR DEPENDS "${CMAKE_CURRENT_BINARY_DIR}/third_party/PREFIX_OUT/lib/libXrandr.a")
set_target_properties(LIBXRANDR PROPERTIES ADDITIONAL_CLEAN_FILES "${CMAKE_CURRENT_BINARY_DIR}/third_party/libXrandr-${LIBXRANDR_VERSION};${CMAKE_CURRENT_BINARY_DIR}/third_party/PREFIX_OUT")

add_dependencies(LIBXRANDR LIBXEXT)
add_dependencies(LIBXRANDR LIBXRENDER)

add_custom_command(OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/third_party/PREFIX_OUT/lib/libXinerama.a"
    COMMAND mkdir -p "${CMAKE_CURRENT_BINARY_DIR}/third_party"
    COMMAND test -e "${CMAKE_CURRENT_SOURCE_DIR}/../third_party/libXinerama-${LIBXINERAMA_VERSION}.tar.xz" || curl -L -o "${CMAKE_CURRENT_SOURCE_DIR}/../third_party/libXinerama-${LIBXINERAMA_VERSION}.tar.xz" "${LIBXINERAMA_DOWNLOAD_URL}"
    COMMAND echo "Checking third_party/libXinerama-${LIBXINERAMA_VERSION}.tar.xz sha256sum..."
    COMMAND sha256sum "${CMAKE_CURRENT_SOURCE_DIR}/../third_party/libXinerama-${LIBXINERAMA_VERSION}.tar.xz" | grep "${LIBXINERAMA_SHA256SUM}"
    COMMAND tar -xf "${CMAKE_CURRENT_SOURCE_DIR}/../third_party/libXinerama-${LIBXINERAMA_VERSION}.tar.xz" -C "${CMAKE_CURRENT_BINARY_DIR}/third_party"
    COMMAND cd "${CMAKE_CURRENT_BINARY_DIR}/third_party/libXinerama-${LIBXINERAMA_VERSION}" && env "PKG_CONFIG_PATH=${CMAKE_CURRENT_BINARY_DIR}/third_party/PREFIX_OUT/lib/pkgconfig:${CMAKE_CURRENT_BINARY_DIR}/third_party/PREFIX_OUT/share/pkgconfig" CC=${CROSS_CC} CXX=${CROSS_CXX} AR=${CROSS_AR} RANLIB=${CROSS_RANLIB} ./configure ${CROSS_HOST_FLAG} "--prefix=${CMAKE_CURRENT_BINARY_DIR}/third_party/PREFIX_OUT" --enable-static=yes --enable-shared=no
    COMMAND make -C "${CMAKE_CURRENT_BINARY_DIR}/third_party/libXinerama-${LIBXINERAMA_VERSION}" install
    VERBATIM)

add_custom_target(LIBXINERAMA DEPENDS "${CMAKE_CURRENT_BINARY_DIR}/third_party/PREFIX_OUT/lib/libXinerama.a")
set_target_properties(LIBXINERAMA PROPERTIES ADDITIONAL_CLEAN_FILES "${CMAKE_CURRENT_BINARY_DIR}/third_party/libXinerama-${LIBXINERAMA_VERSION};${CMAKE_CURRENT_BINARY_DIR}/third_party/PREFIX_OUT")

add_dependencies(LIBXINERAMA LIBX11)
add_dependencies(LIBXINERAMA LIBXEXT)

add_custom_command(OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/third_party/PREFIX_OUT/lib/libXfixes.a"
    COMMAND mkdir -p "${CMAKE_CURRENT_BINARY_DIR}/third_party"
    COMMAND test -e "${CMAKE_CURRENT_SOURCE_DIR}/../third_party/libXfixes-${LIBXFIXES_VERSION}.tar.xz" || curl -L -o "${CMAKE_CURRENT_SOURCE_DIR}/../third_party/libXfixes-${LIBXFIXES_VERSION}.tar.xz" "${LIBXFIXES_DOWNLOAD_URL}"
    COMMAND echo "Checking third_party/libXfixes-${LIBXFIXES_VERSION}.tar.xz sha256sum..."
    COMMAND sha256sum "${CMAKE_CURRENT_SOURCE_DIR}/../third_party/libXfixes-${LIBXFIXES_VERSION}.tar.xz" | grep "${LIBXFIXES_SHA256SUM}"
    COMMAND tar -xf "${CMAKE_CURRENT_SOURCE_DIR}/../third_party/libXfixes-${LIBXFIXES_VERSION}.tar.xz" -C "${CMAKE_CURRENT_BINARY_DIR}/third_party"
    COMMAND cd "${CMAKE_CURRENT_BINARY_DIR}/third_party/libXfixes-${LIBXFIXES_VERSION}" && env "PKG_CONFIG_PATH=${CMAKE_CURRENT_BINARY_DIR}/third_party/PREFIX_OUT/lib/pkgconfig:${CMAKE_CURRENT_BINARY_DIR}/third_party/PREFIX_OUT/share/pkgconfig" CC=${CROSS_CC} CXX=${CROSS_CXX} AR=${CROSS_AR} RANLIB=${CROSS_RANLIB} ./configure ${CROSS_HOST_FLAG} "--prefix=${CMAKE_CURRENT_BINARY_DIR}/third_party/PREFIX_OUT" --enable-static=yes --enable-shared=no
    COMMAND make -C "${CMAKE_CURRENT_BINARY_DIR}/third_party/libXfixes-${LIBXFIXES_VERSION}" install
    VERBATIM)

add_custom_target(LIBXFIXES DEPENDS "${CMAKE_CURRENT_BINARY_DIR}/third_party/PREFIX_OUT/lib/libXfixes.a")
set_target_properties(LIBXFIXES PROPERTIES ADDITIONAL_CLEAN_FILES "${CMAKE_CURRENT_BINARY_DIR}/third_party/libXfixes-${LIBXFIXES_VERSION};${CMAKE_CURRENT_BINARY_DIR}/third_party/PREFIX_OUT")

add_dependencies(LIBXFIXES LIBX11)

add_custom_command(OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/third_party/PREFIX_OUT/lib/libXcursor.a"
    COMMAND mkdir -p "${CMAKE_CURRENT_BINARY_DIR}/third_party"
    COMMAND test -e "${CMAKE_CURRENT_SOURCE_DIR}/../third_party/libXcursor-${LIBXCURSOR_VERSION}.tar.xz" || curl -L -o "${CMAKE_CURRENT_SOURCE_DIR}/../third_party/libXcursor-${LIBXCURSOR_VERSION}.tar.xz" "${LIBXCURSOR_DOWNLOAD_URL}"
    COMMAND echo "Checking third_party/libXcursor-${LIBXCURSOR_VERSION}.tar.xz sha256sum..."
    COMMAND sha256sum "${CMAKE_CURRENT_SOURCE_DIR}/../third_party/libXcursor-${LIBXCURSOR_VERSION}.tar.xz" | grep "${LIBXCURSOR_SHA256SUM}"
    COMMAND tar -xf "${CMAKE_CURRENT_SOURCE_DIR}/../third_party/libXcursor-${LIBXCURSOR_VERSION}.tar.xz" -C "${CMAKE_CURRENT_BINARY_DIR}/third_party"
    COMMAND cd "${CMAKE_CURRENT_BINARY_DIR}/third_party/libXcursor-${LIBXCURSOR_VERSION}" && env "PKG_CONFIG_PATH=${CMAKE_CURRENT_BINARY_DIR}/third_party/PREFIX_OUT/lib/pkgconfig:${CMAKE_CURRENT_BINARY_DIR}/third_party/PREFIX_OUT/share/pkgconfig" CC=${CROSS_CC} CXX=${CROSS_CXX} AR=${CROSS_AR} RANLIB=${CROSS_RANLIB} ./configure ${CROSS_HOST_FLAG} "--prefix=${CMAKE_CURRENT_BINARY_DIR}/third_party/PREFIX_OUT" --enable-static=yes --enable-shared=no
    COMMAND make -C "${CMAKE_CURRENT_BINARY_DIR}/third_party/libXcursor-${LIBXCURSOR_VERSION}" install
    VERBATIM)

add_custom_target(LIBXCURSOR DEPENDS "${CMAKE_CURRENT_BINARY_DIR}/third_party/PREFIX_OUT/lib/libXcursor.a")
set_target_properties(LIBXCURSOR PROPERTIES ADDITIONAL_CLEAN_FILES "${CMAKE_CURRENT_BINARY_DIR}/third_party/libXcursor-${LIBXCURSOR_VERSION};${CMAKE_CURRENT_BINARY_DIR}/third_party/PREFIX_OUT")

add_dependencies(LIBXCURSOR LIBXFIXES)
add_dependencies(LIBXCURSOR LIBXRENDER)

add_custom_command(OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/third_party/PREFIX_OUT/lib/libXi.a"
    COMMAND mkdir -p "${CMAKE_CURRENT_BINARY_DIR}/third_party"
    COMMAND test -e "${CMAKE_CURRENT_SOURCE_DIR}/../third_party/libXi-${LIBXI_VERSION}.tar.xz" || curl -L -o "${CMAKE_CURRENT_SOURCE_DIR}/../third_party/libXi-${LIBXI_VERSION}.tar.xz" "${LIBXI_DOWNLOAD_URL}"
    COMMAND echo "Checking third_party/libXi-${LIBXI_VERSION}.tar.xz sha256sum..."
    COMMAND sha256sum "${CMAKE_CURRENT_SOURCE_DIR}/../third_party/libXi-${LIBXI_VERSION}.tar.xz" | grep "${LIBXI_SHA256SUM}"
    COMMAND tar -xf "${CMAKE_CURRENT_SOURCE_DIR}/../third_party/libXi-${LIBXI_VERSION}.tar.xz" -C "${CMAKE_CURRENT_BINARY_DIR}/third_party"
    COMMAND cd "${CMAKE_CURRENT_BINARY_DIR}/third_party/libXi-${LIBXI_VERSION}" && env "PKG_CONFIG_PATH=${CMAKE_CURRENT_BINARY_DIR}/third_party/PREFIX_OUT/lib/pkgconfig:${CMAKE_CURRENT_BINARY_DIR}/third_party/PREFIX_OUT/share/pkgconfig" CC=${CROSS_CC} CXX=${CROSS_CXX} AR=${CROSS_AR} RANLIB=${CROSS_RANLIB} ./configure ${CROSS_HOST_FLAG} "--prefix=${CMAKE_CURRENT_BINARY_DIR}/third_party/PREFIX_OUT" --enable-static=yes --enable-shared=no --enable-docs=no --disable-malloc0returnsnull
    COMMAND make -C "${CMAKE_CURRENT_BINARY_DIR}/third_party/libXi-${LIBXI_VERSION}" install
    VERBATIM)

add_custom_target(LIBXI DEPENDS "${CMAKE_CURRENT_BINARY_DIR}/third_party/PREFIX_OUT/lib/libXi.a")
set_target_properties(LIBXI PROPERTIES ADDITIONAL_CLEAN_FILES "${CMAKE_CURRENT_BINARY_DIR}/third_party/libXi-${LIBXI_VERSION};${CMAKE_CURRENT_BINARY_DIR}/third_party/PREFIX_OUT")

add_dependencies(LIBXI LIBX11)
add_dependencies(LIBXI LIBXEXT)
add_dependencies(LIBXI LIBXFIXES)

add_custom_command(OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/third_party/raylib_BUILD/raylib/libraylib.a"
    COMMAND mkdir -p "${CMAKE_CURRENT_BINARY_DIR}/third_party"
    COMMAND test -e "${CMAKE_CURRENT_SOURCE_DIR}/../third_party/raylib-${RAYLIB_VERSION}.tar.gz" || curl -L -o "${CMAKE_CURRENT_SOURCE_DIR}/../third_party/raylib-${RAYLIB_VERSION}.tar.gz" "${RAYLIB_DOWNLOAD_URL}"
    COMMAND echo "Checking third_party/raylib-${RAYLIB_VERSION}.tar.gz sha256sum..."
    COMMAND sha256sum "${CMAKE_CURRENT_SOURCE_DIR}/../third_party/raylib-${RAYLIB_VERSION}.tar.gz" | grep "${RAYLIB_SHA256SUM}"
    COMMAND tar -xf "${CMAKE_CURRENT_SOURCE_DIR}/../third_party/raylib-${RAYLIB_VERSION}.tar.gz" -C "${CMAKE_CURRENT_BINARY_DIR}/third_party"
    COMMAND cd "${CMAKE_CURRENT_BINARY_DIR}" && patch -p1 < "${CMAKE_CURRENT_SOURCE_DIR}/../third_party/raylib-disable-busy-wait-loop.patch"
    COMMAND cd "${CMAKE_CURRENT_BINARY_DIR}" && patch -p1 < "${CMAKE_CURRENT_SOURCE_DIR}/../third_party/raylib-enable-jpg.patch"
    COMMAND test -e "${CMAKE_CURRENT_SOURCE_DIR}/../third_party/glfw-${GLFW_VERSION}.tar.gz" || curl -L -o "${CMAKE_CURRENT_SOURCE_DIR}/../third_party/glfw-${GLFW_VERSION}.tar.gz" "${GLFW_DOWNLOAD_URL}"
    COMMAND echo "Checking third_party/glfw-${GLFW_VERSION}.tar.gz sha256sum..."
    COMMAND sha256sum "${CMAKE_CURRENT_SOURCE_DIR}/../third_party/glfw-${GLFW_VERSION}.tar.gz" | grep "${GLFW_SHA256SUM}"
    COMMAND tar -xf "${CMAKE_CURRENT_SOURCE_DIR}/../third_party/glfw-${GLFW_VERSION}.tar.gz" -C "${CMAKE_CURRENT_BINARY_DIR}/third_party"
    COMMAND sed -i -e "/^    add_subdirectory(external\\/glfw)/cadd_subdirectory(${CMAKE_CURRENT_BINARY_DIR}/third_party/glfw-${GLFW_VERSION} ${CMAKE_CURRENT_BINARY_DIR}/third_party/glfw-${GLFW_VERSION}/BUILD_DIR)" "${CMAKE_CURRENT_BINARY_DIR}/third_party/raylib-${RAYLIB_VERSION}/cmake/GlfwImport.cmake"
    COMMAND env "CMAKE_PREFIX_PATH=${CMAKE_CURRENT_BINARY_DIR}/third_party/PREFIX_OUT" CC=${CROSS_CC} CXX=${CROSS_CXX} AR=${CROSS_AR} RANLIB=${CROSS_RANLIB} cmake -S "${CMAKE_CURRENT_BINARY_DIR}/third_party/raylib-${RAYLIB_VERSION}" -B "${CMAKE_CURRENT_BINARY_DIR}/third_party/raylib_BUILD" -DCMAKE_BUILD_TYPE=Release -DGLFW_BUILD_WAYLAND=ON -DBUILD_EXAMPLES=OFF "-DWAYLAND_SCANNER_EXECUTABLE=${CMAKE_CURRENT_BINARY_DIR}/third_party/native/PREFIX_OUT/bin/wayland-scanner" "-DOPENGL_INCLUDE_DIR=${CMAKE_CURRENT_BINARY_DIR}/third_party/PREFIX_OUT/include"
    COMMAND make -C "${CMAKE_CURRENT_BINARY_DIR}/third_party/raylib_BUILD" raylib
    VERBATIM)

add_custom_target(LIBRAYLIB DEPENDS "${CMAKE_CURRENT_BINARY_DIR}/third_party/raylib_BUILD/raylib/libraylib.a")
set_target_properties(LIBRAYLIB PROPERTIES ADDITIONAL_CLEAN_FILES "${CMAKE_CURRENT_BINARY_DIR}/third_party/raylib-${RAYLIB_VERSION};${CMAKE_CURRENT_BINARY_DIR}/third_party/raylib_BUILD;${CMAKE_CURRENT_BINARY_DIR}/third_party/glfw-${GLFW_VERSION}")

add_dependencies(LIBRAYLIB LIBXKBCOMMON)
add_dependencies(LIBRAYLIB LIBX11)
add_dependencies(LIBRAYLIB LIBXRANDR)
add_dependencies(LIBRAYLIB LIBXINERAMA)
add_dependencies(LIBRAYLIB LIBXCURSOR)
add_dependencies(LIBRAYLIB LIBXI)
add_dependencies(LIBRAYLIB LIBXDMCP)

add_dependencies(mpd_info_screen2 LIBRAYLIB)
add_dependencies(unittests LIBRAYLIB)

add_custom_command(OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/third_party/expat-${EXPAT_VERSION}/lib/.libs/libexpat.a"
    COMMAND mkdir -p "${CMAKE_CURRENT_BINARY_DIR}/third_party"
    COMMAND test -e "${CMAKE_CURRENT_SOURCE_DIR}/../third_party/expat-${EXPAT_VERSION}.tar.lz" || curl -o "${CMAKE_CURRENT_SOURCE_DIR}/../third_party/expat-${EXPAT_VERSION}.tar.lz" -L "${EXPAT_DOWNLOAD_URL}"
    COMMAND echo "Checking third_party/expat-${EXPAT_VERSION}.tar.lz sha256sum..."
    COMMAND sha256sum "${CMAKE_CURRENT_SOURCE_DIR}/../third_party/expat-${EXPAT_VERSION}.tar.lz" | grep "${EXPAT_SHA256SUM}"
    COMMAND tar -xf "${CMAKE_CURRENT_SOURCE_DIR}/../third_party/expat-${EXPAT_VERSION}.tar.lz" -C "${CMAKE_CURRENT_BINARY_DIR}/third_party"
    COMMAND cd "${CMAKE_CURRENT_BINARY_DIR}/third_party/expat-${EXPAT_VERSION}" && CC=${CROSS_CC} CXX=${CROSS_CXX} AR=${CROSS_AR} RANLIB=${CROSS_RANLIB} ./buildconf.sh && CC=${CROSS_CC} CXX=${CROSS_CXX} AR=${CROSS_AR} RANLIB=${CROSS_RANLIB} ./configure --prefix=/usr --sysconfdir=/etc --mandir=/usr/share/man --enable-static=yes --enable-shared=no ${CROSS_HOST_FLAG}
    COMMAND make -C "${CMAKE_CURRENT_BINARY_DIR}/third_party/expat-${EXPAT_VERSION}"
    VERBATIM)

add_custom_target(LIBEXPAT DEPENDS "${CMAKE_CURRENT_BINARY_DIR}/third_party/expat-${EXPAT_VERSION}/lib/.libs/libexpat.a")
set_target_properties(LIBEXPAT PROPERTIES ADDITIONAL_CLEAN_FILES "${CMAKE_CURRENT_BINARY_DIR}/third_party/expat-${EXPAT_VERSION}")

add_custom_command(OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/third_party/zlib-${ZLIB_VERSION}/libz.a"
    COMMAND mkdir -p "${CMAKE_CURRENT_BINARY_DIR}/third_party"
    COMMAND test -e "${CMAKE_CURRENT_SOURCE_DIR}/../third_party/zlib-${ZLIB_VERSION}.tar.xz" || curl -o "${CMAKE_CURRENT_SOURCE_DIR}/../third_party/zlib-${ZLIB_VERSION}.tar.xz" -L "${ZLIB_DOWNLOAD_URL}"
    COMMAND echo "Checking third_party/zlib-${ZLIB_VERSION}.tar.xz sha256sum..."
    COMMAND sha256sum "${CMAKE_CURRENT_SOURCE_DIR}/../third_party/zlib-${ZLIB_VERSION}.tar.xz" | grep "${ZLIB_SHA256SUM}"
    COMMAND tar -xf "${CMAKE_CURRENT_SOURCE_DIR}/../third_party/zlib-${ZLIB_VERSION}.tar.xz" -C "${CMAKE_CURRENT_BINARY_DIR}/third_party"
    COMMAND cd "${CMAKE_CURRENT_BINARY_DIR}/third_party/zlib-${ZLIB_VERSION}" && CC=${CROSS_CC} CXX=${CROSS_CXX} AR=${CROSS_AR} RANLIB=${CROSS_RANLIB} ./configure --prefix=/usr --static
    COMMAND make -C "${CMAKE_CURRENT_BINARY_DIR}/third_party/zlib-${ZLIB_VERSION}"
    VERBATIM)

add_custom_target(LIBZLIB DEPENDS "${CMAKE_CURRENT_BINARY_DIR}/third_party/zlib-${ZLIB_VERSION}/libz.a")
set_target_properties(LIBZLIB PROPERTIES ADDITIONAL_CLEAN_FILES "${CMAKE_CURRENT_BINARY_DIR}/third_party/zlib-${ZLIB_VERSION}")

add_custom_command(OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/third_party/gzip-${GZIP_VERSION}/lib/libgzip.a"
    COMMAND mkdir -p "${CMAKE_CURRENT_BINARY_DIR}/third_party"
    COMMAND test -e "${CMAKE_CURRENT_SOURCE_DIR}/../third_party/gzip-${GZIP_VERSION}.tar.xz" || curl -o "${CMAKE_CURRENT_SOURCE_DIR}/../third_party/gzip-${GZIP_VERSION}.tar.xz" -L "${GZIP_DOWNLOAD_URL}"
    COMMAND echo "Checking third_party/gzip-${GZIP_VERSION}.tar.xz sha256sum..."
    COMMAND sha256sum "${CMAKE_CURRENT_SOURCE_DIR}/../third_party/gzip-${GZIP_VERSION}.tar.xz" | grep "${GZIP_SHA256SUM}"
    COMMAND tar -xf "${CMAKE_CURRENT_SOURCE_DIR}/../third_party/gzip-${GZIP_VERSION}.tar.xz" -C "${CMAKE_CURRENT_BINARY_DIR}/third_party"
    COMMAND echo "Checking third_party/gzip-${GZIP_VERSION}.patch sha256sum..."
    COMMAND sha256sum "${CMAKE_CURRENT_SOURCE_DIR}/../third_party/gzip-${GZIP_VERSION}.patch" | grep b592e0f7c32653e583b4b155eb9cb0f86b84b9ca5f4b8d7efe91db178e4d8f88
    COMMAND patch -d "${CMAKE_CURRENT_BINARY_DIR}/third_party/gzip-${GZIP_VERSION}" -p1 < "${CMAKE_CURRENT_SOURCE_DIR}/../third_party/gzip-${GZIP_VERSION}.patch"
    COMMAND cd "${CMAKE_CURRENT_BINARY_DIR}/third_party/gzip-${GZIP_VERSION}" && CC=${CROSS_CC} CXX=${CROSS_CXX} AR=${CROSS_AR} RANLIB=${CROSS_RANLIB} ./configure --prefix=/usr ${CROSS_HOST_FLAG}
    COMMAND make -C "${CMAKE_CURRENT_BINARY_DIR}/third_party/gzip-${GZIP_VERSION}"
    VERBATIM)

add_custom_target(LIBGZIP DEPENDS "${CMAKE_CURRENT_BINARY_DIR}/third_party/gzip-${GZIP_VERSION}/lib/libgzip.a")
set_target_properties(LIBGZIP PROPERTIES ADDITIONAL_CLEAN_FILES "${CMAKE_CURRENT_BINARY_DIR}/third_party/gzip-${GZIP_VERSION}")

add_custom_command(OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/third_party/bzip2-bzip2-${BZIP2_VERSION}/libbz2.a"
    COMMAND mkdir -p "${CMAKE_CURRENT_BINARY_DIR}/third_party"
    COMMAND test -e "${CMAKE_CURRENT_SOURCE_DIR}/../third_party/bzip2-bzip2-${BZIP2_VERSION}.tar.gz" || curl -o "${CMAKE_CURRENT_SOURCE_DIR}/../third_party/bzip2-bzip2-${BZIP2_VERSION}.tar.gz" -L "${BZIP2_DOWNLOAD_URL}"
    COMMAND echo "Checking third_party/bzip2-bzip2-${BZIP2_VERSION}.tar.gz sha256sum..."
    COMMAND sha256sum "${CMAKE_CURRENT_SOURCE_DIR}/../third_party/bzip2-bzip2-${BZIP2_VERSION}.tar.gz" | grep "${BZIP2_SHA256SUM}"
    COMMAND tar -xf "${CMAKE_CURRENT_SOURCE_DIR}/../third_party/bzip2-bzip2-${BZIP2_VERSION}.tar.gz" -C "${CMAKE_CURRENT_BINARY_DIR}/third_party"
    COMMAND sed -i -e "/^CC=/cCC=${CROSS_CC}" -e "/^AR=/cAR=${CROSS_AR}" -e "/^RANLIB=/cRANLIB=${CROSS_RANLIB}" "${CMAKE_CURRENT_BINARY_DIR}/third_party/bzip2-bzip2-${BZIP2_VERSION}/Makefile"
    COMMAND CC=${CROSS_CC} CXX=${CROSS_CXX} AR=${CROSS_AR} RANLIB=${CROSS_RANLIB} make -C "${CMAKE_CURRENT_BINARY_DIR}/third_party/bzip2-bzip2-${BZIP2_VERSION}" libbz2.a
    VERBATIM)

add_custom_target(LIBBZIP2 DEPENDS "${CMAKE_CURRENT_BINARY_DIR}/third_party/bzip2-bzip2-${BZIP2_VERSION}/libbz2.a")
set_target_properties(LIBBZIP2 PROPERTIES ADDITIONAL_CLEAN_FILES "${CMAKE_CURRENT_BINARY_DIR}/third_party/bzip2-bzip2-${BZIP2_VERSION}")

add_custom_command(
    OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/third_party/brotli-${BROTLI_VERSION}/BUILD/libbrotlicommon.a"
           "${CMAKE_CURRENT_BINARY_DIR}/third_party/brotli-${BROTLI_VERSION}/BUILD/libbrotlidec.a"
           "${CMAKE_CURRENT_BINARY_DIR}/third_party/brotli-${BROTLI_VERSION}/BUILD/libbrotlienc.a"
    COMMAND mkdir -p "${CMAKE_CURRENT_BINARY_DIR}/third_party"
    COMMAND test -e "${CMAKE_CURRENT_SOURCE_DIR}/../third_party/brotli-${BROTLI_VERSION}.tar.gz" || curl -o "${CMAKE_CURRENT_SOURCE_DIR}/../third_party/brotli-${BROTLI_VERSION}.tar.gz" -L "${BROTLI_DOWNLOAD_URL}"
    COMMAND echo "Checking third_party/brotli-${BROTLI_VERSION}.tar.gz sha256sum..."
    COMMAND sha256sum "${CMAKE_CURRENT_SOURCE_DIR}/../third_party/brotli-${BROTLI_VERSION}.tar.gz" | grep "${BROTLI_SHA256SUM}"
    COMMAND tar -xf "${CMAKE_CURRENT_SOURCE_DIR}/../third_party/brotli-${BROTLI_VERSION}.tar.gz" -C "${CMAKE_CURRENT_BINARY_DIR}/third_party"
    COMMAND CC=${CROSS_CC} CXX=${CROSS_CXX} AR=${CROSS_AR} RANLIB=${CROSS_RANLIB} cmake -S "${CMAKE_CURRENT_BINARY_DIR}/third_party/brotli-${BROTLI_VERSION}" -B "${CMAKE_CURRENT_BINARY_DIR}/third_party/brotli-${BROTLI_VERSION}/BUILD" -DCMAKE_BUILD_TYPE=Release -DBUILD_SHARED_LIBS=Off
    COMMAND make -C "${CMAKE_CURRENT_BINARY_DIR}/third_party/brotli-${BROTLI_VERSION}/BUILD"
    VERBATIM)

add_custom_target(LIBBROTLI
    DEPENDS "${CMAKE_CURRENT_BINARY_DIR}/third_party/brotli-${BROTLI_VERSION}/BUILD/libbrotlicommon.a"
            "${CMAKE_CURRENT_BINARY_DIR}/third_party/brotli-${BROTLI_VERSION}/BUILD/libbrotlidec.a"
            "${CMAKE_CURRENT_BINARY_DIR}/third_party/brotli-${BROTLI_VERSION}/BUILD/libbrotlienc.a")
set_target_properties(LIBBROTLI PROPERTIES ADDITIONAL_CLEAN_FILES "${CMAKE_CURRENT_BINARY_DIR}/third_party/brotli-${BROTLI_VERSION}")

add_custom_command(OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/third_party/libpng-${LIBPNG_VERSION}/.libs/libpng16.a"
    COMMAND mkdir -p "${CMAKE_CURRENT_BINARY_DIR}/third_party"
    COMMAND test -e "${CMAKE_CURRENT_SOURCE_DIR}/../third_party/libpng-${LIBPNG_VERSION}.tar.xz" || curl -o "${CMAKE_CURRENT_SOURCE_DIR}/../third_party/libpng-${LIBPNG_VERSION}.tar.xz" -L "${LIBPNG_DOWNLOAD_URL}"
    COMMAND echo "Checking third_party/libpng-${LIBPNG_VERSION}.tar.xz sha256sum..."
    COMMAND sha256sum "${CMAKE_CURRENT_SOURCE_DIR}/../third_party/libpng-${LIBPNG_VERSION}.tar.xz" | grep "${LIBPNG_SHA256SUM}"
    COMMAND tar -xf "${CMAKE_CURRENT_SOURCE_DIR}/../third_party/libpng-${LIBPNG_VERSION}.tar.xz" -C "${CMAKE_CURRENT_BINARY_DIR}/third_party"
    COMMAND cd "${CMAKE_CURRENT_BINARY_DIR}/third_party/libpng-${LIBPNG_VERSION}" && CC=${CROSS_CC} CXX=${CROSS_CXX} AR=${CROSS_AR} RANLIB=${CROSS_RANLIB} CPPFLAGS=-I${CMAKE_CURRENT_BINARY_DIR}/third_party/zlib-${ZLIB_VERSION} LDFLAGS=-L${CMAKE_CURRENT_BINARY_DIR}/third_party/zlib-${ZLIB_VERSION} ./configure --prefix=/usr --enable-shared=no --enable-static=yes ${CROSS_HOST_FLAG}
    COMMAND make -C "${CMAKE_CURRENT_BINARY_DIR}/third_party/libpng-${LIBPNG_VERSION}"
    VERBATIM)

add_custom_target(LIBPNG DEPENDS "${CMAKE_CURRENT_BINARY_DIR}/third_party/libpng-${LIBPNG_VERSION}/.libs/libpng16.a")
set_target_properties(LIBPNG PROPERTIES ADDITIONAL_CLEAN_FILES "${CMAKE_CURRENT_BINARY_DIR}/third_party/libpng-${LIBPNG_VERSION}")

add_dependencies(LIBPNG LIBZLIB)

add_custom_command(OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/third_party/harfbuzz-${HARFBUZZ_VERSION}/BUILD/libharfbuzz.a"
    COMMAND mkdir -p "${CMAKE_CURRENT_BINARY_DIR}/third_party"
    COMMAND test -e "${CMAKE_CURRENT_SOURCE_DIR}/../third_party/harfbuzz-${HARFBUZZ_VERSION}.tar.xz" || curl -o "${CMAKE_CURRENT_SOURCE_DIR}/../third_party/harfbuzz-${HARFBUZZ_VERSION}.tar.xz" -L "${HARFBUZZ_DOWNLOAD_URL}"
    COMMAND echo "Checking third_party/harfbuzz.${HARFBUZZ_VERSION}.tar.xz sha256sum..."
    COMMAND sha256sum "${CMAKE_CURRENT_SOURCE_DIR}/../third_party/harfbuzz-${HARFBUZZ_VERSION}.tar.xz" | grep "${HARFBUZZ_SHA256SUM}"
    COMMAND tar -xf "${CMAKE_CURRENT_SOURCE_DIR}/../third_party/harfbuzz-${HARFBUZZ_VERSION}.tar.xz" -C "${CMAKE_CURRENT_BINARY_DIR}/third_party"
    COMMAND env "PKG_CONFIG_PATH=${CMAKE_CURRENT_BINARY_DIR}/third_party/PREFIX_OUT/lib/pkgconfig" meson setup -D freetype=disabled -D glib=disabled -D gobject=disabled -D graphite2=disabled -D graphite=disabled -D icu=disabled -D cairo=disabled -D chafa=disabled -D png=disabled -D zlib=disabled -D fontations=disabled -D harfrust=disabled -D kbts=disabled -D icu_builtin=false -D experimental_api=false -D utilities=disabled -D tests=disabled -D benchmark=disabled -D docs=disabled --prefix "${CMAKE_CURRENT_BINARY_DIR}/third_party/PREFIX_OUT" --buildtype release --prefer-static --default-library static --default-both-libraries static "$<IF:$<STREQUAL:\"${MESON_CROSS_FILE}\",\"\">,,--native-file>" "$<IF:$<STREQUAL:\"${MESON_CROSS_FILE}\",\"\">,,${MESON_CROSS_FILE}>" "$<IF:$<STREQUAL:\"${MESON_CROSS_FILE}\",\"\">,,--cross-file>" "$<IF:$<STREQUAL:\"${MESON_CROSS_FILE}\",\"\">,,${MESON_CROSS_FILE}>" "${CMAKE_CURRENT_BINARY_DIR}/third_party/harfbuzz-${HARFBUZZ_VERSION}/BUILD" "${CMAKE_CURRENT_BINARY_DIR}/third_party/harfbuzz-${HARFBUZZ_VERSION}"
    COMMAND ninja -C "${CMAKE_CURRENT_BINARY_DIR}/third_party/harfbuzz-${HARFBUZZ_VERSION}/BUILD" lib
    COMMAND ln -s "${CMAKE_CURRENT_BINARY_DIR}/third_party/harfbuzz-${HARFBUZZ_VERSION}/BUILD/src/libharfbuzz.a" "${CMAKE_CURRENT_BINARY_DIR}/third_party/harfbuzz-${HARFBUZZ_VERSION}/BUILD/libharfbuzz.a"
    COMMAND_EXPAND_LISTS
    VERBATIM)

add_custom_target(LIBHARFBUZZ DEPENDS "${CMAKE_CURRENT_BINARY_DIR}/third_party/harfbuzz-${HARFBUZZ_VERSION}/BUILD/libharfbuzz.a")
set_target_properties(LIBHARFBUZZ PROPERTIES ADDITIONAL_CLEAN_FILES "${CMAKE_CURRENT_BINARY_DIR}/third_party/harfbuzz-${HARFBUZZ_VERSION}")

if(CROSS_COMP_ENABLED)
    add_dependencies(LIBHARFBUZZ MESON_CROSS)
endif()

add_custom_command(OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/third_party/freetype/objs/.libs/libfreetype.a"
    COMMAND mkdir -p "${CMAKE_CURRENT_BINARY_DIR}/third_party"
    COMMAND test -e "${CMAKE_CURRENT_SOURCE_DIR}/../third_party/freetype_repo" || git clone --depth=1 --no-single-branch --mirror "https://github.com/freetype/freetype.git" "${CMAKE_CURRENT_SOURCE_DIR}/../third_party/freetype_repo"
    COMMAND cd "${CMAKE_CURRENT_SOURCE_DIR}/../third_party/freetype_repo" && git fetch
    COMMAND echo "Checking third_party/freetype_repo \"git archive --format=tar ${FREETYPE_TAG}\" sha256sum..."
    COMMAND env "GIT_DIR=${CMAKE_CURRENT_SOURCE_DIR}/../third_party/freetype_repo" git archive --format=tar "${FREETYPE_TAG}" | sha256sum | grep "${FREETYPE_SHA256SUM}"
    COMMAND git clone "file://${CMAKE_CURRENT_SOURCE_DIR}/../third_party/freetype_repo" "${CMAKE_CURRENT_BINARY_DIR}/third_party/freetype"
    COMMAND cd "${CMAKE_CURRENT_BINARY_DIR}/third_party/freetype" && git checkout "${FREETYPE_TAG}"
    COMMAND cd "${CMAKE_CURRENT_BINARY_DIR}/third_party/freetype" && CC=${CROSS_CC} CXX=${CROSS_CXX} AR=${CROSS_AR} RANLIB=${CROSS_RANLIB} ./autogen.sh && env CC=${CROSS_CC} CXX=${CROSS_CXX} AR=${CROSS_AR} RANLIB=${CROSS_RANLIB} CPPFLAGS=-I${CMAKE_CURRENT_BINARY_DIR}/third_party/brotli-${BROTLI_VERSION}/c/include\ -I${CMAKE_CURRENT_BINARY_DIR}/third_party/zlib-${ZLIB_VERSION}\ -I${CMAKE_CURRENT_BINARY_DIR}/third_party/bzip2-bzip2-${BZIP2_VERSION} ./configure --prefix=/usr --sysconfdir=/etc --mandir=/usr/share/man --enable-static=yes --enable-shared=no ${CROSS_HOST_FLAG}
    COMMAND make -C "${CMAKE_CURRENT_BINARY_DIR}/third_party/freetype"
    VERBATIM)

add_custom_target(LIBFREETYPE2 DEPENDS "${CMAKE_CURRENT_BINARY_DIR}/third_party/freetype/objs/.libs/libfreetype.a")
set_target_properties(LIBFREETYPE2 PROPERTIES ADDITIONAL_CLEAN_FILES "${CMAKE_CURRENT_BINARY_DIR}/third_party/freetype")

add_dependencies(LIBFREETYPE2 LIBZLIB)
add_dependencies(LIBFREETYPE2 LIBGZIP)
add_dependencies(LIBFREETYPE2 LIBBZIP2)
add_dependencies(LIBFREETYPE2 LIBBROTLI)
add_dependencies(LIBFREETYPE2 LIBPNG)

add_custom_command(OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/third_party/fontconfig-${FONTCONFIG_VERSION}/src/.libs/libfontconfig.a"
    COMMAND mkdir -p "${CMAKE_CURRENT_BINARY_DIR}/third_party"
    COMMAND test -e "${CMAKE_CURRENT_SOURCE_DIR}/../third_party/fontconfig-${FONTCONFIG_VERSION}.tar.bz2" || curl -o "${CMAKE_CURRENT_SOURCE_DIR}/../third_party/fontconfig-${FONTCONFIG_VERSION}.tar.bz2" -L "${FONTCONFIG_DOWNLOAD_URL}"
    COMMAND echo "Checking third_party/fontconfig-${FONTCONFIG_VERSION}.tar.bz2 sha256sum..."
    COMMAND sha256sum "${CMAKE_CURRENT_SOURCE_DIR}/../third_party/fontconfig-${FONTCONFIG_VERSION}.tar.bz2" | grep "${FONTCONFIG_SHA256SUM}"
    COMMAND tar -xf "${CMAKE_CURRENT_SOURCE_DIR}/../third_party/fontconfig-${FONTCONFIG_VERSION}.tar.bz2" -C "${CMAKE_CURRENT_BINARY_DIR}/third_party"
    COMMAND cd "${CMAKE_CURRENT_BINARY_DIR}/third_party/fontconfig-${FONTCONFIG_VERSION}" && env CC=${CROSS_CC} CXX=${CROSS_CXX} AR=${CROSS_AR} RANLIB=${CROSS_RANLIB} EXPAT_CFLAGS=-I${CMAKE_CURRENT_BINARY_DIR}/third_party/expat-${EXPAT_VERSION}/lib EXPAT_LIBS=-L${CMAKE_CURRENT_BINARY_DIR}/third_party/expat-${EXPAT_VERSION}/lib/.libs\ -lexpat FREETYPE_CFLAGS=-I${CMAKE_CURRENT_BINARY_DIR}/third_party/freetype/include FREETYPE_LIBS=-L${CMAKE_CURRENT_BINARY_DIR}/third_party/freetype/objs/.libs\ -lfreetype LDFLAGS=-L${CMAKE_CURRENT_BINARY_DIR}/third_party/zlib-${ZLIB_VERSION}\ -L${CMAKE_CURRENT_BINARY_DIR}/third_party/bzip2-bzip2-${BZIP2_VERSION}\ -L${CMAKE_CURRENT_BINARY_DIR}/third_party/libpng-${LIBPNG_VERSION}/.libs\ -L${CMAKE_CURRENT_BINARY_DIR}/third_party/brotli-${BROTLI_VERSION}/BUILD\ -L${CMAKE_CURRENT_BINARY_DIR}/third_party/harfbuzz-${HARFBUZZ_VERSION}/BUILD ${CROSS_VA_COPY_ENV} ./autogen.sh --sysconfdir=/etc --prefix=/usr --mandir=/usr/share/man --enable-static=yes --enable-shared=no ${CROSS_HOST_FLAG}
    COMMAND make -C "${CMAKE_CURRENT_BINARY_DIR}/third_party/fontconfig-${FONTCONFIG_VERSION}/fc-lang"
    COMMAND make -C "${CMAKE_CURRENT_BINARY_DIR}/third_party/fontconfig-${FONTCONFIG_VERSION}/fc-case"
    COMMAND make -C "${CMAKE_CURRENT_BINARY_DIR}/third_party/fontconfig-${FONTCONFIG_VERSION}/fc-const"
    COMMAND make -C "${CMAKE_CURRENT_BINARY_DIR}/third_party/fontconfig-${FONTCONFIG_VERSION}/fc-genericfamily"
    COMMAND make -C "${CMAKE_CURRENT_BINARY_DIR}/third_party/fontconfig-${FONTCONFIG_VERSION}/src" fcftalias.h
    COMMAND make -C "${CMAKE_CURRENT_BINARY_DIR}/third_party/fontconfig-${FONTCONFIG_VERSION}/src" fcobjshash.h
    COMMAND make -C "${CMAKE_CURRENT_BINARY_DIR}/third_party/fontconfig-${FONTCONFIG_VERSION}/src" libfontconfig.la
    VERBATIM)

add_custom_target(LIBFONTCONFIG DEPENDS "${CMAKE_CURRENT_BINARY_DIR}/third_party/fontconfig-${FONTCONFIG_VERSION}/src/.libs/libfontconfig.a")
set_target_properties(LIBFONTCONFIG PROPERTIES ADDITIONAL_CLEAN_FILES "${CMAKE_CURRENT_BINARY_DIR}/third_party/fontconfig-${FONTCONFIG_VERSION}")

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

target_compile_options(mpd_info_screen2 PRIVATE -I${CMAKE_CURRENT_BINARY_DIR}/third_party/raylib-${RAYLIB_VERSION}/src -I${CMAKE_CURRENT_BINARY_DIR}/third_party/fontconfig-${FONTCONFIG_VERSION}
    PRIVATE -Wall -Wformat -Wformat=2 -Wconversion -Wimplicit-fallthrough  -Werror=format-security  -U_FORTIFY_SOURCE -D_FORTIFY_SOURCE=3  -D_GLIBCXX_ASSERTIONS  -fstrict-flex-arrays=3  -fstack-clash-protection -fstack-protector-strong  -Wl,-z,nodlopen -Wl,-z,noexecstack  -Wl,-z,relro -Wl,-z,now  -Wl,--as-needed -Wl,--no-copy-dt-needed-entriesa -fPIE -pie $<$<STREQUAL:"Release","${CMAKE_BUILD_TYPE}">:-O2 -DNDEBUG -fno-delete-null-pointer-checks -fno-strict-overflow -fno-strict-aliasing -ftrivial-auto-var-init=zero> $<$<STREQUAL:"Debug","${CMAKE_BUILD_TYPE}">:-Werror -Og -g> $<$<BOOL:${FORCE_DEBUG_FLAG}>:-g>
)
target_link_libraries(mpd_info_screen2 "${CMAKE_CURRENT_BINARY_DIR}/third_party/raylib_BUILD/raylib/libraylib.a" "${CMAKE_CURRENT_BINARY_DIR}/third_party/fontconfig-${FONTCONFIG_VERSION}/src/.libs/libfontconfig.a" "${CMAKE_CURRENT_BINARY_DIR}/third_party/expat-${EXPAT_VERSION}/lib/.libs/libexpat.a" "${CMAKE_CURRENT_BINARY_DIR}/third_party/freetype/objs/.libs/libfreetype.a" "${CMAKE_CURRENT_BINARY_DIR}/third_party/bzip2-bzip2-${BZIP2_VERSION}/libbz2.a" "${CMAKE_CURRENT_BINARY_DIR}/third_party/brotli-${BROTLI_VERSION}/BUILD/libbrotlidec.a" "${CMAKE_CURRENT_BINARY_DIR}/third_party/brotli-${BROTLI_VERSION}/BUILD/libbrotlienc.a" "${CMAKE_CURRENT_BINARY_DIR}/third_party/brotli-${BROTLI_VERSION}/BUILD/libbrotlicommon.a" "${CMAKE_CURRENT_BINARY_DIR}/third_party/libpng-${LIBPNG_VERSION}/.libs/libpng16.a" "${CMAKE_CURRENT_BINARY_DIR}/third_party/zlib-${ZLIB_VERSION}/libz.a" "${CMAKE_CURRENT_BINARY_DIR}/third_party/gzip-${GZIP_VERSION}/lib/libgzip.a" "${CMAKE_CURRENT_BINARY_DIR}/third_party/harfbuzz-${HARFBUZZ_VERSION}/BUILD/libharfbuzz.a" "${CMAKE_CURRENT_BINARY_DIR}/third_party/PREFIX_OUT/lib/libX11.a" "${CMAKE_CURRENT_BINARY_DIR}/third_party/PREFIX_OUT/lib/libX11-xcb.a" "${CMAKE_CURRENT_BINARY_DIR}/third_party/PREFIX_OUT/lib/libxcb.a" "${CMAKE_CURRENT_BINARY_DIR}/third_party/PREFIX_OUT/lib/libXau.a" "${CMAKE_CURRENT_BINARY_DIR}/third_party/PREFIX_OUT/lib/libXdmcp.a" ${EXTERNAL_GLFW_LINKER_LIBS})
target_compile_options(unittests PRIVATE -I${CMAKE_CURRENT_BINARY_DIR}/third_party/raylib-${RAYLIB_VERSION}/src -I${CMAKE_CURRENT_BINARY_DIR}/third_party/fontconfig-${FONTCONFIG_VERSION}
    PRIVATE -Wall -Wformat -Wformat=2 -Wconversion -Wimplicit-fallthrough  -Werror=format-security  -U_FORTIFY_SOURCE -D_FORTIFY_SOURCE=3  -D_GLIBCXX_ASSERTIONS  -fstrict-flex-arrays=3  -fstack-clash-protection -fstack-protector-strong  -Wl,-z,nodlopen -Wl,-z,noexecstack  -Wl,-z,relro -Wl,-z,now  -Wl,--as-needed -Wl,--no-copy-dt-needed-entriesa -fPIE -pie $<$<STREQUAL:"Release","${CMAKE_BUILD_TYPE}">:-O2 -DNDEBUG -fno-delete-null-pointer-checks -fno-strict-overflow -fno-strict-aliasing -ftrivial-auto-var-init=zero> $<$<STREQUAL:"Debug","${CMAKE_BUILD_TYPE}">:-Werror -Og -g> $<$<BOOL:${FORCE_DEBUG_FLAG}>:-g>
)
target_link_libraries(unittests "${CMAKE_CURRENT_BINARY_DIR}/third_party/raylib_BUILD/raylib/libraylib.a" "${CMAKE_CURRENT_BINARY_DIR}/third_party/fontconfig-${FONTCONFIG_VERSION}/src/.libs/libfontconfig.a" "${CMAKE_CURRENT_BINARY_DIR}/third_party/expat-${EXPAT_VERSION}/lib/.libs/libexpat.a" "${CMAKE_CURRENT_BINARY_DIR}/third_party/freetype/objs/.libs/libfreetype.a" "${CMAKE_CURRENT_BINARY_DIR}/third_party/bzip2-bzip2-${BZIP2_VERSION}/libbz2.a" "${CMAKE_CURRENT_BINARY_DIR}/third_party/brotli-${BROTLI_VERSION}/BUILD/libbrotlidec.a" "${CMAKE_CURRENT_BINARY_DIR}/third_party/brotli-${BROTLI_VERSION}/BUILD/libbrotlienc.a" "${CMAKE_CURRENT_BINARY_DIR}/third_party/brotli-${BROTLI_VERSION}/BUILD/libbrotlicommon.a" "${CMAKE_CURRENT_BINARY_DIR}/third_party/libpng-${LIBPNG_VERSION}/.libs/libpng16.a" "${CMAKE_CURRENT_BINARY_DIR}/third_party/zlib-${ZLIB_VERSION}/libz.a" "${CMAKE_CURRENT_BINARY_DIR}/third_party/gzip-${GZIP_VERSION}/lib/libgzip.a" "${CMAKE_CURRENT_BINARY_DIR}/third_party/harfbuzz-${HARFBUZZ_VERSION}/BUILD/libharfbuzz.a" "${CMAKE_CURRENT_BINARY_DIR}/third_party/PREFIX_OUT/lib/libX11.a" "${CMAKE_CURRENT_BINARY_DIR}/third_party/PREFIX_OUT/lib/libX11-xcb.a" "${CMAKE_CURRENT_BINARY_DIR}/third_party/PREFIX_OUT/lib/libxcb.a" "${CMAKE_CURRENT_BINARY_DIR}/third_party/PREFIX_OUT/lib/libXau.a" "${CMAKE_CURRENT_BINARY_DIR}/third_party/PREFIX_OUT/lib/libXdmcp.a" ${EXTERNAL_GLFW_LINKER_LIBS} atomic)

if(EXISTS "/usr/bin/clang-format")
    add_custom_target(CLANG_FORMAT COMMAND /usr/bin/clang-format -i --style=file ${mpd_info_screen2_SOURCES} ${unittests_SOURCES} ${mpd_info_screen2_HEADERS}
        VERBATIM)
    add_dependencies(mpd_info_screen2 CLANG_FORMAT)
    add_dependencies(unittests CLANG_FORMAT)
endif()

find_program(GIT_BINARY NAMES git)
find_program(SH_BINARY NAMES sh)
find_program(ECHO_BINARY NAMES echo)
find_program(ENV_BINARY NAMES env)
find_program(SED_BINARY NAMES sed)

if(GIT_BINARY AND SH_BINARY AND ECHO_BINARY AND ENV_BINARY AND SED_BINARY)
    message("git_version.h enabled")
    add_custom_target(GitVersion
        COMMAND sh -c "echo -n \"#define COM_SEODISPARATE_MPD_INFO_SCREEN2_GIT_VERSION_STRING \\\"\" > ${CMAKE_CURRENT_BINARY_DIR}/git_version.h"
        COMMAND sh -c "env GIT_DIR=\"${CMAKE_CURRENT_SOURCE_DIR}/../.git\" git describe --tags --long >> ${CMAKE_CURRENT_BINARY_DIR}/git_version.h"
        COMMAND sed -i -e "1s/$/\\\"/" "${CMAKE_CURRENT_BINARY_DIR}/git_version.h"
        BYPRODUCTS "${CMAKE_CURRENT_BINARY_DIR}/git_version.h"
        VERBATIM
    )
    set_source_files_properties("${CMAKE_CURRENT_SOURCE_DIR}/../src/main.cc"
        PROPERTIES
            COMPILE_OPTIONS "--include=${CMAKE_CURRENT_BINARY_DIR}/git_version.h"
            OBJECT_DEPENDS "${CMAKE_CURRENT_BINARY_DIR}/git_version.h"
    )
    set(MPD_INFO_SCREEN2_GIT_VERSION TRUE)
else()
    message("git_version.h disabled")
    set(MPD_INFO_SCREEN2_GIT_VERSION FALSE)
endif()

if(MPD_INFO_SCREEN2_EXTRA_VERSION)
    message("Using extra-version ${MPD_INFO_SCREEN2_EXTRA_VERSION}")
    file(GENERATE OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/extra-version-string.h"
        CONTENT "#define COM_SEODISPARATE_MPD_INFO_SCREEN2_EXTRA_VERSION_STRING \"${MPD_INFO_SCREEN2_EXTRA_VERSION}\""
        NEWLINE_STYLE UNIX)
    if(MPD_INFO_SCREEN2_GIT_VERSION)
        set_source_files_properties("${CMAKE_CURRENT_SOURCE_DIR}/../src/main.cc"
            PROPERTIES
                COMPILE_OPTIONS "--include=${CMAKE_CURRENT_BINARY_DIR}/extra-version-string.h;--include=${CMAKE_CURRENT_BINARY_DIR}/git_version.h"
        )
    else()
        set_source_files_properties("${CMAKE_CURRENT_SOURCE_DIR}/../src/main.cc"
            PROPERTIES
                COMPILE_OPTIONS "--include=${CMAKE_CURRENT_BINARY_DIR}/extra-version-string.h"
        )
    endif()
endif()
