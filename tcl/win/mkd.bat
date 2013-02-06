@echo off
rem RCS: @(#) $Id: mkd.bat 144 2003-02-05 10:56:26Z mdejong $

if exist %1\. goto end

if "%OS%" == "Windows_NT" goto winnt

md %1
if errorlevel 1 goto end

goto success

:winnt
md %1
if errorlevel 1 goto end

:success
echo created directory %1

:end


