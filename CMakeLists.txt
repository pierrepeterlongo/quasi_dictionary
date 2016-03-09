project(quasi_dictionnary)

cmake_minimum_required(VERSION 2.8)
################################################################################
# TOOL
################################################################################


# we give the headers directories from : 
#       - from project source 
#       - from BooPHF
include_directories (src  BooPHF)

# we define the files to be compiled
file (GLOB_RECURSE  ProjectFiles  src/*)

# we define the artifact to be built: the project binary
add_executable        (${PROJECT_NAME}  ${ProjectFiles})

# we define which libraries to be linked with project binary
target_link_libraries (${PROJECT_NAME})


