#include "Logger.h"
#include <iostream>
#include "Mainwindow.h"

Logger *Logger::instance = nullptr;

Logger &Logger::getInstance()
{
    static Logger instance;
    return instance;
}

void Logger::log(LogLevel newlevel, string message)
{
    if (this->Level <= newlevel)
    {
        if (file)
        {
            string currentTime = this->getCurrentDate();
            fprintf(file, "%s   ", buffer);
            fprintf(file, currentTime.c_str());
            string levelString = LevelNames[newlevel] + "   ";
            fprintf(file, levelString.c_str());
            fprintf(file, message.c_str());
            fprintf(file, "\n");

            string showString = currentTime + levelString + message;

        }
    }
}

string Logger::getCurrentDate(){
    time_t now = time(0);
    struct tm  tstruct;
    char  buf[80];
    tstruct = *localtime(&now);

    strftime(buf, sizeof(buf), "%Y-%m-%d %X     ", &tstruct);

    return string(buf);
};

void Logger::enable_file_output()
{
    if (file != 0)
    {
        fclose(file);
    }

    file = fopen(filepath, "a");

    if (file == 0)
    {
        printf("Logger: Failed to open file at %s", filepath);
    }
}

void Logger::free_file()
{
    fclose(file);
    file = 0;
}

void Logger::EnableFileOutput()
{
    Logger& logger_instance = getInstance();
    logger_instance.filepath = "log.txt";
    logger_instance.enable_file_output();
}
