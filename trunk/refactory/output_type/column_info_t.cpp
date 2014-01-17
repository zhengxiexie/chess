/*
 * column_info_t.cpp
 *
 *  Created on: 2013-6-6
 *      Author: guoweijiang
 */

#include "column_info_t.h"

void diy_column_info_t::set_outparameter(idl::column_info_t* out,
                                         MCPDIYColumnRecord* diy_record)
{
    LOGD("[GWJ] %s:  start", __FUNCTION__);

    if(diy_record != NULL)
    {
        out->set_id(diy_record->id.c_str());
        out->set_name(diy_record->name.c_str());
        out->set_type_id(diy_record->type_id.c_str());
    }
    else
    {
        LOGD("[GWJ] %s: record NULL Empty!", __FUNCTION__);
    }
}
