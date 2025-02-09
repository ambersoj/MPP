#include "Logger.hpp"
#include <ctime>
#include <iomanip>
#include <sstream>

// Get Singleton Instance
Logger& Logger::getInstance() {
    static Logger instance;
    return instance;
}

// Private Constructor
Logger::Logger() : logFileName("logs/mpp.log") {
    logFile.open(logFileName, std::ios::app);
    if (!logFile.is_open()) {
        std::cerr << "[LOGGER] ERROR: Could not open log file: " << logFileName << std::endl;
    }
}

// Destructor
Logger::~Logger() {
    if (logFile.is_open()) {
        logFile.close();
    }
}

// Set log file manually (optional)
void Logger::setLogFile(const std::string& filename) {
    std::lock_guard<std::mutex> lock(logMutex);
    logFileName = filename;
    logFile.close();
    logFile.open(logFileName, std::ios::app);
    if (!logFile.is_open()) {
        std::cerr << "[LOGGER] ERROR: Could not open log file: " << logFileName << std::endl;
    }
}

// Log function
void Logger::log(const std::string& message) {
    std::lock_guard<std::mutex> lock(logMutex);

    // Get current timestamp
    std::time_t now = std::time(nullptr);
    std::tm timeStruct = *std::localtime(&now);
    std::ostringstream timestamp;
    timestamp << std::put_time(&timeStruct, "[%Y-%m-%d %H:%M:%S]");

    // Full log message
    std::string logMessage = timestamp.str() + " " + message;

    // Print to console
    std::cout << logMessage << std::endl;

    // Write to file
    writeToFile(logMessage);
}

// Write to file (private function)
void Logger::writeToFile(const std::string& message) {
    if (logFile.is_open()) {
        logFile << message << std::endl;
    }
}
