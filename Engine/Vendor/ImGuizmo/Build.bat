@ECHO off

g++ -g -shared Engine/Vendor/ImGuizmo/*.cpp -IEngine/Vendor/ImGuizmo -IEngine/Vendor/ImGui -LBin -lImGui -o Bin/ImGuizmo.dll