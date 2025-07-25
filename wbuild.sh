CODE_FILES=$(find src/ -name "*.cpp")
ENINGE_FILES=$(find include/Engine/ -name "*.cpp")

em++ -o index.html $CODE_FILES $ENGINE_FILES -std=c++17 -Os -Wall ./lib/libraylib.web.a -I. -I include/ -L. -L lib/ -s USE_GLFW=3 --shell-file shell.html -DPLATFORM_WEB