cmake_minimum_required(VERSION 3.24)
project(module)

add_library(module STATIC)

target_sources(module PRIVATE
    module.cc
)

# These are public so Module can be inlined into AudioStream
target_include_directories(module PUBLIC 
    ./
    ../board/
    ..
)

target_link_libraries(module PRIVATE cpputil)

target_compile_options(module PRIVATE 
	-fno-function-sections 
	-fno-data-sections
)

target_compile_definitions(module PRIVATE 
)

set_target_properties(module PROPERTIES CXX_STANDARD 23)



