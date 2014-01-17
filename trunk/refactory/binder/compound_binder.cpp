/*
 * compound_binder.cpp
 *
 *  Created on: 2013-5-28
 *      Author: guoweijiang
 */

#include "compound_binder.h"

const char* CompoundBindedRecorder::ClassName = "CompoundBindedRecorder";

bool CompoundBindedRecorder::is_right_module_index(int module_index, int* out_mod_index)
{
    int size = this->module_records.size();
    int all_sz = this->modules_info.size();

    LOGD("[GWJ] %s: start. mod_index[%d], total_mod[%d], all_sz[%d]",
            __FUNCTION__, module_index, size, all_sz);

    if(module_index < 0 || module_index >= all_sz)
    {
        LOGD("[GWJ] %s: mod_index[%d], all_sz[%d] Out of Bound Error!",
                __FUNCTION__, module_index, all_sz);

        return false;
    }

    if(this->modules_info[module_index] == 0)
    {
        LOGD("[GWJ] %s: mod_index[%d] Is-Empty-Module!",
                        __FUNCTION__, module_index);
        return false;
    }

    int modify = 0;
    for(int i = 0; i < module_index; i++)
    {
        if(this->modules_info[i] == 0)
        {
            modify++;
            LOGD("[GWJ] %s: i[%d] Is-Empty-Module. modify[%d]",
                 __FUNCTION__, i, modify);
        }
    }

    (*out_mod_index) = module_index - modify;

    LOGD("[GWJ] %s: mod_index[%d]. real_index[%d]. modify[%d] Not-Empty-Module!",
          __FUNCTION__, module_index, (*out_mod_index), modify);

    return true;
}

/*
 *  获取黏合器中的数据：
 *
 *  module_index : 黏合剂中第几个 module
 *  vec_index    : 指定 module 中 key_bucket 的索引
 */
AbstractMCPRecord* CompoundBindedRecorder::get_record(int module_index,
                                                      int vec_index)
{
    int real_module_index = 0;
    if(!this->is_right_module_index(module_index,
                                    &real_module_index))
    {
        LOGD("[GWJ] %s: module_index[%d]. Is Empty Moduel",
             __FUNCTION__, module_index);

        return NULL;
    }

    KeyBucket* mod_key_bucket = this->module_records[real_module_index];

    if(mod_key_bucket == NULL)
    {
        LOGD("[GWJ] %s: KeyBucket NULL Error! Check Init Add-KeyBucket",
             __FUNCTION__);

        return NULL;
    }

    int size = mod_key_bucket->size();

    if(vec_index < 0 || vec_index >= size)
    {
        LOGD("[GWJ] %s: start. vec_index[%d], total_keyBuc[%d] out of bound Error!",
                 __FUNCTION__, vec_index, size);

        return NULL;
    }

    LOGD("[GWJ] %s: OK end", __FUNCTION__);
    return (*mod_key_bucket)[vec_index];
}

/*
 *  获取黏合在一起的某一个 module 中 KeyBucket 的大小
 */
int CompoundBindedRecorder::get_key_bucket_size(int module_index)
{
    int real_module_index = 0;
    if(!this->is_right_module_index(module_index,
                                    &real_module_index))
    {
        LOGD("[GWJ] %s: module_index[%d]. Is Empty Moduel",
             __FUNCTION__, module_index);

        return 0;
    }

    KeyBucket* mod_key_bucket = this->module_records[real_module_index];

    if(mod_key_bucket == NULL)
    {
        LOGD("[GWJ] %s: KeyBucket NULL Error! Check Init Add-KeyBucket",
             __FUNCTION__);

        return 0;
    }
    else
    {
        return mod_key_bucket->size();
    }
}

/*
 *  从不同 module 中增加具有相同 key_value 的 key_bucket
 */
int CompoundBindedRecorder::add_key_bucket(KeyBucket* records)
{
    if(records == NULL || records->size() == 0)
    {
        LOGD("[GWJ] %s: Empty keyBucket[%x]",
              __FUNCTION__, records);

        this->modules_info.push_back(0);
    }
    else
    {
        LOGD("[GWJ] %s: Add keyBucket[%x]",
                __FUNCTION__, records);

        this->modules_info.push_back(1);
        this->module_records.push_back(records);
    }

    return 1;
}

CompoundBinder::CompoundBinder()
{
    this->binded_result.clear();
}

