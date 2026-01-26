#define NOB_IMPLEMENTATION
#define NOB_STRIP_PREFIX
#define NOB_WARN_DEPRECATED
#include "nob.h"
#include "config.h"
#include "folders.h"
//#define mem_check

int main(void)
{
    Cmd cmd = {0};
#ifdef BAR
    nob_log(INFO, "BAR feature is enabled");
#endif // BAR
    const char *output_path = BUILD_FOLDER"main";
    const char *input_path = SRC_USER_FOLDER"main.c";
    nob_cmd_append(&cmd, "bear");
    nob_cmd_append(&cmd, "--");
    nob_cc(&cmd);
    nob_cc_flags(&cmd);

#ifdef mem_check
    nob_log(INFO, "mem_check feature is enabled");
    nob_cmd_append(&cmd, "-g", "-L"LIBRARY_FOLDER, "-fsanitize=address", "-fno-omit-frame-pointer");
#else
    nob_cmd_append(&cmd, "-g", "-L"LIBRARY_FOLDER);
#endif // mem_check

    nob_cmd_append(&cmd, "-I"BUILD_FOLDER, "-I.", "-I"THIRDPARTY_INCLUDE, "-I"INCLUDE);
    nob_cc_output(&cmd, output_path);
    nob_cc_inputs(&cmd, input_path, "external/lib/glad.c", SRC_FOLDER"shader.c", SRC_FOLDER"model.c", SRC_FOLDER"quat.c",
		    SRC_FOLDER"window.c", SRC_FOLDER"camera.c", SRC_FOLDER"state.c", SRC_FOLDER"uniform.c", SRC_FOLDER"texture.c",
		    SRC_FOLDER"binary_tree.c", SRC_FOLDER"sprite.c");

    // files for building user modifiable parts of code
    nob_cc_inputs(&cmd, SRC_USER_FOLDER"input.c", SRC_USER_FOLDER"buffers.c",
		    SRC_USER_FOLDER"instanced_positions.c", SRC_USER_FOLDER"matrix.c");

    nob_cmd_append(&cmd, "-lglfw3", "-lm", "-ldl", "-lpthread", "-lGL", "-lX11");
    nob_cmd_append(&cmd, "-Wno-unused-variable", "-Wno-unused-parameter");
    nob_cmd_append(&cmd, "-Wno-sign-compare");
    if (!cmd_run(&cmd)) return 1;
    return 0;
}
