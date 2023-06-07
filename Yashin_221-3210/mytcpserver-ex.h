#ifndef MYSERVER_H
#define MYSERVER_H
#include <QByteArray>
#include <QDebug>
#include <QMap>
#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QtNetwork>

class MyTcpServer : public QObject
{
    Q_OBJECT
public:
    explicit MyTcpServer(QObject *parent = nullptr);
    ~MyTcpServer();
public slots:
    void slotNewConnection();
    void slotClientDisconnected();

    void slotServerRead();
    //void slotReadClient();
private:
    QTcpServer *mTcpServer;
    QMap<int, QTcpSocket *> mTcpSocket;
    int server_status;

    void pyramid(int arr[], int count);
    void pyramidSortStep(int arr[], int n, int step);
    void pyramidDown(int arr[], int start, int end);
};
#endif // MYSERVER_H







