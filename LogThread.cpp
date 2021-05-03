#include "LogThread.h"

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
            fseek (logFile , 0 , SEEK_END);

            fileSize = ftell (logFile);

            rewind (logFile);

            fileText = (char*) malloc (sizeof(char)*fileSize);

            fread(fileText, fileSize+1, 1, logFile);

            fclose(logFile);

            emit logData(fileText);
            msleep(sleepTime);
        }
    }
}
