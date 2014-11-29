#include "serialtest.h"
#include<iostream>

SerialTest::Settings currentsetting;//定义设定值结构体的结构体变量
QSerialPort serialtest;

qint64 c_sendnumber,c_receivenumber;

SerialTest::SerialTest(QSerialPort *parent):QSerialPort (parent),m_receivedata("Receive Label"),m_receivenumber("0"),m_sendnumber("0")
{
    QObject::connect(&serialtest, SIGNAL(readyRead()),this, SLOT(receivefrom()));//将端口收到数据产生的信号绑定receivefrom()函数;
}

//打开端口并设置:函数的参数（……Index由qml中combobox的currentIndex决定），由按钮触发
void SerialTest::openAndSetPort(int PortNameIndex,
                                int BaudRateIndex,
                                int DatabitsIndex,
                                int ParityIndex,
                                int StopbitsIndex,
                                int FlowcontrolIndex)
{
    ////////////////////1.得到当前选择的各项设置//////////////////////////////
    //得到当前端口名
    QString allname[6]={"COM1","COM2","COM3","COM4","COM5","COM6"};//列举所有的端口名
    currentsetting.name=allname[PortNameIndex];//由qml里表示name的combobox的currentIndex来确定当前的name
    std::cout<<" ok setPortName to "+ currentsetting.name.toStdString()<< std::endl;//通过输出来验证设定成功

    //得到当前波特率
    qint32 allbauRate[4]={9600,19200,38400,115200};
    currentsetting.baudRate=allbauRate[BaudRateIndex];

    //得到当前发送位数
    QSerialPort::DataBits allDatabits[4]={QSerialPort::Data5,
                                          QSerialPort::Data6,
                                          QSerialPort::Data7,
                                          QSerialPort::Data8};
    currentsetting.dataBits=allDatabits[DatabitsIndex];

    //得到当前Parity
    QSerialPort::Parity allparity[5]={QSerialPort::NoParity,
                                      QSerialPort::EvenParity,
                                      QSerialPort::OddParity,
                                      QSerialPort::MarkParity,
                                      QSerialPort::SpaceParity};
    currentsetting.parity=allparity[ParityIndex];

    //得到当前停止位
    QSerialPort::StopBits allstopBits[3]={QSerialPort::OneStop,
                                          QSerialPort::OneAndHalfStop,
                                          QSerialPort::TwoStop};
    currentsetting.stopBits=allstopBits[StopbitsIndex];

    //得到当前FlowControl
    QSerialPort::FlowControl allflowControl[3]={QSerialPort::NoFlowControl,
                                                QSerialPort::HardwareControl,
                                                QSerialPort::SoftwareControl};
    currentsetting.flowControl=allflowControl[FlowcontrolIndex];


////////////////////2.设定当前端口名//////////////////////////////
    serialtest.setPortName(currentsetting.name);

////////////////////3.打开这一端口并按照当前设置信息进行设置//////////////////////////////
    if (serialtest.open(QIODevice::ReadWrite))//打开这一端口
    {
        std::cout<<"open port sucess"<<std::endl;

        if(serialtest.setBaudRate(currentsetting.baudRate)//设置各项信息
                && serialtest.setDataBits(currentsetting.dataBits)
                && serialtest.setParity(currentsetting.parity)
                && serialtest.setStopBits(currentsetting.stopBits)
                && serialtest.setFlowControl(currentsetting.flowControl))
        {
            std::cout<<"set sucess"<<std::endl;
        }
    }
}


////////////////////4.发送数据//////////////////////////////
void SerialTest::sendto(QString sendmessage)//此函数由qml里的send按钮触发，sendmessage来源于qml文本框的当前文本，
{
    QByteArray data = sendmessage.toLocal8Bit()+'\r';//将QString转为QByteArray，并加上'\r'（回车符）,因为芯片要求在回车符之后再返回数据
    qint64 testwritenumber=serialtest.write(data);//写入数据

    m_receivedata=m_receivedata+"\n";//加上换行符便于显示

    c_sendnumber=c_sendnumber+testwritenumber-1;//发送数据字节数统计（减去回车符）
    setsendnumber(QString ::number(c_sendnumber));//更新发送的数据字节总数


}

void SerialTest::setsendnumber(QString sendnumber)//更新发送的数据字节总数，触发sendnumberChanged()的消息响应函数sendnumber()来更新显示
{
    m_sendnumber=sendnumber;
    emit sendnumberChanged();
}

QString SerialTest::sendnumber()//响应sendnumberChanged()消息
{
    return m_sendnumber;
}

////////////////////4.接收数据//////////////////////////////
void SerialTest::receivefrom()//由readyRead()消息出发（在前边进行绑定），当串口收到数据此消息被激活（对于串口，每发送出去一个字节，都会将此字节返回，触发readyread消息，当芯片有特殊指令时，收到的信息更多，比如对sim900，发送0000，芯片就会受到0000，但是发送AT，会受到 AT OK）
{
    QByteArray data = serialtest.readAll();//读取所有收到的数据

    QString receivedata=data.data();//将QByteArray转为QString来显示
    m_receivedata= m_receivedata+receivedata;//将某次收到的数据进行累加，因为如果不累加的话每次有readyread就会触发此函数，会重置m_receivedata，覆盖之前收到的数据
    emit receivedataChanged();//发送消息触发receivedata()，更新当前收到的数据显示receivedata

    qint64 testreadnumber=data.length();//接收数据字节数统计
    c_receivenumber=c_receivenumber+testreadnumber;

    setreceivenumber(QString ::number(c_receivenumber));//更新接收的数据字节总数
}

void SerialTest::setreceivenumber(QString receivenumber)//更新接收的数据字节总数
{
    m_receivenumber=receivenumber;
    emit receivenumberChanged();;
}

QString SerialTest::receivenumber()//响应receivenumberChanged()消息
{
    return m_receivenumber;
}


QString SerialTest::receivedata()//qml读取receivedata值的时候就会触发此函数,或者emit receivedataChanged()更新当前收到的数据显示时触发
{
    return m_receivedata;
}
void SerialTest::setreceivedata(QString receivedata)//其任务已被receive from函数完成，但是在清空数据时用到这个函数
{
    m_receivedata=receivedata;
    emit receivedataChanged();
}



////////////////////5.关闭端口//////////////////////////////
void SerialTest::closePort()//由按钮出发
{
    serialtest.close();
    std::cout<<"close port sucess"<<std::endl;
}



////////////////////6.清空计数//////////////////////////////
void SerialTest::clearnumber()//由按钮出发
{
    c_sendnumber=0;
    c_receivenumber=0;
}
