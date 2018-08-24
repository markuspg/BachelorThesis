QT -= core gui

TARGET = project

TEMPLATE = app

HEADERS += \
    src/bound_calculator.h \
    src/enums.h \
    src/improver.h \
    src/machine.h \
    src/object.h \
    src/problem.h \
    src/process.h \
    src/scheduler.h \
    src/tempstorage.h \

SOURCES += \
    src/bound_calculator.cpp \
    src/improver.cpp \
    src/machine.cpp \
    src/main.cpp \
    src/object.cpp \
    src/problem.cpp \
    src/process.cpp \
    src/scheduler.cpp \
    src/tempstorage.cpp \
