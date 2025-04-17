@echo off
cd /d "%~dp0"
REM 파이썬 경로 확인 필요 시 아래 수정
..\MapTool\dist\ObjectParser.exe

IF %ERRORLEVEL% NEQ 0 (
    echo [ERROR] Failed to generate object register.
    exit /b %ERRORLEVEL%
)