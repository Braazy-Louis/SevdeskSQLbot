# RechnungAutomatisierung

Eine einfache C++-Anwendung zur Demonstration der Datenbankverbindung mit ODBC.

## Voraussetzungen

- CMake 3.10 oder höher
- C++17-kompatibler Compiler (MSVC, GCC, Clang)
- ODBC-Treiber für MySQL

## Projekt erstellen

```bash
# Build-Verzeichnis erstellen
mkdir build
cd build

# Build-Dateien generieren
cmake ..

# Projekt bauen
cmake --build .
```

## Konfiguration

Die Anwendung verwendet standardmäßig folgende Datenbankeinstellungen:

- Server: 193.203.168.148
- Datenbank: u950594034_A6Hnf
- Benutzername: u950594034_sVzHH
- Passwort: Rj9VJczDzZ
- Port: 3306

Diese Einstellungen können in der `Application.cpp`-Datei angepasst werden.

## Verwendung

Nach dem Bauen des Projekts kann die Anwendung wie folgt ausgeführt werden:

```bash
./RechnungAutomatisierung
```

Die Anwendung wird:
1. Verfügbare ODBC-Treiber auflisten
2. Eine Verbindung zur Datenbank herstellen
3. Datenbankinformationen anzeigen
4. Auf Benutzereingabe warten, bevor sie beendet wird

## Projektstruktur

- `main.cpp` - Haupteinstiegspunkt der Anwendung
- `Application.h/cpp` - Hauptanwendungsklasse
- `Database.h/cpp` - Datenbankverbindungsklasse
- `DatabaseConfig.h/cpp` - Datenbankkonfigurationsklasse

## Fehlerbehandlung

Die Anwendung behandelt folgende Fehler:
- Initialisierungsfehler
- Datenbankverbindungsfehler
- Allgemeine Ausnahmen

## Lizenz

MIT-Lizenz 