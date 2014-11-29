#ifndef SERIALTEST_H
#define SERIALTEST_H
#include <QObject>
#include <QtSerialPort/QSerialPort>
class SerialTest : public QSerialPort
{
    Q_OBJECT
    Q_PROPERTY(QString receivedata READ receivedata WRITE setreceivedata NOTIFY receivedataChanged)//从串口收到的数据
    Q_PROPERTY(QString sendnumber READ sendnumber WRITE setsendnumber NOTIFY sendnumberChanged)//发送的数据字节统计
    Q_PROPERTY(QString receivenumber READ receivenumber WRITE setreceivenumber NOTIFY receivenumberChanged)//接收的数据字节统计
public:
    struct Settings {//端口设定结构体
        QString name;
        qint32 baudRate;
        QSerialPort::DataBits dataBits;
        QSerialPort::Parity parity;
        QSerialPort::StopBits stopBits;
        QSerialPort::FlowControl flowControl;
    };
    SerialTest(QSerialPort *parent = 0);


    QString receivedata(void);
    void setreceivedata(QString receivedata);

    QString sendnumber();
    void setsendnumber(QString sendnumber);

    QString receivenumber();
    void setreceivenumber(QString receivenumber);

    Q_INVOKABLE void openAndSetPort(int PortNameIndex,int BaudRateIndex,int DatabitsIndex,int ParityIndex,int StopbitsIndex,int FlowcontrolIndex);//打开并设定端口;
    Q_INVOKABLE void closePort();//关闭端口;
    Q_INVOKABLE void sendto(QString sendmessage);//发送数据;
    Q_INVOKABLE void clearnumber();//数据统计清零;

signals:
    void receivedataChanged();

    void receivenumberChanged();
    void sendnumberChanged();


public slots:
    void receivefrom();//信号（收到数据激发的信号）响应函数

private:
    QString m_receivedata;
    QString m_sendnumber,m_receivenumber;
};

#endif // SERIALTEST_H
