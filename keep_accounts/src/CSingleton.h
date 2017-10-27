/***************************************************************************
    Copyright (C) 2017 by ZhaoDongshuang
    Author: ZhaoDongshuang
    Email: imtoby@126.com
    Date: 2017/10/27
    File: CSingleton.h
 ***************************************************************************/
#ifndef CSINGLETON_H_38DD9CAF_15EF_5323_9AF1_6D9F312CA4D9
#define CSINGLETON_H_38DD9CAF_15EF_5323_9AF1_6D9F312CA4D9
#include <stdlib.h>

template <typename T>
class CSingleton
{
public:
    static T *instance() {
        if (0 == t) {
            t = new (T)();
            atexit(destory);
        }
        return t;
    }

private:
    static void destory() {
        delete t;
        t = 0;
    }

    CSingleton(){}
    ~CSingleton(){}

private:
    static T* volatile t;
};

template <typename T>
T* volatile CSingleton<T>::t = 0;

#endif // CSINGLETON_H_38DD9CAF_15EF_5323_9AF1_6D9F312CA4D9
