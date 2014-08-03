#include <QCoreApplication>
#include <QTimer>
#include <QtNetwork>
#include <QFile>
#include <QDebug>
#include "logreader.h"
#include <iostream>

#define PORT 25050
#define FILE_PATH "D:\\ntu\\ureca\\robot-x-vehicle-sim\\fake_log.csv"


int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    QTimer timer;
    QTimer heartbeatTimer;
    Logreader logger(FILE_PATH, "127.0.0.1", 25050);
    logger.readAndTransmit();
    logger.transmitHeartbeat();

    QObject::connect(&timer,SIGNAL(timeout()), &logger, SLOT(readAndTransmit()));
    timer.start(1000);
    QObject::connect(&heartbeatTimer, SIGNAL(timeout()), &logger, SLOT(transmitHeartbeat()));
    heartbeatTimer.start(1000);

    return a.exec();
}
