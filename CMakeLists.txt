cmake_minimum_required(VERSION 3.16)
project(week07_pipe C)

set(CMAKE_C_STANDARD 11)
add_compile_options(-Wall -Wextra)

add_executable(1_pipe 1_pipe.c)
add_executable(2_pipeline 2_pipeline.c)
add_executable(3_fifo_read 3_fifo_read.c)
add_executable(4_fifo_write 4_fifo_write.c)
