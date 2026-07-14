cmake_minimum_required(VERSION 3.27)

project(termpad C)

add_executable(${PROJECT_NAME}
  src/Main.c
  src/logger.c
  src/utils.c
  src/stringutils.c
)

target_include_directories(${PROJECT_NAME} PUBLIC "include")
target_include_directories(${PROJECT_NAME} PUBLIC "termbox2")