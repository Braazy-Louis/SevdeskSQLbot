#pragma once

#include <string>
#include <vector>
#include <iostream>
#include <iomanip>
#include <ctime>

// Allgemeine Hilfsfunktionen für die Anwendung
namespace Utils {
    // Formatierte Tabellenausgabe
    void printTable(const std::vector<std::string>& headers, const std::vector<std::vector<std::string>>& data);
    
    // Aktuelles Datum und Uhrzeit als formatierter String
    std::string getCurrentDateTime();
    
    // Währungsformatierung mit optionalem Währungssymbol
    std::string formatCurrency(double amount, const std::string& currencySymbol = "€");
    
    // Datumsformatierung im Format JJJJ-MM-TT
    std::string formatDate(int year, int month, int day);
    
    // Parst ein Datum im Format JJJJ-MM-TT
    bool parseDate(const std::string& dateStr, int& year, int& month, int& day);
    
    // Prüft ob ein String eine gültige Zahl ist
    bool isNumber(const std::string& str);
    
    // Konvertiert einen String in eine Zahl mit Standardwert
    double toNumber(const std::string& str, double defaultValue = 0.0);
};
