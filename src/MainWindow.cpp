#include "MainWindow.h"
#include <QApplication>

MainWindow::MainWindow(Hotel* hotel, BookingManager* manager, QWidget* parent)
    : QMainWindow(parent), hotel(hotel), manager(manager) {
    setWindowTitle("Hotel Stuttgart — Verwaltung (Admin)");
    setMinimumSize(950, 620);

    tabs = new QTabWidget(this);
    tabs->addTab(createRoomTab(),    "Zimmer");
    tabs->addTab(createGuestTab(),   "Gaeste");
    tabs->addTab(createBookingTab(), "Buchung erstellen");
    tabs->addTab(createManageTab(),  "Buchungen verwalten");
    setCentralWidget(tabs);

    // Fix 5: Logout Toolbar
    QToolBar* toolbar = addToolBar("Main");
    toolbar->setMovable(false);
    QWidget* spacer = new QWidget();
    spacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    toolbar->addWidget(spacer);
    QLabel* adminLabel = new QLabel("  <b>Admin</b>  ");
    toolbar->addWidget(adminLabel);
    QPushButton* btnLogout = new QPushButton("Ausloggen");
    btnLogout->setStyleSheet("background-color:#c0392b; color:white; font-weight:bold; border-radius:4px; padding:4px 12px;");
    toolbar->addWidget(btnLogout);
    connect(btnLogout, &QPushButton::clicked, this, [this](){
        QApplication::quit();
    });

    refreshRoomTable();
}

MainWindow::~MainWindow() {}

// ── Tab 1: Zimmer ──
QWidget* MainWindow::createRoomTab() {
    QWidget* w = new QWidget();
    QVBoxLayout* layout = new QVBoxLayout(w);

    roomTable = new QTableWidget(0, 4);
    roomTable->setHorizontalHeaderLabels({"Zimmer-Nr.", "Betten", "Preis/Nacht (EUR)", "Status"});
    roomTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    roomTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    roomTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    layout->addWidget(roomTable);

    QGroupBox* form = new QGroupBox("Neues Zimmer hinzufuegen");
    QHBoxLayout* fl = new QHBoxLayout(form);
    inputRoomNumber = new QSpinBox(); inputRoomNumber->setRange(1,999); inputRoomNumber->setValue(301);
    inputBeds       = new QSpinBox(); inputBeds->setRange(1,10); inputBeds->setValue(2);
    inputPrice      = new QDoubleSpinBox(); inputPrice->setRange(10,9999); inputPrice->setValue(100); inputPrice->setSuffix(" EUR");
    btnAddRoom = new QPushButton("Zimmer hinzufuegen"); btnAddRoom->setFixedHeight(32);
    fl->addWidget(new QLabel("Nr:")); fl->addWidget(inputRoomNumber);
    fl->addWidget(new QLabel("Betten:")); fl->addWidget(inputBeds);
    fl->addWidget(new QLabel("Preis:")); fl->addWidget(inputPrice);
    fl->addWidget(btnAddRoom);
    layout->addWidget(form);

    connect(btnAddRoom, &QPushButton::clicked, this, &MainWindow::onAddRoom);
    return w;
}

void MainWindow::onAddRoom() {
    int nr = inputRoomNumber->value();
    if (hotel->getRoomByNumber(nr)) {
        QMessageBox::warning(this,"Fehler","Zimmer-Nr. "+QString::number(nr)+" existiert bereits!"); return;
    }
    hotel->addRoom(nr, inputBeds->value(), inputPrice->value());
    refreshRoomTable();
}

void MainWindow::refreshRoomTable() {
    auto rooms = hotel->getAllRooms();
    roomTable->setRowCount((int)rooms.size());
    for (int i = 0; i < (int)rooms.size(); i++) {
        Room* r = rooms[i];
        roomTable->setItem(i,0,new QTableWidgetItem(QString::number(r->getRoomNumber())));
        roomTable->setItem(i,1,new QTableWidgetItem(QString::number(r->getBeds())));
        roomTable->setItem(i,2,new QTableWidgetItem(QString::number(r->getPricePerNight(),'f',2)));
        // Fix 4: Status aus Buchungen lesen
        Booking* b = manager->getActiveBookingForRoom(r->getRoomNumber());
        QString statusText = b ? QString("Belegt bis %1").arg(QString::fromStdString(b->getCheckOut())) : "Frei";
        QTableWidgetItem* s = new QTableWidgetItem(statusText);
        s->setForeground(b ? Qt::red : Qt::darkGreen);
        roomTable->setItem(i,3,s);
    }
}

