#pragma once

#include <string>
#include <vector>
#include <fstream>
#include <sstream>

// Benutzerdatenstruktur
struct UserData {
    std::string kundenNr = "";
    std::string anrede = "";
    std::string titel = "";
    std::string nachname = "";
    std::string vorname = "";
    std::string organisation = "";
    std::string namenszusatz = "";
    std::string position = "";
    std::string kategorie = "";
    std::string iban = "";
    std::string bic = "";
    std::string umstId = "";
    std::string strasse = "";
    std::string plz = "";
    std::string ort = "";
    std::string land = "";
    std::string adresseKategorie = "";
    std::string telefon = "";
    std::string telefonKategorie = "";
    std::string mobil = "";
    std::string fax = "";
    std::string email = "";
    std::string emailKategorie = "";
    std::string webseite = "";
    std::string webseiteKategorie = "";
    std::string beschreibung = "";
    std::string geburtstag = "";
    std::string tags = "";
    std::string debitorenNr = "";
    std::string kreditorenNr = "";
    std::string leitwegId = "";
    std::string steuernummer = "";
    std::string skontoTage = "";
    std::string skontoProzent = "";
    std::string zahlungszielTage = "";
    std::string kundenrabatt = "";
    std::string istKundenrabattProzentual = "";
};

// Exportiert Benutzerdaten in eine CSV-Datei
void exportToCsv(const std::vector<UserData>& users, const std::string& filename);

// Hilfsfunktion zur Konvertierung von std::string zu std::wstring
std::wstring convertToWideString(const std::string& str);

// Hilfsfunktion zum Formatieren von CSV-Feldern in UTF-16
std::wstring quoteCsvField(const std::wstring& field); 