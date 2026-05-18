# Präsentation morgen - Hotel Booking System

## 1. Projektidee

Unser Projekt ist ein Hotel Booking System mit Qt und C++.
Ein Reisender kann nach verfügbaren Zimmern suchen, indem er Personenanzahl, Anreise- und Abreisedatum eingibt. Danach zeigt die App passende freie Zimmer inklusive Preis an. Anschließend kann ein Zimmer gebucht werden.

## 2. Warum Qt und C++?

Wir nutzen Qt, weil wir damit eine Desktop-GUI erstellen können. C++ nutzen wir für die Programmlogik, zum Beispiel für Zimmer, Buchungen, Suche, Preisberechnung und Verfügbarkeitsprüfung.

## 3. Aktueller Stand ca. 25%

Aktuell ist eine MVP-Version fertig:

- Qt-Projekt wurde erstellt
- Grundlegende GUI wurde gebaut
- Beispielzimmer sind im Programm gespeichert
- Zimmer können gesucht werden
- Gesamtpreis wird berechnet
- Zimmer können gebucht werden
- Doppelbuchungen werden verhindert
- Buchungen werden in einer Tabelle angezeigt

## 4. Was ist noch nicht fertig?

- Datenbank oder Datei-Speicherung
- Zimmerverwaltung für Hotelmitarbeiter
- schöneres Design
- bessere Fehlerbehandlung
- weitere Tests

## 5. Rollen im Team

- Product Owner: entscheidet, welche Funktionen zuerst wichtig sind
- Scrum Master: achtet auf Aufgaben, Zeit und Scrum-Prozess
- Developer: alle drei Teammitglieder entwickeln das Projekt

## 6. Sprint-Ziel

Unser erstes Sprint-Ziel war:
Eine erste lauffähige Qt-Anwendung erstellen, mit der Zimmer gesucht und gebucht werden können.

## 7. Demo-Ablauf

1. Wir starten die Anwendung.
2. Wir geben Gastname, Personenanzahl und Reisedaten ein.
3. Wir klicken auf „Zimmer suchen“.
4. Die App zeigt verfügbare Zimmer mit Gesamtpreis an.
5. Wir wählen ein Zimmer aus und buchen es.
6. Die Buchung erscheint in der Buchungstabelle.
7. Bei erneuter Suche ist das gebuchte Zimmer nicht mehr verfügbar.

## 8. Nächste Schritte

Im nächsten Sprint wollen wir:

- Zimmerverwaltung hinzufügen
- Buchungen dauerhaft speichern, zum Beispiel mit SQLite
- GUI verbessern
- Weitere Testfälle ergänzen
