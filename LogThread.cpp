#include "LogThread.h"
#include "Logger.h"
#include <iostream>

LogThread::LogThread(int type, int time, QObject *parent)
    : QThread(parent)
{
    sleepTime = time;
    threadType = type;
}

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

            } else{

                printf("no hay file");
                Logger::EnableFileOutput();
                Logger::Info("se crea .txt para el log");
            }


            emit logData(fileText);
            msleep(sleepTime);
        }
    }
}
