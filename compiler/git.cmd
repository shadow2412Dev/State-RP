@echo off

set input_file="%~dp0..\.git\HEAD"
set search_word="ref: refs/heads/"
set output_file="%~dp0..\gamemodes\modules\git_branch"
set prefix_text=#define BRANCH_NAME

for /f "tokens=*" %%a in ('findstr /i %search_word% %input_file%') do set output_string=%%a

set output_string=%output_string:~16%
set output_string=%prefix_text% "%output_string%"

echo %output_string% > %output_file%

setlocal enabledelayedexpansion

set "TZ=Europe/Moscow"
for /f "tokens=1-3 delims=:" %%a in ("%time%") do (
    set /a "hour=%%a", "min=%%b", "sec=%%c"
)
for /f "tokens=1-3 delims=." %%a in ('powershell "(get-date).ToString('dd.MM.yyyy HH:mm:ss')"') do (
    set "day=%%a"
    set "month=%%b"
    set "year=%%c"
)
	set output_string=#define DATE_COMPILER "BUILD: %day%.%month%.%year%"
    echo %output_string% >> %output_file%
)
exit /b