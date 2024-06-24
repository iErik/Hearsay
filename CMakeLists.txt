cmake_minimum_required(VERSION 3.6)

project(Slack.c
  LANGUAGES C
  VERSION 1.0
)

add_executable(Slack
  src/main.cpp
  src/debug.cpp
  src/scene.cpp
  src/draw.cpp
  src/utils.cpp
)

target_include_directories(OpenGLTest
  PRIVATE include/
)
