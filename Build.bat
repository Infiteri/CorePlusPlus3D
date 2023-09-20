@ECHO OFF
CLS

REM Record the start time
SET Start_Time=%TIME%

REM Execute the make commands
make -f "Engine.mak" all
make -f "Sandbox.mak" all

REM Record the end time
SET End_Time=%TIME%

REM Calculate the time difference
CALL :TimeDifference %Start_Time% %End_Time%
ECHO Total Execution Time: %Time_Difference%

GOTO :EOF

:TimeDifference
REM Calculate the time difference between two time values
SETLOCAL
FOR /F "tokens=1-4 delims=:." %%a IN ("%1") DO (
    SET /A "Start_Hour=%%a", "Start_Minute=1%%b-100", "Start_Second=1%%c-100", "Start_Millisecond=1%%d-100"
)
FOR /F "tokens=1-4 delims=:." %%a IN ("%2") DO (
    SET /A "End_Hour=%%a", "End_Minute=1%%b-100", "End_Second=1%%c-100", "End_Millisecond=1%%d-100"
)
SET /A "Hour_Diff=End_Hour-Start_Hour", "Minute_Diff=End_Minute-Start_Minute", "Second_Diff=End_Second-Start_Second", "Millisecond_Diff=End_Millisecond-Start_Millisecond"
IF %Millisecond_Diff% LSS 0 (
    SET /A "Second_Diff=Second_Diff-1", "Millisecond_Diff=Millisecond_Diff+100"
)
IF %Second_Diff% LSS 0 (
    SET /A "Minute_Diff=Minute_Diff-1", "Second_Diff=Second_Diff+60"
)
IF %Minute_Diff% LSS 0 (
    SET /A "Hour_Diff=Hour_Diff-1", "Minute_Diff=Minute_Diff+60"
)
ENDLOCAL & SET "Time_Difference=%Hour_Diff%:%Minute_Diff%:%Second_Diff%.%Millisecond_Diff%"


GOTO :EOF





