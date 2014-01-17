/*
 * mutex_shell.h
 *
 *  Created on: 2013-5-16
 *      Author: guoweijiang
 */

#ifndef MUTEX_SHELL_H_
#define MUTEX_SHELL_H_

#include <pthread.h>

/*
 *  简单多线程同步锁(壳)
 */
class MutexShell
{
private:
    pthread_mutex_t* child_locker;

public:
    MutexShell(pthread_mutex_t* locker);
    ~MutexShell();
};

#endif /* MUTEX_SHELL_H_ */