// ── Tab 2: Gaeste (Fix 3: Löschen) ──
QWidget* MainWindow::createGuestTab() {
    QWidget* w = new QWidget();
    QVBoxLayout* layout = new QVBoxLayout(w);

    guestTable = new QTableWidget(0, 3);
    guestTable->setHorizontalHeaderLabels({"Name", "Email", "Telefon"});
    guestTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    guestTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    guestTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    layout->addWidget(guestTable);

    QGroupBox* form = new QGroupBox("Neuen Gast hinzufuegen");
    QHBoxLayout* fl = new QHBoxLayout(form);
    inputGuestName  = new QLineEdit(); inputGuestName->setPlaceholderText("Name");
    inputGuestEmail = new QLineEdit(); inputGuestEmail->setPlaceholderText("Email");
    inputGuestPhone = new QLineEdit(); inputGuestPhone->setPlaceholderText("Telefon");
    btnAddGuest     = new QPushButton("Gast hinzufuegen"); btnAddGuest->setFixedHeight(32);
    fl->addWidget(new QLabel("Name:"));  fl->addWidget(inputGuestName);
    fl->addWidget(new QLabel("Email:")); fl->addWidget(inputGuestEmail);
    fl->addWidget(new QLabel("Tel:"));   fl->addWidget(inputGuestPhone);
    fl->addWidget(btnAddGuest);
    layout->addWidget(form);

    // Fix 3: Löschen Button
    btnDeleteGuest = new QPushButton("Ausgewaehlten Gast loeschen");
    btnDeleteGuest->setFixedHeight(32);
    btnDeleteGuest->setStyleSheet("background-color:#c0392b; color:white; font-weight:bold; border-radius:4px;");
    layout->addWidget(btnDeleteGuest);

    connect(btnAddGuest,    &QPushButton::clicked, this, &MainWindow::onAddGuest);
    connect(btnDeleteGuest, &QPushButton::clicked, this, &MainWindow::onDeleteGuest);
    return w;
}

void MainWindow::onAddGuest() {
    QString name = inputGuestName->text().trimmed();
    if (name.isEmpty()) { QMessageBox::warning(this,"Fehler","Name ist Pflichtfeld!"); return; }
    manager->addGuest(name.toStdString(), inputGuestEmail->text().toStdString(), inputGuestPhone->text().toStdString());
    refreshGuestTable(); refreshGuestCombo();
    inputGuestName->clear(); inputGuestEmail->clear(); inputGuestPhone->clear();
}

// Fix 3: Gast löschen
void MainWindow::onDeleteGuest() {
    int row = guestTable->currentRow();
    if (row < 0) { QMessageBox::warning(this,"Fehler","Bitte einen Gast auswaehlen!"); return; }
    QString name = guestTable->item(row,0)->text();
    auto reply = QMessageBox::question(this,"Loeschen?",
                                       "Gast '"+name+"' wirklich loeschen?", QMessageBox::Yes|QMessageBox::No);
    if (reply == QMessageBox::Yes) {
        manager->removeGuest(name.toStdString());
        refreshGuestTable(); refreshGuestCombo();
    }
}

void MainWindow::refreshGuestTable() {
    auto guests = manager->getAllGuests();
    guestTable->setRowCount((int)guests.size());
    for (int i = 0; i < (int)guests.size(); i++) {
        guestTable->setItem(i,0,new QTableWidgetItem(QString::fromStdString(guests[i]->getName())));
        guestTable->setItem(i,1,new QTableWidgetItem(QString::fromStdString(guests[i]->getEmail())));
        guestTable->setItem(i,2,new QTableWidgetItem(QString::fromStdString(guests[i]->getPhone())));
    }
}

void MainWindow::refreshGuestCombo() {
    comboGuest->clear();
    for (auto g : manager->getAllGuests())
        comboGuest->addItem(QString::fromStdString(g->getName()));
}

