#ifndef C_FACTORIAL_LOGGER_H
#define C_FACTORIAL_LOGGER_H

#include <stdio.h>
#include <ctime>
#include <string>

using namespace std;
class Logger {

private:

    // singleton
    static Logger *instance;
    static Logger *getInstance();
    Logger(const Logger&) = delete;
    Logger& operator= (const Logger&) = delete;

    // Logger stuff
    LogLevel Level = ALL;
    string LevelNames["ALL","INFO","WARNING","ERROR"];
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

    enum LogLevel
    {
        All,
        INFO,
        WARNING,
        ERROR
    };


    static void SetPriority(LogPriority new_priority)
    {
        get_instance().priority = new_priority;
    }

    static void EnableFileOutput();

    static void Info(const char* message)
    {
        get_instance().log(INFO, InfoPriority);
    }

    static void Warn(const char* message)
    {
        get_instance().log(WARNING, message);
    }

    static void Error(const char* message)
    {
        get_instance().log(ERROR, message);
    }

};


#endif
