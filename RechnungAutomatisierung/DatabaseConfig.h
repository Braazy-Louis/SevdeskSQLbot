#pragma once

#include <string>

// Konfiguration für Datenbankverbindung
class DatabaseConfig {
public:
    // Standardkonstruktor
    DatabaseConfig();

    // Konstruktor mit allen Parametern
    DatabaseConfig(
        const std::string& driverName,  // ODBC-Treiber
        const std::string& server,      // Serveradresse
        const std::string& database,    // Datenbankname
        const std::string& username,    // Benutzername
        const std::string& password,    // Passwort
        int port = 3306                 // Port
    );

    // Getter und Setter für Konfigurationsparameter
    std::string getDriverName() const;
    void setDriverName(const std::string& driverName);
    std::string getServer() const;
    void setServer(const std::string& server);
    std::string getDatabase() const;
    void setDatabase(const std::string& database);
    std::string getUsername() const;
    void setUsername(const std::string& username);
    std::string getPassword() const;
    void setPassword(const std::string& password);
    int getPort() const;
    void setPort(int port);

    // Erstellt die Verbindungszeichenfolge
    std::string getConnectionString() const;

private:
    std::string m_driverName;
    std::string m_server;
    std::string m_database;
    std::string m_username;
    std::string m_password;
    int m_port;
}; 