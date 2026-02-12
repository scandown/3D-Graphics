#define NOB_IMPLEMENTATION
#define NOB_STRIP_PREFIX
#define NOB_WARN_DEPRECATED
#define COMPILER "gcc"
#include "nob.h"
#include "src_build/folders.h"
#include <string.h>


int main(int argc, char **argv) {

	Cmd cmd = {0};
	Nob_String_Builder sb = {0};

	if (argc > 1) {
		if (!strncmp(argv[1], "r", 1)) {
			nob_cmd_append(&cmd, "./"BUILD_FOLDER"main");

			cmd_run(&cmd);
			return 0;
		}
	}
	NOB_GO_REBUILD_URSELF_PLUS(argc, argv, "nob.h", "src_build/folders.h");

	if (!nob_mkdir_if_not_exists(BUILD_FOLDER)) return 1;


	const char *conf_path = BUILD_FOLDER"config.h";
	int exists = file_exists(conf_path);
	if (exists < 0) return 1;
	if (exists == 0) {
		nob_log(INFO, "Generating initial %s", conf_path);
		sb_append_cstr(&sb, "#ifndef CONFIG_H_\n");
		sb_append_cstr(&sb, "#define CONFIG_H_\n");
		sb_append_cstr(&sb, "// #define FOO // Enables FOO feature\n");
		sb_append_cstr(&sb, "// #define BAR // Enables BAR feature\n");
		sb_append_cstr(&sb, "#endif // CONFIG_H_\n");
		if (!nob_write_entire_file(conf_path, sb.items, sb.count)) return 1;
		sb.count = 0;
		nob_log(INFO, "==================================");
		nob_log(INFO, "EDIT %s TO CONFIGURE YOUR BUILD!!!", conf_path);
		nob_log(INFO, "==================================");
	}


	const char *output_path = BUILD_FOLDER"nob_configed";
	const char *input_path = SRC_BUILD_FOLDER"nob_configed.c";
	nob_cmd_append(&cmd, COMPILER);
	//nob_cmd_append(&cmd, "--no-warnings");
	nob_cc_flags(&cmd);
	nob_cmd_append(&cmd, "-I.", "-I"BUILD_FOLDER, "-I"SRC_BUILD_FOLDER); // -I is usually the same across all compilers
	nob_cc_output(&cmd, output_path);
	nob_cc_inputs(&cmd, input_path);
	if (!cmd_run(&cmd)) return 1;

	nob_cmd_append(&cmd, output_path);
	if (!cmd_run(&cmd)) return 1;

	if (argc > 1) {
		if (!strncmp(argv[1], "br", 2)) {
			nob_cmd_append(&cmd, "./"BUILD_FOLDER"main");

			cmd_run(&cmd);
		}
		if (!strncmp(argv[1], "ar", 2)) {
			Nob_File_Paths files = {0};

			nob_read_entire_dir("build_obj", &files);

			Cmd ar_cmd = {0};
			char *archive_file_name = "build/libt.a";

			nob_cmd_append(&ar_cmd, "ar", "rcs", archive_file_name);
			for (int i = 0; i < files.count; i++) {
				char *file = files.items[i];
				char file_cat[strlen(file)+sizeof("build_obj/")];

				strncpy(file_cat, "build_obj/", sizeof(file_cat));
				strncat(file_cat, file, strlen(file));

				if (file[strlen(file)-1] == 'o') {
					printf("%s\n", file_cat);
					nob_cmd_append(&ar_cmd, nob_temp_strdup(file_cat));
				}
			}



			cmd_run(&ar_cmd);
			
			nob_cmd_append(&cmd, output_path);
			nob_cmd_append(&cmd, "ar");
			if (!cmd_run(&cmd)) return 1;
		}
	}

	return 0;
}
