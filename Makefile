ifdef RELEASE
	CXX_COMMON_FLAGS := -O2 -DNDEBUG -fno-delete-null-pointer-checks -fno-strict-overflow -fno-strict-aliasing -ftrivial-auto-var-init=zero
else
	CXX_COMMON_FLAGS := -Werror -Og -g
endif

WORKING_DIR != pwd

CXX_LINKER_FLAGS := -Ithird_party/include
CXX_FLAGS := \
	-std=c++23 \
	-Wall -Wformat -Wformat=2 -Wconversion -Wimplicit-fallthrough \
	-Werror=format-security \
	-U_FORTIFY_SOURCE -D_FORTIFY_SOURCE=3 \
	-D_GLIBCXX_ASSERTIONS \
	-fstrict-flex-arrays=3 \
	-fstack-clash-protection -fstack-protector-strong \
	-Wl,-z,nodlopen -Wl,-z,noexecstack \
	-Wl,-z,relro -Wl,-z,now \
	-Wl,--as-needed -Wl,--no-copy-dt-needed-entries \
	-fPIE -pie \
	${CXX_COMMON_FLAGS}

SOURCES := \
	src/main.cc \
	src/args.cc \
	src/mpd_client.cc \
	src/constants.cc \
	src/helpers.cc \
	src/signal_handler.cc \
	src/mpd_display.cc

HEADERS := \
	src/args.h \
	src/mpd_client.h \
	src/constants.h \
	src/helpers.h \
	src/signal_handler.h \
	src/mpd_display.h

OBJDIR := objdir
OBJECTS := $(addprefix ${OBJDIR}/,$(subst .cc,.cc.o,${SOURCES}))

all: mpd_info_screen2 unittest

mpd_info_screen2: ${OBJECTS} third_party/lib/libraylib.a
	${CXX} -o mpd_info_screen2 ${CXX_LINKER_FLAGS} ${CXX_FLAGS} $^

unittest: ${OBJDIR}/src/test.cc.o $(filter-out ${OBJDIR}/src/main.cc.o,${OBJECTS}) third_party/lib/libraylib.a
	${CXX} -o unittest -g -Og $^

${OBJDIR}/%.cc.o: %.cc ${HEADERS} | format
	@mkdir -p $(dir $@)
	${CXX} -o $@ -c ${CXX_FLAGS} $<

third_party/lib/libraylib.a: third_party/raylib-5.5.tar.gz
	@mkdir -p third_party/lib
	@mkdir -p third_party/include
	tar -xf third_party/raylib-5.5.tar.gz -C third_party
	cmake -S third_party/raylib-5.5 -B third_party/raylib_BUILD -DCMAKE_BUILD_TYPE=Release
	${MAKE} -C third_party/raylib_BUILD raylib
	cp third_party/raylib_BUILD/raylib/libraylib.a third_party/lib/libraylib.a
	rm -rf third_party/raylib_BUILD
	pushd third_party/raylib-5.5/src && find . -regex '.*\.h$$' -exec install -D -m644 '{}' "${WORKING_DIR}/third_party/include/{}" ';' && popd
	rm -rf third_party/raylib-5.5

third_party/raylib-5.5.tar.gz:
	@mkdir -p third_party
	curl -L -o third_party/raylib-5.5.tar.gz https://github.com/raysan5/raylib/archive/refs/tags/5.5.tar.gz
	sha256sum -c third_party/raylib-5.5_SHA256SUMS.txt || (rm -f third_party/raylib-5.5.tar.gz && false)

.PHONY: clean format

clean:
	rm -f mpd_info_screen2
	rm -f unittest
	rm -rf ${OBJDIR}
	rm -rf third_party/lib
	rm -rf third_party/include
	rm -rf third_party/raylib-5.5
	rm -rf third_party/raylib_BUILD

format:
	test -x /usr/bin/clang-format && clang-format -i --style=file ${SOURCES} ${HEADERS} src/test.cc || true