// ── Tab 3: Buchung erstellen (Fix 2: exakte Betten + Datum) ──
QWidget* MainWindow::createBookingTab() {
    QWidget* w = new QWidget();
    QVBoxLayout* layout = new QVBoxLayout(w);

    QGroupBox* searchBox = new QGroupBox("Zimmer suchen");
    QHBoxLayout* sl = new QHBoxLayout(searchBox);
    inputPersons   = new QSpinBox(); inputPersons->setRange(1,10); inputPersons->setValue(1);
    adminSearchIn  = new QDateEdit(QDate::currentDate()); adminSearchIn->setCalendarPopup(true);
    adminSearchOut = new QDateEdit(QDate::currentDate().addDays(3)); adminSearchOut->setCalendarPopup(true);
    btnSearch = new QPushButton("Suchen"); btnSearch->setFixedHeight(32);
    sl->addWidget(new QLabel("Personen:")); sl->addWidget(inputPersons);
    sl->addWidget(new QLabel("Von:"));     sl->addWidget(adminSearchIn);
    sl->addWidget(new QLabel("Bis:"));     sl->addWidget(adminSearchOut);
    sl->addWidget(btnSearch); sl->addStretch();
    layout->addWidget(searchBox);

    searchResultTable = new QTableWidget(0, 4);
    searchResultTable->setHorizontalHeaderLabels({"Zimmer-Nr.","Betten","Preis/Nacht","Status"});
    searchResultTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    searchResultTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    searchResultTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    layout->addWidget(searchResultTable);

    QGroupBox* bookBox = new QGroupBox("Buchung erstellen");
    QVBoxLayout* bl = new QVBoxLayout(bookBox);
    QHBoxLayout* row1 = new QHBoxLayout();
    comboGuest    = new QComboBox();
    inputCheckIn  = new QDateEdit(QDate::currentDate()); inputCheckIn->setCalendarPopup(true);
    inputCheckOut = new QDateEdit(QDate::currentDate().addDays(3)); inputCheckOut->setCalendarPopup(true);
    row1->addWidget(new QLabel("Gast:")); row1->addWidget(comboGuest);
    row1->addWidget(new QLabel("Check-in:")); row1->addWidget(inputCheckIn);
    row1->addWidget(new QLabel("Check-out:")); row1->addWidget(inputCheckOut);

    QHBoxLayout* row2 = new QHBoxLayout();
    labelPrice = new QLabel("Gesamtpreis: -- EUR");
    labelPrice->setStyleSheet("font-weight:bold; font-size:14px; color:#1a6e3c;");
    btnBook = new QPushButton("Jetzt buchen"); btnBook->setFixedHeight(36);
    btnBook->setStyleSheet("background-color:#1a6e3c; color:white; font-weight:bold; border-radius:4px;");
    row2->addWidget(labelPrice); row2->addStretch(); row2->addWidget(btnBook);
    bl->addLayout(row1); bl->addLayout(row2);
    layout->addWidget(bookBox);

    connect(btnSearch, &QPushButton::clicked, this, &MainWindow::onSearch);
    connect(btnBook,   &QPushButton::clicked, this, &MainWindow::onCreateBooking);
    connect(inputCheckIn,        &QDateEdit::dateChanged, this, [this](){ updatePriceLabel(); });
    connect(inputCheckOut,       &QDateEdit::dateChanged, this, [this](){ updatePriceLabel(); });
    connect(searchResultTable, &QTableWidget::itemSelectionChanged, this, [this](){ updatePriceLabel(); });
    return w;
}

// Fix 2: Exakte Betten + Datum für Admin
void MainWindow::onSearch() {
    int persons = inputPersons->value();
    std::string cIn  = adminSearchIn->date().toString("yyyy-MM-dd").toStdString();
    std::string cOut = adminSearchOut->date().toString("yyyy-MM-dd").toStdString();

    auto allRooms = hotel->getAllRooms();
    searchResultTable->setRowCount(0);
    int row = 0;
    for (auto r : allRooms) {
        if (r->getBeds() != persons) continue;
        searchResultTable->insertRow(row);
        searchResultTable->setItem(row,0,new QTableWidgetItem(QString::number(r->getRoomNumber())));
        searchResultTable->setItem(row,1,new QTableWidgetItem(QString::number(r->getBeds())+" Betten"));
        searchResultTable->setItem(row,2,new QTableWidgetItem(QString::number(r->getPricePerNight(),'f',2)+" EUR"));
        bool avail = manager->isRoomAvailableForDates(r->getRoomNumber(), cIn, cOut);
        QTableWidgetItem* s = new QTableWidgetItem(avail ? "Verfuegbar" : "Belegt");
        s->setForeground(avail ? Qt::darkGreen : Qt::red);
        searchResultTable->setItem(row,3,s);
        row++;
    }
    if (row == 0) QMessageBox::information(this,"Keine Zimmer","Keine Zimmer mit "+QString::number(persons)+" Bett(en) vorhanden.");
}

void MainWindow::updatePriceLabel() {
    int row = searchResultTable->currentRow();
    if (row < 0) { labelPrice->setText("Gesamtpreis: -- EUR"); return; }
    double ppn = searchResultTable->item(row,2)->text().replace(" EUR","").toDouble();
    int nights  = inputCheckIn->date().daysTo(inputCheckOut->date());
    if (nights <= 0) { labelPrice->setText("Check-out muss nach Check-in sein!"); return; }
    labelPrice->setText(QString("Gesamtpreis: %1 EUR  (%2 Naechte x %3 EUR)")
                            .arg(ppn*nights,0,'f',2).arg(nights).arg(ppn,0,'f',2));
}

