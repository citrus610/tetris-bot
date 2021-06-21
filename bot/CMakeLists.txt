# bot cmake

# set the project name
project (bot)

# create a sources variable with a link to all cpp files to compile
set(SOURCES
    src/bot.cpp
    src/beam.cpp
    src/eval.cpp
    src/node.cpp
    src/bitboard.cpp
    src/definition.cpp
    src/vec.cpp
)

# ad a library with the above sources
add_library(${PROJECT_NAME} ${SOURCES})
add_library(sub::bot ALIAS ${PROJECT_NAME})

target_include_directories( ${PROJECT_NAME}
    PUBLIC 
	${PROJECT_SOURCE_DIR}/include
)

