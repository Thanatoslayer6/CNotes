cmake_minimum_required(VERSION 3.5.0)

project(noets VERSION 1.0.0)

cmake_policy(SET CMP0072 NEW)

include_directories(include)

file(GLOB sources src/**.c)

add_executable(
    noets
    ${sources}
)



