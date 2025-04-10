#pragma once

#include "Database.h"
#include "DatabaseConfig.h"
#include "CsvExporter.h"
#include <string>
#include <memory>
#include <vector>
#include <map>
#include <random>
#include <chrono>

// Hauptanwendungsklasse
class Application {
public:
    Application();
    ~Application();
    
    bool initialize();
    int run();
    bool connectToDatabase();
    bool readEntriesAndFormTypes();
    
private:
    void processEntry(int entry_id, const std::string& form_type);
    void processAbmeldungWohnung(int entry_id);
    void processAenderungBeitragskonto(int entry_id);
    void processErstanmeldungWohnung(int entry_id);
    void processAnmeldungWeitererWohnung(int entry_id);
    void processKontaktformular(int entry_id);
    std::string readMetaValue(int entry_id, const std::string& meta_key);
    std::string formatDateDDMMYYYY(const std::string& dbDate);
    std::string sanitizePhoneNumber(const std::string& phoneNumber);
    std::string generateUniqueId();

    std::unique_ptr<Database> m_database;
    DatabaseConfig m_config;
    bool m_isInitialized;
    std::vector<UserData> m_userDataList;
}; 