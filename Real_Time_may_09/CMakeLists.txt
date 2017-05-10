cmake_minimum_required(VERSION 3.6)
project(Real_Time_may_09)

set(CMAKE_C_STANDARD 99)

set(SOURCE_FILES main.c process.c classifiers.c  collection.c process_testing_data.c
        do_not_use_file.c trial.c training_file.c constants.c training_data_new.c
        consumer.c strides.c features.c turning_file.c)

add_executable(Real_Time_may_09 ${SOURCE_FILES})
