QT       += core gui
QT       += openglwidgets
QT       += multimedia
QT       += concurrent


greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

include(Main/Main.pri)
include(UI/UI.pri)
include(Utils/Utils.pri)
include(GameObjects/GameObjects.pri)
include(Game/Game.pri)
include(Weapons/Weapons.pri)
include(Graphics/Graphics.pri)

INCLUDEPATH += "C:/Users/aaron/OneDrive/Tiedostot/Aaro/Personal/Projects/QT/SpaceInvadersQT/Thirdparty/OpenAL/include"
LIBS += -L"C:/Users/aaron/OneDrive/Tiedostot/Aaro/Personal/Projects/QT/SpaceInvadersQT/Thirdparty/OpenAL/libs/Win64" -lOpenAL32

INCLUDEPATH += "C:/Users/aaron/OneDrive/Tiedostot/Aaro/Personal/Projects/QT/SpaceInvadersQT/Thirdparty/libsndfile/include"
LIBS += -L"C:/Users/aaron/OneDrive/Tiedostot/Aaro/Personal/Projects/QT/SpaceInvadersQT/Thirdparty/libsndfile/lib" -llibsndfile-1

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    Resources.qrc
