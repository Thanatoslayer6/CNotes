cmake_minimum_required(VERSION 3.5.0)

project(noets VERSION 1.0.0)

cmake_policy(SET CMP0072 NEW)

include_directories(include)

file(GLOB sources src/**.c)

add_executable(noets ${sources})

# Installation
install(TARGETS noets DESTINATION bin)

# Uninstall
add_custom_target("uninstall" COMMENT "Uninstall installed files")
add_custom_command(
    TARGET "uninstall"
    POST_BUILD
    COMMENT "Uninstall files with install_manifest.txt"
    COMMAND xargs rm -vf < install_manifest.txt || echo Nothing in
            install_manifest.txt to be uninstalled!
)