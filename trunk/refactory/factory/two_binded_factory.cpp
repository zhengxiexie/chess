/*
 * two_binded_factory.cpp
 *
 *  Created on: 2013-5-16
 *      Author: guoweijiang
 */

#include "two_binded_factory.h"

TwoBindedRecord* TwoBindedRecordFactory::fetch_object(const char* primary_key)
{
    LOGD("[GWJ] %s: start. primary_key[%s]", __FUNCTION__, primary_key);

    TwoBindedRecord* obj = ObjectPoolFactory<TwoBindedRecord>::instance().fetch_object();
    obj->key_value = primary_key;

    return obj;
}

void TwoBindedRecordFactory::recycle_object(TwoBindedRecord* obj)
{
    ObjectPoolFactory<TwoBindedRecord>::instance().recycle_object(obj);
}
