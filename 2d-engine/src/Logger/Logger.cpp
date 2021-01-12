#include "Logger.h"
#include <iostream>
#include <string>
#include <chrono>
#include <ctime>
#include <time.h>

std::vector<LogEntry> Logger::messages;

std::string CurrentDateTimeToString() {
    time_t now = time(0);
    tm ltm;
    localtime_s(&ltm, &now);
    std::string output( 22, '\0');
    std::strftime(&output[0], output.size(), " %d-%b-%Y %H:%M:%S", &ltm);
    return output;
}

void Logger::Log(const std::string& message) {
    LogEntry logEntry;
    logEntry.type = LOG_INFO;
    logEntry.message = "LOG: [" + CurrentDateTimeToString() + "]: " + message;
    std::cout << logEntry.message << std::endl;
    messages.push_back(logEntry);
}

void Logger::Err(const std::string& message) {
    LogEntry logEntry;
    logEntry.type = LOG_ERROR;
    logEntry.message = "ERR: [" + CurrentDateTimeToString() + "]: " + message;
    messages.push_back(logEntry);
    std::cerr << logEntry.message<< std::endl;
}
