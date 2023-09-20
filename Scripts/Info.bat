@echo off
setlocal enabledelayedexpansion

REM Function to count lines of code in C++ source and header files
set "cpp_lines=0"
for /r "%~1" %%f in (*.cpp) do (
    for /f %%l in ('type "%%f" ^| find /c /v ""') do (
        set /a "cpp_lines+=%%l"
    )
)

set "h_lines=0"
for /r "%~1" %%f in (*.h) do (
    for /f %%l in ('type "%%f" ^| find /c /v ""') do (
        set /a "h_lines+=%%l"
    )
)

REM Function to count the number of files in the folder
set "file_count=0"
for /r "%~1" %%f in (*) do (
    set /a "file_count+=1"
)

set "code_count=0"
set /a "code_count+=%cpp_lines%"
set /a "code_count+=%h_lines%"

REM Display information about the folder
echo Folder: %~1
echo Number of Files: %file_count%
echo Lines of C++ Code: %cpp_lines%
echo Lines of H Code: %h_lines%
echo Lines of C++ and H Code: %code_count%

endlocal
