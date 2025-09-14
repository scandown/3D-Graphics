#define NOB_IMPLEMENTATION
#define NOB_STRIP_PREFIX
#define NOB_WARN_DEPRECATED
#include "nob.h"
#include "config.h"
#include "folders.h"

int main(void)
{
    Cmd cmd = {0};
#ifdef FOO
    nob_log(INFO, "FOO feature is enabled");
#endif // FOO
#ifdef BAR
    nob_log(INFO, "BAR feature is enabled");
#endif // BAR
    const char *output_path = BUILD_FOLDER"main";
    const char *input_path = SRC_FOLDER"main.c";
    nob_cc(&cmd);
    nob_cc_flags(&cmd);
    nob_cmd_append(&cmd, "-I"BUILD_FOLDER, "-I.", "-I"THIRDPARTY_INCLUDE, "-I"INCLUDE);

    nob_cmd_append(&cmd, "-g", "-L"LIBRARY_FOLDER);
    nob_cc_output(&cmd, output_path);
    nob_cc_inputs(&cmd, input_path, "external/lib/glad.c", SRC_FOLDER"shader.c", SRC_FOLDER"spaces.c");
    nob_cmd_append(&cmd, "-lglfw3", "-lm", "-ldl", "-lpthread", "-lGL", "-lX11");
    if (!cmd_run(&cmd)) return 1;
    return 0;
}
