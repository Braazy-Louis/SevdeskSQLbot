#include "Database.h"
#include <iostream>
#include <sstream>

Database::Database() : m_hEnv(NULL), m_hDbc(NULL), m_hStmt(NULL), m_isConnected(false) {
    initializeEnvironment();
}

Database::~Database() {
    disconnect();
    cleanupResources();
}

void Database::initializeEnvironment() {
    // Umgebungshandle zuweisen
    if (SQL_SUCCESS != SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &m_hEnv)) {
        m_lastError = "Fehler beim Zuweisen des Umgebungshandles";
        return;
    }
    
    // ODBC-Version festlegen
    if (SQL_SUCCESS != SQLSetEnvAttr(m_hEnv, SQL_ATTR_ODBC_VERSION, (SQLPOINTER)SQL_OV_ODBC3, 0)) {
        checkError(m_hEnv, SQL_HANDLE_ENV, "Fehler beim Festlegen der ODBC-Version");
        SQLFreeHandle(SQL_HANDLE_ENV, m_hEnv);
        m_hEnv = NULL;
        return;
    }
}

void Database::cleanupResources() {
    if (m_hEnv) {
        SQLFreeHandle(SQL_HANDLE_ENV, m_hEnv);
        m_hEnv = NULL;
    }
}

std::vector<std::string> Database::listAvailableDrivers() {
    std::vector<std::string> drivers;
    
    if (!m_hEnv) {
        m_lastError = "Umgebung nicht initialisiert";
        return drivers;
    }
    
    SQLCHAR driverDesc[256];
    SQLCHAR driverAttr[256];
    SQLSMALLINT driverDescLength;
    SQLSMALLINT driverAttrLength;
    SQLUSMALLINT direction = SQL_FETCH_FIRST;
    
    // Alle Treiber durchlaufen
    while (SQL_SUCCESS == SQLDrivers(m_hEnv, direction, driverDesc, sizeof(driverDesc), &driverDescLength,
                                    driverAttr, sizeof(driverAttr), &driverAttrLength)) {
        drivers.push_back(reinterpret_cast<char*>(driverDesc));
        direction = SQL_FETCH_NEXT;
    }
    
    return drivers;
}

bool Database::connect(const std::string& connectionString) {
    if (!m_hEnv) {
        m_lastError = "Umgebung nicht initialisiert";
        return false;
    }
    
    // Verbindungshandle zuweisen
    if (SQL_SUCCESS != SQLAllocHandle(SQL_HANDLE_DBC, m_hEnv, &m_hDbc)) {
        m_lastError = "Fehler beim Zuweisen des Verbindungshandles";
        return false;
    }
    
    // Verbindungszeitüberschreitung festlegen
    SQLSetConnectAttr(m_hDbc, SQL_ATTR_CONNECTION_TIMEOUT, (SQLPOINTER)30, 0);
    
    // Mit der Datenbank verbinden
    SQLRETURN ret = SQLDriverConnect(m_hDbc, NULL, (SQLCHAR*)connectionString.c_str(), SQL_NTS, NULL, 0, NULL, SQL_DRIVER_NOPROMPT);
    
    if (SQL_SUCCESS != ret && SQL_SUCCESS_WITH_INFO != ret) {
        checkError(m_hDbc, SQL_HANDLE_DBC, "Fehler beim Verbinden mit der Datenbank");
        SQLFreeHandle(SQL_HANDLE_DBC, m_hDbc);
        m_hDbc = NULL;
        return false;
    }
    
    m_isConnected = true;
    return true;
}

bool Database::connect(
    const std::string& driverName,
    const std::string& server,
    const std::string& database,
    const std::string& username,
    const std::string& password,
    int port
) {
    // Verbindungsstring erstellen
    std::stringstream connStr;
    connStr << "DRIVER={" << driverName << "};"
            << "SERVER=" << server << ";"
            << "DATABASE=" << database << ";"
            << "UID=" << username << ";"
            << "PWD=" << password << ";"
            << "PORT=" << port << ";";
    
    return connect(connStr.str());
}

