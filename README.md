QtTcp---Server
==============

### QtTcp多线程Server
> 继承自QTcpServer，多线程处理Qtcpsocket,有两种方式分配线程，固定线程和固定线程处理的连接数。
> 
> 代码基于Qt5和C++11，建议win下用mingw版Qt。
> 
> 代码中的信号槽用的用Qt5的新语法，以便使用lambda表达式，
> 
> 所以有很多lambda表达式的应用。
> 
> 
> 
> [我博客关于此实现的简单说明](http://www.dushibaiyu.com/2013/12/qtcpserver多线程实现.html)<br />

    注：此为我学习所写，实际应用很多地方没考虑到、、分配的地方应该还有可以优化地方，如果您发现问题请告诉我，有改进最好也告诉我下、、
    QLibeventTcpServer为用libevent的时间选择器、、速度好像的确有所增加，但是多线程的话就没有优势了、、
=================
每个线程有自己的事件循环。
现在线程分三级，主线程负责监听端口，新建链接，并移动到其他线程中。
socket线程，负责接收和发送数据，一些小处理也可以放在其中。（线程数目可控）
QtConcurrent ： 负责处理发送过来的数据。(动态的线程数目。)


===========================
Next：
    Qt的事件循环在win下是基于select的，在类unix下，如果有glib则是基于glib，否则也是基于select的，glib在linux下用的poll。
    这些均不是平台最优的I/O复用。
    下一步，使用其他事件循环替代Qt默认的时间循环。
    
==============
 QLibeventTcpServer移到了old分支
 还有原来的测试代码移动到了old分支