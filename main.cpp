#include <QCoreApplication>
#include <QTimer>
#include <QtNetwork>
#include <QFile>
#include <QDebug>
#include "logreader.h"
#include <iostream>

#define PORT 25050
#define FILE_PATH "C:\\Qt\\Projects\\build-LogReader-Desktop_Qt_5_3_0_MinGW_32bit-Debug\\debug\\fake_log_short.csv"


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
