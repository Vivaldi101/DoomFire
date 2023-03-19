@echo off

IF NOT EXIST ..\build mkdir ..\build
pushd ..\build
cl -MT -nologo -Gm- -GR- -EHa- -Ox -Oi -Zi -FC -W3 -wd4201 -wd4100 -wd4189 ../code/main.c /link -incremental:no -opt:ref user32.lib gdi32.lib winmm.lib /out:fire.exe
popd
