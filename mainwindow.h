#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLineEdit>
#include <QSpinBox>
#include <QDateEdit>
#include <QPushButton>
#include <QTableWidget>
#include <QLabel>
#include "hotelbookingsystem.h"

class MainWindow : public QMainWindow
{
private:
    HotelBookingSystem hotelSystem;
    QVector<Room> currentSearchResults;

    QLineEdit* guestNameEdit;
    QSpinBox* personsSpinBox;
    QDateEdit* startDateEdit;
    QDateEdit* endDateEdit;
    QPushButton* searchButton;
    QPushButton* bookButton;
    QTableWidget* availableRoomsTable;
    QTableWidget* bookingsTable;
    QLabel* statusLabel;

    void setupUi();
    void setupDemoData();
    void searchRooms();
    void bookSelectedRoom();
    void updateAvailableRoomsTable();
    void updateBookingsTable();
    int selectedRoomNumber() const;

public:
    explicit MainWindow(QWidget* parent = nullptr);
};

#endif // MAINWINDOW_H
