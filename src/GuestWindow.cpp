#include "GuestWindow.h"
#include "DataManager.h"
#include <QApplication>
#include <cstdlib>
#include <ctime>

static std::string generateCode() {
    static bool seeded = false;
    if (!seeded) { std::srand((unsigned)std::time(nullptr)); seeded = true; }
    const std::string chars = "ABCDEFGHJKLMNPQRSTUVWXYZ23456789";
    std::string code;
    for (int i = 0; i < 6; i++) code += chars[std::rand() % chars.size()];
    return code;
}

GuestWindow::GuestWindow(Hotel* hotel, BookingManager* manager,
                         UserManager* userManager,
                         const std::string& guestName,
                         const std::string& guestEmail,
                         QWidget* parent)
    : QMainWindow(parent), hotel(hotel), manager(manager),
    userManager(userManager), guestName(guestName), guestEmail(guestEmail) {

    setWindowTitle(QString("Hotel Stuttgart — Willkommen, %1!").arg(QString::fromStdString(guestName)));
    setMinimumSize(850, 550);

    guestIndex = manager->findGuestIndex(guestName);

    tabs = new QTabWidget(this);
    tabs->addTab(createSearchTab(),     "Zimmer suchen & buchen");
    tabs->addTab(createMyBookingsTab(), "Meine Buchungen");
    setCentralWidget(tabs);

    // Toolbar mit Logout
    QToolBar* toolbar = addToolBar("Main");
    toolbar->setMovable(false);
    QWidget* spacer = new QWidget();
    spacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    toolbar->addWidget(spacer);
    QLabel* userLabel = new QLabel(QString("  <b>%1</b> (%2)  ")
                                       .arg(QString::fromStdString(guestName))
                                       .arg(QString::fromStdString(guestEmail)));
    userLabel->setStyleSheet("color:#1a3c5e;");
    toolbar->addWidget(userLabel);
    QPushButton* btnLogout = new QPushButton("Ausloggen");
    btnLogout->setStyleSheet("background-color:#c0392b; color:white; font-weight:bold; border-radius:4px; padding:4px 12px;");
    toolbar->addWidget(btnLogout);
    connect(btnLogout, &QPushButton::clicked, this, [this](){
        DataManager::saveAll(this->hotel, this->manager, this->userManager);
        QApplication::quit();
    });

    refreshMyBookings();
}

QWidget* GuestWindow::createSearchTab() {
    QWidget* w = new QWidget();
    QVBoxLayout* layout = new QVBoxLayout(w);

    QGroupBox* sb = new QGroupBox("Zimmer suchen");
    QHBoxLayout* sl = new QHBoxLayout(sb);
    inputPersons   = new QSpinBox(); inputPersons->setRange(1,10); inputPersons->setValue(1);
    searchCheckIn  = new QDateEdit(QDate::currentDate()); searchCheckIn->setCalendarPopup(true);
    searchCheckOut = new QDateEdit(QDate::currentDate().addDays(2)); searchCheckOut->setCalendarPopup(true);
    btnSearch = new QPushButton("Suchen"); btnSearch->setFixedHeight(32);
    sl->addWidget(new QLabel("Personen:")); sl->addWidget(inputPersons);
    sl->addWidget(new QLabel("Von:"));     sl->addWidget(searchCheckIn);
    sl->addWidget(new QLabel("Bis:"));     sl->addWidget(searchCheckOut);
    sl->addWidget(btnSearch); sl->addStretch();
    layout->addWidget(sb);

    searchTable = new QTableWidget(0, 4);
    searchTable->setHorizontalHeaderLabels({"Zimmer-Nr.", "Betten", "Preis/Nacht", "Status"});
    searchTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    searchTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    searchTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    layout->addWidget(searchTable);

    labelRoomInfo = new QLabel("");
    labelRoomInfo->setStyleSheet("color:#c0392b; font-size:12px; padding:4px;");
    labelRoomInfo->setWordWrap(true);
    layout->addWidget(labelRoomInfo);

    QGroupBox* bb = new QGroupBox("Buchung erstellen");
    QVBoxLayout* bl = new QVBoxLayout(bb);

    // Nur Telefon nötig — Name/Email kommen vom Account
    QHBoxLayout* row0 = new QHBoxLayout();
    inputGuestPhone = new QLineEdit(); inputGuestPhone->setPlaceholderText("Telefonnummer (optional)");
    row0->addWidget(new QLabel("Telefon:")); row0->addWidget(inputGuestPhone); row0->addStretch();

    QHBoxLayout* row1 = new QHBoxLayout();
    inputCheckIn  = new QDateEdit(QDate::currentDate()); inputCheckIn->setCalendarPopup(true);
    inputCheckOut = new QDateEdit(QDate::currentDate().addDays(2)); inputCheckOut->setCalendarPopup(true);
    row1->addWidget(new QLabel("Check-in:")); row1->addWidget(inputCheckIn);
    row1->addWidget(new QLabel("Check-out:")); row1->addWidget(inputCheckOut);
    row1->addStretch();

    QHBoxLayout* row2 = new QHBoxLayout();
    labelPrice = new QLabel("Gesamtpreis: -- EUR");
    labelPrice->setStyleSheet("font-weight:bold; font-size:14px; color:#1a6e3c;");
    btnBook = new QPushButton("Jetzt buchen"); btnBook->setFixedHeight(36);
    btnBook->setStyleSheet("background-color:#1a6e3c; color:white; font-weight:bold; border-radius:4px;");
    row2->addWidget(labelPrice); row2->addStretch(); row2->addWidget(btnBook);

    bl->addLayout(row0); bl->addLayout(row1); bl->addLayout(row2);
    layout->addWidget(bb);

    connect(btnSearch,   &QPushButton::clicked, this, &GuestWindow::onSearch);
    connect(btnBook,     &QPushButton::clicked, this, &GuestWindow::onBook);
    connect(inputCheckIn,  &QDateEdit::dateChanged, this, &GuestWindow::updatePriceLabel);
    connect(inputCheckOut, &QDateEdit::dateChanged, this, &GuestWindow::updatePriceLabel);
    connect(searchTable, &QTableWidget::itemSelectionChanged, this, &GuestWindow::onRoomSelected);
    return w;
}

