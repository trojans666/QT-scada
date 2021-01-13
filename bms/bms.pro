TEMPLATE = app
TARGET = bms

INCLUDEPATH += src \

HEADERS += \
    src/terror.h \
    src/tsys.h

SOURCES += \
    src/terror.cpp \
    src_call/main.cpp \
    src/tsys.cpp
