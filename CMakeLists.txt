cmake_minimum_required(VERSION 3.31)
project(mpd_info_screen2 CXX)

set(mpd_info_screen2_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/src/args.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/src/mpd_client.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/src/constants.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/src/helpers.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/src/signal_handler.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/src/mpd_display.cc
)

set(unittest_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/test.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/src/args.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/src/mpd_client.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/src/constants.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/src/helpers.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/src/signal_handler.cc
)

set(mpd_info_screen2_HEADERS
    ${CMAKE_CURRENT_SOURCE_DIR}/src/args.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/constants.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/helpers.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/mpd_client.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/signal_handler.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/mpd_display.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/print_helper.h
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
if(NOT DEFINED CROSS_CC)
    set(CROSS_CC "/usr/bin/gcc")
endif()
if(NOT DEFINED CROSS_CXX)
    set(CROSS_CXX "/usr/bin/g++")
endif()
if(DEFINED CROSS_HOST)
    set(CROSS_HOST_FLAG "--host=${CROSS_HOST}")
endif()

add_custom_command(OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/third_party/raylib_BUILD/raylib/libraylib.a"
    COMMAND mkdir -p "${CMAKE_CURRENT_BINARY_DIR}/third_party"
    COMMAND test -e "${CMAKE_CURRENT_SOURCE_DIR}/third_party/raylib-5.5.tar.gz" || curl -L -o "${CMAKE_CURRENT_SOURCE_DIR}/third_party/raylib-5.5.tar.gz" https://github.com/raysan5/raylib/archive/refs/tags/5.5.tar.gz
    COMMAND cd "${CMAKE_CURRENT_SOURCE_DIR}" && sha256sum -c "${CMAKE_CURRENT_SOURCE_DIR}/third_party/raylib-5.5_SHA256SUMS.txt"
    COMMAND tar -xf "${CMAKE_CURRENT_SOURCE_DIR}/third_party/raylib-5.5.tar.gz" -C "${CMAKE_CURRENT_BINARY_DIR}/third_party"
    COMMAND cd "${CMAKE_CURRENT_BINARY_DIR}" && patch -p1 < "${CMAKE_CURRENT_SOURCE_DIR}/third_party/disable-busy-wait-loop.patch"
    COMMAND cd "${CMAKE_CURRENT_BINARY_DIR}" && patch -p1 < "${CMAKE_CURRENT_SOURCE_DIR}/third_party/font-atlas-size-conservative.patch"
    COMMAND test -e "${CMAKE_CURRENT_SOURCE_DIR}/third_party/glfw-3.4.tar.gz" || curl -L -o "${CMAKE_CURRENT_SOURCE_DIR}/third_party/glfw-3.4.tar.gz" "https://github.com/glfw/glfw/archive/refs/tags/3.4.tar.gz"
    COMMAND sha256sum "${CMAKE_CURRENT_SOURCE_DIR}/third_party/glfw-3.4.tar.gz" | grep c038d34200234d071fae9345bc455e4a8f2f544ab60150765d7704e08f3dac01
    COMMAND tar -xf "${CMAKE_CURRENT_SOURCE_DIR}/third_party/glfw-3.4.tar.gz" -C "${CMAKE_CURRENT_BINARY_DIR}/third_party"
    COMMAND sed -i -e "/^    add_subdirectory(external/cadd_subdirectory(${CMAKE_CURRENT_BINARY_DIR}/third_party/glfw-3.4 ${CMAKE_CURRENT_BINARY_DIR}/third_party/glfw-3.4/BUILD_DIR)" "${CMAKE_CURRENT_BINARY_DIR}/third_party/raylib-5.5/cmake/GlfwImport.cmake"
    COMMAND CC=${CROSS_CC} CXX=${CROSS_CXX} AR=${CROSS_AR} RANLIB=${CROSS_RANLIB} cmake -S "${CMAKE_CURRENT_BINARY_DIR}/third_party/raylib-5.5" -B "${CMAKE_CURRENT_BINARY_DIR}/third_party/raylib_BUILD" -DCMAKE_BUILD_TYPE=Release
    COMMAND make -C "${CMAKE_CURRENT_BINARY_DIR}/third_party/raylib_BUILD" raylib
    VERBATIM)

add_custom_target(LIBRAYLIB DEPENDS "${CMAKE_CURRENT_BINARY_DIR}/third_party/raylib_BUILD/raylib/libraylib.a")
set_target_properties(LIBRAYLIB PROPERTIES ADDITIONAL_CLEAN_FILES "${CMAKE_CURRENT_BINARY_DIR}/third_party/raylib-5.5;${CMAKE_CURRENT_BINARY_DIR}/third_party/raylib_BUILD;${CMAKE_CURRENT_BINARY_DIR}/third-party/glfw-3.4")

add_dependencies(mpd_info_screen2 LIBRAYLIB)
add_dependencies(unittests LIBRAYLIB)

add_custom_command(OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/third_party/expat-2.7.4/lib/.libs/libexpat.a"
    COMMAND mkdir -p "${CMAKE_CURRENT_BINARY_DIR}/third_party"
    COMMAND test -e "${CMAKE_CURRENT_SOURCE_DIR}/third_party/expat-2.7.4.tar.gz" || curl -o "${CMAKE_CURRENT_SOURCE_DIR}/third_party/expat-2.7.4.tar.gz" -L "https://github.com/libexpat/libexpat/releases/download/R_2_7_4/expat-2.7.4.tar.gz"
    COMMAND sha256sum "${CMAKE_CURRENT_SOURCE_DIR}/third_party/expat-2.7.4.tar.gz" | grep 461ecc8aa98ab1a68c2db788175665d1a4db640dc05bf0e289b6ea17122144ec
    COMMAND tar -xf "${CMAKE_CURRENT_SOURCE_DIR}/third_party/expat-2.7.4.tar.gz" -C "${CMAKE_CURRENT_BINARY_DIR}/third_party"
    COMMAND cd "${CMAKE_CURRENT_BINARY_DIR}/third_party/expat-2.7.4" && CC=${CROSS_CC} CXX=${CROSS_CXX} AR=${CROSS_AR} RANLIB=${CROSS_RANLIB} ./buildconf.sh && CC=${CROSS_CC} CXX=${CROSS_CXX} AR=${CROSS_AR} RANLIB=${CROSS_RANLIB} ./configure --prefix=/usr --sysconfdir=/etc --mandir=/usr/share/man --enable-static=yes --enable-shared=no ${CROSS_HOST_FLAG}
    COMMAND make -C "${CMAKE_CURRENT_BINARY_DIR}/third_party/expat-2.7.4"
    VERBATIM)

add_custom_target(LIBEXPAT DEPENDS "${CMAKE_CURRENT_BINARY_DIR}/third_party/expat-2.7.4/lib/.libs/libexpat.a")
set_target_properties(LIBEXPAT PROPERTIES ADDITIONAL_CLEAN_FILES "${CMAKE_CURRENT_BINARY_DIR}/third_party/expat-2.7.4")

add_custom_command(OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/third_party/zlib-1.3.1/libz.a"
    COMMAND mkdir -p "${CMAKE_CURRENT_BINARY_DIR}/third_party"
    COMMAND test -e "${CMAKE_CURRENT_SOURCE_DIR}/third_party/zlib-1.3.1.tar.gz" || curl -o "${CMAKE_CURRENT_SOURCE_DIR}/third_party/zlib-1.3.1.tar.gz" -L "https://github.com/madler/zlib/releases/download/v1.3.1/zlib-1.3.1.tar.gz"
    COMMAND sha256sum "${CMAKE_CURRENT_SOURCE_DIR}/third_party/zlib-1.3.1.tar.gz" | grep 9a93b2b7dfdac77ceba5a558a580e74667dd6fede4585b91eefb60f03b72df23
    COMMAND tar -xf "${CMAKE_CURRENT_SOURCE_DIR}/third_party/zlib-1.3.1.tar.gz" -C "${CMAKE_CURRENT_BINARY_DIR}/third_party"
    COMMAND cd "${CMAKE_CURRENT_BINARY_DIR}/third_party/zlib-1.3.1" && CC=${CROSS_CC} CXX=${CROSS_CXX} AR=${CROSS_AR} RANLIB=${CROSS_RANLIB} ./configure --prefix=/usr --static
    COMMAND make -C "${CMAKE_CURRENT_BINARY_DIR}/third_party/zlib-1.3.1"
    VERBATIM)

add_custom_target(LIBZLIB DEPENDS "${CMAKE_CURRENT_BINARY_DIR}/third_party/zlib-1.3.1/libz.a")
set_target_properties(LIBZLIB PROPERTIES ADDITIONAL_CLEAN_FILES "${CMAKE_CURRENT_BINARY_DIR}/third_party/zlib-1.3.1")

add_custom_command(OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/third_party/gzip-1.14/lib/libgzip.a"
    COMMAND mkdir -p "${CMAKE_CURRENT_BINARY_DIR}/third_party"
    COMMAND test -e "${CMAKE_CURRENT_SOURCE_DIR}/third_party/gzip-1.14.tar.gz" || curl -o "${CMAKE_CURRENT_SOURCE_DIR}/third_party/gzip-1.14.tar.gz" -L "https://mirrors.ocf.berkeley.edu/gnu/gzip/gzip-1.14.tar.gz"
    COMMAND sha256sum "${CMAKE_CURRENT_SOURCE_DIR}/third_party/gzip-1.14.tar.gz" | grep 613d6ea44f1248d7370c7ccdeee0dd0017a09e6c39de894b3c6f03f981191c6b
    COMMAND tar -xf "${CMAKE_CURRENT_SOURCE_DIR}/third_party/gzip-1.14.tar.gz" -C "${CMAKE_CURRENT_BINARY_DIR}/third_party"
    COMMAND cd "${CMAKE_CURRENT_BINARY_DIR}/third_party/gzip-1.14" && CC=${CROSS_CC} CXX=${CROSS_CXX} AR=${CROSS_AR} RANLIB=${CROSS_RANLIB} ./configure --prefix=/usr ${CROSS_HOST_FLAG}
    COMMAND make -C "${CMAKE_CURRENT_BINARY_DIR}/third_party/gzip-1.14"
    VERBATIM)

add_custom_target(LIBGZIP DEPENDS "${CMAKE_CURRENT_BINARY_DIR}/third_party/gzip-1.14/lib/libgzip.a")
set_target_properties(LIBGZIP PROPERTIES ADDITIONAL_CLEAN_FILES "${CMAKE_CURRENT_BINARY_DIR}/third_party/gzip-1.14")

add_custom_command(OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/third_party/bzip2-bzip2-1.0.8/libbz2.a"
    COMMAND mkdir -p "${CMAKE_CURRENT_BINARY_DIR}/third_party"
    COMMAND test -e "${CMAKE_CURRENT_SOURCE_DIR}/third_party/bzip2-bzip2-1.0.8.tar.gz" || curl -o "${CMAKE_CURRENT_SOURCE_DIR}/third_party/bzip2-bzip2-1.0.8.tar.gz" -L "https://gitlab.com/bzip2/bzip2/-/archive/bzip2-1.0.8/bzip2-bzip2-1.0.8.tar.gz?ref_type=tags"
    COMMAND sha256sum "${CMAKE_CURRENT_SOURCE_DIR}/third_party/bzip2-bzip2-1.0.8.tar.gz" | grep db106b740252669664fd8f3a1c69fe7f689d5cd4b132f82ba82b9afba27627df
    COMMAND tar -xf "${CMAKE_CURRENT_SOURCE_DIR}/third_party/bzip2-bzip2-1.0.8.tar.gz" -C "${CMAKE_CURRENT_BINARY_DIR}/third_party"
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
    COMMAND test -e "${CMAKE_CURRENT_SOURCE_DIR}/third_party/brotli-1.2.0.tar.gz" || curl -o "${CMAKE_CURRENT_SOURCE_DIR}/third_party/brotli-1.2.0.tar.gz" -L "https://github.com/google/brotli/archive/refs/tags/v1.2.0.tar.gz"
    COMMAND sha256sum "${CMAKE_CURRENT_SOURCE_DIR}/third_party/brotli-1.2.0.tar.gz" | grep 816c96e8e8f193b40151dad7e8ff37b1221d019dbcb9c35cd3fadbfe6477dfec
    COMMAND tar -xf "${CMAKE_CURRENT_SOURCE_DIR}/third_party/brotli-1.2.0.tar.gz" -C "${CMAKE_CURRENT_BINARY_DIR}/third_party"
    COMMAND CC=${CROSS_CC} CXX=${CROSS_CXX} AR=${CROSS_AR} RANLIB=${CROSS_RANLIB} cmake -S "${CMAKE_CURRENT_BINARY_DIR}/third_party/brotli-1.2.0" -B "${CMAKE_CURRENT_BINARY_DIR}/third_party/brotli-1.2.0/BUILD" -DCMAKE_BUILD_TYPE=Release -DBUILD_SHARED_LIBS=Off
    COMMAND make -C "${CMAKE_CURRENT_BINARY_DIR}/third_party/brotli-1.2.0/BUILD"
    VERBATIM)

add_custom_target(LIBBROTLI
    DEPENDS "${CMAKE_CURRENT_BINARY_DIR}/third_party/brotli-1.2.0/BUILD/libbrotlicommon.a"
            "${CMAKE_CURRENT_BINARY_DIR}/third_party/brotli-1.2.0/BUILD/libbrotlidec.a"
            "${CMAKE_CURRENT_BINARY_DIR}/third_party/brotli-1.2.0/BUILD/libbrotlienc.a")
set_target_properties(LIBBROTLI PROPERTIES ADDITIONAL_CLEAN_FILES "${CMAKE_CURRENT_BINARY_DIR}/third_party/brotli-1.2.0")

add_custom_command(OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/third_party/libpng-1.6.55/.libs/libpng16.a"
    COMMAND mkdir -p "${CMAKE_CURRENT_BINARY_DIR}/third_party"
    COMMAND test -e "${CMAKE_CURRENT_SOURCE_DIR}/third_party/libpng-1.6.55.tar.gz" || curl -o "${CMAKE_CURRENT_SOURCE_DIR}/third_party/libpng-1.6.55.tar.gz" -L "http://prdownloads.sourceforge.net/libpng/libpng-1.6.55.tar.gz?download"
    COMMAND sha256sum "${CMAKE_CURRENT_SOURCE_DIR}/third_party/libpng-1.6.55.tar.gz" | grep 4b0abab6d219e95690ebe4db7fc9aa95f4006c83baaa022373c0c8442271283d
    COMMAND tar -xf "${CMAKE_CURRENT_SOURCE_DIR}/third_party/libpng-1.6.55.tar.gz" -C "${CMAKE_CURRENT_BINARY_DIR}/third_party"
    COMMAND cd "${CMAKE_CURRENT_BINARY_DIR}/third_party/libpng-1.6.55" && CC=${CROSS_CC} CXX=${CROSS_CXX} AR=${CROSS_AR} RANLIB=${CROSS_RANLIB} CPPFLAGS=-I${CMAKE_CURRENT_BINARY_DIR}/third_party/zlib-1.3.1 LDFLAGS=-L${CMAKE_CURRENT_BINARY_DIR}/third_party/zlib-1.3.1 ./configure --prefix=/usr --enable-shared=no --enable-static=yes ${CROSS_HOST_FLAG}
    COMMAND make -C "${CMAKE_CURRENT_BINARY_DIR}/third_party/libpng-1.6.55"
    VERBATIM)

add_custom_target(LIBPNG DEPENDS "${CMAKE_CURRENT_BINARY_DIR}/third_party/libpng-1.6.55/.libs/libpng16.a")
set_target_properties(LIBPNG PROPERTIES ADDITIONAL_CLEAN_FILES "${CMAKE_CURRENT_BINARY_DIR}/third_party/libpng-1.6.55")

add_dependencies(LIBPNG LIBZLIB)

add_custom_command(OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/third_party/harfbuzz-12.3.2/BUILD/libharfbuzz.a"
    COMMAND mkdir -p "${CMAKE_CURRENT_BINARY_DIR}/third_party"
    COMMAND test -e "${CMAKE_CURRENT_SOURCE_DIR}/third_party/harfbuzz-12.3.2.tar.gz" || curl -o "${CMAKE_CURRENT_SOURCE_DIR}/third_party/harfbuzz-12.3.2.tar.gz" -L "https://github.com/harfbuzz/harfbuzz/archive/refs/tags/12.3.2.tar.gz"
    COMMAND sha256sum "${CMAKE_CURRENT_SOURCE_DIR}/third_party/harfbuzz-12.3.2.tar.gz" | grep 3ca6621821e19266710ec1a0cb6146fdc832a75828f7c55fea5502c2a810c3c8
    COMMAND tar -xf "${CMAKE_CURRENT_SOURCE_DIR}/third_party/harfbuzz-12.3.2.tar.gz" -C "${CMAKE_CURRENT_BINARY_DIR}/third_party"
    COMMAND CC=${CROSS_CC} CXX=${CROSS_CXX} AR=${CROSS_AR} RANLIB=${CROSS_RANLIB} cmake -S "${CMAKE_CURRENT_BINARY_DIR}/third_party/harfbuzz-12.3.2" -B "${CMAKE_CURRENT_BINARY_DIR}/third_party/harfbuzz-12.3.2/BUILD" -DCMAKE_BUILD_TYPE=Release
    COMMAND make -C "${CMAKE_CURRENT_BINARY_DIR}/third_party/harfbuzz-12.3.2/BUILD" harfbuzz
    VERBATIM)

add_custom_target(LIBHARFBUZZ DEPENDS "${CMAKE_CURRENT_BINARY_DIR}/third_party/harfbuzz-12.3.2/BUILD/libharfbuzz.a")
set_target_properties(LIBHARFBUZZ PROPERTIES ADDITIONAL_CLEAN_FILES "${CMAKE_CURRENT_BINARY_DIR}/third_party/harfbuzz-12.3.2")

add_custom_command(OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/third_party/freetype-2.14.1/objs/.libs/libfreetype.a"
    COMMAND mkdir -p "${CMAKE_CURRENT_BINARY_DIR}/third_party"
    COMMAND test -e "${CMAKE_CURRENT_SOURCE_DIR}/third_party/freetype-2.14.1.tar.gz" || curl -o "${CMAKE_CURRENT_SOURCE_DIR}/third_party/freetype-2.14.1.tar.gz" -L "https://download.savannah.gnu.org/releases/freetype/freetype-2.14.1.tar.gz"
    COMMAND sha256sum "${CMAKE_CURRENT_SOURCE_DIR}/third_party/freetype-2.14.1.tar.gz" | grep 174d9e53402e1bf9ec7277e22ec199ba3e55a6be2c0740cb18c0ee9850fc8c34
    COMMAND tar -xf "${CMAKE_CURRENT_SOURCE_DIR}/third_party/freetype-2.14.1.tar.gz" -C "${CMAKE_CURRENT_BINARY_DIR}/third_party"
    COMMAND cd "${CMAKE_CURRENT_BINARY_DIR}/third_party/freetype-2.14.1" && CC=${CROSS_CC} CXX=${CROSS_CXX} AR=${CROSS_AR} RANLIB=${CROSS_RANLIB} ./autogen.sh && CC=${CROSS_CC} CXX=${CROSS_CXX} AR=${CROSS_AR} RANLIB=${CROSS_RANLIB} "CPPFLAGS=-I${CMAKE_CURRENT_BINARY_DIR}/third_party/brotli-1.2.0/c/include -I${CMAKE_CURRENT_BINARY_DIR}/third_party/zlib-1.3.1 -I${CMAKE_CURRENT_BINARY_DIR}/third_party/bzip2-bzip2-1.0.8" ./configure --prefix=/usr --sysconfdir=/etc --mandir=/usr/share/man --enable-static=yes --enable-shared=no ${CROSS_HOST_FLAG}
    COMMAND make -C "${CMAKE_CURRENT_BINARY_DIR}/third_party/freetype-2.14.1"
    )

add_custom_target(LIBFREETYPE2 DEPENDS "${CMAKE_CURRENT_BINARY_DIR}/third_party/freetype-2.14.1/objs/.libs/libfreetype.a")
set_target_properties(LIBFREETYPE2 PROPERTIES ADDITIONAL_CLEAN_FILES "${CMAKE_CURRENT_BINARY_DIR}/third_party/freetype-2.14.1")

add_dependencies(LIBFREETYPE2 LIBZLIB)
add_dependencies(LIBFREETYPE2 LIBGZIP)
add_dependencies(LIBFREETYPE2 LIBBZIP2)
add_dependencies(LIBFREETYPE2 LIBBROTLI)
add_dependencies(LIBFREETYPE2 LIBPNG)

add_custom_command(OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/third_party/fontconfig-2.17.1/src/.libs/libfontconfig.a"
    COMMAND mkdir -p "${CMAKE_CURRENT_BINARY_DIR}/third_party"
    COMMAND test -e "${CMAKE_CURRENT_SOURCE_DIR}/third_party/fontconfig-2.17.1.tar.gz" || curl -o "${CMAKE_CURRENT_SOURCE_DIR}/third_party/fontconfig-2.17.1.tar.gz" -L "https://gitlab.freedesktop.org/fontconfig/fontconfig/-/archive/2.17.1/fontconfig-2.17.1.tar.gz"
    COMMAND sha256sum "${CMAKE_CURRENT_SOURCE_DIR}/third_party/fontconfig-2.17.1.tar.gz" | grep 82e73b26adad651b236e5f5d4b3074daf8ff0910188808496326bd3449e5261d
    COMMAND tar -xf "${CMAKE_CURRENT_SOURCE_DIR}/third_party/fontconfig-2.17.1.tar.gz" -C "${CMAKE_CURRENT_BINARY_DIR}/third_party"
    COMMAND cd "${CMAKE_CURRENT_BINARY_DIR}" && patch -p1 < "${CMAKE_CURRENT_SOURCE_DIR}/third_party/fontconfig_autogen.patch"
    COMMAND cd "${CMAKE_CURRENT_BINARY_DIR}/third_party/fontconfig-2.17.1" && CC=${CROSS_CC} CXX=${CROSS_CXX} AR=${CROSS_AR} RANLIB=${CROSS_RANLIB} "EXPAT_CFLAGS=-I${CMAKE_CURRENT_BINARY_DIR}/third_party/expat-2.7.4/lib" "EXPAT_LIBS=-L${CMAKE_CURRENT_BINARY_DIR}/third_party/expat-2.7.4/lib/.libs -lexpat" "FREETYPE_CFLAGS=-I${CMAKE_CURRENT_BINARY_DIR}/third_party/freetype-2.14.1/include" "FREETYPE_LIBS=-L${CMAKE_CURRENT_BINARY_DIR}/third_party/freetype-2.14.1/objs/.libs -lfreetype" "LDFLAGS=-L${CMAKE_CURRENT_BINARY_DIR}/third_party/zlib-1.3.1 -L${CMAKE_CURRENT_BINARY_DIR}/third_party/bzip2-bzip2-1.0.8 -L${CMAKE_CURRENT_BINARY_DIR}/third_party/libpng-1.6.55/.libs -L${CMAKE_CURRENT_BINARY_DIR}/third_party/brotli-1.2.0/BUILD -L${CMAKE_CURRENT_BINARY_DIR}/third_party/harfbuzz-12.3.2/BUILD" ./autogen.sh --sysconfdir=/etc --prefix=/usr --mandir=/usr/share/man --enable-static=yes --enable-shared=no ${CROSS_HOST_FLAG}
    COMMAND make -C "${CMAKE_CURRENT_BINARY_DIR}/third_party/fontconfig-2.17.1/fc-lang"
    COMMAND make -C "${CMAKE_CURRENT_BINARY_DIR}/third_party/fontconfig-2.17.1/fc-case"
    COMMAND make -C "${CMAKE_CURRENT_BINARY_DIR}/third_party/fontconfig-2.17.1/src" fcftalias.h
    COMMAND make -C "${CMAKE_CURRENT_BINARY_DIR}/third_party/fontconfig-2.17.1/src" fcobjshash.h
    COMMAND make -C "${CMAKE_CURRENT_BINARY_DIR}/third_party/fontconfig-2.17.1/src" libfontconfig.la
    )

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
target_link_libraries(mpd_info_screen2 "${CMAKE_CURRENT_BINARY_DIR}/third_party/raylib_BUILD/raylib/libraylib.a" "${CMAKE_CURRENT_BINARY_DIR}/third_party/fontconfig-2.17.1/src/.libs/libfontconfig.a" "${CMAKE_CURRENT_BINARY_DIR}/third_party/expat-2.7.4/lib/.libs/libexpat.a" "${CMAKE_CURRENT_BINARY_DIR}/third_party/freetype-2.14.1/objs/.libs/libfreetype.a" "${CMAKE_CURRENT_BINARY_DIR}/third_party/bzip2-bzip2-1.0.8/libbz2.a" "${CMAKE_CURRENT_BINARY_DIR}/third_party/brotli-1.2.0/BUILD/libbrotlidec.a" "${CMAKE_CURRENT_BINARY_DIR}/third_party/brotli-1.2.0/BUILD/libbrotlienc.a" "${CMAKE_CURRENT_BINARY_DIR}/third_party/brotli-1.2.0/BUILD/libbrotlicommon.a" "${CMAKE_CURRENT_BINARY_DIR}/third_party/libpng-1.6.55/.libs/libpng16.a" "${CMAKE_CURRENT_BINARY_DIR}/third_party/zlib-1.3.1/libz.a" "${CMAKE_CURRENT_BINARY_DIR}/third_party/gzip-1.14/lib/libgzip.a" "${CMAKE_CURRENT_BINARY_DIR}/third_party/harfbuzz-12.3.2/BUILD/libharfbuzz.a" ${EXTERNAL_GLFW_LINKER_LIBS})
target_compile_options(unittests PRIVATE -I${CMAKE_CURRENT_BINARY_DIR}/third_party/raylib-5.5/src -I${CMAKE_CURRENT_BINARY_DIR}/third_party/fontconfig-2.17.1
    PRIVATE -Wall -Wformat -Wformat=2 -Wconversion -Wimplicit-fallthrough  -Werror=format-security  -U_FORTIFY_SOURCE -D_FORTIFY_SOURCE=3  -D_GLIBCXX_ASSERTIONS  -fstrict-flex-arrays=3  -fstack-clash-protection -fstack-protector-strong  -Wl,-z,nodlopen -Wl,-z,noexecstack  -Wl,-z,relro -Wl,-z,now  -Wl,--as-needed -Wl,--no-copy-dt-needed-entriesa -fPIE -pie $<$<STREQUAL:"Release","${CMAKE_BUILD_TYPE}">:-O2 -DNDEBUG -fno-delete-null-pointer-checks -fno-strict-overflow -fno-strict-aliasing -ftrivial-auto-var-init=zero> $<$<STREQUAL:"Debug","${CMAKE_BUILD_TYPE}">:-Werror -Og -g> $<$<BOOL:${FORCE_DEBUG_FLAG}>:-g>
)
target_link_libraries(unittests "${CMAKE_CURRENT_BINARY_DIR}/third_party/raylib_BUILD/raylib/libraylib.a" "${CMAKE_CURRENT_BINARY_DIR}/third_party/fontconfig-2.17.1/src/.libs/libfontconfig.a" "${CMAKE_CURRENT_BINARY_DIR}/third_party/expat-2.7.4/lib/.libs/libexpat.a" "${CMAKE_CURRENT_BINARY_DIR}/third_party/freetype-2.14.1/objs/.libs/libfreetype.a" "${CMAKE_CURRENT_BINARY_DIR}/third_party/bzip2-bzip2-1.0.8/libbz2.a" "${CMAKE_CURRENT_BINARY_DIR}/third_party/brotli-1.2.0/BUILD/libbrotlidec.a" "${CMAKE_CURRENT_BINARY_DIR}/third_party/brotli-1.2.0/BUILD/libbrotlienc.a" "${CMAKE_CURRENT_BINARY_DIR}/third_party/brotli-1.2.0/BUILD/libbrotlicommon.a" "${CMAKE_CURRENT_BINARY_DIR}/third_party/libpng-1.6.55/.libs/libpng16.a" "${CMAKE_CURRENT_BINARY_DIR}/third_party/zlib-1.3.1/libz.a" "${CMAKE_CURRENT_BINARY_DIR}/third_party/gzip-1.14/lib/libgzip.a" "${CMAKE_CURRENT_BINARY_DIR}/third_party/harfbuzz-12.3.2/BUILD/libharfbuzz.a" ${EXTERNAL_GLFW_LINKER_LIBS})

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
        COMMAND git describe --long --tags | xargs -I verstring echo -e "#define MPD_INFO_SCREEN_2_VERSION \"verstring\"" > MPD_INFO_SCREEN2_VER_STR.h
        VERBATIM)
    add_dependencies(mpd_info_screen2 MPD_INFO_SCREEN2_VER_STR)
    set_source_files_properties(${CMAKE_CURRENT_SOURCE_DIR}/src/main.cc PROPERTIES COMPILE_OPTIONS "-include;${CMAKE_CURRENT_BINARY_DIR}/MPD_INFO_SCREEN2_VER_STR.h")
endif()