QWidget* GuestWindow::createMyBookingsTab() {
    QWidget* w = new QWidget();
    QVBoxLayout* layout = new QVBoxLayout(w);
    myBookingsTable = new QTableWidget(0, 6);
    myBookingsTable->setHorizontalHeaderLabels({"ID","Zimmer","Check-in","Check-out","Preis","Check-in Code"});
    myBookingsTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    myBookingsTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    myBookingsTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    layout->addWidget(myBookingsTable);
    btnCancelMine = new QPushButton("Buchung stornieren"); btnCancelMine->setFixedHeight(36);
    btnCancelMine->setStyleSheet("background-color:#c0392b; color:white; font-weight:bold; border-radius:4px;");
    layout->addWidget(btnCancelMine);
    connect(btnCancelMine, &QPushButton::clicked, this, &GuestWindow::onCancelMyBooking);
    return w;
}

void GuestWindow::onSearch() {
    int persons = inputPersons->value();
    std::string cIn  = searchCheckIn->date().toString("yyyy-MM-dd").toStdString();
    std::string cOut = searchCheckOut->date().toString("yyyy-MM-dd").toStdString();
    auto allRooms = hotel->getAllRooms();
    searchTable->setRowCount(0);
    int row = 0;
    for (auto r : allRooms) {
        if (r->getBeds() != persons) continue;
        searchTable->insertRow(row);
        searchTable->setItem(row,0,new QTableWidgetItem(QString::number(r->getRoomNumber())));
        searchTable->setItem(row,1,new QTableWidgetItem(QString::number(r->getBeds())+" Betten"));
        searchTable->setItem(row,2,new QTableWidgetItem(QString::number(r->getPricePerNight(),'f',2)+" EUR"));
        bool avail = manager->isRoomAvailableForDates(r->getRoomNumber(), cIn, cOut);
        QTableWidgetItem* s = new QTableWidgetItem(avail ? "Verfuegbar" : "Belegt");
        s->setForeground(avail ? Qt::darkGreen : Qt::red);
        searchTable->setItem(row,3,s);
        row++;
    }
    labelRoomInfo->setText("");
    if (row == 0)
        QMessageBox::information(this,"Keine Zimmer",
                                 QString("Keine Zimmer mit %1 Bett(en) vorhanden.").arg(persons));
}

void GuestWindow::onRoomSelected() {
    int row = searchTable->currentRow();
    if (row < 0) { labelRoomInfo->setText(""); updatePriceLabel(); return; }
    QString status = searchTable->item(row,3)->text();
    if (status == "Belegt") {
        int roomNr = searchTable->item(row,0)->text().toInt();
        Booking* b = manager->getActiveBookingForRoom(roomNr);
        if (b) labelRoomInfo->setText(
                QString("Zimmer %1 ist belegt von %2 bis %3. Früheste Neubuchung ab 5 Tagen nach Abreise.")
                    .arg(roomNr)
                    .arg(QString::fromStdString(b->getCheckIn()))
                    .arg(QString::fromStdString(b->getCheckOut())));
        btnBook->setEnabled(false);
    } else {
        labelRoomInfo->setText("");
        btnBook->setEnabled(true);
        updatePriceLabel();
    }
}

void GuestWindow::updatePriceLabel() {
    int row = searchTable->currentRow();
    if (row < 0) { labelPrice->setText("Gesamtpreis: -- EUR"); return; }
    if (searchTable->item(row,3) && searchTable->item(row,3)->text() == "Belegt") return;
    double ppn = searchTable->item(row,2)->text().replace(" EUR","").toDouble();
    int nights  = inputCheckIn->date().daysTo(inputCheckOut->date());
    if (nights <= 0) { labelPrice->setText("Check-out muss nach Check-in sein!"); return; }
    labelPrice->setText(QString("Gesamtpreis: %1 EUR  (%2 Naechte x %3 EUR)")
                            .arg(ppn*nights,0,'f',2).arg(nights).arg(ppn,0,'f',2));
}

