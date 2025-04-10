#include "Application.h"
#include "CsvExporter.h"
#include <iostream>
#include <limits>
#include <thread>
#include <chrono>
#include <sstream>
#include <algorithm>
#include <cctype>
#include <random>
#include <unordered_set>

Application::Application() : m_isInitialized(false) {
    m_database = std::make_unique<Database>();
}

Application::~Application() {
    // Aufräumarbeiten
}

bool Application::initialize()
{
    // Datenbankverbindungsdaten
    m_config.setDriverName("MySQL ODBC 9.2 Unicode Driver");
    m_config.setServer("*******");
    m_config.setDatabase("********");
    m_config.setUsername("**********");
    m_config.setPassword("**********");
    m_config.setPort(3306);
    
    m_isInitialized = true;
    return true;
}

int Application::run() {
    if (!m_isInitialized)
    {       
        std::cerr << "Fehler: Anwendung nicht initialisiert." << std::endl;
        Sleep(20000);
        return 1;
    }
    
    std::cout << "\n===== RechnungAutomatisierung =====\n"; 
    
    bool success = connectToDatabase();
    
    if (success) {
        std::cout << "Erfolgreich mit der Datenbank verbunden!" << std::endl;
        std::cout << "Datenbankinformationen:" << std::endl;
        std::cout << "  Server: " << m_config.getServer() << std::endl;
        std::cout << "  Datenbank: " << m_config.getDatabase() << std::endl;
        std::cout << "  Benutzername: " << m_config.getUsername() << std::endl;
        std::cout << "  Port: " << m_config.getPort() << std::endl;
        
        // Daten aus der Datenbank lesen
        if (!readEntriesAndFormTypes()) {
            std::cerr << "Fehler beim Lesen der Eintraege." << std::endl;
            return 1;
        }

        // Daten in CSV exportieren
        exportToCsv(m_userDataList, "exported_contacts.csv");
    } else {
        std::cerr << "Verbindung zur Datenbank fehlgeschlagen: " << m_database->getLastError() << std::endl;
        return 1;
    }
    
    std::cout << "\nDruecken Sie Enter, um zu beenden...";
    std::cin.get();
    
    return 0;
}

bool Application::connectToDatabase() {
    // Datenbankverbindung herstellen
    bool success = m_database->connect(
        m_config.getDriverName(),
        m_config.getServer(),
        m_config.getDatabase(),
        m_config.getUsername(),
        m_config.getPassword(),
        m_config.getPort()
    );
    
    return success;
}

bool Application::readEntriesAndFormTypes() {
    std::cout << "\nLese Eintraege und Formulartypen..." << std::endl;
    
    // Bereits verarbeitete Einträge aus rechnungDone lesen
    std::unordered_set<int> processedEntryIds;
    std::string queryDone = "SELECT entry_id FROM rechnungDone";
    std::cout << "Lese bereits verarbeitete IDs aus rechnungDone..." << std::endl;
    m_database->executeQuery(queryDone, [&processedEntryIds](const std::vector<std::string>& row) {
        if (!row.empty()) {
            try {
                processedEntryIds.insert(std::stoi(row[0]));
            } catch (const std::exception& e) {
                std::cerr << "Fehler beim Konvertieren der entry_id aus rechnungDone: " << e.what() << std::endl;
            }
        }
    });
    std::cout << processedEntryIds.size() << " bereits verarbeitete Eintraege gefunden." << std::endl;

    // Einträge und Formulartypen aus der Datenbank lesen
    std::string queryEntries = "SELECT entry_id, meta_value FROM wp_gf_entry_meta WHERE meta_key = 1";
    
    std::vector<std::pair<int, std::string>> entries;
    
    m_database->executeQuery(queryEntries, [&entries](const std::vector<std::string>& row) {
        if (row.size() >= 2) {
            try {
                int entry_id = std::stoi(row[0]);
                entries.push_back({entry_id, row[1]});
            } catch (const std::exception& e) {
                std::cerr << "Fehler beim Konvertieren der entry_id: " << e.what() << std::endl;
            }
        }
    });
    
    std::cout << "\nGefundene Eintraege in wp_gf_entry_meta:" << std::endl;
    std::cout << "=========================================" << std::endl;
    
    m_userDataList.clear();
    int processedCount = 0;
    int skippedCount = 0;

    for (const auto& entry : entries) {
        // Prüfen ob Eintrag bereits verarbeitet wurde
        if (processedEntryIds.count(entry.first)) {
            skippedCount++;
            continue;
        }

        std::cout << "Verarbeite neuen Entry ID: " << entry.first << ", Formulartyp: " << entry.second << std::endl;
        
        // Eintrag verarbeiten
        processEntry(entry.first, entry.second);
        processedCount++;

        // Eintrag als verarbeitet markieren
        std::string insertQuery = "INSERT INTO rechnungDone (entry_id) VALUES (" + std::to_string(entry.first) + ")";
        m_database->executeQuery(insertQuery, nullptr);
        std::cout << "  -> Entry ID " << entry.first << " als verarbeitet markiert." << std::endl;
    }

    std::cout << "\nVerarbeitung abgeschlossen." << std::endl;
    std::cout << "  Verarbeitet: " << processedCount << " neue Eintraege." << std::endl;
    std::cout << "  Uebersprungen: " << skippedCount << " bereits verarbeitete Einträge." << std::endl;

    return true;
}

