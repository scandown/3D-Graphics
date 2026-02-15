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
int change_c_files_times(char *dir, char *build_dir);
bool is_file_times_same(char *file1, char *file2);
int add_compilation_target(Cmd *cmd, char *dir, char *build_dir, char **excluded_files, size_t excluded_files_size);
int move_object_files(char *dir, char *build_dir);
void change_obj_time(unsigned int for_loop_size, unsigned int index_offset);





int main(int argc, char **argv) {

	Cmd cmd = {0};

	nob_cmd_append(&cmd, "bear");
	nob_cmd_append(&cmd, "--");

	nob_cmd_append(&cmd, COMPILER);
	nob_cmd_append(&cmd, "-g");
	nob_cmd_append(&cmd, "-c");
	nob_cmd_append(&cmd, "-L"LIBRARY_FOLDER);
	nob_cmd_append(&cmd, "-I"BUILD_FOLDER, "-I.", "-I"THIRDPARTY_INCLUDE, "-I"INCLUDE);

#ifdef mem_check
	nob_log(INFO, "mem_check feature is enabled");
	nob_cmd_append(&cmd, "-L"LIBRARY_FOLDER, "-fsanitize=address", "-fno-omit-frame-pointer");
#endif


	char *excluded_files[] = {"src/user/main.c"};

	add_compilation_target(&cmd, "src/", "build_obj/",
			excluded_files, sizeof(excluded_files) / sizeof(char *));

	add_compilation_target(&cmd, "src/user/", "build_obj/",
			excluded_files, sizeof(excluded_files) / sizeof(char *));
	cmd_run(&cmd);

	move_object_files("src/", "build_obj/");
	move_object_files("src/user/", "build_obj/");

	change_c_files_times("src/", "build_obj/");
	change_c_files_times("src/user/", "build_obj/");


	char *directories[2] = {"build_obj/", "build_obj/user/"};
	
	Cmd link_cmd = {0};

	if (argc > 1 && (strncmp(argv[1], "ar", 2) == 0)) {
		nob_cmd_append(&link_cmd, "ar");
		nob_cmd_append(&link_cmd, "rcs", BUILD_FOLDER"libt.a");
	} else {
		nob_cmd_append(&link_cmd, "gcc");
	}
	nob_cmd_append(&link_cmd, "external/lib/glad.c");
	for (int i = 0; i < sizeof(directories) / sizeof(char *); i++) {
		Nob_File_Paths files = {0};
		nob_read_entire_dir(directories[i], &files);
		for (int j = 0; j < files.count; j++) {
			char *file = files.items[j];
			if (strncmp(file+strlen(file)-2, ".o", 2) == 0) {
				char str[strlen(file) + sizeof(directories[i]) + 1];
				strncpy(str, directories[i], sizeof(str));
				strncat(str, file, strlen(file));
				nob_cmd_append(&link_cmd, nob_temp_strdup(str));
			}
		}
	}

	if (argc == 1) {
		nob_cmd_append(&link_cmd, "-Lexternal/lib/LINUX", "-Iinclude", "-Iexternal/include");
		nob_cmd_append(&link_cmd, "-lglfw3", "-lm", "-lGL");
		nob_cmd_append(&link_cmd, "-o", "build/main");
	}
	cmd_run(&link_cmd);

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

int change_c_files_times(char *dir, char *build_dir){
	Nob_File_Paths files = {0};
	nob_read_entire_dir(dir, &files);

	for (int i = 0; i < files.count; i++) {
		char *file = files.items[i];
		if (strncmp(file+strlen(file)-2, ".c", 2) == 0) {

			char file_cat[strlen(file) + strlen(dir) + 1];
			strncpy(file_cat, dir, sizeof(file_cat));
			strncat(file_cat, file, strlen(file));


			char *last_slash = strrchr(file_cat, '/');
			char *first_slash = strchr(file_cat, '/');
			
			int extra_dir_size = last_slash - first_slash;
			if (first_slash == NULL) {
				fprintf(stderr, "ERROR: Couldn't find '/' in %s\n", file_cat);
				return -1;
			}
			
			char new_file_cat[strlen(file) + strlen(build_dir) + extra_dir_size + 1];
			strncpy(new_file_cat, build_dir, sizeof(new_file_cat));
			strncat(new_file_cat, first_slash+1, extra_dir_size);
			strncat(new_file_cat, file, strlen(file));
			new_file_cat[strlen(new_file_cat)-1] = 'o';


			struct stat filestat;
			stat(file_cat,&filestat);
			change_time(new_file_cat, filestat.st_mtim.tv_sec);
		}
	}
	return 0;
}


int add_compilation_target(Cmd *cmd, char *dir, char *build_dir,
		char **excluded_files, size_t excluded_files_size) {

	Nob_File_Paths files = {0};
	nob_read_entire_dir(dir, &files);

	for (int i = 0; i < files.count; i++) {
		char *file = files.items[i];
		bool excecute = true;
		for (int j = 0; j < excluded_files_size; j++) {
			char full_file_path[strlen(dir) + strlen(file) + 1];
			strncpy(full_file_path, dir, sizeof(full_file_path)); 
			strncat(full_file_path, file, strlen(file)); 
			int cmp = strncmp(full_file_path, excluded_files[j], strlen(excluded_files[j]));
			if (cmp == 0) {
				excecute = false;
			}
		}
		if (!excecute) {
			continue;
		}
		if (strncmp(file+strlen(file)-2, ".c", 2) == 0) {
			char file_cat[strlen(file) + strlen(dir) + 1];
			strncpy(file_cat, dir, sizeof(file_cat));
			strncat(file_cat, file, strlen(file));

			char *last_slash = strrchr(file_cat, '/');
			char *first_slash = strchr(file_cat, '/');
			
			int extra_dir_size = last_slash - first_slash;
			if (first_slash == NULL) {
				fprintf(stderr, "ERROR: Couldn't find '/' in %s\n", file_cat);
				return -1;
			}
			
			char file_object[strlen(file) + strlen(build_dir) + extra_dir_size + 1];
			strncpy(file_object, build_dir, sizeof(file_object));
			strncat(file_object, first_slash+1, extra_dir_size);
			strncat(file_object, file, strlen(file));
			file_object[strlen(file_object) - 1] = 'o';




			if (access(file_object, F_OK) != 0) {
				nob_cmd_append(cmd, nob_temp_strdup(file_cat));
			} else if (is_file_times_same(file_cat, file_object) == false) {
				nob_cmd_append(cmd, nob_temp_strdup(file_cat));	
			}
		}
	}
	return 0;
}


bool is_file_times_same(char *file1, char *file2) {
	struct stat file1_stat;
	stat(file1,&file1_stat);

	struct stat file2_stat;
	stat(file2,&file2_stat);

	return (file2_stat.st_mtim.tv_sec == file1_stat.st_mtim.tv_sec);
}

int move_object_files(char *dir, char *build_dir) {
	Nob_File_Paths files = {0};
	nob_read_entire_dir(dir, &files);

	for (int i = 0; i < files.count; i++) {
		Cmd cmd = {0};
		char *file = files.items[i];

		if (strncmp(file+strlen(file)-2, ".c", 2) == 0) {
			nob_cmd_append(&cmd, "mv");
			char file_cat[strlen(file) + strlen(dir) + 1];
			strncpy(file_cat, dir, sizeof(file_cat));
			strncat(file_cat, file, strlen(file));


			char *last_slash = strrchr(file_cat, '/');
			char *first_slash = strchr(file_cat, '/');
			
			int extra_dir_size = last_slash - first_slash;
			if (first_slash == NULL) {
				fprintf(stderr, "ERROR: Couldn't find '/' in %s\n", file_cat);
				return -1;
			}
			
			char new_file_cat[strlen(file) + strlen(build_dir) + extra_dir_size + 1];
			strncpy(new_file_cat, build_dir, sizeof(new_file_cat));
			strncat(new_file_cat, first_slash+1, extra_dir_size);
			strncat(new_file_cat, file, strlen(file));


			char *current_file = file;
			current_file[strlen(current_file) - 1] = 'o';
			new_file_cat[strlen(new_file_cat) - 1] = 'o';

			if (access(current_file, F_OK) != 0) {
				continue;
			}

			nob_cmd_append(&cmd, current_file);
			nob_cmd_append(&cmd, nob_temp_strdup(new_file_cat));	
			nob_cmd_run(&cmd);
		}
	}
	return 0;

}
