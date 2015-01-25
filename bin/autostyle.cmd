:: Description: This script changes the style format of
::              all the source code of the project.

:: Setup the command line
@echo off
title Autostyle

:: Go to the directory where the script is run
cd /d %~dp0

:: Style and format the source code recursively
astyle --style=allman -C -S -xG -Y -XW -w -f -F -p -xd -k3 -y -xj -c -K -L --suffix=none --recursive ../*.cpp ../*.h ../*.hxx ../*.cxx

:: Notify the user that we have finished
echo.
echo Code styling complete!
echo. 

:: Let the user see the output
pause

