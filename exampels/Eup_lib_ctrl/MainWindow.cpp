#include "MainWindow.h"
#include "ui_MainWindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(timer_slot()));
    m_host = "239.255.170.170";
    m_port = 17017;
    udp = new UdpSocket();
    msg_count = 0;
    inited = false;
    if(!udp->Bind(m_host, m_port))
    {
        ui->recv_data->setText(QString::fromStdString(udp->getErrorString()));
        ui->sender_btn->setEnabled(false);
    }

    if(!udp->setMulticastGroup(m_host))
    {
        ui->recv_data->setText(QString::fromStdString(udp->getErrorString()));
        ui->sender_btn->setEnabled(false);
    }

    inited = true;

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::timer_slot()
{
    if(!ui->sender_btn->isEnabled())
    {
        timer->stop();
        return;
    }


   int datalen = 600;
   char in_buf[datalen];
   memset(in_buf, 0, datalen);

   if(udp->ReceiveData(in_buf, 1024, m_host, &m_port) > 0)
   {
      ui->recv_data->setText(QString(in_buf));
      return;
   }

   ui->recv_data->setText(QString::fromStdString(udp->getErrorString()));

}

void MainWindow::on_sender_btn_clicked()
{
    if(timer->isActive())
    {
        timer->stop();
        ui->sender_btn->setText("Старт");
        return;
    }

    if(ui->spin_timer->value())
    {
        timer->start(ui->spin_timer->value() *1000);
        ui->sender_btn->setText("Стопэ");
    }
}

void MainWindow::on_spin_timer_editingFinished()
{

}
