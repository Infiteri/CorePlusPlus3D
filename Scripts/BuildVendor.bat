@echo off

mkdir Bin

xcopy /s /y "Engine\Vendor\GLFW\lib\glfw3.dll" "Bin\"
xcopy /s /y "Engine\Vendor\YAML\yaml.dll" "Bin\"
cd Engine/Vendor/glad
call Build.bat
cd ../../../
xcopy /s /y "Engine\Vendor\ImGui\ImGui.dll" "Bin\"
xcopy /s /y "Engine\Vendor\ImGuizmo\ImGuizmo.dll" "Bin\"
xcopy /s /y "Engine\Vendor\YAML\yaml.dll" "Bin\"