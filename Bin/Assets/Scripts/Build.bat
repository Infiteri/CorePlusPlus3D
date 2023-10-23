@ECHO OFF

g++ -g -shared -IAssets/Scripts Assets/Scripts/*.cpp Assets/Scripts/DVDScripts/*.cpp -I../Engine/Source -D_DEBUG -L. -lEngine -o GameLibrary.dll 