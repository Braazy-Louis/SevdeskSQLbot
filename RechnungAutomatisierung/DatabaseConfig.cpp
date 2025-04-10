#include "DatabaseConfig.h"
#include <sstream>

DatabaseConfig::DatabaseConfig()
    : m_driverName("MySQL ODBC 9.2 Unicode Driver") // https://cdn.mysql.com//Downloads/Connector-ODBC/9.2/mysql-connector-odbc-9.2.0-winx64.msi    
    , m_server("")
    , m_database("")
    , m_username("")
    , m_password("")
    , m_port(3306)
{
}

DatabaseConfig::DatabaseConfig(
    const std::string& driverName,
    const std::string& server,
    const std::string& database,
    const std::string& username,
    const std::string& password,
    int port
)
    : m_driverName(driverName)
    , m_server(server)
    , m_database(database)
    , m_username(username)
    , m_password(password)
    , m_port(port)
{
}

std::string DatabaseConfig::getDriverName() const {
    return m_driverName;
}

void DatabaseConfig::setDriverName(const std::string& driverName) {
    m_driverName = driverName;
}

std::string DatabaseConfig::getServer() const {
    return m_server;
}

void DatabaseConfig::setServer(const std::string& server) {
    m_server = server;
}

std::string DatabaseConfig::getDatabase() const {
    return m_database;
}

void DatabaseConfig::setDatabase(const std::string& database) {
    m_database = database;
}

std::string DatabaseConfig::getUsername() const {
    return m_username;
}

void DatabaseConfig::setUsername(const std::string& username) {
    m_username = username;
}

std::string DatabaseConfig::getPassword() const {
    return m_password;
}

void DatabaseConfig::setPassword(const std::string& password) {
    m_password = password;
}

int DatabaseConfig::getPort() const {
    return m_port;
}

void DatabaseConfig::setPort(int port) {
    m_port = port;
}

std::string DatabaseConfig::getConnectionString() const {
    std::stringstream connStr;
    connStr << "DRIVER={" << m_driverName << "};"
            << "SERVER=" << m_server << ";"
            << "DATABASE=" << m_database << ";"
            << "UID=" << m_username << ";"
            << "PWD=" << m_password << ";"
            << "PORT=" << m_port << ";";
    
    return connStr.str();
} 