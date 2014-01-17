/*
 * compound_binded_factory.h
 *
 *  Created on: 2013-6-5
 *      Author: guoweijiang
 */

#ifndef COMPOUND_BINDED_FACTORY_H_
#define COMPOUND_BINDED_FACTORY_H_

#include "../binder/compound_binder.h"

class CompoundBindedRecorder;

class CompoundBindedRecordFactory
{
public:
    static CompoundBindedRecorder* fetch_object(const char* primary_key);
    static void recycle_object(CompoundBindedRecorder* obj);
};

#endif /* COMPOUND_BINDED_FACTORY_H_ */
