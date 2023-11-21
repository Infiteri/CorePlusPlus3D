@ECHO OFF

g++ -g -shared -IAssets/Scripts Assets/Scripts/PongScripts/*.cpp -I../Engine/Source -D_DEBUG -L. -lEngine -o GameLibrary.dll 