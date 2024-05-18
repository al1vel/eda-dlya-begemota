cmake_minimum_required(VERSION 3.28)
project(kp6)


set(CMAKE_CXX_STANDARD 20)

add_executable(kp6 main.cpp
        person.h
        grades.h
        student_database.h
        functions.h)