void Application::processEntry(int entry_id, const std::string& form_type) {
    if (form_type == "Abmeldung Ihrer Wohnung") {
        processAbmeldungWohnung(entry_id);
    } else if (form_type == "Änderung zum Beitragskonto mitteilen") {
        processAenderungBeitragskonto(entry_id);
    } else if (form_type == "Erstanmeldung einer Wohnung zum Rundfunkbeitrag") {
        processErstanmeldungWohnung(entry_id);
    } else if (form_type == "Anmeldung einer weiteren Wohnung") {
        processAnmeldungWeitererWohnung(entry_id);
    } else if (form_type == "Kontaktformular (Sonstiges)") {
        processKontaktformular(entry_id);
    } else {
        std::cout << "  Unbekannter Formulartyp: " << form_type << std::endl;
    }
}

void Application::processAbmeldungWohnung(int entry_id) {
   
    
    UserData userData;
    userData.kategorie = "Kunde";
    userData.land = "Deutschland";
    userData.adresseKategorie = "Privat";
    userData.emailKategorie = "Privat";

    std::string reason = readMetaValue(entry_id, "7");
    userData.beschreibung = "Abmeldung Grund: " + reason;
    
    if (!reason.empty()) {
     
        
    
        if (reason == "ich zu einem anderen Beitragszahler ziehe.") {
          
            userData.kundenNr = readMetaValue(entry_id, "37");
            if (userData.kundenNr.empty()) { userData.kundenNr = generateUniqueId(); }
            userData.anrede = readMetaValue(entry_id, "96.2");
  

            userData.vorname = readMetaValue(entry_id, "96.3");
            userData.nachname = readMetaValue(entry_id, "96.6"); 

            userData.plz = readMetaValue(entry_id, "39.3");
            std::string gStraße = readMetaValue(entry_id, "39.1");
			std::string Hausnummer = readMetaValue(entry_id, "39.2");
			userData.strasse =  gStraße + " " + Hausnummer;
			userData.ort = readMetaValue(entry_id, "39.5");
			userData.land = readMetaValue(entry_id, "39.6");     

            userData.email = readMetaValue(entry_id, "5");
            std::string rawGbDate = readMetaValue(entry_id, "40");
            userData.geburtstag = formatDateDDMMYYYY(rawGbDate);     

        } 
        else if (reason == "ich dauerhaft ins Ausland ziehe.")
        {
            userData.kundenNr = readMetaValue(entry_id, "30");
            if (userData.kundenNr.empty()) { userData.kundenNr = generateUniqueId(); }
            userData.anrede = readMetaValue(entry_id, "95.2");          

            userData.vorname = readMetaValue(entry_id, "95.3");
            userData.nachname = readMetaValue(entry_id, "95.6");      

            userData.plz = readMetaValue(entry_id, "33.5");
            std::string gStraße = readMetaValue(entry_id, "33.1");
            std::string Hausnummer = readMetaValue(entry_id, "33.2");
            userData.strasse = gStraße + " " + Hausnummer;
            userData.ort = readMetaValue(entry_id, "33.3");
            userData.land = readMetaValue(entry_id, "33.6");     

            userData.email = readMetaValue(entry_id, "5");
            std::string rawGbDate = readMetaValue(entry_id, "32");
            userData.geburtstag = formatDateDDMMYYYY(rawGbDate);
       
        }
        else if (reason == "sonstige Gründe zutreffen.") 
        {
            userData.kundenNr = readMetaValue(entry_id, "26");
            if (userData.kundenNr.empty()) { userData.kundenNr = generateUniqueId(); }
            userData.anrede = readMetaValue(entry_id, "94.2");        

            userData.vorname = readMetaValue(entry_id, "94.3");
            userData.nachname = readMetaValue(entry_id, "94.6");

            userData.plz = readMetaValue(entry_id, "29.5");
            std::string gStraße = readMetaValue(entry_id, "29.1");
            std::string Hausnummer = readMetaValue(entry_id, "29.2");
            userData.strasse = gStraße + " " + Hausnummer;
            userData.ort = readMetaValue(entry_id, "29.3");
            userData.land = readMetaValue(entry_id, "29.6");

            userData.email = readMetaValue(entry_id, "5");
            std::string rawGbDate = readMetaValue(entry_id, "28");
            userData.geburtstag = formatDateDDMMYYYY(rawGbDate);   
      
        }
        m_userDataList.push_back(userData);
    }
    
    
    
}

