#ifndef FOLDERS_H_
#define FOLDERS_H_

#define BUILD_FOLDER "build/"
#define SRC_BUILD_FOLDER "src_build/"
#define SRC_USER_FOLDER "src/user/"
#define SRC_FOLDER "src/"
#define THIRDPARTY_INCLUDE "external/include"
#define INCLUDE "include/"

#define BUILD_OBJ_LINUX "build_obj/"
#define BUILD_OBJ_WIN "build_obj_win/"

#define PLATFORM_LINUX
//#define PLATFORM_WINDOWS






#ifdef PLATFORM_LINUX
	#define BUILD_OBJ_DIR "build_obj/"
#elifdef PLATFORM_WINDOWS
	#define BUILD_OBJ_DIR "build_obj_win/"
#endif


#ifdef PLATFORM_LINUX
	//external/lib/LINUX/libglfw3linux.a
	#define LIBRARY_FOLDER "external/lib/LINUX/"
#elifdef PLATFORM_WINDOWS
	#define LIBRARY_FOLDER "external/lib/WIN/"
#endif


#endif // FOLDERS_H_
