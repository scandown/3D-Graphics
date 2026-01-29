#define NOB_IMPLEMENTATION
#define NOB_STRIP_PREFIX
#define NOB_WARN_DEPRECATED
#include "nob.h"
#include "config.h"
#include "folders.h"
#include <string.h>
//#define mem_check

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

int main(void) {
	Cmd cmd = {0};
	const char *output_path = BUILD_FOLDER"main";
	const char *input_path = SRC_USER_FOLDER"main.c";
	/*
	nob_cmd_append(&cmd, "bear");
	nob_cmd_append(&cmd, "--");
	*/

	nob_cc(&cmd);
	nob_cc_flags(&cmd);
	nob_cmd_append(&cmd, "-g");

	nob_cmd_append(&cmd, "-c");

#ifdef mem_check
	nob_log(INFO, "mem_check feature is enabled");
	nob_cmd_append(&cmd, "-L"LIBRARY_FOLDER, "-fsanitize=address", "-fno-omit-frame-pointer");
#else
	nob_cmd_append(&cmd, "-L"LIBRARY_FOLDER);
#endif

	nob_cmd_append(&cmd, "-I"BUILD_FOLDER, "-I.", "-I"THIRDPARTY_INCLUDE, "-I"INCLUDE);

	//date -r (FILE) "+%s"
	char new_file_c[sizeof(files) / MAX_FILE_LEN + sizeof(user_files) / MAX_FILE_LEN][MAX_FILE_LEN + sizeof(SRC_USER_FOLDER)] = {0};
	nob_cmd_append(&cmd, input_path);
	for (int i = 0; i < sizeof(files) / MAX_FILE_LEN; i++) {
		char *file = files[i];
		strncpy(new_file_c[i], SRC_FOLDER, sizeof(SRC_FOLDER));
		strncat(new_file_c[i], file, MAX_FILE_LEN);
		printf("%s\n", new_file_c[i]);
		nob_cmd_append(&cmd, new_file_c[i]);
	}

	for (int i = 0; i < sizeof(user_files) / MAX_FILE_LEN; i++) {
		char *file = user_files[i];
		strncpy(new_file_c[i + sizeof(files) / MAX_FILE_LEN], SRC_USER_FOLDER, sizeof(SRC_USER_FOLDER));
		strncat(new_file_c[i + sizeof(files) / MAX_FILE_LEN], file, MAX_FILE_LEN);
		printf("%s\n", new_file_c[i]);
		nob_cmd_append(&cmd, new_file_c[i]);
	}

	if (!cmd_run(&cmd)) return 1;

	Cmd move_obj_cmd = {0};
	nob_cmd_append(&move_obj_cmd, "mv");
	for (int i = 0; i < sizeof(files) / MAX_FILE_LEN; i++) {
		char *file = files[i];
		file[strlen(files[i])-1] = 'o';
		nob_cmd_append(&move_obj_cmd, file);
	}
	for (int i = 0; i < sizeof(user_files) / MAX_FILE_LEN; i++) {
		char *file = user_files[i];
		file[strlen(user_files[i])-1] = 'o';
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
	}
	nob_cmd_append(&link_obj_cmd, "-L"LIBRARY_FOLDER);
	nob_cmd_append(&link_obj_cmd, "-lglfw3", "-lm", "-ldl", "-lpthread", "-lGL", "-lX11");
	nob_cmd_append(&link_obj_cmd, "-I"BUILD_FOLDER, "-I.", "-I"THIRDPARTY_INCLUDE, "-I"INCLUDE);
	cmd_run(&link_obj_cmd);
	return 0;
}