void Application::processAenderungBeitragskonto(int entry_id)
{
    UserData userData;
    userData.kategorie = "Kunde";
    userData.land = "Deutschland";
    userData.adresseKategorie = "Privat";
    userData.emailKategorie = "Privat";

    userData.kundenNr = readMetaValue(entry_id, "42");
    if (userData.kundenNr.empty()) { userData.kundenNr = generateUniqueId(); }
    userData.anrede = readMetaValue(entry_id, "97.2");

    userData.vorname = readMetaValue(entry_id, "97.3");
    userData.nachname = readMetaValue(entry_id, "97.6");

    userData.plz = readMetaValue(entry_id, "46.5");
    std::string gStraße = readMetaValue(entry_id, "46.1");
    std::string Hausnummer = readMetaValue(entry_id, "46.2");
    userData.strasse = gStraße + " " + Hausnummer;
    userData.ort = readMetaValue(entry_id, "46.3");
    userData.land = readMetaValue(entry_id, "46.6"); 

    userData.email = readMetaValue(entry_id, "5");
    std::string rawGbDate = readMetaValue(entry_id, "44");
    userData.geburtstag = formatDateDDMMYYYY(rawGbDate);

    m_userDataList.push_back(userData);
}

void Application::processErstanmeldungWohnung(int entry_id)
{   
    UserData userData;
    userData.kategorie = "Kunde";
    userData.land = "Deutschland";
    userData.adresseKategorie = "Privat";
    userData.emailKategorie = "Privat";
    userData.telefonKategorie = "Privat";

    std::string rawTelefon = readMetaValue(entry_id, "4");
    userData.telefon = sanitizePhoneNumber(rawTelefon);
    userData.kundenNr = generateUniqueId();
    userData.anrede = readMetaValue(entry_id, "99.2");   

    userData.vorname = readMetaValue(entry_id, "99.3");
    userData.nachname = readMetaValue(entry_id, "99.6");

    userData.plz = readMetaValue(entry_id, "55.5");
    std::string gStraße = readMetaValue(entry_id, "55.1");
    std::string Hausnummer = readMetaValue(entry_id, "55.2");
    userData.strasse = gStraße + " " + Hausnummer;
    userData.ort = readMetaValue(entry_id, "55.3");
    userData.land = readMetaValue(entry_id, "55.6");

    userData.email = readMetaValue(entry_id, "5");
    std::string rawGbDate = readMetaValue(entry_id, "56");
    userData.geburtstag = formatDateDDMMYYYY(rawGbDate);

    m_userDataList.push_back(userData);
}

