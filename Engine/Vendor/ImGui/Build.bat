SET cppFiles= ./*.cpp ./backends/*.cpp

g++ -g -shared -I. -I../GLFW -lglfw3 -L../../../Bin %cppFiles% -o ../../../Bin/ImGui.dll 