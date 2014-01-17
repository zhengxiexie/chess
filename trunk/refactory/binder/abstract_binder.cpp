/*
 * abstract_binder.cpp
 *
 *  专门处理将多个查询结果按照某种方式黏合在一起的模块
 *
 *  Created on: 2013-4-27
 *      Author: guoweijiang
 */
#include "abstract_binder.h"

const char* TwoBindedRecord::ClassName = "TwoBindedRecord";

AbstractBinder::AbstractBinder()
{
    this->binded_result.clear();
}

AbstractBinder::~AbstractBinder()
{
    int size = this->binded_result.size();

    for(int i = 0; i < size; i++)
    {
        TwoBindedRecordFactory::recycle_object(this->binded_result[i]);
    }

    this->binded_result.clear();
}

int AbstractBinder::get_result_count()
{
    return this->binded_result.size();
}

TwoBindedRecord* AbstractBinder::get_binded_record(int index)
{
    int size = this->binded_result.size();

    if(index < 0 || index >= size || size == 0)
    {
        return NULL;
    }

    return this->binded_result[index];
}

void AbstractBinder::bind_two_modules(AbstractMCPContent* primary_module,
                                      AbstractMCPContent* secondary_module)
{
    LOGD("[GWJ] %s: start", __FUNCTION__);

    if(primary_module == NULL || secondary_module == NULL)
    {
        LOGD("[GWJ] %s: NULL Module Error!", __FUNCTION__);
        return;
    }

    int primary_key_size = primary_module->get_key_size();
    LOGD("[GWJ] %s: primary_key_size[%d]", __FUNCTION__, primary_key_size);

    for(int j = 0; j < primary_key_size; j++)
    {
        const char* pri_key = primary_module->get_key_value(j);
        const char* sec_key = secondary_module->get_key_value(pri_key);

        if(sec_key == NULL)
        {
            LOGD("[GWJ] %s: Primary Key[%s] has No Same Secondary Records",
                 __FUNCTION__, pri_key);

            /*
             *  没有附表可以黏合时，只将主表放入黏合结果集
             */
            TwoBindedRecord* record = TwoBindedRecordFactory::fetch_object(pri_key);
            record->set_primary_records(primary_module->get_key_bucket(j));
            this->binded_result.push_back(record);
        }
        else
        {
            LOGD("[GWJ] %s: Primary Key[%s] has Same Secondary Key[%s] Begin to Bind",
                 __FUNCTION__, pri_key, sec_key);

            TwoBindedRecord* record = TwoBindedRecordFactory::fetch_object(pri_key);

            KeyBucket* pri_vec = primary_module->get_key_bucket(j);
            record->set_primary_records(pri_vec);
            record->set_secondary_records(secondary_module->get_key_bucket(sec_key));
            this->binded_result.push_back(record);
        }
    }
}
