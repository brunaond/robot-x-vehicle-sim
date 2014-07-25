#include <QCoreApplication>
#include <QTimer>
#include <QtNetwork>
#include <QFile>
#include <QDebug>
#include "logreader.h"
#include <iostream>

#define PORT 25050
#define FILE_PATH "C:\\_work\\robot-x-vehicle-sim\\fake_log.csv"


int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    QTimer timer;
    Logreader logger(FILE_PATH, "127.0.0.1", 25050);
    logger.readAndTransmit();

    QObject::connect(&timer,SIGNAL(timeout()), &logger, SLOT(readAndTransmit()));
    timer.start(1000);

    return a.exec();
}
