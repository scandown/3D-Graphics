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
	"instanced_positions.c",
};


#define total_file_sizes sizeof(files) / MAX_FILE_LEN + sizeof(user_files) / MAX_FILE_LEN


//void add_compilation_target(Cmd *cmd, char files[][MAX_FILE_LEN], int *num_files, unsigned int for_loop_size, unsigned int index_offset, char *src_folder);
int add_compilation_target(Cmd *cmd, char *directory, char *build_dir);
int get_object_file(char *directory, char *build_dir);
void change_obj_time(unsigned int for_loop_size, unsigned int index_offset);





int main(int argc, char **argv) {

	Cmd cmd = {0};


	int files_len = sizeof(files) / MAX_FILE_LEN;
	int user_files_len = sizeof(user_files) / MAX_FILE_LEN;

	nob_cmd_append(&cmd, "bear");
	nob_cmd_append(&cmd, "--");

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



	add_compilation_target(&cmd, "src/", "build/");
	add_compilation_target(&cmd, "src/user/", "build/");
	cmd_run(&cmd);

	get_object_file("src/", "build_obj/");
	get_object_file("src/user/", "build_obj/");

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



int add_compilation_target(Cmd *cmd, char *directory, char *build_dir) {
	Nob_File_Paths files = {0};
	nob_read_entire_dir(directory, &files);

	for (size_t i = 0; i < files.count; i++) {
		char *file = files.items[i];
		if (file[strlen(file)-1] == 'c') {
			char file_cat[strlen(file) + strlen(directory) + 1];
			strncpy(file_cat, directory, sizeof(file_cat));
			strncat(file_cat, file, strlen(file));
			nob_cmd_append(cmd, nob_temp_strdup(file_cat));	

			printf("%s\n", file_cat);

			char *last_slash = strrchr(file_cat, '/');
			char *first_slash = strchr(file_cat, '/');
			
			int extra_dir_size = last_slash - first_slash;
			if (first_slash == NULL) {
				fprintf(stderr, "ERROR: Couldn't find '/' in %s\n", file_cat);
				return -1;
			}
			
			char new_file_cat[strlen(file) + strlen(build_dir) + extra_dir_size];
			strncpy(new_file_cat, build_dir, sizeof(new_file_cat));
			strncat(new_file_cat, first_slash+1, extra_dir_size);
			strncat(new_file_cat, file, strlen(file));
		}
	}
	return 0;
}

int get_object_file(char *directory, char *build_dir) {
	Nob_File_Paths files = {0};
	nob_read_entire_dir(directory, &files);

	for (size_t i = 0; i < files.count; i++) {
		Cmd cmd = {0};
		char *file = files.items[i];

		if (file[strlen(file)-1] == 'c') {
			nob_cmd_append(&cmd, "mv");
			char file_cat[strlen(file) + strlen(directory) + 1];
			strncpy(file_cat, directory, sizeof(file_cat));
			strncat(file_cat, file, strlen(file));


			char *last_slash = strrchr(file_cat, '/');
			char *first_slash = strchr(file_cat, '/');
			
			int extra_dir_size = last_slash - first_slash;
			if (first_slash == NULL) {
				fprintf(stderr, "ERROR: Couldn't find '/' in %s\n", file_cat);
				return -1;
			}
			
			char new_file_cat[strlen(file) + strlen(build_dir) + extra_dir_size];
			strncpy(new_file_cat, build_dir, sizeof(new_file_cat));
			strncat(new_file_cat, first_slash+1, extra_dir_size);
			strncat(new_file_cat, file, strlen(file));


			char *current_file = file;
			current_file[strlen(current_file) - 1] = 'o';

			new_file_cat[strlen(new_file_cat) - 1] = 'o';

			nob_cmd_append(&cmd, current_file);
			nob_cmd_append(&cmd, nob_temp_strdup(new_file_cat));	
			nob_cmd_run(&cmd);
		}
	}
	return 0;

}
