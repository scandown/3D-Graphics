#define NOB_IMPLEMENTATION
#define NOB_STRIP_PREFIX
#define NOB_WARN_DEPRECATED
#define COMPILER "gcc"
#include "nob.h"
#include "config.h"
#include "folders.h"
#include <string.h>

#include <sys/stat.h>
#include <time.h>
#include <utime.h>

//#define mem_check

#define MAX_FILE_LEN 25
char files[][MAX_FILE_LEN] = {
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

#define total_file_sizes sizeof(files) / MAX_FILE_LEN + sizeof(user_files) / MAX_FILE_LEN

char new_file_c[total_file_sizes][MAX_FILE_LEN + sizeof(SRC_USER_FOLDER)] = {0};
char new_file_o[total_file_sizes][MAX_FILE_LEN + sizeof(BUILD_OBJ)] = {0};
int time_c[total_file_sizes] = {0};
int time_o[total_file_sizes] = {0};



void add_compilation_target(Cmd *cmd, char files[][MAX_FILE_LEN], unsigned int for_loop_size, unsigned int index_offset, char *src_folder);





int main(void) {



	int files_len = sizeof(files) / MAX_FILE_LEN;
	int user_files_len = sizeof(user_files) / MAX_FILE_LEN;


	Cmd cmd = {0};
	/*
	const char *output_path = BUILD_FOLDER"main";
	const char *input_path = SRC_USER_FOLDER"main.c";
	*/
	/*
	nob_cmd_append(&cmd, "bear");
	nob_cmd_append(&cmd, "--");
	*/

	nob_cmd_append(&cmd, COMPILER);
	nob_cc_flags(&cmd);
	nob_cmd_append(&cmd, "-g");
	nob_cmd_append(&cmd, "-c");
	nob_cmd_append(&cmd, "-L"LIBRARY_FOLDER);
	nob_cmd_append(&cmd, "-I"BUILD_FOLDER, "-I.", "-I"THIRDPARTY_INCLUDE, "-I"INCLUDE);

#ifdef mem_check
	nob_log(INFO, "mem_check feature is enabled");
	nob_cmd_append(&cmd, "-L"LIBRARY_FOLDER, "-fsanitize=address", "-fno-omit-frame-pointer");
#endif




	add_compilation_target(&cmd, files, files_len, 0, SRC_FOLDER);
	add_compilation_target(&cmd, user_files, user_files_len, files_len, SRC_USER_FOLDER);


	if (!cmd_run(&cmd)) {
		printf("No recompile required\n");
		return 1;
	}


	Cmd move_obj_cmd = {0};
	nob_cmd_append(&move_obj_cmd, "mv");
	for (int i = 0; i < files_len; i++) {
		char *file = files[i];
		file[strlen(file)-1] = 'o';
		nob_cmd_append(&move_obj_cmd, file);
	}
	for (int i = 0; i < user_files_len; i++) {
		char *file = user_files[i];
		file[strlen(file)-1] = 'o';
		nob_cmd_append(&move_obj_cmd, file);
		printf("%s\n", file);
	}
	nob_cmd_append(&move_obj_cmd, BUILD_OBJ);
	cmd_run(&move_obj_cmd);

	Cmd link_obj_cmd = {0};
	nob_cc(&link_obj_cmd);
	nob_cmd_append(&link_obj_cmd, "src/user/main.c");
	nob_cmd_append(&link_obj_cmd, "external/lib/glad.c");

	const int BUILD_DIR_LEN = sizeof(BUILD_OBJ);
	for (int i = 0; i < files_len; i++) {
		char *file = files[i];
		strncpy(new_file_o[i], BUILD_OBJ, BUILD_DIR_LEN);
		strncat(new_file_o[i], file, MAX_FILE_LEN);
		//printf("%s\n", new_file[i]);
		nob_cmd_append(&link_obj_cmd, new_file_o[i]);
	}

	for (int i = 0; i < user_files_len; i++) {
		char *file = user_files[i];
		strncpy(new_file_o[i + files_len], BUILD_OBJ, BUILD_DIR_LEN);
		strncat(new_file_o[i + files_len], file, MAX_FILE_LEN);
		nob_cmd_append(&link_obj_cmd, new_file_o[i + files_len]);
	}
	nob_cmd_append(&link_obj_cmd, "-L"LIBRARY_FOLDER);
	nob_cmd_append(&link_obj_cmd, "-lglfw3", "-lm", "-ldl", "-lpthread", "-lGL", "-lX11");
	nob_cmd_append(&link_obj_cmd, "-I"BUILD_FOLDER, "-I.", "-I"THIRDPARTY_INCLUDE, "-I"INCLUDE);
	cmd_run(&link_obj_cmd);
	return 0;
}




int change_time(char *filename, time_t mtime) {
  struct stat foo;
  //time_t mtime;
  struct utimbuf new_times;

  if (stat(filename, &foo) < 0) {
    perror(filename);
    return 1;
  }
  //mtime = foo.st_mtime; /* seconds since the epoch */

  new_times.actime = time(NULL);
  new_times.modtime = mtime;    /* set mtime to current time */
  if (utime(filename, &new_times) < 0) {
    perror(filename);
    return 1;
  }
  return 0;
}

void add_compilation_target(Cmd *cmd, char files[][MAX_FILE_LEN], unsigned int for_loop_size, unsigned int index_offset, char *src_folder) {
	for (unsigned int i = 0; i < for_loop_size; i++) {
		char *file = files[i];
		strncpy(new_file_c[i + index_offset], src_folder, strlen(src_folder));
		strncat(new_file_c[i + index_offset], file, MAX_FILE_LEN);

		const int BUILD_DIR_LEN = sizeof(BUILD_OBJ);
		strncpy(new_file_o[i + index_offset], BUILD_OBJ, BUILD_DIR_LEN);
		strncat(new_file_o[i + index_offset], file, MAX_FILE_LEN);


		struct stat filestat;
		stat(new_file_c[i], &filestat);
		time_c[i] = filestat.st_mtim.tv_sec;


		if (stat(new_file_o[i], &filestat) == -1) {
			nob_cmd_append(cmd, new_file_c[i + index_offset]);
		}

		stat(new_file_o[i], &filestat);
		time_o[i] = filestat.st_mtim.tv_sec;



		if (time_c[i] != time_o[i]) {
			nob_cmd_append(cmd, new_file_c[i + index_offset]);
		}
	}
}
