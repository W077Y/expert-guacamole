﻿cmake_minimum_required (VERSION 3.15)

set(target_name "EXMATH")

IF(DEFINED sub_dir_tree_val)
	MESSAGE_TREEVIEW(${target_name})
ENDIF()

add_library(${target_name} STATIC)

target_sources(${target_name}
	PUBLIC "${CMAKE_CURRENT_LIST_DIR}/ExMath.hpp"
	
	PUBLIC "${CMAKE_CURRENT_LIST_DIR}/inc/ExMath_traits.hpp"

	PUBLIC "${CMAKE_CURRENT_LIST_DIR}/src/ExMath.cpp"
	)

target_include_directories(	${target_name}
	PUBLIC  "${CMAKE_CURRENT_LIST_DIR}")


target_compile_features(${target_name} PUBLIC cxx_std_20)


