# RechnungAutomatisierung

Ein professionelles C++-System zur automatisierten Verarbeitung und Verwaltung von Kundendaten und Formularen mit CSV-Export-Funktionalität.

## 🚀 Funktionen

- **Formularverarbeitung**: Automatische Verarbeitung verschiedener Formulartypen:
  - Erstanmeldung Wohnung
  - Anmeldung weiterer Wohnung
  - Abmeldung Wohnung
  - Änderung Beitragskonto
  - Kontaktformulare

- **Datenbank-Integration**: 
  - Sichere ODBC-Verbindung zu MySQL
  - Effiziente Datenverwaltung
  - Robuste Fehlerbehandlung

- **CSV-Export**:
  - UTF-16-kodierte Exports
  - Automatische Zeitstempel
  - Standardisierte Datenformatierung

- **Datentransformation**:
  - Telefonnummern-Formatierung
  - Datumskonvertierung
  - Unique-ID-Generierung

## 💻 Technische Details

- Moderne C++-Implementierung (C++17)
- Objektorientiertes Design
- Thread-sicher
- Speichereffizient durch Smart Pointer
- Plattformübergreifend (Windows, Linux)

## 🛠️ Installation

### Voraussetzungen
- CMake 3.10+
- C++17-kompatibler Compiler
- MySQL ODBC-Treiber


## 📖 Verwendung

1. Konfiguration in `DatabaseConfig.h` anpassen
2. Programm ausführen
3. CSV-Exports werden automatisch im Arbeitsverzeichnis erstellt

## 🔧 Konfiguration

Die Datenbankverbindung kann über die `DatabaseConfig`-Klasse angepasst werden:
- Server
- Datenbank
- Benutzername
- Passwort
- Port

## 📁 Projektstruktur
