# 🏨 Hotel Booking System — Sprint Report
**Projekt:** Hotel Booking System  
**Team:** Abdul, Samih, Zain  
**Hochschule Esslingen — Software Engineering (Scrum)**  
**Gesamt-Dauer:** 4 Wochen (2 Sprints à 2 Wochen)

---

## 👥 Rollen

| Person | Rolle |
|--------|-------|
| **Zaen** | Scrum Master (SM) |
| **Abdul** | Developer |
| **Samih** | Developer |

---

## 📋 Product Backlog

| ID | User Story | Priorität |
|----|-----------|-----------|
| US-01 | Als Admin möchte ich Zimmer anlegen und verwalten | 🔴 Hoch |
| US-02 | Als Admin möchte ich Gäste anlegen und verwalten | 🔴 Hoch |
| US-03 | Als Gast möchte ich Zimmer nach Personen und Datum suchen | 🔴 Hoch |
| US-04 | Als Gast möchte ich ein Zimmer buchen mit Preisberechnung | 🔴 Hoch |
| US-05 | Als Admin möchte ich alle Buchungen sehen und stornieren | 🟡 Mittel |
| US-06 | Als Gast möchte ich meine eigenen Buchungen sehen und stornieren | 🟡 Mittel |
| US-07 | Als System sollen alle Daten nach Neustart noch vorhanden sein | 🔴 Hoch |
| US-08 | Als Gast möchte ich mich registrieren und einloggen können | 🟡 Mittel |
| US-09 | Als Gast möchte ich nach Buchung einen Check-in Code erhalten | 🟢 Niedrig |
| US-10 | Als Gast möchte ich das UI übersichtlich und benutzerfreundlich finden | 🟢 Niedrig |
| US-11 | Als Admin möchte ich Statistiken zur Auslastung sehen | 🟢 Niedrig |

---

---

# 🏃 Sprint 1 — Grundgerüst
**Zeitraum:** Woche 1–2  
**Ziel:** Lauffähige Basis mit Zimmer- und Buchungsverwaltung

## Sprint 1 Backlog
US-01, US-02, US-04, US-07

---

### 🏗️ Architektur & Projektstruktur

| Aufgabe | Status | Erledigt von |
|---------|--------|-------------|
| Anforderungen analysieren und User Stories erstellen | ✅ Erledigt | Z. |
| Klassendiagramm erstellen (Hotel, Room, Guest, Booking, BookingManager) | ✅ Erledigt | A. |
| Qt Creator Projekt aufsetzen (.pro Datei, Ordnerstruktur `include/` + `src/`) | ✅ Erledigt | A. |
| GitLab Repository einrichten und alle ins Team einladen | ✅ Erledigt | Z. |
| Definition of Done festlegen | ✅ Erledigt | S. |

---

### 🛏️ US-01: Zimmer verwalten

| Aufgabe | Status | Erledigt von |
|---------|--------|-------------|
| Klasse `Room` implementieren (Nummer, Betten, Preis/Nacht) | ✅ Erledigt | S. |
| Klasse `Hotel` implementieren (Zimmer hinzufügen, abrufen) | ✅ Erledigt | S. |
| Admin GUI: Zimmer-Tabelle anzeigen | ✅ Erledigt | A. |
| Admin GUI: Neues Zimmer hinzufügen (Formular) | ✅ Erledigt | A. |

> 🐛 **Bug gefunden:** Zimmer-Status wurde nach Buchung nicht aktualisiert — Zimmer zeigte immer "Frei" an.  
> ✅ **Bug gelöst (A.):** Status wird jetzt direkt aus den Buchungsdaten berechnet statt als Boolean-Attribut gespeichert.

| Aufgabe | Status | Erledigt von |
|---------|--------|-------------|
| Zimmer-Status (Frei/Belegt) korrekt anzeigen | ✅ Erledigt | A. |

---

### 👤 US-02: Gäste verwalten

| Aufgabe | Status | Erledigt von |
|---------|--------|-------------|
| Klasse `Guest` implementieren (Name, Email, Telefon) | ✅ Erledigt | Z. |
| `BookingManager` implementieren (zentrale Steuerklasse) | ✅ Erledigt | S. |
| Admin GUI: Gäste-Tabelle anzeigen | ✅ Erledigt | Z. |
| Admin GUI: Neuen Gast hinzufügen | ✅ Erledigt | Z. |

> 🐛 **Bug gefunden:** Gast konnte mehrfach mit gleichem Namen angelegt werden — führte zu falschen Buchungszuweisungen.  
> ✅ **Bug gelöst (Z.):** Validierung hinzugefügt, doppelte Namen werden mit Fehlermeldung abgewiesen.

---

### 📅 US-04: Zimmer buchen

| Aufgabe | Status | Erledigt von |
|---------|--------|-------------|
| Klasse `Booking` implementieren (ID, Zimmer, Gast, Datum, Preis) | ✅ Erledigt | Z. |
| Buchung erstellen mit automatischer Preisberechnung (Nächte × Preis) | ✅ Erledigt | Z. |
| Preisvorschau vor Buchung anzeigen | ✅ Erledigt | A. |
| Admin GUI: Buchung erstellen | ✅ Erledigt | A. |