void Database::disconnect() {
    if (m_hDbc) {
        if (m_isConnected) {
            SQLDisconnect(m_hDbc);
            m_isConnected = false;
        }
        
        SQLFreeHandle(SQL_HANDLE_DBC, m_hDbc);
        m_hDbc = NULL;
    }
    
    if (m_hStmt) {
        SQLFreeHandle(SQL_HANDLE_STMT, m_hStmt);
        m_hStmt = NULL;
    }
}

bool Database::isConnected() const {
    return m_isConnected;
}

bool Database::executeQuery(const std::string& query) {
    if (!m_isConnected || !m_hDbc) {
        m_lastError = "Nicht mit der Datenbank verbunden";
        return false;
    }
    
    // Anweisungshandle zuweisen, falls noch nicht zugewiesen
    if (!m_hStmt) {
        if (SQL_SUCCESS != SQLAllocHandle(SQL_HANDLE_STMT, m_hDbc, &m_hStmt)) {
            m_lastError = "Fehler beim Zuweisen des Anweisungshandles";
            return false;
        }
    }
    
    // Abfrage ausführen
    if (SQL_SUCCESS != SQLExecDirect(m_hStmt, (SQLCHAR*)query.c_str(), SQL_NTS)) {
        checkError(m_hStmt, SQL_HANDLE_STMT, "Fehler beim Ausführen der Abfrage");
        return false;
    }
    
    return true;
}

bool Database::executeQuery(
    const std::string& query,
    std::function<void(const std::vector<std::string>&)> resultProcessor
) {
    if (!m_isConnected || !m_hDbc) {
        m_lastError = "Nicht mit der Datenbank verbunden";
        return false;
    }
    
    // Anweisungshandle zuweisen, falls noch nicht zugewiesen
    if (!m_hStmt) {
        if (SQL_SUCCESS != SQLAllocHandle(SQL_HANDLE_STMT, m_hDbc, &m_hStmt)) {
            m_lastError = "Fehler beim Zuweisen des Anweisungshandles";
            return false;
        }
    }
    
    // Abfrage ausführen
    if (SQL_SUCCESS != SQLExecDirect(m_hStmt, (SQLCHAR*)query.c_str(), SQL_NTS)) {
        checkError(m_hStmt, SQL_HANDLE_STMT, "Fehler beim Ausführen der Abfrage");
        return false;
    }
    
    // Spaltenanzahl ermitteln
    SQLSMALLINT columnCount;
    SQLNumResultCols(m_hStmt, &columnCount);
    
    // Spaltenpuffer vorbereiten
    std::vector<SQLCHAR*> columnBuffers(columnCount);
    std::vector<SQLLEN> columnLengths(columnCount);
    
    for (SQLSMALLINT i = 0; i < columnCount; i++) {
        columnBuffers[i] = new SQLCHAR[256];
        SQLBindCol(m_hStmt, i + 1, SQL_C_CHAR, columnBuffers[i], 256, &columnLengths[i]);
    }
    
    // Ergebnisse abrufen und verarbeiten
    while (SQL_SUCCESS == SQLFetch(m_hStmt)) {
        std::vector<std::string> row;
        
        for (SQLSMALLINT i = 0; i < columnCount; i++) {
            if (columnLengths[i] == SQL_NULL_DATA) {
                row.push_back("NULL");
            } else {
                row.push_back(reinterpret_cast<char*>(columnBuffers[i]));
            }
        }
        
        resultProcessor(row);
    }
    
    // Spaltenpuffer aufräumen
    for (SQLSMALLINT i = 0; i < columnCount; i++) {
        delete[] columnBuffers[i];
    }
    
    return true;
}

std::string Database::getLastError() const {
    return m_lastError;
}

void Database::checkError(SQLHANDLE handle, SQLSMALLINT type, const char* message) {
    SQLCHAR sqlState[6];
    SQLCHAR messageText[SQL_MAX_MESSAGE_LENGTH];
    SQLINTEGER nativeError;
    SQLSMALLINT length;
    
    SQLGetDiagRec(type, handle, 1, sqlState, &nativeError, messageText, sizeof(messageText), &length);
    
    std::stringstream errorStream;
    errorStream << message << ": " << messageText << " (SQL State: " << sqlState << ", Native Error: " << nativeError << ")";
    
    m_lastError = errorStream.str();
} 