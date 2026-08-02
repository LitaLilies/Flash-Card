cmake_minimum_required(VERSION 3.10)
project(FlashCard)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(flashcard
    main.cpp
    flashcard.cpp
    ui.cpp
)

# Windows specific
if(WIN32)
    target_compile_options(flashcard PRIVATE /utf-8)
endif()
