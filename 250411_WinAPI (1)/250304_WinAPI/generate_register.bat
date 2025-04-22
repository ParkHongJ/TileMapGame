@echo off
echo [INFO] 실행 시작!
echo 현재 경로: %CD%

if exist "..\MapTool\dist\ObjectParser.exe" (
    echo [INFO] ObjectParser.exe 찾음
    ..\MapTool\dist\ObjectParser.exe
    echo [INFO] 실행 완료. ERRORLEVEL=%ERRORLEVEL%
) else (
    echo [ERROR] ObjectParser.exe 없음!
    exit /b 1
)

cd /d "%~dp0"
REM 파이썬 경로 확인 필요 시 아래 수정
..\MapTool\dist\ObjectParser.exe

IF %ERRORLEVEL% NEQ 0 (
    echo [ERROR] Failed to generate object register.
    exit /b %ERRORLEVEL%
)