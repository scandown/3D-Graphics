#define NOB_IMPLEMENTATION
#define NOB_STRIP_PREFIX
#define NOB_WARN_DEPRECATED


#define STB_DS_IMPLEMENTATION
#include "stb_ds.h"

#include "nob.h"
#include "config.h"
#include "folders.h"
#include <string.h>


#ifdef PLATFORM_LINUX
#define COMPILER "gcc"
#define ARCHIVE "ar"
#elifdef PLATFORM_WINDOWS
#define COMPILER "x86_64-w64-mingw32-gcc"
#define ARCHIVE "x86_64-w64-mingw32-ar"
#endif

#include <sys/stat.h>
#include <time.h>
#include <utime.h>

//#define mem_check

#define MAX_FILE_LEN 25
char files[][MAX_FILE_LEN] = {
	"window.c",
	"shader.c",
	"model.c",
	"quat.c",
	"camera.c",
	"uniform.c",
	"texture.c",
	"binary_tree.c",
	"matrix.c"
};

char user_files[][MAX_FILE_LEN] = {
	"input.c",
	"buffers.c",
	"instanced_positions.c",
	"sprite.c"
};


#define total_file_sizes sizeof(files) / MAX_FILE_LEN + sizeof(user_files) / MAX_FILE_LEN

int change_c_files_times(char *dir, char *build_dir);
bool is_file_times_same(char *file1, char *file2);
int add_compilation_target(Cmd *cmd, char *dir, char *build_dir, char **excluded_files, size_t excluded_files_size);
int move_object_files(char *dir, char *build_dir);
void change_obj_time(unsigned int for_loop_size, unsigned int index_offset);





int main() {

	Cmd cmd = {0};


	unsigned int object_single_file_size = 0;
	unsigned int c_single_file_size = 0;

	unsigned int total_files = sizeof(files) / MAX_FILE_LEN;


	nob_cmd_append(&cmd, "cc");
	nob_cmd_append(&cmd, "-I"BUILD_FOLDER, "-I"INCLUDE, "-I"THIRDPARTY_INCLUDE);
	nob_cmd_append(&cmd, "-Wfatal-errors");
	nob_cmd_append(&cmd, "-g", "-c");
	nob_cmd_append(&cmd, "external/lib/rgfw.c");
	unsigned int total_object_sizes = 0;
	for (int i = 0; i < total_files; ++i) {
		object_single_file_size = strlen(BUILD_OBJ_DIR) + strlen(files[i]) + 1;
		total_object_sizes += object_single_file_size;
	}

	char file_name_array[total_object_sizes];
	char *file_name_array_copy = file_name_array;


	stbds_string_arena my_arena={0};
	for (int i = 0; i < total_files; ++i) {

		c_single_file_size = strlen(SRC_FOLDER) + strlen(files[i]) + 1;
		char c_new_file_string[c_single_file_size];
		strcpy(c_new_file_string, SRC_FOLDER);
		strcat(c_new_file_string, files[i]);


		char *filename = stbds_stralloc(&my_arena, c_new_file_string);
		nob_cmd_append(&cmd, filename);
	}
	if (!nob_cmd_run(&cmd)) return 1;


	Cmd move_cmd = {0};
	nob_cmd_append(&move_cmd, "mv");
	nob_cmd_append(&move_cmd, "rgfw.o");

	stbds_strreset(&my_arena);
	for (int i = 0; i < total_files; ++i) {
		char file_dir[strlen(files[i]) + 1];
		strcpy(file_dir, files[i]);
		file_dir[strlen(files[i]) - 1] = 'o';

		char *filename = stbds_stralloc(&my_arena, file_dir);

		nob_cmd_append(&move_cmd, filename);
	}
	stbds_strreset(&my_arena);

	nob_cmd_append(&move_cmd, BUILD_OBJ_DIR);
	nob_cmd_run(&move_cmd);


	Cmd link_cmd = {0};

	nob_cmd_append(&link_cmd, ARCHIVE);
	nob_cmd_append(&link_cmd, "rcs", BUILD_FOLDER"libt.a");
	nob_cmd_append(&link_cmd, "build_obj/rgfw.o");

	for (int i = 0; i < total_files; ++i) {
		object_single_file_size = strlen(BUILD_OBJ_DIR) + strlen(files[i]) + 1;
		char object_new_file_string[object_single_file_size];
		strcpy(object_new_file_string, BUILD_OBJ_DIR);
		strcat(object_new_file_string, files[i]);
		object_new_file_string[object_single_file_size-2] = 'o';


		char *file_name = stbds_stralloc(&my_arena, object_new_file_string);

		nob_cmd_append(&link_cmd, file_name);


	}

	nob_cmd_run(&link_cmd);

	
	return 0;
}



