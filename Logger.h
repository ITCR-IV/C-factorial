#ifndef C_FACTORIAL_LOGGER_H
#define C_FACTORIAL_LOGGER_H

#include <stdio.h>
#include <ctime>
#include <string>

using namespace std;



class Logger {
    enum LogLevel
    {
        All,
        INFO,
        WARNING,
        ERROR
    };

private:

    // singleton
    static Logger *instance;
    static Logger *getInstance();
    Logger(const Logger&) = delete;
    Logger& operator= (const Logger&) = delete;

    // Logger stuff
    LogLevel Level = All;
    string LevelNames[5] = { "ALL","INFO","WARNING", "ERROR", "NEEDED" };
    const char * filepath = 0;
    FILE* file = 0;

    Logger() {}

    ~Logger()
    {
        free_file();
    }
    void log(LogLevel newlevel, string message);

    string getCurrentDate();

    void enable_file_output();

    void free_file();

public:


    static void SetPriority(LogLevel newLevel)
    {
        getInstance()->Level = newLevel;
    }

    static void EnableFileOutput();

    static void Info(const char* message)
    {
        getInstance()->log(INFO, message);
    }

    static void Warn(const char* message)
    {
        getInstance()->log(WARNING, message);
    }

    static void Error(const char* message)
    {
        getInstance()->log(ERROR, message);
    }

};


#endif
