HEADERS += \
    $$PWD/GameObject.h \
    $$PWD/Position.h

SOURCES += \
    $$PWD/GameObject.cpp \

include(Projectiles/Projectiles.pri)
include(Ships/Ships.pri)
include(Collectables/Collectables.pri)
