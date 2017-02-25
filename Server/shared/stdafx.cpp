#include "stdafx.h"

#if IsUnixDef //Start of #ifdef for Unix specific defines % includes

Guard::Guard(){
	m_target  = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t));
	pthread_mutex_init(m_target, NULL);}

Guard::Guard(pthread_mutex_t * target)
{
	target = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t));
	m_target = target; //It is critical to do the assignment after calling malloc.
	//TODO(onurcanbektas): implement a efficient error checking mechanism
	pthread_mutex_init(m_target, NULL);
	lock();
}

void Guard::lock(){
	m_lockReturnValue = pthread_mutex_lock(m_target);
	if (m_lockReturnValue == EDEADLK){
		printf("A deadlock has occured, a thread is blocked waiting for the m_target.\n");
	}else if(m_lockReturnValue == EINVAL){
		printf("The value specified by m_target in Thread class is invalid.\n");
	}
}

void Guard::unlock(){
	m_unlockReturnValue = pthread_mutex_unlock(m_target);
	if (m_unlockReturnValue == EINVAL)
	{
		printf("The value specified by m_target in Thread class is invalid.\n");
	}else if(m_unlockReturnValue == EPERM){
		printf("The current thread does not hold a lock on m_target.\n");
	}
}

Guard::~Guard(){
	pthread_mutex_unlock(m_target);
	free(m_target);
}

#endif //End of #ifdef for Unix specific defines & includes
