#!/bin/bash

# Compiler options
CC=g++
CFLAGS="-O1 -Wall -std=c++17 -Wno-missing-braces"

# Paths
INCLUDE_DIR="include/"
LIB_DIR="lib/"

# Files
CODE_FILES=$(find src/ -name "*.cpp")

# Build command
$CC $CODE_FILES $LIB_DIR/libraylib.mac.a -o app $CFLAGS -I $INCLUDE_DIR -L $LIB_DIR -lraylib.mac -framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo

# This copies all the needed things for the app and puts it in
# /Users/sher.jesse/temp_app
# This allows for it to be ran without the OneDrive problem
DIR_NAME=$(basename "$(pwd)")
mkdir /Users/sher.jesse/temp_app/$DIR_NAME
rsync -av \
  --exclude 'src' \
  --exclude 'include' \
  --exclude 'lib' \
  . "/Users/sher.jesse/temp_app/$DIR_NAME/app"
/Users/sher.jesse/temp_app/$DIR_NAME/app