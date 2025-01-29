QT       += core gui
QT       += sql
QT       += charts

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    changepassworddialog.cpp \
    databasemanager.cpp \
    main.cpp \
    loginpage.cpp \
    mainwindow.cpp \
    manageavailabilitypage.cpp \
    managecoursesdialog.cpp \
    managecoursespage.cpp \
    managegradespage.cpp \
    manageschedulespage.cpp \
    manageusersdialog.cpp \
    manageuserspage.cpp \
    menupage.cpp \
    mygradespage.cpp \
    myprofilepage.cpp \
    myschedulepage.cpp \
    searchuserdialog.cpp \
    searchuserpage.cpp

HEADERS += \
    changepassworddialog.h \
    databasemanager.h \
    loginpage.h \
    mainwindow.h \
    manageavailabilitypage.h \
    managecoursesdialog.h \
    managecoursespage.h \
    managegradespage.h \
    manageschedulespage.h \
    manageusersdialog.h \
    manageuserspage.h \
    menupage.h \
    mygradespage.h \
    myprofilepage.h \
    myschedulepage.h \
    searchuserdialog.h \
    searchuserpage.h

FORMS += \
    changepassworddialog.ui \
    loginpage.ui \
    mainwindow.ui \
    manageavailabilitypage.ui \
    managecoursesdialog.ui \
    managecoursespage.ui \
    managegradespage.ui \
    manageschedulespage.ui \
    manageusersdialog.ui \
    manageuserspage.ui \
    menupage.ui \
    mygradespage.ui \
    myprofilepage.ui \
    myschedulepage.ui \
    searchuserdialog.ui \
    searchuserpage.ui


# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
