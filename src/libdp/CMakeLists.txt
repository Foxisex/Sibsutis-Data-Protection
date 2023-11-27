set(target_name dp)

add_library(${target_name} STATIC
	libdp/lab1.cpp
	libdp/lab2.cpp
	libdp/lab3.cpp
	libdp/lab4.cpp)
	
include(CompileOptions)
set_compile_options(${target_name})

target_include_directories(
  ${target_name}
  PUBLIC
  ${PROJECT_SOURCE_DIR}/include
  ${PROJECT_SOURCE_DIR}/external)


target_link_libraries(
	${target_name}
	PRIVATE
	m
	
)