void MainWindow::onCreateBooking() {
    int row = searchResultTable->currentRow();
    if (row < 0) { QMessageBox::warning(this,"Fehler","Bitte ein Zimmer auswaehlen!"); return; }
    if (searchResultTable->item(row,3)->text() == "Belegt") {
        QMessageBox::warning(this,"Belegt","Dieses Zimmer ist im Zeitraum nicht verfuegbar!"); return;
    }
    if (comboGuest->count() == 0) { QMessageBox::warning(this,"Fehler","Bitte zuerst Gast anlegen!"); return; }
    int nights = inputCheckIn->date().daysTo(inputCheckOut->date());
    if (nights <= 0) { QMessageBox::warning(this,"Fehler","Check-out muss nach Check-in liegen!"); return; }
    int roomNr = searchResultTable->item(row,0)->text().toInt();
    std::string cIn  = inputCheckIn->date().toString("yyyy-MM-dd").toStdString();
    std::string cOut = inputCheckOut->date().toString("yyyy-MM-dd").toStdString();
    if (!manager->isRoomAvailableForDates(roomNr, cIn, cOut)) {
        QMessageBox::warning(this,"Fehler","Zimmer ist in diesem Zeitraum bereits belegt!"); return;
    }
    Booking* b = manager->createBooking(roomNr, comboGuest->currentIndex(), cIn, cOut, nights);
    if (!b) { QMessageBox::critical(this,"Fehler","Buchung fehlgeschlagen."); return; }
    refreshRoomTable(); refreshBookingTable(); onSearch();
    QMessageBox::information(this,"Erfolg!",
                             QString("Buchung %1 erstellt!\nGast: %2\nZimmer: %3\nPreis: %4 EUR")
                                 .arg(QString::fromStdString(b->getBookingId()))
                                 .arg(QString::fromStdString(b->getGuest()->getName()))
                                 .arg(b->getRoom()->getRoomNumber())
                                 .arg(b->getTotalPrice(),0,'f',2));
}

// ── Tab 4: Buchungen verwalten ──
QWidget* MainWindow::createManageTab() {
    QWidget* w = new QWidget();
    QVBoxLayout* layout = new QVBoxLayout(w);
    bookingTable = new QTableWidget(0, 6);
    bookingTable->setHorizontalHeaderLabels({"ID","Gast","Zimmer","Check-in","Check-out","Preis/Status"});
    bookingTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    bookingTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    bookingTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    layout->addWidget(bookingTable);
    QHBoxLayout* btnRow = new QHBoxLayout();
    btnCancel = new QPushButton("Buchung stornieren"); btnCancel->setFixedHeight(36);
    btnCancel->setStyleSheet("background-color:#c0392b; color:white; font-weight:bold; border-radius:4px;");
    QPushButton* btnRefresh = new QPushButton("Aktualisieren"); btnRefresh->setFixedHeight(36);
    btnRow->addWidget(btnCancel); btnRow->addWidget(btnRefresh);
    layout->addLayout(btnRow);
    connect(btnCancel,  &QPushButton::clicked, this, &MainWindow::onCancelBooking);
    connect(btnRefresh, &QPushButton::clicked, this, &MainWindow::refreshBookingTable);
    return w;
}

void MainWindow::refreshBookingTable() {
    auto bookings = manager->getAllBookings();
    bookingTable->setRowCount((int)bookings.size());
    for (int i = 0; i < (int)bookings.size(); i++) {
        Booking* b = bookings[i];
        bookingTable->setItem(i,0,new QTableWidgetItem(QString::fromStdString(b->getBookingId())));
        bookingTable->setItem(i,1,new QTableWidgetItem(QString::fromStdString(b->getGuest()->getName())));
        bookingTable->setItem(i,2,new QTableWidgetItem(QString::number(b->getRoom()->getRoomNumber())));
        bookingTable->setItem(i,3,new QTableWidgetItem(QString::fromStdString(b->getCheckIn())));
        bookingTable->setItem(i,4,new QTableWidgetItem(QString::fromStdString(b->getCheckOut())));
        QString st = QString::number(b->getTotalPrice(),'f',2)+" EUR | "+(b->isCancelled()?"Storniert":"Aktiv");
        QTableWidgetItem* item = new QTableWidgetItem(st);
        item->setForeground(b->isCancelled() ? Qt::red : Qt::darkGreen);
        bookingTable->setItem(i,5,item);
    }
}

void MainWindow::onCancelBooking() {
    int row = bookingTable->currentRow();
    if (row < 0) { QMessageBox::warning(this,"Fehler","Bitte eine Buchung auswaehlen!"); return; }
    QString bid = bookingTable->item(row,0)->text();
    auto reply = QMessageBox::question(this,"Stornieren?","Buchung "+bid+" stornieren?",
                                       QMessageBox::Yes|QMessageBox::No);
    if (reply == QMessageBox::Yes) {
        manager->cancelBooking(bid.toStdString());
        refreshBookingTable(); refreshRoomTable();
    }
}