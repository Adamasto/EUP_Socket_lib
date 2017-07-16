#include "up_server.h"


up_server::up_server(QObject *parent) : QObject(parent)
{
    m_sock = new QUdpSocket();
    host_addr = QHostAddress("239.255.170.170");
    m_port = 17017;
    nember_packet = 0;

    m_sock->bind(QHostAddress::AnyIPv4, m_port, QUdpSocket::ShareAddress);
    m_sock->joinMulticastGroup(host_addr);

    connect(m_sock, SIGNAL(readyRead()), this, SLOT(readed_slot()));
    senderTimer = new QTimer();
    connect(senderTimer, SIGNAL(timeout()), this, SLOT(sender_slot()));

    qDebug() << "Sever to start!";


}

bool up_server::startSender(quint32 time)
{
    nember_packet = 1;
    if(senderTimer->isActive())
        senderTimer->stop();

    senderTimer->start(time);
    return senderTimer->isActive();
}

bool up_server::stopTimer()
{
    senderTimer->stop();
    return !senderTimer->isActive();
}

void up_server::readed_slot()
{

//    QHostAddress client_addr = m_sock->peerAddress();
//    while (m_sock->hasPendingDatagrams())
//    {
//        QByteArray datagram;
//        datagram.resize(m_sock->pendingDatagramSize());
//        m_sock->readDatagram(datagram.data(), datagram.size());
//        QString msg = tr("Received datagram: %1")
//                             .arg(datagram.data());

//        qDebug() << msg;

//        QString out_msg = QString("Multicast message %1").arg(datagram.data());

//        QByteArray datagram_out = QByteArray::fromStdString(out_msg.toStdString());
//        m_sock->writeDatagram(datagram_out.data(), datagram_out.size(),
//                                 client_addr, 45454);

////        emit reciver_msg(msg);
//    }
////    QString out_msg = QString("Multicast message %1").arg(datagram.data());

////    QByteArray datagram_out = QByteArray::fromStdString(out_msg.toStdString());
////    m_sock->writeDatagram(datagram_out.data(), datagram_out.size(),
//    //                             client_addr, 45454);
}

void up_server::sender_slot()
{
    QString out_msg = QString("Multicast message value: %1").arg(nember_packet);

    QByteArray datagram_out = QByteArray::fromStdString(out_msg.toStdString());
    m_sock->writeDatagram(datagram_out.data(), host_addr, m_port);
    nember_packet ++;
    qDebug() << out_msg;
}
