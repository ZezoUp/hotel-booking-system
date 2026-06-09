#pragma once
#include <QMainWindow>
#include <QTabWidget>
#include <QTableWidget>
#include <QSpinBox>
#include <QDateEdit>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QHeaderView>
#include <QMessageBox>
#include <QToolBar>
#include <QSizePolicy>
#include <QFont>
#include "Hotel.h"
#include "BookingManager.h"
#include "UserManager.h"

class GuestWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit GuestWindow(Hotel* hotel, BookingManager* manager,
                         UserManager* userManager,
                         const std::string& guestName,
                         const std::string& guestEmail,
                         QWidget* parent = nullptr);
private slots:
    void onSearch();
    void onBook();
    void onRoomSelected();
    void onCancelMyBooking();
    void refreshMyBookings();
    void updatePriceLabel();
private:
    Hotel*          hotel;
    BookingManager* manager;
    UserManager*    userManager;
    std::string     guestName;
    std::string     guestEmail;
    int             guestIndex;
    QTabWidget*   tabs;
    QSpinBox*     inputPersons;
    QDateEdit*    searchCheckIn;
    QDateEdit*    searchCheckOut;
    QPushButton*  btnSearch;
    QTableWidget* searchTable;
    QLabel*       labelRoomInfo;
    QLineEdit*    inputGuestPhone;
    QDateEdit*    inputCheckIn;
    QDateEdit*    inputCheckOut;
    QLabel*       labelPrice;
    QPushButton*  btnBook;
    QTableWidget* myBookingsTable;
    QPushButton*  btnCancelMine;
    QWidget* createSearchTab();
    QWidget* createMyBookingsTab();
};