cmake_minimum_required(VERSION 3.17)
project(SortedMultiMap_hashtable_with_separate_chaining)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(SortedMultiMap_hashtable_with_separate_chaining
        App.cpp
        ExtendedTest.cpp
        ExtendedTest.h
        ShortTest.cpp
        ShortTest.h
        SMMIterator.cpp
        SMMIterator.h
        SortedMultiMap.cpp
        SortedMultiMap.h)
