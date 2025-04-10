#pragma once

#include <string>
#include <vector>
#include <iostream>
#include <iomanip>
#include <ctime>

// Hilfsfunktionen
namespace Utils {
    void printTable(const std::vector<std::string>& headers, const std::vector<std::vector<std::string>>& data);
    std::string getCurrentDateTime();
    std::string formatCurrency(double amount, const std::string& currencySymbol = "€");
    std::string formatDate(int year, int month, int day);
    bool parseDate(const std::string& dateStr, int& year, int& month, int& day);
    bool isNumber(const std::string& str);
    double toNumber(const std::string& str, double defaultValue = 0.0);
}; 