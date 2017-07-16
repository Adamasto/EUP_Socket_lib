#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include "UdpSocket.h"


using namespace std;
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    SOCKET *sock;

public slots:
    void timer_slot();

private slots:
    void on_sender_btn_clicked();

    void on_spin_timer_editingFinished();

private:
    Ui::MainWindow *ui;
    QTimer *timer;
    UdpSocket *udp;

    int msg_count;

    bool inited;

    string m_host;
    int m_port;

};

#endif // MAINWINDOW_H
