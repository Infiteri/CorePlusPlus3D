@echo off

echo Building Vendor
Scripts/BuildVendor.bat

echo Copy Assets
Scripts/CopyAssets.bat

echo Build
Build.bat

