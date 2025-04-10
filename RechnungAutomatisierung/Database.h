#pragma once

#include <windows.h>
#include <sql.h>
#include <sqlext.h>
#include <string>
#include <vector>
#include <memory>
#include <functional>

// Vorwärtsdeklarationen
class DatabaseConnection;
class DatabaseStatement;

// Datenbankklasse
class Database {
public:
    Database();
    ~Database();

    std::vector<std::string> listAvailableDrivers();
    bool connect(const std::string& connectionString);
    bool connect(
        const std::string& driverName,
        const std::string& server,
        const std::string& database,
        const std::string& username,
        const std::string& password,
        int port = 3306
    );
    void disconnect();
    bool isConnected() const;
    bool executeQuery(const std::string& query);
    bool executeQuery(
        const std::string& query,
        std::function<void(const std::vector<std::string>&)> resultProcessor
    );
    std::string getLastError() const;

private:
    SQLHENV m_hEnv;
    SQLHDBC m_hDbc;
    SQLHSTMT m_hStmt;
    bool m_isConnected;
    std::string m_lastError;
    
    void checkError(SQLHANDLE handle, SQLSMALLINT type, const char* message);
    void initializeEnvironment();
    void cleanupResources();
}; 