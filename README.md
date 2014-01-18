QtTcp---Server
==============

QtTcp多线程Server
继承自QTcpServer，多线程处理Qtcpsocket,有两种方式分配线程，固定线程和固定线程处理的连接数。

代码基于Qt5和C++11，建议win下用mingw版Qt。

代码中的信号曹大多用Qt5的新语法，以便使用lambda表达式，

所以有很多lambda表达式的应用。

里面有大量的qDebug() <<“*”<<QThread::currentThreadId();语句，以便观察线程状态，

实际应用请注释掉。

http://www.dushibaiyu.com/2013/12/qtcpserver多线程实现.html

注：此为我学习所写，实际应用很多地方没考虑到、、分配的地方应该还有可以游湖阿德地方，如果您发现问题请告诉我，有改进最好也告诉我下、、