CompoundBinder::~CompoundBinder()
{
    int size = this->binded_result.size();

    for(int i = 0; i < size; i++)
    {
        CompoundBindedRecordFactory::recycle_object(this->binded_result[i]);
    }

    this->binded_result.clear();
}

int CompoundBinder::get_result_count()
{
    return this->binded_result.size();
}

/*
 *  获取黏合器中某一个 module 中 KeyBucket 的大小
 */
int CompoundBinder::get_bucket_size(int k_index,
                                    int module_index)
{
    int size = this->binded_result.size();

    LOGD("[GWJ] %s: start. k_index[%d], totoa_mod[%d], module_index[%d]",
         __FUNCTION__, k_index, size, module_index);

    if(k_index < 0 || k_index >= size)
    {
        LOGD("[GWJ] %s: start. k_index[%d], total_mod[%d] out of bound Error!",
             __FUNCTION__, k_index, size);

        return -1;
    }

    CompoundBindedRecorder* recorder = this->binded_result[k_index];
    return recorder->get_key_bucket_size(module_index);
}

/*
 *  获取黏合器中某一个 module 中 KeyBucket 里指定的一条记录
 */
AbstractMCPRecord* CompoundBinder::get_record_value(int k_index,
                                                    int module_index,
                                                    int vec_index)
{
    int size = this->binded_result.size();

    LOGD("[GWJ] %s: start. k_index[%d], totoa_mod[%d], module_index[%d], vec_index[%d]",
         __FUNCTION__, k_index, size, module_index, vec_index);

    if(k_index < 0 || k_index >= size)
    {
        LOGD("[GWJ] %s: start. k_index[%d], total_mod[%d] out of bound Error!",
             __FUNCTION__, k_index, size);

        return NULL;
    }

    CompoundBindedRecorder* recorder = this->binded_result[k_index];
    return recorder->get_record(module_index, vec_index);
}

/*
 *  增加黏合器中的 module
 */
void CompoundBinder::left_join(AbstractMCPContent* joint_module)
{
    LOGD("[GWJ] %s: start", __FUNCTION__);

    if(joint_module == NULL)
    {
        LOGD("[GWJ] %s: NULL Module Error!", __FUNCTION__);
        return;
    }

    int primary_key_size = this->binded_result.size();
    LOGD("[GWJ] %s: primary_key_size[%d]", __FUNCTION__, primary_key_size);

    if(primary_key_size == 0)
    {
        int joint_key_size = joint_module->get_key_size();

        LOGD("[GWJ] %s: First Joint Module To Be Binded. Joint Key Size[%d]",
             __FUNCTION__, joint_key_size);

        for(int i = 0; i < joint_key_size; i++)
        {
            const char* pri_key = joint_module->get_key_value(i);

            LOGD("[GWJ] %s: First Joint Module Key[%s] Record",
                 __FUNCTION__, pri_key);

            CompoundBindedRecorder* record = CompoundBindedRecordFactory::fetch_object(pri_key);
            record->add_key_bucket(joint_module->get_key_bucket(i));
            this->binded_result.push_back(record);
        }
    }
    else
    {
        int joint_key_size = joint_module->get_key_size();

        LOGD("[GWJ] %s: Exist Joint Module Size[%d]! New Joint Key Size[%d]",
             __FUNCTION__, primary_key_size, joint_key_size);

        for(int i = 0; i < primary_key_size; i++)
        {
            const char* pri_key = this->binded_result[i]->get_key_value();
            const char* jnt_key = joint_module->get_key_value(pri_key);

            if(IS_VALUED_STRING(jnt_key))
            {
                LOGD("[GWJ] %s: Primary Key[%s] has Same Joint Key[%s] at Index[%d] Begin to Bind",
                        __FUNCTION__, pri_key, jnt_key, i);

                KeyBucket* joint_kb = joint_module->get_key_bucket(jnt_key);
                this->binded_result[i]->add_key_bucket(joint_kb);
            }
            else
            {
                LOGD("[GWJ] %s: Primary Key[%s] No Same KEY in Joint Module. Index[%d]. Use NULL Empty!",
                     __FUNCTION__, pri_key, i);

                this->binded_result[i]->add_key_bucket(NULL);
            }
        }
    }
}

const char* CompoundBindedRecorder::get_key_value()
{
    return this->key_value;
}
