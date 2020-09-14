# game-of-life
A GUI to build and run a Conway's game of life, but also a way to learn and see the difference of performance using different method (thread, AVX, ...)
through the code

# Status
Basic version, just simulate a game of live with cell activated ourself, or with the help of a rle file

# Licence
This code is under the LGPL3 license, and use Qt, also under the LGPL3 license

Basically, if you modify and ditribute this code, you will have to share your new version of the code under LGPL3 too (everyone can study, copy, execute, ... your code)

But if you just use it in your code (e.g a simple "#include ..."), you can distribute it under any terms, and even make it a proprietary software

# Qt
Qt is a widely used free and open-source widget toolkit for creating graphical user interfaces  
You will need at least version 5.9.5  

Go to https://www.qt.io/download and upload the open source version of Qt, if you don't have it  

For Ubuntu's users, you can type : "sudo apt-get install qt5-default"

# rle files
To know how are made the rle files for game of life, tou can go to "https://www.conwaylife.com/wiki/Run_Length_Encoded"

# Use
The first time, in your console, type : "qmake -o Makefile GameOfLife.pro"

Do this again if you modify the "GameOfLife.pro" file or add new "QObject" class. This command create the Makefile

Then to compile, juste type : "make"
