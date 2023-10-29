@ECHO OFF

g++ -g -shared -IAssets/Scripts Assets/Scripts/PlatfomerScripts/*.cpp -I../Engine/Source -D_DEBUG -L. -lEngine -o GameLibrary.dll 