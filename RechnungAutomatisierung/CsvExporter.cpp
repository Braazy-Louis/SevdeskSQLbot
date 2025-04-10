#pragma warning(disable : 4996)
#include "CsvExporter.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <windows.h>
#include <ctime>
#include <iomanip>
#include <chrono>

// Aktuelles Datum und Uhrzeit als String
std::string getCurrentDateTime() {
    const auto now = std::chrono::system_clock::now();
    const auto time = std::chrono::system_clock::to_time_t(now);
    std::stringstream ss;
    ss << std::put_time(std::localtime(&time), "%Y%m%d_%H%M%S");
    return ss.str();
}

// CSV-Felder in Anführungszeichen setzen
std::string quoteCsvField(const std::string& field) {
    std::stringstream ss;
    ss << '"';
    for (char c : field) {
        if (c == '"') {
            ss << """"; // Anführungszeichen escapen
        } else {
            ss << c;
        }
    }
    ss << '"';
    return ss.str();
}

// Benutzerdaten in CSV exportieren
void exportToCsv(const std::vector<UserData>& users, const std::string& baseFilename) {
    // Dateiname mit Zeitstempel erstellen
    std::string timestamp = getCurrentDateTime();
    std::string filename = "export_" + timestamp + ".csv";
    
    // Datei im Binärmodus öffnen für UTF-16 BOM
    std::ofstream outFile(filename, std::ios::binary);
    
    if (!outFile.is_open()) {
        std::cerr << "Fehler: Konnte Datei " << filename << " nicht zum Schreiben öffnen." << std::endl;
        return;
    }

    // UTF-16 BOM schreiben (0xFEFF)
    unsigned char bom[] = { 0xFF, 0xFE };
    outFile.write(reinterpret_cast<char*>(bom), 2);

    // Kopfzeile schreiben
    std::wstring header = L"\"Kunden-Nr.\";\"Anrede\";\"Titel\";\"Nachname\";\"Vorname\";\"Organisation\";\"Namenszusatz\";\"Position \";\"Kategorie\";\"IBAN\";\"BIC\";\"UmSt.-ID\";\"Strasse\";\"PLZ\";\"Ort\";\"Land\";\"Adresse-Kategorie\";\"Telefon\";\"Telefon-Kategorie\";\"Mobil\";\"Fax\";\"E-Mail\";\"E-Mail-Kategorie\";\"Webseite\";\"Webseite-Kategorie\";\"Beschreibung\";\"Geburtstag\";\"Tags\";\"Debitoren-Nr.\";\"Kreditoren-Nr.\";\"Leitweg-ID / Leitwegsnummer\";\"Steuernummer\";\"Skonto Tage\";\"Skonto Prozent\";\"Zahlungsziel Tage\";\"Kundenrabatt\";\"Ist Kundenrabatt prozentual\";\n";
    outFile.write(reinterpret_cast<const char*>(header.c_str()), header.length() * sizeof(wchar_t));

    // Datenzeilen schreiben
    for (const auto& user : users) {
        std::wstringstream row;
        row << quoteCsvField(convertToWideString(user.kundenNr)) << L";"
            << quoteCsvField(convertToWideString(user.anrede)) << L";"
            << quoteCsvField(convertToWideString(user.titel)) << L";"
            << quoteCsvField(convertToWideString(user.nachname)) << L";"
            << quoteCsvField(convertToWideString(user.vorname)) << L";"
            << quoteCsvField(convertToWideString(user.organisation)) << L";"
            << quoteCsvField(convertToWideString(user.namenszusatz)) << L";"
            << quoteCsvField(convertToWideString(user.position)) << L";"
            << quoteCsvField(convertToWideString(user.kategorie)) << L";"
            << quoteCsvField(convertToWideString(user.iban)) << L";"
            << quoteCsvField(convertToWideString(user.bic)) << L";"
            << quoteCsvField(convertToWideString(user.umstId)) << L";"
            << quoteCsvField(convertToWideString(user.strasse)) << L";"
            << quoteCsvField(convertToWideString(user.plz)) << L";"
            << quoteCsvField(convertToWideString(user.ort)) << L";"
            << quoteCsvField(convertToWideString(user.land)) << L";"
            << quoteCsvField(convertToWideString(user.adresseKategorie)) << L";"
            << quoteCsvField(convertToWideString(user.telefon)) << L";"
            << quoteCsvField(convertToWideString(user.telefonKategorie)) << L";"
            << quoteCsvField(convertToWideString(user.mobil)) << L";"
            << quoteCsvField(convertToWideString(user.fax)) << L";"
            << quoteCsvField(convertToWideString(user.email)) << L";"
            << quoteCsvField(convertToWideString(user.emailKategorie)) << L";"
            << quoteCsvField(convertToWideString(user.webseite)) << L";"
            << quoteCsvField(convertToWideString(user.webseiteKategorie)) << L";"
            << quoteCsvField(convertToWideString(user.beschreibung)) << L";"
            << quoteCsvField(convertToWideString(user.geburtstag)) << L";"
            << quoteCsvField(convertToWideString(user.tags)) << L";"
            << quoteCsvField(convertToWideString(user.debitorenNr)) << L";"
            << quoteCsvField(convertToWideString(user.kreditorenNr)) << L";"
            << quoteCsvField(convertToWideString(user.leitwegId)) << L";"
            << quoteCsvField(convertToWideString(user.steuernummer)) << L";"
            << quoteCsvField(convertToWideString(user.skontoTage)) << L";"
            << quoteCsvField(convertToWideString(user.skontoProzent)) << L";"
            << quoteCsvField(convertToWideString(user.zahlungszielTage)) << L";"
            << quoteCsvField(convertToWideString(user.kundenrabatt)) << L";"
            << quoteCsvField(convertToWideString(user.istKundenrabattProzentual)) << L";\n";

        std::wstring rowStr = row.str();
        outFile.write(reinterpret_cast<const char*>(rowStr.c_str()), rowStr.length() * sizeof(wchar_t));
    }

    outFile.close();

    if (!outFile.good()) {
         std::cerr << "Fehler: Beim Schreiben in die Datei " << filename << " ist ein Fehler aufgetreten." << std::endl;
    } else {
        std::cout << "Daten erfolgreich nach " << filename << " exportiert." << std::endl;
    }
}

// String in Wide-String konvertieren
std::wstring convertToWideString(const std::string& str) {
    if (str.empty()) return std::wstring();
    int size_needed = MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), NULL, 0);
    std::wstring wstrTo(size_needed, 0);
    MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), &wstrTo[0], size_needed);
    return wstrTo;
}

// CSV-Felder in UTF-16 formatieren
std::wstring quoteCsvField(const std::wstring& field) {
    std::wstringstream ss;
    ss << L'"';
    for (wchar_t c : field) {
        if (c == L'"') {
            ss << L""""; // Anführungszeichen escapen
        } else {
            ss << c;
        }
    }
    ss << L'"';
    return ss.str();
} 