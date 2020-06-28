cmake_minimum_required(VERSION 3.10)

project(mkguid
	VERSION 0.1.0
	DESCRIPTION "Generates GUIDs"
	LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(mkguid
	main.cpp
)
