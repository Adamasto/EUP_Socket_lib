#ifndef UP_SERVER_H
#define UP_SERVER_H

#include <QObject>
#include <QUdpSocket>
#include <iostream>
#include <stdio.h>
#include <string.h>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <windows.h>
#include <QTimer>


class up_server : public QObject
{
    Q_OBJECT
public:
       explicit up_server(QObject *parent = 0);

    QUdpSocket *m_sock;


    bool startSender(quint32 time = 1000);
    bool stopTimer();

public slots:
    void readed_slot();
    void sender_slot();
signals:
    void reciver_msg(QString msg);

private:
    QHostAddress host_addr;
    quint16 m_port;
    QTimer *senderTimer;
    quint32 nember_packet;
};

#endif // UP_SERVER_H
