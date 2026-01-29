#ifndef FOLDERS_H_
#define FOLDERS_H_

#define BUILD_FOLDER "build/"
#define BUILD_OBJ "build_obj/"
#define SRC_BUILD_FOLDER "src_build/"
#define SRC_USER_FOLDER "src/user/"
#define SRC_FOLDER "src/"
#define THIRDPARTY_INCLUDE "external/include"
#define INCLUDE "include/"

#ifdef __linux__
	//external/lib/LINUX/libglfw3linux.a
	#define LIBRARY_FOLDER "external/lib/LINUX/"
#elifdef __MINGW32__
	#define LIBRARY_FOLDER "external/lib/WIN/"
#endif


#endif // FOLDERS_H_
