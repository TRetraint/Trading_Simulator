QT += widgets charts
QMAKE_CXXFLAGS = -std=c++11
QMAKE_LFLAGS = -std=c++11

SOURCES += \
    indicateur.cpp \
    indicateursviewer.cpp \
    main.cpp \
    evolutionviewer.cpp \
    note.cpp \
    pilote.cpp \
    simulation.cpp \
    trading.cpp \
    transactionsviewer.cpp \
    user.cpp \
    volumeviewer.cpp

HEADERS += \
    evolutionviewer.h \
    includes.h \
    indicateur.h \
    indicateursviewer.h \
    note.h \
    pilote.h \
    simulation.h \
    trading.h \
    transactionsviewer.h \
    user.h \
    volumeviewer.h
