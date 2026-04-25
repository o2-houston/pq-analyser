cmake_minimum_required(VERSION 4.1)
project(pq_analyser C)

set(CMAKE_C_STANDARD 11)

add_executable(pq_analyser
        main.c
        io.c
        waveform.c
        io.h
        waveform.h)