# Tetris bot
Badly implemented tetris bot

## How to build
### Install SFML
This program use SFML to create the tetris client. Download the latest version of SFML here: https://www.sfml-dev.org/download.php
### Build the tetris client
- Clone the repository.
- In [`tetris_sfml/CMakeLists.txt`](tetris_sfml/CMakeLists.txt), set your SFML directory `set(SFML_DIR "<insert-your-sfml-dir-here>/lib/cmake/SFML")`.
- `cd` to your repository, `mkdir build`, `cd build` and `cmake ..` to build. The binary can be found in the `build` folder.
- Remember to copy the [`tetris_sfml/Content`](tetris_sfml/Content) folder to the `build` directory, the tetris client needs those files.
