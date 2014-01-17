/*
 * mutex_shell.cpp
 *
 *  Created on: 2013-5-16
 *      Author: guoweijiang
 */
#include "mutex_shell.h"
#include "../common.h"

MutexShell::MutexShell(pthread_mutex_t* locker)
{
    this->child_locker = locker;
    pthread_mutex_lock(locker);
    LOGFC("[GWJ] %s: start. Lock", __FUNCTION__);
}

MutexShell::~MutexShell()
{
    pthread_mutex_unlock(this->child_locker);
    LOGFC("[GWJ] %s: end. Unlock", __FUNCTION__);
}
