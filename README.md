# Hotel Booking System - Qt/C++ MVP

Dies ist eine kleine 25%-Version für die erste Projektpräsentation.

## Funktionsumfang

- Beispielzimmer sind fest im Programm gespeichert
- Suche nach Zimmern anhand von Personenanzahl, Anreisedatum und Abreisedatum
- Anzeige verfügbarer Zimmer in einer Tabelle
- Preisberechnung nach Anzahl der Nächte
- Buchung eines ausgewählten Zimmers
- Verhindert Doppelbuchungen im gleichen Zeitraum
- Übersicht aktueller Buchungen

## Noch nicht umgesetzt

- Keine Datenbank
- Keine dauerhafte Speicherung nach Programmende
- Keine Admin-Zimmerverwaltung
- Kein Login
- Noch kein finales Design

## Starten in Qt Creator

1. Qt Creator öffnen
2. `CMakeLists.txt` öffnen
3. Kit auswählen, z. B. Desktop Qt 6.x
4. Projekt konfigurieren
5. Build & Run

## Demo-Ablauf

1. App starten
2. Gastname eingeben
3. Personenanzahl auswählen
4. Anreise- und Abreisedatum auswählen
5. Auf `Zimmer suchen` klicken
6. Ein Zimmer auswählen
7. Auf `Ausgewähltes Zimmer buchen` klicken
8. Die Buchung erscheint unten in der Buchungstabelle
9. Erneut suchen: Das gebuchte Zimmer ist für den Zeitraum nicht mehr verfügbar
