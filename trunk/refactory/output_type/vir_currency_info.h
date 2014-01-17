/*
 * currency_info.h
 *
 *  Created on: 2013-6-20
 *      Author: guoweijiang
 */

#ifndef CURRENCY_INFO_H_
#define CURRENCY_INFO_H_

#include "common_header.h"

class vir_currency_info_t
{
public:
    static void set_outparameter(idl::game_dev_vir_currency_info* out,
                                 MysqlTable& records);
};


#endif /* CURRENCY_INFO_H_ */