void Application::processAnmeldungWeitererWohnung(int entry_id) 
{
    UserData userData;
    userData.kategorie = "Kunde";
    userData.land = "Deutschland";
    userData.adresseKategorie = "Privat";
    userData.emailKategorie = "Privat";

    userData.kundenNr = readMetaValue(entry_id, "61"); 
    if (userData.kundenNr.empty()) { userData.kundenNr = generateUniqueId(); }
    userData.anrede = readMetaValue(entry_id, "100.2");  

    userData.vorname = readMetaValue(entry_id, "100.3");
    userData.nachname = readMetaValue(entry_id, "100.6");   

    userData.plz = readMetaValue(entry_id, "64.5");
    std::string gStraße = readMetaValue(entry_id, "64.1");
    std::string Hausnummer = readMetaValue(entry_id, "64.2");
    userData.strasse = gStraße + " " + Hausnummer;
    userData.ort = readMetaValue(entry_id, "64.3");
    userData.land = readMetaValue(entry_id, "64.6");  

    userData.email = readMetaValue(entry_id, "5");
    std::string rawGbDate = readMetaValue(entry_id, "65");
    userData.geburtstag = formatDateDDMMYYYY(rawGbDate);   

    m_userDataList.push_back(userData);
}

void Application::processKontaktformular(int entry_id)
{
   
    UserData userData;
    userData.kategorie = "Kunde";
    userData.land = "Deutschland";
    userData.adresseKategorie = "Privat";
    userData.emailKategorie = "Privat";
    userData.telefonKategorie = "Privat";

    std::string rawTelefon = readMetaValue(entry_id, "4");
    userData.telefon = sanitizePhoneNumber(rawTelefon);
    userData.kundenNr = generateUniqueId();
    userData.anrede = readMetaValue(entry_id, "101.2");


    userData.vorname = readMetaValue(entry_id, "101.3");
    userData.nachname = readMetaValue(entry_id, "101.6");
 

    userData.plz = readMetaValue(entry_id, "79.5");
    std::string gStraße = readMetaValue(entry_id, "79.1");
    std::string Hausnummer = readMetaValue(entry_id, "79.2");
    userData.strasse = gStraße + " " + Hausnummer;
    userData.ort = readMetaValue(entry_id, "79.3");
    userData.land = readMetaValue(entry_id, "79.6");


    userData.email = readMetaValue(entry_id, "5");
    std::string rawGbDate = readMetaValue(entry_id, " ");
    userData.geburtstag = formatDateDDMMYYYY(rawGbDate);


    m_userDataList.push_back(userData);
  
}

std::string Application::formatDateDDMMYYYY(const std::string& dbDate) {
    if (dbDate.length() == 10 && dbDate[4] == '-' && dbDate[7] == '-') {
        // Datum von YYYY-MM-DD in DD.MM.YYYY umwandeln
        std::string year = dbDate.substr(0, 4);
        std::string month = dbDate.substr(5, 2);
        std::string day = dbDate.substr(8, 2);
        
        try {
            std::stoi(year);
            std::stoi(month);
            std::stoi(day);
            return day + "." + month + "." + year;
        } catch (const std::exception& e) {
            return dbDate;
        }
    }
    return dbDate; 
}

std::string Application::sanitizePhoneNumber(const std::string& phoneNumber) {
    // Telefonnummer bereinigen - nur Ziffern behalten
    std::string sanitizedNumber;
    for (char c : phoneNumber) {
        if (std::isdigit(static_cast<unsigned char>(c))) {
            sanitizedNumber += c;
        }
    }
    return sanitizedNumber;
}

std::string Application::readMetaValue(int entry_id, const std::string& meta_key) {
    // Metadaten aus der Datenbank lesen
    std::string query = "SELECT meta_value FROM wp_gf_entry_meta WHERE entry_id = " + 
                        std::to_string(entry_id) + " AND meta_key = '" + meta_key + "'";
    
    std::string result;
    
    m_database->executeQuery(query, [&result](const std::vector<std::string>& row) {
        if (!row.empty()) {
            result = row[0];
        }
    });
    
    return result;
}

std::string Application::generateUniqueId() {
    // Eindeutige 8-stellige ID generieren
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<long long> distrib(10000000LL, 99999999LL); 

    long long generated_id = distrib(gen);
    
    return std::to_string(generated_id);
} 
