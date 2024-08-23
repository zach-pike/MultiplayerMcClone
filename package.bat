@echo off

mkdir game
cp -r shader/ game/
cp -r resources/ game/

cp glfw3.dll game/
cp build/src/Main.exe game/

tar -a -c -f game.zip game/*
rmdir /S /Q game