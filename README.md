# game-of-life
A GUI to build and run a Conway's game of life, but also a way to learn and see the difference of performance using different method (thread, AVX, ...)

# Licence
This code is under the LGPL3 license, and use Qt, also under the LGPL3 license

# Qt
Qt is a widely used free and open-source widget toolkit for creating graphical user interfaces,
Go to https://www.qt.io/download and upload the open source version of Qt, if you don't have it

# Use
The first time, in your console, type :
  qmake -o Makefile GameOfLife.pro
Do this again if you modify the "GameOfLife.pro" file, this command create the Makefile

Then to compile, juste type :
  make
