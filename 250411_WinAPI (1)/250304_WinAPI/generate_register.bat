@echo off
echo [INFO] ���� ����!
echo ���� ���: %CD%

if exist "..\MapTool\dist\ObjectParser.exe" (
    echo [INFO] ObjectParser.exe ã��
    ..\MapTool\dist\ObjectParser.exe
    echo [INFO] ���� �Ϸ�. ERRORLEVEL=%ERRORLEVEL%
) else (
    echo [ERROR] ObjectParser.exe ����!
    exit /b 1
)

cd /d "%~dp0"
REM ���̽� ��� Ȯ�� �ʿ� �� �Ʒ� ����
..\MapTool\dist\ObjectParser.exe

IF %ERRORLEVEL% NEQ 0 (
    echo [ERROR] Failed to generate object register.
    exit /b %ERRORLEVEL%
)