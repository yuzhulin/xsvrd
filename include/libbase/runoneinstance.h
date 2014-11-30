#ifndef __RUNONEINSTANCE_H__
#define __RUNONEINSTANCE_H__

class CRunOneInstance
{
public:
    CRunOneInstance(const char* pszLockFile)
        :m_bAlreadyRun(false)
    {
#ifdef WIN32
        m_hMutex = CreateMutex(NULL, false, pszLockFile);
        if( GetLastError() == ERROR_ALREADY_EXISTS )
        {
            m_bAlreadyRun = true;
        }
#else
        lock_fd = open(pszLockFile, O_RDWR|O_CREAT, 0640);
        if(lock_fd > 0 )
        {
	        int ret = flock(lock_fd, LOCK_EX | LOCK_NB);
			if(ret < 0 )
			{
				printf("lock file failed, server is already running!\n");
				m_bAlreadyRun =  true;
			}
		}
#endif
    }

    ~CRunOneInstance()
    {
#ifdef _WIN32
		if( m_hMutex )
		{
			CloseHandle(m_hMutex);
			m_hMutex = NULL;
		}
#else
		if( lock_fd > 0 )
		{
			close(lock_fd);
		}
#endif
    }
    
public:
    bool RunOneInstance()
    {
        return !m_bAlreadyRun;
    }

private:
    
#ifdef _WIN32
    HANDLE m_hMutex;
#else
    int lock_fd;
#endif

    bool   m_bAlreadyRun;
};


#endif

