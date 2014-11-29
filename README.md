TestSerial
==========
通过qmlc++混合编程基于QSerialPort的异步通信（记得在pro中加上QT+=serialport），主要步骤包括下面几个：传递一个该类的对象作为setPort()或者setPortName()方法的参数指定想要访问的串口设备。在设置完了串口，你就可以以只读或者只写或者读写模式调用open()方法打开串口。注意：串口都是以互斥的方式访问，这也就是说我们不能打开一个已经打开的串口。成功打开之后，QSerialPort尝试着获取串口当前的配置并初始化它。你也可以使用setBaudRate()，setDataBits()，setParity()，setStopBits()和setFlowControl()方法重新配置它，一旦你知道了串口可用于读或者写，你就可以调用read()或者write()方法，可选的还有readline()和readAll()方法。可以使用close()方法来关闭串口和取消I/O操作。

下边叙述本程序：
serial.h,serial.cpp为主函数，main.qml为主界面，Settings.qml为串口设置界面：

首先，打开并设置串口：
    由main.qml里的打开按钮打开Settings.qml设置界面，设置各个combobox之后，通过点击Apply按钮触发SerialTest::openAndSetPort函数（通过Q_INVOKABLE定义使得能够在qml里边访问），函数变量即为当前qml里各个combobox的currentIndex，由有各个combobox的model值和顺序与SerialTest::openAndSetPort函数中每个参数的可选值相同，所以可以由qml中各个combobox的currentIndex得到SerialTest::openAndSetPort函数中每个端口参数的值，然后由得到的设定值name，打开端口，设置其他端口参数。SerialTest::openAndSetPort函数执行完以后，设置Settings.qml对应的设置窗口不可见，回到主窗口。
第二，发送数据：在主窗口的senddata为名的textfield中输入要发送的内容，点击send按钮，触发SerialTest::sendto(QString sendmessage)函数（通过Q_INVOKABLE定义），其中变量来源于用户输入textfield的text内容，需要将其转为qbytearray来发送，注意：转换后加了'\r'，这是因为芯片要求在回车符（'\r'）之后再返回数据，比如对sim900芯片，发送0000，芯片就会收到0000，但是发送AT'\r'，会受到 AT'\r'OK。所以加上'\r'，然后进行写操作，发送qbytearray数据到串口，并对发送的自己数进行计数，计数由定义的 
Q_PROPERTY(QString sendnumber READ sendnumber WRITE setsendnumber NOTIFY sendnumberChanged)来完成， 
执行setsendnumber(QString ::number(c_sendnumber))，将当前计数的值进行设定，此函数更新m_sendnumber的值位当前计数，并emit sendnumberChanged()发送消息，使qml中text:"NumberofSendData:"+serialtest.sendnumber（56行）更新serialtest.sendnumber值，这时就回来通过读取QStringSerialTest::sendnumber()函数，而返回值m_sendnumber就是当前计数值，这时c++传值到qml的方法，如果要在qml向c++传值，只需在qml里执行SerialTest::setsendnumber(QStringsendnumber)函数，但是前提是在头文件里将此函数设置为Q_INVOKABLE函数或者在public slots:内定义函数，另一种qml向c++传值方法就是在定义一个函数，同样需要设置为Q_INVOKABLE函数或者在publicslots:内定义，然后在qml里使用，将qml的值由此函数送到c++即可，有时候还需要在qml里使用function先做一些处理。
第三，接收数据：对于串口来说，每发送一个字节的数据，就会返回收到这个数据，这时候使用QSerialPort就会产生一个信号：readyRead（）。将此信号与函数receivefrom()进行绑定： QObject::connect(&serialtest, SIGNAL(readyRead()),this, SLOT(receivefrom()))，就可以在发送完数据后得到readyRead（）信号时触发SerialTest::receivefrom()函数，读取数据，转换为QString来显示，这里有一个问题需要注意，就是readyRead（）信号可能多次产生，可能收到的数据还没有显示，新的数据又来了，将其覆盖，所以有个方法就是每次send之后收到的所有消息进行字符串累加，这样就可以避免这个问题（搞了好久才搞定的）。此函数后续的emit就不说了，和前面的类似，只不过前边的是显示计数，这里显示接受的数据，而且这里把set函数及Q_PROPERTY的WRITE函数的功能放到receivefrom函数来实现了（主要就是更新m_receivedata值和emit receivedataChanged()消息两个功能）。下边receivenumber和签署sendnumber一样。
最后关闭窗口和清空计数，分别有两个按钮来响应，需要将两个函数设置成Q_INVOKABLE使得qml能够调用。

    

