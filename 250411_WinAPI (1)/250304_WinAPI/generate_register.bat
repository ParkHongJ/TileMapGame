@echo off
cd /d "%~dp0"
REM ���̽� ��� Ȯ�� �ʿ� �� �Ʒ� ����
..\MapTool\dist\ObjectParser.exe

IF %ERRORLEVEL% NEQ 0 (
    echo [ERROR] Failed to generate object register.
    exit /b %ERRORLEVEL%
)