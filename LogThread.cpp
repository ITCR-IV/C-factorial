#include "LogThread.h"
#include "Logger.h"
#include <iostream>

/*!
 * \brief Constructor for LogThread
 *
 * \param type indicates the type of thread that is going to run
 * \param time indicates the sleep time for the thread
 */
LogThread::LogThread(int type, int time, QObject *parent)
    : QThread(parent)
{
    sleepTime = time;
    threadType = type;
}


/*!
 * \brief Override method that handles the logic of the thread
 *
 */

void LogThread::run()
{
    while (true) {
        if (threadType == 1){
            FILE * logFile;

            long fileSize;

            char * fileText;

            logFile = fopen("log.txt", "r");
            if (logFile != NULL){
                fseek (logFile , 0 , SEEK_END);

                fileSize = ftell (logFile);

                rewind (logFile);

                fileText = (char*) malloc (sizeof(char)*fileSize);

                fread(fileText, fileSize+1, 1, logFile);

                fclose(logFile);

                emit logData(fileText);

            } else{
                Logger::EnableFileOutput();
                Logger::Info("se crea .txt para el log");
            }
            msleep(sleepTime);
        }
    }
}
