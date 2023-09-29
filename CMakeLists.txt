cmake_minimum_required(VERSION 3.26)
project(DeselbyStd)

set(CMAKE_CXX_STANDARD 20)

include_directories(include)

add_executable(DeselbyStd
        include/MutableCategoricalArray.h
        include/random.h
        include/runtimetupleutils.h
        include/stlstream.h
        include/typeutils.h
        test/main.cpp)
