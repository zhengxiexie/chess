/*
 * object_pool_factory.h
 *
 *  Created on: 2013-5-16
 *      Author: guoweijiang
 */

#ifndef OBJECT_POOL_FACTORY_H_
#define OBJECT_POOL_FACTORY_H_

#include <list>

#include "../common.h"
#include "mutex_shell.h"

using namespace std;

/*
 *  【注意】
 *  从对象池获取的对象需要实现  clear() 接口
 */
template<typename T>
class ObjectPoolFactory
{
    // 工厂皆单体
    NSP_SINGLETONIZE(ObjectPoolFactory<T>)

    // 单体多线程同步
    pthread_mutex_t self_locker;

    list<T*> using_list;
    list<T*> free_list;

public:
    virtual ~ObjectPoolFactory(void);

    T* fetch_object();
    void recycle_object(T* obj);
    void recycle_all();
};

template<typename T>
ObjectPoolFactory<T>::ObjectPoolFactory()
{
    pthread_mutex_init(&this->self_locker, NULL);
    this->using_list.clear();
    this->free_list.clear();
}

template<typename T>
ObjectPoolFactory<T>::~ObjectPoolFactory()
{
    LOGFC("[GWJ] %s: start", __FUNCTION__);
    this->recycle_all();
    pthread_mutex_destroy(&this->self_locker);
}

template<typename T>
T* ObjectPoolFactory<T>::fetch_object()
{
    MutexShell mutex(&this->self_locker);

    int f_size = this->free_list.size();
#if MEM_HEAP_TEST
    int u_size = this->using_list.size();
#endif

    LOGFC("[GWJ] %s[%s]: start. free size[%d], using size[%d]",
            __FUNCTION__, T::ClassName, f_size, u_size);

    if(f_size == 0)
    {
        LOGFC("[GWJ] %s[%s]: No free Object obtain. New!",
              __FUNCTION__, T::ClassName);

        T* new_obj = new T;
        this->using_list.push_back(new_obj);

#if MEM_HEAP_TEST
        u_size = this->using_list.size();
        LOGFC("[GWJ] %s[%s]: start. using size[%d]", __FUNCTION__, T::ClassName, u_size);
#endif

        return new_obj;
    }
    else
    {
        LOGFC("[GWJ] %s[%s]: Can get Free Object", __FUNCTION__, T::ClassName);

        typename list<T*>::iterator it = this->free_list.begin();
        T* obj = (*it);
        this->using_list.push_back(obj);
        this->free_list.erase(it);

#if MEM_HEAP_TEST
        f_size = this->free_list.size();
        u_size = this->using_list.size();
#endif

        LOGFC("[GWJ] %s[%s]: Use Free Obj. free_size[%d], using_size[%d]",
                __FUNCTION__, T::ClassName, f_size, u_size);

        return obj;
    }
}

template<typename T>
void ObjectPoolFactory<T>::recycle_object(T* obj)
{
    MutexShell mutex(&this->self_locker);

#if MEM_HEAP_TEST
    int f_size = this->free_list.size();
    int u_size = this->using_list.size();
#endif

    LOGFC("[GWJ] %s[%s]: start. free size[%d], using size[%d]",
            __FUNCTION__, T::ClassName, f_size, u_size);

    typename list<T*>::iterator it = this->using_list.begin();
    typename list<T*>::iterator end = this->using_list.end();

    for(; it != end; it++)
    {
        if(obj == (*it))
        {
            LOGFC("[GWJ] %s[%s]: Is using object", __FUNCTION__, T::ClassName);

            obj->clear();
            this->using_list.erase(it);

#if MEM_HEAP_TEST
            f_size = this->free_list.size();
            u_size = this->using_list.size();
#else
            int f_size = this->free_list.size();
#endif

            if(f_size > OBJECT_POOL_SIZE)
            {
                delete obj;

#if MEM_HEAP_TEST
                f_size = this->free_list.size();
                u_size = this->using_list.size();
#endif
                LOGFC("[GWJ] %s[%s]: Free_size[%d] Using_size[%d]. Up Bound and Delete Object",
                      __FUNCTION__, T::ClassName, f_size, u_size);
            }
            else
            {
                this->free_list.push_back(obj);

#if MEM_HEAP_TEST
                f_size = this->free_list.size();
                u_size = this->using_list.size();
#endif
                LOGFC("[GWJ] %s[%s]: Free_size[%d] Using_size[%d]. In Bound and Recycle Object",
                      __FUNCTION__, T::ClassName, f_size, u_size);
            }

            return;
        }
    }

    LOGFC("[GWJ] %s[%s]: Not In using object", __FUNCTION__, T::ClassName);
    return;
}

template<typename T>
void ObjectPoolFactory<T>::recycle_all()
{
    MutexShell mutex(&this->self_locker);

#if MEM_HEAP_TEST
    int f_size = this->free_list.size();
    int u_size = this->using_list.size();
#endif

    LOGFC("[GWJ] %s[%s]: start. free size[%d], using size[%d]",
            __FUNCTION__, T::ClassName, f_size, u_size);

    typename list<T*>::iterator it = this->using_list.begin();
    typename list<T*>::iterator end = this->using_list.end();

    for(; it != end; it++)
    {
        delete (*it);
    }

    it = this->free_list.begin();
    end = this->free_list.end();

    for(; it != end; it++)
    {
        delete (*it);
    }

    this->free_list.clear();
    this->using_list.clear();
}

#endif /* OBJECT_POOL_FACTORY_H_ */
