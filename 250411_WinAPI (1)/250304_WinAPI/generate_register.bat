@echo off
REM 파이썬 경로 확인 필요 시 아래 수정
python ..\MapTool\ObjectParser.py

IF %ERRORLEVEL% NEQ 0 (
    echo [ERROR] Failed to generate object register.
    exit /b %ERRORLEVEL%
)