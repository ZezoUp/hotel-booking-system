#include "mainwindow.h"

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QGroupBox>
#include <QHeaderView>
#include <QMessageBox>
#include <QLocale>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
{
    setupUi();
    setupDemoData();
    updateBookingsTable();
}

void MainWindow::setupUi()
{
    setWindowTitle("Hotel Booking System - Qt/C++ MVP");
    resize(900, 650);

    QWidget* central = new QWidget(this);
    QVBoxLayout* mainLayout = new QVBoxLayout(central);

    QLabel* title = new QLabel("Hotel Booking System", this);
    title->setStyleSheet("font-size: 24px; font-weight: bold; margin-bottom: 8px;");
    mainLayout->addWidget(title);

    QGroupBox* inputBox = new QGroupBox("Suche und Buchung", this);
    QFormLayout* formLayout = new QFormLayout(inputBox);

    guestNameEdit = new QLineEdit(this);
    guestNameEdit->setPlaceholderText("z. B. Max Mustermann");

    personsSpinBox = new QSpinBox(this);
    personsSpinBox->setMinimum(1);
    personsSpinBox->setMaximum(6);
    personsSpinBox->setValue(2);

    startDateEdit = new QDateEdit(QDate::currentDate().addDays(1), this);
    startDateEdit->setCalendarPopup(true);
    startDateEdit->setDisplayFormat("dd.MM.yyyy");

    endDateEdit = new QDateEdit(QDate::currentDate().addDays(3), this);
    endDateEdit->setCalendarPopup(true);
    endDateEdit->setDisplayFormat("dd.MM.yyyy");

    formLayout->addRow("Gastname:", guestNameEdit);
    formLayout->addRow("Personenanzahl:", personsSpinBox);
    formLayout->addRow("Anreisedatum:", startDateEdit);
    formLayout->addRow("Abreisedatum:", endDateEdit);

    QHBoxLayout* buttonLayout = new QHBoxLayout();
    searchButton = new QPushButton("Zimmer suchen", this);
    bookButton = new QPushButton("Ausgewähltes Zimmer buchen", this);
    buttonLayout->addWidget(searchButton);
    buttonLayout->addWidget(bookButton);
    formLayout->addRow(buttonLayout);

    mainLayout->addWidget(inputBox);

    QGroupBox* roomsBox = new QGroupBox("Verfügbare Zimmer", this);
    QVBoxLayout* roomsLayout = new QVBoxLayout(roomsBox);
    availableRoomsTable = new QTableWidget(this);
    availableRoomsTable->setColumnCount(4);
    availableRoomsTable->setHorizontalHeaderLabels({"Zimmer", "Betten", "Preis/Nacht", "Gesamtpreis"});
    availableRoomsTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    availableRoomsTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    availableRoomsTable->setSelectionMode(QAbstractItemView::SingleSelection);
    availableRoomsTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    roomsLayout->addWidget(availableRoomsTable);
    mainLayout->addWidget(roomsBox);

    QGroupBox* bookingsBox = new QGroupBox("Aktuelle Buchungen", this);
    QVBoxLayout* bookingsLayout = new QVBoxLayout(bookingsBox);
    bookingsTable = new QTableWidget(this);
    bookingsTable->setColumnCount(5);
    bookingsTable->setHorizontalHeaderLabels({"Gast", "Zimmer", "Von", "Bis", "Gesamtpreis"});
    bookingsTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    bookingsTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    bookingsLayout->addWidget(bookingsTable);
    mainLayout->addWidget(bookingsBox);

    statusLabel = new QLabel("Bereit. Bitte Suchdaten eingeben und auf 'Zimmer suchen' klicken.", this);
    statusLabel->setStyleSheet("padding: 6px;");
    mainLayout->addWidget(statusLabel);

    setCentralWidget(central);

    connect(searchButton, &QPushButton::clicked, this, &MainWindow::searchRooms);
    connect(bookButton, &QPushButton::clicked, this, &MainWindow::bookSelectedRoom);
}

void MainWindow::setupDemoData()
{
    hotelSystem.addRoom(Room(101, 1, 60.0));
    hotelSystem.addRoom(Room(102, 2, 90.0));
    hotelSystem.addRoom(Room(201, 3, 120.0));
    hotelSystem.addRoom(Room(202, 4, 160.0));
    hotelSystem.addRoom(Room(301, 2, 110.0));

    // Eine Beispielbuchung, damit man die Verfügbarkeitsprüfung direkt zeigen kann.
    hotelSystem.bookRoom("Demo Gast", 102, QDate::currentDate().addDays(5), QDate::currentDate().addDays(8));
}

