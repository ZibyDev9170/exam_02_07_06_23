#include "mytcpserver-ex.h"
#include <QDebug>
#include <QCoreApplication>

MyTcpServer::~MyTcpServer()
{
    //mTcpSocket->close();
    mTcpServer->close();
    server_status=0;
}
MyTcpServer::MyTcpServer(QObject *parent) : QObject(parent){
    mTcpServer = new QTcpServer(this);
    connect(mTcpServer, &QTcpServer::newConnection, this,
            &MyTcpServer::slotNewConnection);

    if (!mTcpServer->listen(QHostAddress::Any, 33333)) {
        qDebug() << "server is not started";
    } else {
        server_status = 1;
        qDebug() << "server is started";
    }
}

void MyTcpServer::slotNewConnection(){
    if (server_status == 1) {
        QTcpSocket *cur_mTcpSocket;
        cur_mTcpSocket = mTcpServer->nextPendingConnection();
        cur_mTcpSocket->write("Hello, World!!! I am echo server!\r\n");
        connect(cur_mTcpSocket, &QTcpSocket::readyRead, this,
                &MyTcpServer::slotServerRead);
        connect(cur_mTcpSocket, &QTcpSocket::disconnected, this,
                &MyTcpServer::slotClientDisconnected);
        mTcpSocket.insert(cur_mTcpSocket->socketDescriptor(), cur_mTcpSocket);
    }
}

void MyTcpServer::slotServerRead(){ 
    QTcpSocket *curSocket = static_cast<QTcpSocket*>(sender());
    QByteArray userData = curSocket->readAll();

    QString userString(userData);

    QStringList parts = userString.split('&');
    if (parts.size() == 3 && parts[0] == "sort") {
        bool ok;
        int step = parts[1].toInt(&ok);
        if (ok) {
            QStringList numbers = parts[2].split(' ');

            int n = numbers.size();
            int *arr = new int[n];
            for (int i = 0; i < n; i++) {
                arr[i] = numbers[i].toInt();
            }

            pyramidSortStep(arr, n, step);

            QByteArray responseData;
            for (int i = 0; i < n; i++) {
                responseData += QByteArray::number(arr[i]) + " ";
            }

            QString response = "Step " + QString::number(step) + ": " + responseData.trimmed() + "\r\n";
            curSocket->write(response.toUtf8());
            curSocket->flush();
            delete[] arr;
        }
    }
}

void MyTcpServer::slotClientDisconnected(){
    QTcpSocket *cur_mTcpSocket = (QTcpSocket *)sender();
    cur_mTcpSocket->close();
    auto iter = mTcpSocket.begin();
    while (iter.value() != cur_mTcpSocket) iter++;
    mTcpSocket.erase(iter);
}
// 5 3 2 9 7 4 10 1 6 8
// help func to algo
void MyTcpServer::pyramid(int arr[], int count)
    {
        int start = ((count - 2) / 2 - 1) / 2;
        while (start >= 0)
        {
            pyramidDown(arr, start, count - 1);
            --start;
        }
    }

void MyTcpServer::pyramidSortStep(int arr[], int n, int step)
{
    if (step < 0 || step >= n)
        return;

    pyramid(arr, n);

    int end = n - 1;
    while (end > 0)
    {
        std::swap(arr[0], arr[end]);
        --end;
        if (end == step)
            return;
        pyramidDown(arr, 0, end);
    }
}

// main algo
void MyTcpServer::pyramidDown(int arr[], int start, int end)
{
    int root = start;
    while (2 * root + 1 <= end)
    {
        int child = 2 * root + 1;
        int temp = root;
        if (arr[temp] < arr[child])
        {
            temp = child;
        }
        if (child + 1 <= end && arr[temp] < arr[child + 1])
        {
            temp = child + 1;
        }
        if (temp == root)
        {
            return;
        }
        else
        {
            std::swap(arr[root], arr[temp]);
            root = temp;
        }
    }
}

