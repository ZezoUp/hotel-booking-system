QT       += core gui widgets
CONFIG   += c++17
TARGET    = HotelBooking
TEMPLATE  = app

INCLUDEPATH += include

HEADERS += \
    include/Guest.h \
    include/Room.h \
    include/Booking.h \
    include/Hotel.h \
    include/BookingManager.h \
    include/UserManager.h \
    include/LoginDialog.h \
    include/DataManager.h \
    include/MainWindow.h \
    include/GuestWindow.h

SOURCES += \
    src/main.cpp \
    src/Guest.cpp \
    src/Room.cpp \
    src/Booking.cpp \
    src/Hotel.cpp \
    src/BookingManager.cpp \
    src/UserManager.cpp \
    src/LoginDialog.cpp \
    src/DataManager.cpp \
    src/MainWindow.cpp \
    src/GuestWindow.cpp