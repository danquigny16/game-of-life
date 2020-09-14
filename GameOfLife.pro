# Qt modules
QT += widgets multimedia

# g++ version
QMAKE_CXXFLAGS += -std=c++11

# Executable name
TARGET = gameOfLife

# Folders
SRC = "src"
BUILD = "build"

# Dependance and include path
DEPENDPATH += .
INCLUDEPATH += .

# Target file directory
DESTDIR = .

# Intermediate object files directory
OBJECTS_DIR = $${BUILD}

# Intermediate moc files directory
MOC_DIR= $${BUILD}

################################################################################

HEADERS += $${SRC}/GameOfLife.hpp
SOURCES += $${SRC}/GameOfLife.cpp

HEADERS += $${SRC}/CellItem.hpp
SOURCES += $${SRC}/CellItem.cpp

HEADERS += $${SRC}/CellGrid.hpp
SOURCES += $${SRC}/CellGrid.cpp

HEADERS += $${SRC}/RleHandler.hpp
SOURCES += $${SRC}/RleHandler.cpp

SOURCES += $${SRC}/main.cpp
