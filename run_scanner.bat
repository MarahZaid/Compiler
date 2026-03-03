@echo off
REM ===========================
REM Compile + Run Scanner Program
REM ===========================

REM Compile
g++ main.cpp scanner.cpp token_utils.cpp fd.cpp -o scanner.exe
IF %ERRORLEVEL% NEQ 0 (
    echo Compilation failed!
    pause
    exit /b %ERRORLEVEL%
)

REM Check if user provided input file
IF "%1"=="" (
    echo Usage: run_scanner.bat input_file
    pause
    exit /b 1
)

REM Run program with input file
scanner.exe %1
pause