> 🐛 **Bug gefunden:** Preisberechnung zeigte falschen Betrag wenn Check-out vor Check-in lag.  
> ✅ **Bug gelöst (S.):** Validierung hinzugefügt — negative Nächteanzahl wird abgefangen.

---

### 💾 US-07: Datenpersistenz

| Aufgabe | Status | Erledigt von |
|---------|--------|-------------|
| `DataManager` implementieren — Speichern aller Daten in `hotel_data.json` | ✅ Erledigt | A. |
| Daten beim App-Start automatisch laden | ✅ Erledigt | A. |
| Zimmer, Gäste und Buchungen korrekt serialisieren | ✅ Erledigt | A. |

> 🐛 **Bug gefunden:** Nach Neustart der App waren alle Buchungen verschwunden — JSON wurde gespeichert aber nicht korrekt geladen.  
> ✅ **Bug gelöst (A.):** JSON-Parser korrigiert, Felder wurden in falscher Reihenfolge eingelesen.

> 🐛 **Bug gefunden:** App stürzte ab (`std::length_error`) wenn alte JSON-Datei ein anderes Format hatte.  
> ✅ **Bug gelöst (A.):** Fehlerbehandlung im Parser verbessert — ungültige oder fehlende Felder werden übersprungen.

---

## 🔄 Sprint 1 Review

**Vorgeführt:** Zimmer anlegen, Gast anlegen, Buchung erstellen, Daten bleiben nach Neustart erhalten.

**Feedback vom Product Owner (Zain):**
- Grundfunktionen laufen stabil ✅
- Suche nach Datum fehlt noch → wird in Sprint 2 umgesetzt
- Login/Registrierung fehlt noch → wird in Sprint 2 umgesetzt

## 🔄 Sprint 1 Retrospektive

| Was lief gut? | Was kann verbessert werden? |
|--------------|----------------------------|
| Klare Aufgabenteilung | Früher mit Tests anfangen |
| Bugs schnell gefunden | Mehr Absprache bei Klassendesign |
| Datenpersistenz früh eingebaut | UI-Design früher planen |

---

---

# 🏃 Sprint 2 — Features & Fertigstellung
**Zeitraum:** Woche 3–4  
**Ziel:** Login-System, Gasteansicht, Datumsuche, Bugfixes, UI-Verbesserungen

## Sprint 2 Backlog
US-03, US-05, US-06, US-08, US-09

---

### 🔐 US-08: Login & Registrierung

| Aufgabe | Status | Erledigt von |
|---------|--------|-------------|
| `UserManager` implementieren (Accounts mit Email + Passwort) | ✅ Erledigt | Z. |
| Login-Dialog mit Email + Passwort | ✅ Erledigt | Z. |
| Registrierungs-Seite (Name, Email, Telefon, Passwort) | ✅ Erledigt | Z. |
| Validierung: Email-Format, Passwort min. 4 Zeichen, Passwörter übereinstimmend | ✅ Erledigt | Z. |
| Admin-Login (admin@hotel.de / admin123) | ✅ Erledigt | A. |
| Rollenbasierte Ansicht — Admin sieht alles, Gast nur eigene Daten | ✅ Erledigt | A. |

> 🐛 **Bug gefunden:** Login-Textfelder waren unsichtbar — weißer Text auf weißem Hintergrund.  
> ✅ **Bug gelöst (Z.):** Stylesheet korrigiert: `background-color:white; color:black` explizit gesetzt.

> 🐛 **Bug gefunden:** Login-Fenster war zu klein — Eingabefelder wurden nur zu 30% angezeigt.  
> ✅ **Bug gelöst (Z.):** Fenstergröße von 400×300 auf 420×500 angepasst.

| Aufgabe | Status | Erledigt von |
|---------|--------|-------------|
| User-Accounts in JSON speichern und beim Start laden | ✅ Erledigt | A. |
| Nach Logout wieder zum Login-Fenster zurückkehren | ✅ Erledigt | A. |

---

### 🔍 US-03: Zimmer suchen mit Datum

| Aufgabe | Status | Erledigt von |
|---------|--------|-------------|
| `isRoomAvailableForDates()` implementieren — Datum-Überschneidungsprüfung | ✅ Erledigt | S. |
| Suche zeigt nur Zimmer mit exakt passender Bettanzahl | ✅ Erledigt | S. |
| Zimmer werden immer angezeigt — Grün = verfügbar, Rot = belegt | ✅ Erledigt | A. |
| Info bei belegtem Zimmer: von wann bis wann belegt | ✅ Erledigt | A. |
| 5 Tage Puffer nach Abreise vor Neubuchung | ✅ Erledigt | S. |

> 🐛 **Bug gefunden:** Bei Suche nach 1 Person wurden alle Zimmer angezeigt (auch 3-Bett-Zimmer).  
> ✅ **Bug gelöst (S.):** Vergleich von `>=` auf `==` geändert.

