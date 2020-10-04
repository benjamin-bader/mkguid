cmake_minimum_required(VERSION 3.10)

project(mkguid
	VERSION 0.1.0
	DESCRIPTION "Generates GUIDs"
	LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(RUN_IWYU)
	find_program(IWYU_PATH NAMES include-what-you-use iwyu REQUIRED)
	set(CMAKE_CXX_INCLUDE_WHAT_YOU_USE ${IWYU_PATH})
endif(RUN_IWYU)

add_executable(mkguid
	uuid.cpp
	uuid.h
	main.cpp
)

target_compile_options(mkguid PRIVATE
	$<$<CXX_COMPILER_ID:MSVC>:/W4 /WX>
	$<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra -pedantic>
)

install(TARGETS mkguid)
