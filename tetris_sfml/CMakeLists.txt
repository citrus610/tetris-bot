# sfml tetris client cmake

# set the project name
project (tetris_sfml)

# link sfml statically
set(SFML_STATIC_LIBRARIES TRUE)

## set sfml direcory
# example:
set(SFML_DIR "F:/SFML-2.5.1/lib/cmake/SFML")
# set(SFML_DIR "<insert-your-sfml-dir-here>/lib/cmake/SFML")

find_package(SFML 2.5 COMPONENTS graphics window audio network system)

# Create a sources variable with a link to all cpp files to compile
set(SOURCES
    src/Key.cpp
    src/Input.cpp
    src/Game.cpp
    src/Piece.cpp
    src/Board.cpp
    src/Game1.cpp
    src/main.cpp
)

# Add an executable with the above sources
add_executable(${PROJECT_NAME} ${SOURCES})

target_link_libraries(${PROJECT_NAME} sfml-graphics sfml-window sfml-audio sfml-network sfml-system )
target_link_libraries(${PROJECT_NAME} sub::bot )

target_include_directories(${PROJECT_NAME}
    PUBLIC
	${PROJECT_SOURCE_DIR}/include
)