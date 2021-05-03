#ifndef LOGTHREAD_H
#define LOGTHREAD_H

#include <QThread>

class LogThread : public QThread
{
    Q_OBJECT
public:
    LogThread(int type, int sleepTime, QObject *parent = nullptr);
signals:
    void logData(char *);

protected:
    void run() override;

private:
    int sleepTime; //milisegundos
    int threadType;
};

#endif // LOGTHREAD_H
