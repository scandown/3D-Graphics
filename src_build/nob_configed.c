#define NOB_IMPLEMENTATION
#define NOB_STRIP_PREFIX
#define NOB_WARN_DEPRECATED
#include "nob.h"
#include "config.h"
#include "folders.h"
#include <string.h>
//#define mem_check

//#define lib
#define obj

#define MAX_FILE_LEN 25
char files[][MAX_FILE_LEN] = {
	//"external/lib/glad.c",
	"shader.c",
	"model.c",
	"quat.c",
	"window.c",
	"camera.c",
	"state.c",
	"uniform.c",
	"texture.c",
	"binary_tree.c",
	"sprite.c",
	"matrix.c"
};

char user_files[][MAX_FILE_LEN] = {
	"input.c",
	"buffers.c",
	"instanced_positions.c"
};

int main(void)
{
	Cmd cmd = {0};
#ifdef BAR
	nob_log(INFO, "BAR feature is enabled");
#endif // BAR
	const char *output_path = BUILD_FOLDER"main";
	const char *input_path = SRC_USER_FOLDER"main.c";
	/*
	nob_cmd_append(&cmd, "bear");
	nob_cmd_append(&cmd, "--");
	*/


#ifndef lib
	nob_cc(&cmd);
	nob_cc_flags(&cmd);
	nob_cmd_append(&cmd, "-g");
#else
	//nob_cmd_append(&cmd, "ar", "rcs", "libengine.a");
#endif

#ifdef obj
	nob_cmd_append(&cmd, "-c");
	//BUILD_FOLDER = "build_obj/"
#endif

#ifdef mem_check
	nob_log(INFO, "mem_check feature is enabled");
	nob_cmd_append(&cmd, "-L"LIBRARY_FOLDER, "-fsanitize=address", "-fno-omit-frame-pointer");
#else
	nob_cmd_append(&cmd, "-L"LIBRARY_FOLDER);
#endif

	nob_cmd_append(&cmd, "-I"BUILD_FOLDER, "-I.", "-I"THIRDPARTY_INCLUDE, "-I"INCLUDE);
	//nob_cc_output(&cmd, output_path);
	nob_cc_inputs(&cmd, input_path, "external/lib/glad.c", SRC_FOLDER"shader.c", SRC_FOLDER"model.c", SRC_FOLDER"quat.c",
			SRC_FOLDER"window.c", SRC_FOLDER"camera.c", SRC_FOLDER"state.c", SRC_FOLDER"uniform.c", SRC_FOLDER"texture.c",
			SRC_FOLDER"binary_tree.c", SRC_FOLDER"sprite.c", SRC_FOLDER"matrix.c");

	// files for building user modifiable parts of code
	nob_cc_inputs(&cmd, SRC_USER_FOLDER"input.c", SRC_USER_FOLDER"buffers.c",
			SRC_USER_FOLDER"instanced_positions.c");

	if (!cmd_run(&cmd)) return 1;

	Cmd move_obj_cmd = {0};
	nob_cmd_append(&move_obj_cmd, "mv");
	for (int i = 0; i < sizeof(files) / MAX_FILE_LEN; i++) {
		char *file = files[i];
		file[strlen(files[i])-1] = 'o';
		printf("%s\n", file);
		nob_cmd_append(&move_obj_cmd, file);
	}
	for (int i = 0; i < sizeof(user_files) / MAX_FILE_LEN; i++) {
		char *file = user_files[i];
		file[strlen(user_files[i])-1] = 'o';
		printf("%s\n", file);
		nob_cmd_append(&move_obj_cmd, file);
	}
	nob_cmd_append(&move_obj_cmd, BUILD_OBJ);
	cmd_run(&move_obj_cmd);

	Cmd link_obj_cmd = {0};
	nob_cc(&link_obj_cmd);
	nob_cmd_append(&link_obj_cmd, "src/user/main.c");
	nob_cmd_append(&link_obj_cmd, "glad.o");
	char new_file[sizeof(files) / MAX_FILE_LEN + sizeof(user_files) / MAX_FILE_LEN][MAX_FILE_LEN + sizeof(BUILD_OBJ)] = {0};

	for (int i = 0; i < sizeof(files) / MAX_FILE_LEN; i++) {
		char *file = files[i];
		strncpy(new_file[i], BUILD_OBJ, sizeof(BUILD_OBJ));
		strncat(new_file[i], file, MAX_FILE_LEN);
		printf("%s\n", new_file[i]);
		nob_cmd_append(&link_obj_cmd, new_file[i]);
	}

	for (int i = 0; i < sizeof(user_files) / MAX_FILE_LEN; i++) {
		char *file = user_files[i];
		strncpy(new_file[i + sizeof(files) / MAX_FILE_LEN], BUILD_OBJ, sizeof(BUILD_OBJ));
		strncat(new_file[i + sizeof(files) / MAX_FILE_LEN], file, MAX_FILE_LEN);
		nob_cmd_append(&link_obj_cmd, new_file[i + sizeof(files) / MAX_FILE_LEN]);
		//memset(new_file, 0, sizeof(new_file));
	}
	nob_cmd_append(&link_obj_cmd, "-L"LIBRARY_FOLDER);
	nob_cmd_append(&link_obj_cmd, "-lglfw3", "-lm", "-ldl", "-lpthread", "-lGL", "-lX11");
	nob_cmd_append(&link_obj_cmd, "-I"BUILD_FOLDER, "-I.", "-I"THIRDPARTY_INCLUDE, "-I"INCLUDE);
	//nob_cmd_append(&cmd, "-Wno-unused-variable", "-Wno-unused-parameter");
	//nob_cmd_append(&cmd, "-Wno-sign-compare");
	cmd_run(&link_obj_cmd);
	return 0;
}
