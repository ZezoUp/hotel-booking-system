#include <QApplication>
#include "LoginDialog.h"
#include "MainWindow.h"
#include "GuestWindow.h"
#include "Hotel.h"
#include "BookingManager.h"
#include "DataManager.h"
#include "UserManager.h"

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);
    app.setApplicationName("Hotel Booking System");
    app.setStyle("Fusion");

    Hotel*          hotel       = new Hotel("Hotel Stuttgart", "Koenigstrasse 1, Stuttgart");
    BookingManager* manager     = new BookingManager(hotel);
    UserManager*    userManager = new UserManager();

    bool loaded = DataManager::loadAll(hotel, manager, userManager);
    if (!loaded) {
        // Erste Start — Beispielzimmer
        hotel->addRoom(101, 1, 80.0);
        hotel->addRoom(102, 2, 120.0);
        hotel->addRoom(103, 2, 135.0);
        hotel->addRoom(201, 3, 180.0);
        hotel->addRoom(202, 4, 220.0);
        DataManager::saveAll(hotel, manager, userManager);
    }

    int result = 0;
    while (true) {
        // Login Dialog anzeigen
        LoginDialog login(userManager);
        if (login.exec() != QDialog::Accepted) break;

        if (login.getRole() == UserRole::Admin) {
            MainWindow* win = new MainWindow(hotel, manager);
            win->show();
            result = app.exec();
            DataManager::saveAll(hotel, manager, userManager);
            delete win;
        } else {
            // Gast: sicherstellen dass er in BookingManager existiert
            std::string name  = login.getGuestName();
            std::string email = login.getGuestEmail();
            int gIdx = manager->findGuestIndex(name);
            if (gIdx < 0) {
                manager->addGuest(name, email, "");
                DataManager::saveAll(hotel, manager, userManager);
            }
            GuestWindow* win = new GuestWindow(hotel, manager, userManager, name, email);
            win->show();
            result = app.exec();
            DataManager::saveAll(hotel, manager, userManager);
            delete win;
        }

        // Nach Logout: Login Dialog wieder zeigen (while-Schleife)
        // Wenn App beendet wird (nicht nur Logout), verlassen wir die Schleife
        if (result != 0) break;
    }

    delete manager;
    delete hotel;
    delete userManager;
    return result;
}