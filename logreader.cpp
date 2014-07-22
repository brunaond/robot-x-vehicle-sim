#include "logreader.h"


Logreader::Logreader(QString filename, QString address, quint16 port){
    if (address == "127.0.0.1") {
        this->address = new QHostAddress(QHostAddress::LocalHost);
    } else {
        this->address = new QHostAddress(address);
    }
    this->port = port;
    this->log = new QFile(filename);

    if (this->log->open(QIODevice::ReadOnly)) {
        qDebug() << "File: " << filename << "successfuly opened." << "\n";
    } else {
        qDebug() << "Opening file" << filename << "failed." << "\n";
    }
}

Logreader::Logreader(QString filename, QHostAddress address, quint16 port){
    this->address = &address;
    this->port = port;

    this->log = new QFile(filename);

    if (this->log->open(QIODevice::ReadOnly)) {
        qDebug() << "File: " << filename << "successfuly opened." << "\n";
    } else {
        qDebug() << "Opening file" << filename << "failed." << "\n";
    }
}

QString Logreader::getLine(){
    return this->line;
}

int Logreader::readAndTransmit(){
    QString fileLine;
    QString parsedFileLine;
    fileLine = readDataFromFile();
    parsedFileLine = parseData(fileLine);
    //transmitData(parsedFileLine);
    qDebug() << parsedFileLine;
    return 0;
}

QString Logreader::readDataFromFile(){
    QString fileLine;
    if ((log->isReadable()) && (!log->atEnd())){
        fileLine = this->log->readLine();
    } else if (!log->isReadable()) {
        qDebug() << "File " << this->filename << " is not readable.\n";
    } else if (log->atEnd()) {
        log->seek(0);
    }
    return fileLine;
}

QString Logreader::parseData(QString fileLine){
    QList<QString> parsingData;
    QString parsedFileLine;
    QString processData;

    processData.clear();
    parsingData.clear();
    parsedFileLine.clear();

    foreach (QChar var, fileLine) {
        if (var != ';'){
            processData.append(var);
        } else if (var == ';') {
            parsingData.append(processData);
            processData.clear();
        }
    }

    // Chain for transmission assembly
    // TODO: Complete the parsed string and include a clause which will change the string to a status once in a while.
    if (!parsingData.isEmpty()) {
        parsedFileLine.append("$MSG,");
        parsedFileLine.append("LAT");
        parsedFileLine.append(',');
        parsedFileLine.append(parsingData.at(0));
        parsedFileLine.append(',');
        parsedFileLine.append(parsingData.at(1));
        parsedFileLine.append('*');
        parsedFileLine.append(checksum(parsedFileLine));
        parsedFileLine.append('\r');
        parsedFileLine.append('\n');
    }

    return parsedFileLine;
}

void Logreader::transmitData(QString inString){
    transmitter.writeDatagram(inString.toLatin1(), *this->address, port);
}

QString Logreader::checksum(QByteArray inData){
    QString table = "0123456789ABCDEF";
    char result[2];
    QString outChecksum;

    unsigned char checksum = 0x00;


    for (int i = 1; i < inData.length()-1; i++) {
        if (inData.at(i)=='*') {
            break;
        }
        checksum ^= (unsigned char) inData.at(i);
    }

    result[1] = table.at(checksum&0x0F).toLatin1();
    result[0] = table.at((checksum>>4)&0x0F).toLatin1();
    outChecksum.insert(0,result[0]);
    outChecksum.insert(1,result[1]);
    return outChecksum;
}

QString Logreader::checksum(QString inData){
    return checksum(inData.toLatin1());
}
