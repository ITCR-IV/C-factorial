#ifndef LOGGER_H
#define LOGGER_H

#include <stdio.h>
#include <ctime>
#include <string>
#include <iostream>

using namespace std;


//! Class handles the erros, info and warnings outputs
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

    static Logger &getInstance();

    Logger(const Logger&) = delete;
    Logger& operator= (const Logger&) = delete;

    //! init level
    LogLevel Level = All;

    //! array with the names of the levels
    string LevelNames[5] = { "ALL","INFO","WARNING", "ERROR", "NEEDED" };

    //! stores the filepath of the file
    const char * filepath = 0;

    //! stores the file
    FILE* file = 0;
    char buffer[80];

    Logger() {};

    ~Logger()
    {
        free_file();
    };
    void log(LogLevel newlevel, string message);

    string getCurrentDate();

    void enable_file_output();

    void free_file();

public:

    //! set level priority
    static void SetPriority(LogLevel newLevel)
    {
        getInstance().Level = newLevel;
    }

    static void EnableFileOutput();

    //! append a info message on the file
    static void Info(const char* message)
    {
        string tmp = message;
        string tmp2 = tmp + "   ";
        getInstance().log(INFO, tmp2);
    }

    //! append a warning message on the file
    static void Warn(const char* message)
    {
        string tmp = message;
        string tmp2 = tmp + "   ";
        getInstance().log(WARNING, tmp2);
    }

    //! append a error message on the file
    static void Error(const char* message)
    {
        string tmp = message;
        string tmp2 = tmp + "   ";
        getInstance().log(ERROR, tmp2);
    }

};


#endif
