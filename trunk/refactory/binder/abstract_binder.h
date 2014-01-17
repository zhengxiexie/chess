/*
 * abstract_binder.h
 *
 *  专门处理将查询结果按照某种方式黏合在一起
 *
 *  Created on: 2013-4-27
 *      Author: guoweijiang
 */

#ifndef ABSTRACT_BINDER_H_
#define ABSTRACT_BINDER_H_

#include "../common.h"
#include "../module/mcp_game_content.h"
#include "../factory/two_binded_factory.h"

/*
 *  要通过对象池管理
 *  所以要实现 ClassName 和 clear 方法
 */
class TwoBindedRecord
{

// 向对象池工厂开放
friend class TwoBindedRecordFactory;

private:
    const char* key_value;
    KeyBucket* primary_records;
    KeyBucket* secondary_records;

public:

    static const char* ClassName;

    TwoBindedRecord(const char* key)
    {
        this->key_value = key;
        this->primary_records = NULL;
        this->secondary_records = NULL;
    }

    TwoBindedRecord()
    {
        this->key_value = NULL;
        this->primary_records = NULL;
        this->secondary_records = NULL;
    }

    ~TwoBindedRecord()
    {
        this->clear();
    }

    void clear()
    {
        /*
         *   由于 primary_records / secondary_records
         *   也是对象池中分配出来的，所以此处不用做任何动作
         *   等待管理这两项的对象池自己清理即可
         */
        this->key_value = NULL;
        this->primary_records = NULL;
        this->secondary_records = NULL;
    }

    void set_primary_records(KeyBucket* records)
    {
        this->primary_records = records;
    }

    void set_secondary_records(KeyBucket* records)
    {
        this->secondary_records = records;
    }

    AbstractMCPRecord* get_primary_record(int vec_index)
    {
        if(this->primary_records == NULL)
        {
            return NULL;
        }

        int size = this->primary_records->size();

        if(size == 0 || vec_index >= size || vec_index < 0)
        {
            return NULL;
        }
        else
        {
            return (*(this->primary_records))[vec_index];
        }
    }

    AbstractMCPRecord* get_secondary_record(int vec_index)
    {
        if(this->secondary_records == NULL)
        {
            return NULL;
        }

        int size = this->secondary_records->size();

        if(size == 0 || vec_index >= size || vec_index < 0)
        {
            return NULL;
        }
        else
        {
            return (*(this->secondary_records))[vec_index];
        }
    }

    int get_primary_record_size()
    {
        if(this->primary_records == NULL)
        {
            return 0;
        }

        return this->primary_records->size();
    }

    int get_secondary_record_size()
    {
        if(this->secondary_records == NULL)
        {
            return 0;
        }

        return this->secondary_records->size();
    }
};

class AbstractBinder
{
protected:
    vector<TwoBindedRecord*> binded_result;

    void add_single(AbstractMCPContent* primary_module);

    void add_pare(AbstractMCPContent* primary_module,
                  AbstractMCPContent* secondary_module);

public:

    AbstractBinder();
    virtual ~AbstractBinder();

    int get_result_count();

    TwoBindedRecord* get_binded_record(int index);

    /*
     *  一般抽象接口：
     *  抽象类中只实现一种通用做法，将两个结果集进行黏合
     *  如果有特殊业务需求可以继承子类，复写此接口
     */
    virtual void bind_two_modules(AbstractMCPContent* primary_module,
                                  AbstractMCPContent* secondary_module);
};

#endif /* ABSTRACT_BINDER_H_ */
