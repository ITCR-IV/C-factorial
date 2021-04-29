#include "Logger.h"

Logger *Logger::instance = nullptr;

Logger Logger::getInstance()
{
    if (instance == nullptr)
    {
        instance = new Logger();
    }
    return instance;
}

void Logger::log(int newlevel, string message)
{
    if (this->Level <= newlevel)
    {
        if (file)
        {
            string currentTime = this->getCurrentDate();
            fprintf(file, "%s\t", buffer);
            fprintf(file, LevelNames[newlevel]);
            fprintf(file, message);
            fprintf(file, "\n");
        }
    }
}

string Logger::getCurrentDate(){
    time_t now = time(0);
    struct tm  tstruct;
    char  buf[80];
    tstruct = *localtime(&now);

    strftime(buf, sizeof(buf), "%Y-%m-%d %X", &tstruct);

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

static void Logger::EnableFileOutput()
{
    Logger& logger_instance = get_instance();
    logger_instance->filepath = "log.txt";
    logger_instance->enable_file_output();
}