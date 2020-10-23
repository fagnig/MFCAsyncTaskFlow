@echo off
echo Building Doxygen documentation
cd src
doxygen doxycfg
echo Finished building documentation
cd ..
pause