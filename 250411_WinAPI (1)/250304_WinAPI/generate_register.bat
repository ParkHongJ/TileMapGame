@echo off
REM ���̽� ��� Ȯ�� �ʿ� �� �Ʒ� ����
python ..\MapTool\ObjectParser.py

IF %ERRORLEVEL% NEQ 0 (
    echo [ERROR] Failed to generate object register.
    exit /b %ERRORLEVEL%
)