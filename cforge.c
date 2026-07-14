#define CF_DISABLE_FILE_HASH

#include "cforge.h"

#if CF_VERSION_BELOW(1, 1, 0)
#error "CForge too old!"
#endif

#include <stdio.h>
#include <strings.h>

#define LIB_NAME "libcasmjit.a"
#define BUILD_DIR "build"
#define LIB_PATH (BUILD_DIR "/" LIB_NAME)
#define LIB_BDIR BUILD_DIR "/lib"

#define KRITIC_LIB "libs/kritic"
#define TEST_BDIR "build/tests"
#define TEST_PATH "build/unittest"

#define CC_TAG "[  " CF_YELLOW "CC" CF_RESET "  ] "
#define AR_TAG "[  " CF_CYAN "AR" CF_RESET "  ] "
#define LD_TAG "[  " CF_CYAN "LD" CF_RESET "  ] "
#define TT_TAG "[  " CF_GREEN "TT" CF_RESET "  ] "
#define IN_TAG "[  " CF_RED "IN" CF_RESET "  ] "

bool was_rebuilt = false;
bool test_was_rebuilt = false;

static bool compile_pattern(const char* section, const char* pattern, const char* outdir) {
    bool rebuilt = false;

    for CF_GLOBS_EACH(pattern, file) {
        bool force = false;
        char* output = CF_MAP(
            file,
            CF_MAP_EXT("o"),
            CF_MAP_DIRS((char*) outdir),
        );

        if (CF_FILE_NOT_UTD(file) || CF_FILE_NOT_UTD(output) || force) {
            rebuilt = true;
            CF_BANNER(CC_TAG "Compiling %s...", section);
            printf(CC_TAG "  %s\n", file);
            CF_RUNP(
                "cc %s %s -c %s -o %s",
                CF_ENV(cflags),
                CF_ENV(includes),
                file,
                output
            );
            CF_FILE_MARK_UTDP(file);
            CF_FILE_MARK_UTDP(output);
        }
    }

    return rebuilt;
}

CF_CONFIG(release) {
    CF_SET_ENV(mode, "release");

    CF_SET_ENV(cflags, "-O2 -DNDEBUG -march=native");
    CF_SET_ENV(includes, "-Iincludes/");
}

CF_CONFIG(debug) {
    CF_SET_ENV(mode, "debug");

    CF_SET_ENV(cflags, "-g");
    CF_SET_ENV(includes, "-Iincludes/");
}

CF_CONFIG(test) {
    CF_CONFIG_EXTENDS(debug);

    CF_APPEND_ENV(includes, " -Ilibs/kritic/");
}

CF_TARGET(
    release,
    CF_WITH_CONFIG(release),
    CF_DEPENDS(build),
    CF_HELP_STRING("Build in release mode")
) {
    CF_NOP();
}

CF_TARGET(
    debug,
    CF_WITH_CONFIG(debug),
    CF_DEPENDS(build),
    CF_HELP_STRING("Build in debug mode")
) {
    CF_NOP();
}

CF_TARGET(
    test,
    CF_WITH_CONFIG(test),
    CF_DEPENDS(test_ln),
    CF_HELP_STRING("Run unit tests")
) {
    CF_NOP();
}

CF_TARGET(build, CF_DEPENDS(archive), CF_HIDDEN) {
    CF_NOP();
}

CF_TARGET(archive, CF_DEPENDS(compile), CF_HIDDEN) {
    if (CF_FILE_NOT_UTD(LIB_PATH) || was_rebuilt) {
        was_rebuilt = true;
        CF_BANNER(AR_TAG "Archiving...");
        char* object_files = CF_JOIN_GLOB(CF_GLOB(LIB_BDIR "/*.o"), " ");
        printf(AR_TAG "  %s\n", object_files);
        CF_RUN("ar rcs %s %s", LIB_PATH, object_files);
        CF_FILE_MARK_UTD(LIB_PATH);
    }
}

CF_TARGET(compile, CF_HIDDEN) {
    CF_MKDIR(LIB_BDIR);
    was_rebuilt |= compile_pattern("library", "src/*.c", LIB_BDIR "/");
    was_rebuilt |= compile_pattern("library", "src/**/*.c", LIB_BDIR "/");
    was_rebuilt |= compile_pattern("library", "src/**/**/*.c", LIB_BDIR "/");
}

CF_TARGET(test_ln, CF_DEPENDS(release), CF_DEPENDS(test_cc)) {
    if (CF_FILE_NOT_UTD(TEST_PATH) || test_was_rebuilt) {
        CF_BANNER(LD_TAG "Linking tests...");
        char* object_files = CF_JOIN_GLOB(CF_GLOB(TEST_BDIR "/*.o"), " ");
        printf(LD_TAG "  %s\n", object_files);
        CF_RUN(
            "cc %s %s %s -o %s",
            object_files,
            LIB_PATH,
            KRITIC_LIB "/build/libkritic.a",
            TEST_PATH
        );
        CF_FILE_MARK_UTD(TEST_PATH);
    }
}

CF_TARGET(test_cc, CF_DEPENDS(kritic), CF_HIDDEN) {
    CF_MKDIR(TEST_BDIR);
    test_was_rebuilt = compile_pattern(
        "unit tests",
        "tests/*.c",
        TEST_BDIR "/"
    );
}

CF_TARGET(kritic, CF_HIDDEN) {
    CF_RUN("cd %s; make static", KRITIC_LIB);
}
