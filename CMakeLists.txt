# MAC BUILD

# Your CMakeLists.txt for the main App project
# This time what is new is I have added:
# 1.) another 'add_executable'
# 2.) another 'target_link_libraries'
#
# Observe now you will have two projects setup.
# As they stand, they will be identical.
####################################################



# The minimum required version of CMake to ouse
# cmake_minimum_required(VERSION 3.10)
cmake_minimum_required(VERSION 3.13) # used for relative source paths

# Set the project name to your project name
project(App)
project(Test)



# =====================INCLUDE==============
include_directories("./include/")
include_directories("./tst_include/")
if (MSVC)
  include_directories("C:/SFML-2.5.1/include/")
endif (MSVC)

if (UNIX)
  include_directories("/usr/include/")
endif (UNIX)

if (APPLE)
  find_package(SFML 2.5 COMPONENTS graphics window system REQUIRED)
  set (CMAKE_CXX_STANDARD 11)
endif (APPLE)

# Mac sfml libaries for Nate's computer
# include_directories("/usr/include/")
# set (CMAKE_CXX_STANDARD 11)
# find_package(SFML 2.5 COMPONENTS graphics window system REQUIRED)

# grab all the general files
# =====================LIBRARIES============
if (UNIX AND NOT APPLE)
  link_directories("/usr/lib/x86_64-linux-gnu/")
endif (UNIX AND NOT APPLE)

if (MSVC)
  link_directories("C:/SFML-2.5.1/lib")
endif (MSVC)



# =====================EXECUTABLES==========
# interesting commands
# aux_source_directory("./test/*.cpp" Tst) #???
# get_filename_component(Main_Path ./src/main.cpp ABSOLUTE)
# message("Message")
# message("${CMAKE_CURRENT_SOURCE_DIR}/./src/main.cpp")
# message("${Cpp}")
# note: glob not recommended, since only changes to cmakelists trigger rebuilds
# addition of source files thus requires manual rerun of cmake

# App
file(GLOB Hpp "./include/*.hpp")
file(GLOB Cpp "./src/*.cpp")
add_executable(App ${Hpp} ${Cpp})

# Test
file(GLOB Tst "./test/*.cpp")
file(GLOB Tst_Include "./tst_include/*.hpp")
list(REMOVE_ITEM Cpp "${CMAKE_CURRENT_SOURCE_DIR}/./src/main.cpp")
add_executable(Test ${Hpp} ${Cpp} ${Tst_Include} ${Tst} )




# =====================LINK LIBRARIES=======
if (MSVC)
    target_link_libraries(App sfml-graphics sfml-window sfml-system sfml-network opengl32 winmm gdi32)
    target_link_libraries(Test sfml-graphics sfml-window sfml-system sfml-network opengl32 winmm gdi32)
endif (MSVC)

if (UNIX)
   find_package(OpenGL REQUIRED)
   find_package(GLUT REQUIRED)
   include_directories( ${OPENGL_INCLUDE_DIRS}  ${GLUT_INCLUDE_DIRS} )
   target_link_libraries(App ${OPENGL_LIBRARIES})
   target_link_libraries(App sfml-graphics sfml-window sfml-system sfml-network)
   target_link_libraries(Test ${OPENGL_LIBRARIES})
   target_link_libraries(Test sfml-graphics sfml-window sfml-system sfml-network)
endif (UNIX)
