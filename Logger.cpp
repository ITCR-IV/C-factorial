#include "Logger.h"
#include <iostream>
#include "Mainwindow.h"

Logger *Logger::instance = nullptr;

/*!
 * \brief singleton constructor
 *
 */

Logger &Logger::getInstance()
{
    static Logger instance;
    return instance;
}

/*!
 * \brief writes on the log.txt file the log message
 *
 * \param newLevel indicates the level
 * \param message indicates the message to append on the file
 */
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

/*!
 * \brief this method creates the format of the date and hour
 *
 * \return string with the date
 */
string Logger::getCurrentDate(){
    time_t now = time(0);
    struct tm  tstruct;
    char  buf[80];
    tstruct = *localtime(&now);

    strftime(buf, sizeof(buf), "%Y-%m-%d %X     ", &tstruct);

    return string(buf);
};

/*!
 * \brief check if the file exists
 */
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

/*!
 * \brief closes the log.txt file
 *
 */
void Logger::free_file()
{
    fclose(file);
    file = 0;
}

/*!
 * \brief this methot call the singleton and creates the file output
 *
 */
void Logger::EnableFileOutput()
{
    Logger& logger_instance = getInstance();
    logger_instance.filepath = "log.txt";
    logger_instance.enable_file_output();
}
