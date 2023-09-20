SET cppFiles= ./*.cpp ./backends/*.cpp

g++ -g -shared -I. -I../GLFW/include -lglfw3 -L../../../Bin %cppFiles% -o ImGui.dll 