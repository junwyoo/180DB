cmake_minimum_required(VERSION 3.6)
project(Working_System)

set(CMAKE_C_STANDARD 99)

set(SOURCE_FILES main.c process.c classifiers.c  collection.c
        do_not_use_file.c training_file.c constants.c training_data_new.c
        consumer.c strides.c features.c turning_file.c)

add_executable(Working_System
        ${SOURCE_FILES})
