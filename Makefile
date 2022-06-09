# variables
CC = g++
LANG_STD = -std=c++17
COMPILER_FLAGS = -Wall -Wfatal-errors
INCLUDE_PATH = -I"./libs/" -I"/opt/homebrew/include/"
SRC_FILES = ./src/*.cpp \
			./src/Game/*.cpp \
			./src/Logger/*.cpp \
			./src/ECS/*.cpp \
			./src/AssetStore/*.cpp \
			./libs/imgui/*.cpp
OBJ_NAME = game
LINKER_FLAGS = -llua -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer

# Rules
build:
	$(CC) $(LANG_STD) $(COMPILER_FLAGS) $(SRC_FILES) $(INCLUDE_PATH) $(LINKER_FLAGS) -o $(OBJ_NAME);

clean:
	rm ./$(OBJ_NAME);

run:
	./$(OBJ_NAME);