/*
 * two_binded_factory.h
 *
 *  Created on: 2013-5-16
 *      Author: guoweijiang
 */

#ifndef TWO_BINDER_FACTORY_H_
#define TWO_BINDER_FACTORY_H_

#include "../binder/abstract_binder.h"

class TwoBindedRecord;

class TwoBindedRecordFactory
{
public:
    static TwoBindedRecord* fetch_object(const char* primary_key);
    static void recycle_object(TwoBindedRecord* obj);
};
#endif /* TWO_BINDER_FACTORY_H_ */
