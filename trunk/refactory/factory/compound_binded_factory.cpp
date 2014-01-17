/*
 * compound_binded_factory.cpp
 *
 *  Created on: 2013-6-5
 *      Author: guoweijiang
 */

#include "compound_binded_factory.h"

CompoundBindedRecorder* CompoundBindedRecordFactory::fetch_object(const char* primary_key)
{
    LOGD("[GWJ] %s: start. primary_key[%s]", __FUNCTION__, primary_key);

    CompoundBindedRecorder* obj = ObjectPoolFactory<CompoundBindedRecorder>::instance().fetch_object();
    obj->key_value = primary_key;

    return obj;
}

void CompoundBindedRecordFactory::recycle_object(CompoundBindedRecorder* obj)
{
    ObjectPoolFactory<CompoundBindedRecorder>::instance().recycle_object(obj);
}