> 🐛 **Bug gefunden:** Doppelbuchungen waren möglich — zwei Gäste konnten dasselbe Zimmer für denselben Zeitraum buchen.  
> ✅ **Bug gelöst (S.):** Datum-Überschneidungsprüfung vor jeder Buchung eingebaut.

---

### 📋 US-05 & US-06: Buchungen verwalten

| Aufgabe | Status | Erledigt von |
|---------|--------|-------------|
| Admin: Alle Buchungen in Tabelle anzeigen | ✅ Erledigt | S. |
| Admin: Buchung stornieren mit Bestätigungs-Dialog | ✅ Erledigt | S. |
| Gast: Nur eigene Buchungen anzeigen | ✅ Erledigt | Z. |
| Gast: Eigene Buchung stornieren | ✅ Erledigt | Z. |

> 🐛 **Bug gefunden:** Bereits stornierte Buchungen konnten nochmals storniert werden.  
> ✅ **Bug gelöst (A.):** Prüfung auf `isCancelled()` hinzugefügt — stornierte Zeilen werden grau und nicht mehr auswählbar.

| Aufgabe | Status | Erledigt von |
|---------|--------|-------------|
| Stornierte Buchungen ausgegraut und gesperrt | ✅ Erledigt | A. |
| Admin: Gast löschen | ✅ Erledigt | A. |

---

### 🎫 US-09: Check-in Code

| Aufgabe | Status | Erledigt von |
|---------|--------|-------------|
| Zufälligen 6-stelligen alphanumerischen Code generieren | ✅ Erledigt | S. |
| Code direkt nach Buchung im Popup anzeigen | ✅ Erledigt | S. |
| Code in der Buchungstabelle des Gastes anzeigen | ✅ Erledigt | S. |
| Code in JSON speichern und beim Laden wiederherstellen | ✅ Erledigt | A. |

> 🐛 **Bug gefunden:** Logout-Button verursachte Compiler-Fehler — `hotel` und `manager` konnten in Lambda-Funktion nicht erfasst werden.  
> ✅ **Bug gelöst (S.):** `this->hotel` und `this->manager` verwendet, `QApplication::quit()` statt `qApp->quit()`, `#include <QApplication>` hinzugefügt.

> 🐛 **Bug gefunden:** `std::function` nicht deklariert in `DataManager.cpp`.  
> ✅ **Bug gelöst (A.):** `#include <functional>` hinzugefügt.

---

### 🎨 UI Verbesserungen

| Aufgabe | Status | Erledigt von |
|---------|--------|-------------|
| Logout-Button oben rechts (Admin + Gast) | ✅ Erledigt | S. |
| Eingeloggter Benutzer in Toolbar anzeigen | ✅ Erledigt | Z. |
| Fusion-Style für modernes Aussehen auf allen Plattformen | ✅ Erledigt | A. |
| Farbkodierung: Grün = verfügbar/aktiv, Rot = belegt/storniert | ✅ Erledigt | Z. |

---

## 🔲 Noch offen (Backlog für Sprint 3)

### Bekannte Bugs
| Bug | Priorität | Zugewiesen |
|-----|-----------|-----------|
| Check-out Datum kann manuell vor Check-in gesetzt werden | 🔴 Hoch | S. |
| Bei sehr langem Gastnamen wird Tabellenspalte zu breit | 🟡 Mittel | Z. |
| Nach Logout werden Suchergebnisse nicht zurückgesetzt | 🟡 Mittel | A. |

### Geplante Features
| Feature | Priorität | Zugewiesen |
|---------|-----------|-----------|
| US-10: Tabellen sortierbar machen (nach Datum, Preis) | 🟡 Mittel | S. |
| US-10: Bessere Fehlermeldungen mit konkreten Hinweisen | 🟡 Mittel | Z. |
| US-11: Statistiken für Admin (Auslastung, Umsatz) | 🟢 Niedrig | A. |
| Passwort ändern für Gäste | 🟢 Niedrig | Z. |
| Zimmerpreise nachträglich ändern (Admin) | 🟢 Niedrig | S. |

---

## 📊 Gesamt-Übersicht

| Kategorie | Sprint 1 | Sprint 2 | Gesamt |
|-----------|----------|----------|--------|
| User Stories abgeschlossen | 4 | 5 | **9 / 11** |
| Aufgaben erledigt | 18 | 28 | **46** |
| Bugs gefunden & gelöst | 3 | 8 | **11** |
| Bugs noch offen | — | 3 | **3** |
| Fortschritt | 40% | 80% | **~80%** |

---

## 🔄 Sprint 2 Retrospektive

| Was lief gut? | Was kann verbessert werden? |
|--------------|----------------------------|
| Login-System gut strukturiert | Früher testen vor dem Merge |
| Bugs systematisch dokumentiert | Fenstergröße früher testen |
| Gute Aufgabenteilung | Noch mehr Kommunikation bei Bugs |

---

*Hotel Booking System — Team: Abdul, Samih, Zain — Hochschule Esslingen*
