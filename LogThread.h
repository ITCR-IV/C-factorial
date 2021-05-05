#ifndef LOGTHREAD_H
#define LOGTHREAD_H

#include <QThread>

//! Class that handles thread events
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

    //! indicates the sleep time on miliseconds
    int sleepTime;

    //! indicates the type of thread
    int threadType;
};

#endif // LOGTHREAD_H
