@echo off

xcopy "Engine\Assets" "Bin\EngineResources" /s /i /e /y
xcopy "GameAssets" "Bin\Assets" /s /i /e /y