#pragma once
#include <QMainWindow>
#include <QTabWidget>
#include <QTableWidget>
#include <QLineEdit>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QDateEdit>
#include <QComboBox>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QMessageBox>
#include <QHeaderView>
#include <QToolBar>
#include <QSizePolicy>
#include "Hotel.h"
#include "BookingManager.h"

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit MainWindow(Hotel* hotel, BookingManager* manager, QWidget* parent = nullptr);
    ~MainWindow();
private slots:
    void onAddRoom();
    void refreshRoomTable();
    void onAddGuest();
    void onDeleteGuest();       // Fix 3
    void refreshGuestTable();
    void onSearch();
    void onCreateBooking();
    void refreshBookingTable();
    void onCancelBooking();
private:
    Hotel*          hotel;
    BookingManager* manager;
    QTabWidget*     tabs;
    // Zimmer Tab
    QTableWidget*   roomTable;
    QSpinBox*       inputRoomNumber;
    QSpinBox*       inputBeds;
    QDoubleSpinBox* inputPrice;
    QPushButton*    btnAddRoom;
    // Gaeste Tab
    QTableWidget*   guestTable;
    QLineEdit*      inputGuestName;
    QLineEdit*      inputGuestEmail;
    QLineEdit*      inputGuestPhone;
    QPushButton*    btnAddGuest;
    QPushButton*    btnDeleteGuest; // Fix 3
    // Buchung Tab
    QSpinBox*       inputPersons;
    QDateEdit*      adminSearchIn;  // Fix 2
    QDateEdit*      adminSearchOut; // Fix 2
    QPushButton*    btnSearch;
    QTableWidget*   searchResultTable;
    QComboBox*      comboGuest;
    QDateEdit*      inputCheckIn;
    QDateEdit*      inputCheckOut;
    QLabel*         labelPrice;
    QPushButton*    btnBook;
    // Verwalten Tab
    QTableWidget*   bookingTable;
    QPushButton*    btnCancel;

    QWidget* createRoomTab();
    QWidget* createGuestTab();
    QWidget* createBookingTab();
    QWidget* createManageTab();
    void updatePriceLabel();
    void refreshGuestCombo();
};