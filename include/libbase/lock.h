#ifndef _CLOCK_H_
#define _CLOCK_H_

#include "os.h"


class CLock
{
public:
    CLock();
    ~CLock();

public:
    int Lock();
    int Unlock();
   
private:
    pthread_mutex_t m_sMutex;
};

#endif
