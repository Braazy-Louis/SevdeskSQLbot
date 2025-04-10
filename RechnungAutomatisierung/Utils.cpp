#pragma warning(disable : 4996)
#include "Utils.h"
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <cctype>

namespace Utils {

void printTable(const std::vector<std::string>& headers, const std::vector<std::vector<std::string>>& data) {
    if (headers.empty() || data.empty()) {
        std::cout << "No data to display" << std::endl;
        return;
    }
    
    // Calculate column widths
    std::vector<size_t> columnWidths(headers.size(), 0);
    
    // Set minimum width based on headers
    for (size_t i = 0; i < headers.size(); i++) {
        columnWidths[i] = headers[i].length();
    }
    
    // Update widths based on data
    for (const auto& row : data) {
        for (size_t i = 0; i < std::min(row.size(), headers.size()); i++) {
            columnWidths[i] = std::max(columnWidths[i], row[i].length());
        }
    }
    
    // Print headers
    std::cout << "+";
    for (size_t width : columnWidths) {
        std::cout << std::string(width + 2, '-') << "+";
    }
    std::cout << std::endl;
    
    std::cout << "|";
    for (size_t i = 0; i < headers.size(); i++) {
        std::cout << " " << std::left << std::setw(columnWidths[i]) << headers[i] << " |";
    }
    std::cout << std::endl;
    
    // Print separator
    std::cout << "+";
    for (size_t width : columnWidths) {
        std::cout << std::string(width + 2, '-') << "+";
    }
    std::cout << std::endl;
    
    // Print data
    for (const auto& row : data) {
        std::cout << "|";
        for (size_t i = 0; i < std::min(row.size(), headers.size()); i++) {
            std::cout << " " << std::left << std::setw(columnWidths[i]) << row[i] << " |";
        }
        std::cout << std::endl;
    }
    
    // Print footer
    std::cout << "+";
    for (size_t width : columnWidths) {
        std::cout << std::string(width + 2, '-') << "+";
    }
    std::cout << std::endl;
}

std::string getCurrentDateTime() {
    auto now = std::time(nullptr);
    auto tm = *std::localtime(&now);
    std::ostringstream oss;
    oss << std::put_time(&tm, "%Y-%m-%d %H:%M:%S");
    return oss.str();
}

std::string formatCurrency(double amount, const std::string& currencySymbol) {
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(2) << currencySymbol << " " << amount;
    return oss.str();
}

std::string formatDate(int year, int month, int day) {
    std::ostringstream oss;
    oss << std::setfill('0') << std::setw(4) << year << "-"
        << std::setfill('0') << std::setw(2) << month << "-"
        << std::setfill('0') << std::setw(2) << day;
    return oss.str();
}

bool parseDate(const std::string& dateStr, int& year, int& month, int& day) {
    // Expected format: YYYY-MM-DD
    if (dateStr.length() != 10 || dateStr[4] != '-' || dateStr[7] != '-') {
        return false;
    }
    
    try {
        year = std::stoi(dateStr.substr(0, 4));
        month = std::stoi(dateStr.substr(5, 2));
        day = std::stoi(dateStr.substr(8, 2));
        
        // Basic validation
        if (month < 1 || month > 12 || day < 1 || day > 31) {
            return false;
        }
        
        return true;
    } catch (const std::exception&) {
        return false;
    }
}

bool isNumber(const std::string& str) {
    if (str.empty()) {
        return false;
    }
    
    // Check if the string is a valid number
    char* endPtr;
    std::strtod(str.c_str(), &endPtr);
    
    // If endPtr points to the end of the string, it's a valid number
    return *endPtr == '\0';
}

double toNumber(const std::string& str, double defaultValue) {
    if (str.empty()) {
        return defaultValue;
    }
    
    try {
        return std::stod(str);
    } catch (const std::exception&) {
        return defaultValue;
    }
}

} 