void GuestWindow::onBook() {
    int row = searchTable->currentRow();
    if (row < 0) { QMessageBox::warning(this,"Fehler","Bitte ein Zimmer auswaehlen!"); return; }
    if (searchTable->item(row,3)->text() == "Belegt") {
        QMessageBox::warning(this,"Belegt","Dieses Zimmer ist im gewaehlten Zeitraum nicht verfuegbar!"); return;
    }
    int nights = inputCheckIn->date().daysTo(inputCheckOut->date());
    if (nights <= 0) { QMessageBox::warning(this,"Fehler","Check-out muss nach Check-in liegen!"); return; }

    // Telefon aktualisieren falls eingegeben
    QString phone = inputGuestPhone->text().trimmed();
    if (!phone.isEmpty()) {
        auto guests = manager->getAllGuests();
        if (guestIndex >= 0 && guestIndex < (int)guests.size())
            guests[guestIndex]->setPhone(phone.toStdString());
    }

    int roomNr = searchTable->item(row,0)->text().toInt();
    std::string cIn  = inputCheckIn->date().toString("yyyy-MM-dd").toStdString();
    std::string cOut = inputCheckOut->date().toString("yyyy-MM-dd").toStdString();

    if (!manager->isRoomAvailableForDates(roomNr, cIn, cOut)) {
        QMessageBox::warning(this,"Fehler","Zimmer ist in diesem Zeitraum bereits belegt!"); return;
    }

    Booking* b = manager->createBooking(roomNr, guestIndex, cIn, cOut, nights);
    if (!b) { QMessageBox::critical(this,"Fehler","Buchung fehlgeschlagen."); return; }

    std::string code = generateCode();
    manager->setBookingCode(b->getBookingId(), code);
    DataManager::saveAll(hotel, manager, userManager);
    refreshMyBookings(); onSearch();

    QMessageBox::information(this,"Buchung erfolgreich!",
                             QString("Buchungs-ID: %1\nZimmer: %2\nPreis: %3 EUR\n\nIhr Check-in Code: %4\nBitte an der Rezeption vorzeigen!")
                                 .arg(QString::fromStdString(b->getBookingId()))
                                 .arg(b->getRoom()->getRoomNumber())
                                 .arg(b->getTotalPrice(),0,'f',2)
                                 .arg(QString::fromStdString(code)));
}

void GuestWindow::refreshMyBookings() {
    auto bookings = manager->getBookingsForGuest(guestName);
    myBookingsTable->setRowCount((int)bookings.size());
    for (int i = 0; i < (int)bookings.size(); i++) {
        Booking* b = bookings[i];
        myBookingsTable->setItem(i,0,new QTableWidgetItem(QString::fromStdString(b->getBookingId())));
        myBookingsTable->setItem(i,1,new QTableWidgetItem(QString::number(b->getRoom()->getRoomNumber())));
        myBookingsTable->setItem(i,2,new QTableWidgetItem(QString::fromStdString(b->getCheckIn())));
        myBookingsTable->setItem(i,3,new QTableWidgetItem(QString::fromStdString(b->getCheckOut())));
        QString preis = QString::number(b->getTotalPrice(),'f',2)+" EUR";
        std::string code = manager->getBookingCode(b->getBookingId());
        QString codeText = b->isCancelled() ? "-- storniert --" : QString::fromStdString(code);
        QTableWidgetItem* preisItem = new QTableWidgetItem(preis);
        QTableWidgetItem* codeItem  = new QTableWidgetItem(codeText);
        if (b->isCancelled()) {
            for (int col=0; col<4; col++)
                if (myBookingsTable->item(i,col)) {
                    myBookingsTable->item(i,col)->setForeground(Qt::gray);
                    myBookingsTable->item(i,col)->setFlags(Qt::ItemIsEnabled);
                }
            preisItem->setForeground(Qt::gray); preisItem->setFlags(Qt::ItemIsEnabled);
            codeItem->setForeground(Qt::gray);  codeItem->setFlags(Qt::ItemIsEnabled);
        } else {
            preisItem->setForeground(Qt::darkGreen);
            codeItem->setForeground(QColor("#1a3c5e"));
            codeItem->setFont(QFont("Courier",11,QFont::Bold));
        }
        myBookingsTable->setItem(i,4,preisItem);
        myBookingsTable->setItem(i,5,codeItem);
    }
}

void GuestWindow::onCancelMyBooking() {
    int row = myBookingsTable->currentRow();
    if (row < 0) { QMessageBox::warning(this,"Fehler","Bitte eine Buchung auswaehlen!"); return; }
    if (myBookingsTable->item(row,5)->text() == "-- storniert --") {
        QMessageBox::warning(this,"Bereits storniert","Diese Buchung wurde bereits storniert!"); return;
    }
    QString bid = myBookingsTable->item(row,0)->text();
    auto reply = QMessageBox::question(this,"Stornieren?","Buchung "+bid+" wirklich stornieren?",
                                       QMessageBox::Yes|QMessageBox::No);
    if (reply == QMessageBox::Yes) {
        manager->cancelBooking(bid.toStdString());
        DataManager::saveAll(hotel, manager, userManager);
        refreshMyBookings();
    }
}