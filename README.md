QtTcp---Server
==============

### QtTcp多线程Server
> 继承自QTcpServer，多线程处理Qtcpsocket,有两种方式分配线程，固定线程和固定线程处理的连接数。
> 
> 代码基于Qt5和C++11，建议win下用mingw版Qt。
> 
> 代码中的信号曹大多用Qt5的新语法，以便使用lambda表达式，
> 
> 所以有很多lambda表达式的应用。
> 
> 里面有大量的qDebug() <<“*”<<QThread::currentThreadId();语句，以便观察线程状态，
> 
> 实际应用请注释掉。
> 
> [我博客关于此实现的简单说明](http://www.dushibaiyu.com/2013/12/qtcpserver多线程实现.html)<br />

    注：此为我学习所写，实际应用很多地方没考虑到、、分配的地方应该还有可以游湖阿德地方，如果您发现问题请告诉我，有改进最好也告诉我下、、
    TsstServer : 测试此多线程server的性能、、、客户端还是那个客户端，加上了定时器，一遍自动发送消息。
    经个人粗略验证，在Qt5下，windows上，多线程的并不能带来性能上多大提升，倒是资源占用有所曾加、、
    QLibeventTcpServer为用libevent的时间选择器、、速度好像的确有所增加，但是多线程的话就没有优势了、、
