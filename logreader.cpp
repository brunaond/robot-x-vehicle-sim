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
    transmitData(parsedFileLine);
    qDebug() << parsedFileLine;
    counter++;
    if (counter==2){
          packetno ++;
          counter = 0;
        }
    if (packetno > 2){
          packetno = 0;
    }
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
        if ((var != ';')&& (var != '\r')){
            processData.append(var);
        } else if ((var == ';')|| (var == '\r')){
            parsingData.append(processData);
            processData.clear();
        }
    }

    // Chain for transmission assembly
    // TODO: Complete the parsed string and include a clause which will change the string to a status once in a while.
    if (!parsingData.isEmpty()) {
        switch (packetno){
        case 0:parsedFileLine.append("$MSG,");
               parsedFileLine.append("LAT");
               parsedFileLine.append(',');
               parsedFileLine.append(parsingData.at(0));
               parsedFileLine.append(',');
               parsedFileLine.append(parsingData.at(1));
               parsedFileLine.append(',');
               parsedFileLine.append("LON");
               parsedFileLine.append(',');
               parsedFileLine.append(parsingData.at(2));
               parsedFileLine.append(',');
               parsedFileLine.append(parsingData.at(3));
               /*parsedFileLine.append("LATDG");
               parsedFileLine.append(',');
               parsedFileLine.append(parsingData.at(LATDEG));
               parsedFileLine.append(',');
               parsedFileLine.append(parsingData.at(LATMINS));
               parsedFileLine.append(',');
               parsedFileLine.append(parsingData.at(LATSEC));*/
               //code above has no fake log
               parsedFileLine.append('*');
               parsedFileLine.append(checksum(parsedFileLine));
               parsedFileLine.append('\r');
               parsedFileLine.append('\n'); break;
        case 1:parsedFileLine.append("$MSG,");
               parsedFileLine.append("VLC");
               parsedFileLine.append(',');
               parsedFileLine.append(parsingData.at(5));
               parsedFileLine.append(',');
               parsedFileLine.append("ENG");
               parsedFileLine.append("LS");
               parsedFileLine.append(',');
               parsedFileLine.append("xxx");
               parsedFileLine.append(',');
               parsedFileLine.append("ACX");
               parsedFileLine.append(',');
               parsedFileLine.append(parsingData.at(6));
               parsedFileLine.append(',');
               parsedFileLine.append("ACY");
               parsedFileLine.append(',');
               parsedFileLine.append(parsingData.at(7));
               parsedFileLine.append(',');
               parsedFileLine.append("ACZ");
               parsedFileLine.append(',');
               parsedFileLine.append(parsingData.at(8));
               parsedFileLine.append('*');
               parsedFileLine.append(checksum(parsedFileLine));
               parsedFileLine.append('\r');
               parsedFileLine.append('\n'); break;
        case 2:parsedFileLine.append("$MSG,");
               parsedFileLine.append("HDG");
               parsedFileLine.append(',');
               parsedFileLine.append(parsingData.at(4));
               parsedFileLine.append(',');
               parsedFileLine.append("HDT");
               parsedFileLine.append(',');
               parsedFileLine.append("xxx");
               parsedFileLine.append(',');
               parsedFileLine.append("ACP");
               parsedFileLine.append(',');
               parsedFileLine.append(parsingData.at(9));
               parsedFileLine.append(',');
               parsedFileLine.append("ACW");
               parsedFileLine.append(',');
               parsedFileLine.append(parsingData.at(10));
               parsedFileLine.append(',');
               parsedFileLine.append("ACR");
               parsedFileLine.append(',');
               parsedFileLine.append(parsingData.at(11));
               parsedFileLine.append('*');
               parsedFileLine.append(checksum(parsedFileLine));
               parsedFileLine.append('\r');
               parsedFileLine.append('\n'); break;
        /*case 3:parsedFileLine.append("$MSG,");
               parsedFileLine.append("LDR");
               parsedFileLine.append(',');
               for (int i = 1; i<17;i++){
                   parsedFileLine.append(parsingData.at(i));
               }
               parsedFileLine.append("HP");
               parsedFileLine.append(',');
               parsedFileLine.append(parsingData.at(PHDG));
               parsedFileLine.append(',');
               parsedFileLine.append(parsingData.at(PHPTH));
               parsedFileLine.append(',');
               parsedFileLine.append("HPF");
               parsedFileLine.append(',');
               parsedFileLine.append(parsingData.at(FREQ));
               parsedFileLine.append('*');
               parsedFileLine.append(checksum(parsedFileLine));
               parsedFileLine.append('\r');
               parsedFileLine.append('\n'); break;
        case 4:parsedFileLine.append("$MSG,");
               parsedFileLine.append("GPS");
               parsedFileLine.append(',');
               parsedFileLine.append(parsingData.at(1/0));
               parsedFileLine.append(',');
               parsedFileLine.append(parsingData.at(1/0)));
               parsedFileLine.append(',');
               parsedFileLine.append("IMU");
               parsedFileLine.append(',');
               parsedFileLine.append(parsingData.at(1/0)));
               parsedFileLine.append('*');
               parsedFileLine.append(checksum(parsedFileLine));
               parsedFileLine.append('\r');
               parsedFileLine.append('\n'); break;*/
               //NO FAKE LOG

        default: parsedFileLine.append("error");
        }
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