void MainWindow::searchRooms()
{
    QDate startDate = startDateEdit->date();
    QDate endDate = endDateEdit->date();

    if (startDate >= endDate) {
        QMessageBox::warning(this, "Fehler", "Das Abreisedatum muss nach dem Anreisedatum liegen.");
        return;
    }

    int persons = personsSpinBox->value();
    currentSearchResults = hotelSystem.searchAvailableRooms(persons, startDate, endDate);
    updateAvailableRoomsTable();

    statusLabel->setText(QString("%1 verfügbare Zimmer gefunden.").arg(currentSearchResults.size()));
}

void MainWindow::bookSelectedRoom()
{
    QString guestName = guestNameEdit->text().trimmed();
    if (guestName.isEmpty()) {
        QMessageBox::warning(this, "Fehler", "Bitte einen Gastnamen eingeben.");
        return;
    }

    int roomNumber = selectedRoomNumber();
    if (roomNumber == -1) {
        QMessageBox::warning(this, "Fehler", "Bitte zuerst ein Zimmer aus der Tabelle auswählen.");
        return;
    }

    QDate startDate = startDateEdit->date();
    QDate endDate = endDateEdit->date();

    bool success = hotelSystem.bookRoom(guestName, roomNumber, startDate, endDate);
    if (!success) {
        QMessageBox::warning(this, "Buchung nicht möglich", "Das Zimmer ist in diesem Zeitraum nicht verfügbar oder die Eingaben sind ungültig.");
        return;
    }

    QMessageBox::information(this, "Buchung erfolgreich", "Das Zimmer wurde erfolgreich gebucht.");
    currentSearchResults = hotelSystem.searchAvailableRooms(personsSpinBox->value(), startDate, endDate);
    updateAvailableRoomsTable();
    updateBookingsTable();
    statusLabel->setText("Buchung erfolgreich gespeichert. Das Zimmer wird jetzt nicht mehr als verfügbar angezeigt.");
}

void MainWindow::updateAvailableRoomsTable()
{
    availableRoomsTable->setRowCount(currentSearchResults.size());

    QDate startDate = startDateEdit->date();
    QDate endDate = endDateEdit->date();

    for (int row = 0; row < currentSearchResults.size(); ++row) {
        const Room& room = currentSearchResults[row];
        double totalPrice = hotelSystem.calculatePrice(room.getRoomNumber(), startDate, endDate);

        availableRoomsTable->setItem(row, 0, new QTableWidgetItem(QString::number(room.getRoomNumber())));
        availableRoomsTable->setItem(row, 1, new QTableWidgetItem(QString::number(room.getBeds())));
        availableRoomsTable->setItem(row, 2, new QTableWidgetItem(QString::number(room.getPricePerNight(), 'f', 2) + " €"));
        availableRoomsTable->setItem(row, 3, new QTableWidgetItem(QString::number(totalPrice, 'f', 2) + " €"));
    }
}

void MainWindow::updateBookingsTable()
{
    QVector<Booking> bookings = hotelSystem.getBookings();
    bookingsTable->setRowCount(bookings.size());

    for (int row = 0; row < bookings.size(); ++row) {
        const Booking& booking = bookings[row];

        bookingsTable->setItem(row, 0, new QTableWidgetItem(booking.getGuestName()));
        bookingsTable->setItem(row, 1, new QTableWidgetItem(QString::number(booking.getRoomNumber())));
        bookingsTable->setItem(row, 2, new QTableWidgetItem(booking.getStartDate().toString("dd.MM.yyyy")));
        bookingsTable->setItem(row, 3, new QTableWidgetItem(booking.getEndDate().toString("dd.MM.yyyy")));
        bookingsTable->setItem(row, 4, new QTableWidgetItem(QString::number(booking.getTotalPrice(), 'f', 2) + " €"));
    }
}

int MainWindow::selectedRoomNumber() const
{
    QList<QTableWidgetItem*> selectedItems = availableRoomsTable->selectedItems();
    if (selectedItems.isEmpty()) {
        return -1;
    }

    int row = selectedItems.first()->row();
    QTableWidgetItem* roomItem = availableRoomsTable->item(row, 0);
    if (roomItem == nullptr) {
        return -1;
    }

    return roomItem->text().toInt();
}
