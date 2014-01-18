#include "threadhandle.h"

ThreadHandle::ThreadHandle()
{
    tlist.clear();
    ilist.clear();
}

ThreadHandle::~ThreadHandle() //停止所有线程，并释放资源
{
    QThread * tmp;
    for (int i=0; i<tlist.size();++i)
    {
        tmp = tlist.at(i);
        tmp->exit();
        tmp->wait();
        delete tmp;
    }
}

ThreadHandle & ThreadHandle::getClass()
{
    static ThreadHandle th;
    return th;
}

QThread * ThreadHandle::getThread()
{
    if (type == THREADSIZE)
        return findThreadSize();
    else
        return findHandleSize();
}

void ThreadHandle::removeThread(QThread * thread)
{
    int t = tlist.indexOf(thread);
    ilist[t] --;
    if (type == HANDLESIZE) //如果是连接最大值，线程内连接数等于0，且多个线程，就把这个线程处理掉
    {
        if (ilist.at(t) == 0 && ilist.size() > 1)
        {
            ilist.removeAt(t);
            QThread * tmp = tlist.at(t);
            tlist.removeAt(t);
            tmp->exit();
            tmp->wait();
            delete tmp;
        }
    }
}

void ThreadHandle::initThreadType(ThreadType type, unsigned int max)
{
    this->type = type;
    this->size = max;
    if (this->size == 0)
        this->size ++;
    if (type == THREADSIZE)
        initThreadSize();
    else
    {
        QThread * tmp = new QThread;
        tmp->start();
        tlist.append(tmp);
        ilist.append(0);
    }

}

void ThreadHandle::initThreadSize() //建立好线程并启动，
{
    QThread * tmp;
    for (unsigned int i = 0; i < size;++i)
    {
        tmp = new QThread;
        tmp->start();
        tlist.append(tmp);
        ilist.append(0);
    }
}

QThread * ThreadHandle::findHandleSize() //查找到线程里的连接数小于最大值就返回查找到的，找不到就新建一个线程
{
    for (int i = 0;i<ilist.size();++i)
    {
        if (ilist.at(i) < size)
        {
            ilist[i] ++;
            return tlist.at(i);
        }
    }
    QThread * tmp = new QThread;
    tmp->start();
    tlist.append(tmp);
    ilist.append(1);
    return tmp;
}

QThread * ThreadHandle::findThreadSize() //遍历查找所有线程中连接数最小的那个，返回
{
    unsigned int j = 0,t = ilist.at(0);//j记录位置,t几乎上次的值，便于对比大小
    for (int i = 1; i < ilist.size(); ++i)
    {
        if (t > ilist.at(i))
        {
            j = i;
            t = ilist.at(i);
        }
    }
    ilist[j] ++;
    return tlist.at(j);
}

void ThreadHandle::clear()//仅仅清空计数，线程不释放
{
    for (int i = 1; i < ilist.size(); ++i)
    {
       ilist[i] = 0;
    }
}
