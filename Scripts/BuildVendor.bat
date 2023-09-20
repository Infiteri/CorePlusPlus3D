@echo off

mkdir Bin

xcopy /s /y "Engine\Vendor\GLFW\lib\glfw3.dll" "Bin\"

echo Building Glad
cd Engine/Vendor/glad
call Build.bat
cd ../../../
echo Done Building Glad

echo Building ImGui
cd Engine/Vendor/ImGui
call Build.bat
cd ../../../
echo Done Building ImGui