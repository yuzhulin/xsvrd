#include "lock.h"

CLock::CLock()
{
    pthread_mutex_init(&m_sMutex, NULL);
}

CLock::~CLock()
{
     pthread_mutex_destroy(&m_sMutex); 
}

int CLock::Lock()
{
    return pthread_mutex_lock(&m_sMutex);
}

int CLock::Unlock()
{
    return pthread_mutex_unlock(&m_sMutex);
}

