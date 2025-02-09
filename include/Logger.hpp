#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <string>
#include <iostream>
#include <fstream>
#include <mutex>

class Logger {
public:
    static Logger& getInstance();

    void log(const std::string& message);
    void setLogFile(const std::string& filename);

private:
    Logger(); // Private constructor (Singleton)
    ~Logger();

    std::ofstream logFile;
    std::mutex logMutex;
    std::string logFileName;

    void writeToFile(const std::string& message);
};

#endif // LOGGER_HPP
