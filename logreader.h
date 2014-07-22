#ifndef LOGREADER_H
#define LOGREADER_H

#include <QObject>
#include <QFile>
#include <QTimer>
#include <QtNetwork>
#include <QDebug>
#include <iostream>

class Logreader : public QObject
{
    Q_OBJECT

public:    
    Logreader(QString filename, QString address, quint16 port);
    Logreader(QString filename, QHostAddress address, quint16 port);

protected:
    QFile file;
    QString filename;
    QString line;
    QFile *log;
    QTextStream *inStream;
    QByteArray payload;
    QUdpSocket transmitter;
    QHostAddress *address;
    quint16 port;

    QString checksum(QByteArray inData);
    QString checksum(QString inData);

public:
    QString getLine();
public slots:
    int readAndTransmit();

private:
    QString readDataFromFile();
    QString parseData(QString fileLine);
    void transmitData(QString);
};

#endif // LOGREADER_H
