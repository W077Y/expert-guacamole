﻿cmake_minimum_required (VERSION 3.15)

IF(DEFINED sub_dir_tree_val)
	MESSAGE_TREEVIEW("${target_name} EXAMPLES")
ENDIF()


add_subdirectory("./exa_1")



