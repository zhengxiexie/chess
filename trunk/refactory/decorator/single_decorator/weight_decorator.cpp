/*
 * weight_class_decorator.cpp
 *
 *  专门处理按照指定权重排序的装饰器
 *
 *  Created on: 2013-4-26
 *      Author: guoweijiang
 */

#include "weight_decorator.h"

using namespace std;

WeightDecorator::WeightDecorator():AbstractSingleDecorator()
{
    LOGD("[GWJ] %s:  start", __FUNCTION__);
    this->defalt_orderby = NULL;
    this->is_special = true;
    LOGD("[GWJ] %s:  end", __FUNCTION__);
}

WeightDecorator::~WeightDecorator()
{
}

/*
 *  针对某一个特定的 extra_id 的 priority 属性进行排序
 */
WeightType WeightDecorator::get_weight_type(OrderTypeStruct& order_struct)
{
    LOGD("[GWJ] %s:  start. order_type[%s]", __FUNCTION__, order_struct.order_type);

    if(!IS_VALUED_STRING(order_struct.order_type))
    {
        return NO_WEIGH_TYPE;
    }
    else if(IS_SAME_VALUED_STRING(order_struct.order_type, "0"))
    {
        return CLASS_WEIGHT_TYPE;
    }
    else if(IS_SAME_VALUED_STRING(order_struct.order_type, "1"))
    {
        return LATEST_WEIGHT_TYPE;
    }
    else if(IS_SAME_VALUED_STRING(order_struct.order_type, "2"))
    {
        return DOWNLOAD_WEIGHT_TYPE;
    }
    else if(IS_SAME_VALUED_STRING(order_struct.order_type, "3"))
    {
        return POP_WEIGHT_TYPE;
    }
    else if(IS_SAME_VALUED_STRING(order_struct.order_type, "4"))
    {
        return NO_WEIGH_TYPE;
    }
    else if(IS_SAME_VALUED_STRING(order_struct.order_type, "5"))
    {
        return CLIENT_CLASS_WEIGHT_TYPE;
    }
    else if(IS_SAME_VALUED_STRING(order_struct.order_type, "6"))
    {
        return CLIENT_LATEST_WEIGHT_TYPE;
    }
    else if(IS_SAME_VALUED_STRING(order_struct.order_type, "7"))
    {
        return CLIENT_DOWNLOAD_WEIGHT_TYPE;
    }
    else if(IS_SAME_VALUED_STRING(order_struct.order_type, "8"))
    {
        return CLIENT_POP_WEIGHT_TYPE;
    }
    else if(IS_SAME_VALUED_STRING(order_struct.order_type, "9"))
    {
        return CLIENT_WEEKLY_DOWNLOAD_TYPE;
    }
    else if(IS_SAME_VALUED_STRING(order_struct.order_type, "10"))
    {
        this->extra_name = "column";
        this->extra_id = order_struct.extra_tuple["column"];
        this->outer_decorator = order_struct.outer_decorator;
        this->is_aggregate = true;

        LOGD("[GWJ] %s: extra[%s:%s]. Aggregate",
             __FUNCTION__, this->extra_name, this->extra_id);

        return COLUMN_PRIORIY;
    }
    else if(IS_SAME_VALUED_STRING(order_struct.order_type, "11"))
    {
        this->extra_name = "class";
        this->extra_id = order_struct.extra_tuple["class"];
        this->outer_decorator = order_struct.outer_decorator;
        this->is_aggregate = true;

        LOGD("[GWJ] %s: extra[%s:%s]. Aggregate",
                     __FUNCTION__, this->extra_name, this->extra_id);

        return CLASS_PRIORIY;
    }
	//add by zhengxie
	else if(IS_SAME_VALUED_STRING(order_struct.order_type, "12"))
    {
        return DOWNLOAD_DAY_WEIGHT_TYPE;
    }

    else
    {
        return NO_WEIGH_TYPE;
    }
}

void WeightDecorator::order_by_extra_id(bson* command)
{
    LOGD("[GWJ] %s:  start. weight_type[%d], id[%s], key[%s]",
         __FUNCTION__, this->weight_type, this->extra_name, this->extra_id);

    string extra_id_name = string(this->extra_name) + ".id";
    string extra_unwind_name = string("$") + this->extra_name;
    string extra_p_name = string(this->extra_name) + ".p";

    bson_append_string(command, "aggregate", "mcp_content");

    bson_append_start_array(command, "pipeline");

    bson_append_start_object(command, "0");
    bson_append_start_object(command, "$match");
    this->outer_decorator->custom_result_set_query(command);
    bson_append_finish_object(command);
    bson_append_finish_object(command);

    bson_append_start_object(command, "1");
    bson_append_string(command, "$unwind", extra_unwind_name.c_str());
    bson_append_finish_object(command);

    bson_append_start_object(command, "2");
    bson_append_start_object(command, "$match");
    bson_append_string(command, extra_id_name.c_str(), this->extra_id);
    bson_append_finish_object(command);
    bson_append_finish_object(command);

    bson_append_start_object(command, "3");
    bson_append_start_object(command, "$sort");
    bson_append_int(command, extra_p_name.c_str(), -1);
    bson_append_finish_object(command);
    bson_append_finish_object(command);

    bson_append_start_object(command, "4");
    bson_append_int(command, "$skip", this->start_pos);
    bson_append_finish_object(command);

    bson_append_start_object(command, "5");
    bson_append_int(command, "$limit", this->page_size);
    bson_append_finish_object(command);

    // finish "pipeline"
    bson_append_finish_object(command);
    return;
}

/*
 *  实现一般抽象接口：
 */
int WeightDecorator::get_count_query(bson* query)
{
    LOGD("[GWJ] %s:  start. weight_type[%d]", __FUNCTION__, this->weight_type);

    if(this->outer_decorator != NULL)
    {
        LOGD("[GWJ] %s: Use Outer Decorate", __FUNCTION__);
        return this->outer_decorator->get_count_query(query);
    }
    else
    {
        LOGD("[GWJ] %s: No Outer Decorate ERROR!!", __FUNCTION__);
        return -1;
    }
}

/*
 *  实现抽象接口：
 */
int WeightDecorator::custom_result_set_query(bson* query)
{
    LOGD("[GWJ] %s:  start. weight_type[%d]", __FUNCTION__, this->weight_type);

    if(NO_WEIGH_TYPE == this->weight_type)
    {
        // 没有权重时，且没设置默认排序时，按照 priority 降序排列
        bson_append_start_object(query, "$orderby");

        if(this->defalt_orderby != NULL )
        {
            bson_append_int(query, this->defalt_orderby, -1);
        }
        else
        {
            LOGD("[GWJ] %s: default orderby `priority`", __FUNCTION__);
            bson_append_int(query, "priority", -1);
        }

        bson_append_finish_object(query);
    }
    else
    {
        if(this->weight_type == COLUMN_PRIORIY ||
           this->weight_type == CLASS_PRIORIY  )
        {
            this->order_by_extra_id(query);
            return 0;
        }
        else
        {
            bson_append_start_object(query, "$orderby");

            switch(this->weight_type)
            {
                case CLASS_WEIGHT_TYPE:
                    bson_append_int(query, "class_weight", -1);
                    bson_append_int(query, "modify_date", -1);
                    LOGD("[GWJ] %s: class-w order", __FUNCTION__);
                    break;

                case LATEST_WEIGHT_TYPE:
                    bson_append_int(query, "latest_weight", -1);
                    bson_append_int(query, "modify_date", -1);
                    LOGD("[GWJ] %s: latest-w order", __FUNCTION__);
                    break;

                case DOWNLOAD_WEIGHT_TYPE:
                    bson_append_int(query, "ptype_5", -1);
                    bson_append_int(query, "download_weight", -1);
                    LOGD("[GWJ] %s: download-w order", __FUNCTION__);
                    break;

				//add by zhengxie
				case DOWNLOAD_DAY_WEIGHT_TYPE:
                    bson_append_int(query, "ptype_1", -1);
                    bson_append_int(query, "download_weight", -1);
                    LOGD("[GWJ] %s: download-w-by-day order", __FUNCTION__);
                    break;

                case POP_WEIGHT_TYPE:
                    bson_append_int(query, "pop_weight", -1);
                    bson_append_int(query, "star", -1);
                    LOGD("[GWJ] %s: pop-w order", __FUNCTION__);
                    break;

                case CLIENT_CLASS_WEIGHT_TYPE:
                    bson_append_int(query, "client_class_weight", -1);
                    bson_append_int(query, "modify_date", -1);
                    LOGD("[GWJ] %s: client-class-w order", __FUNCTION__);
                    break;

                case CLIENT_LATEST_WEIGHT_TYPE:
                    bson_append_int(query, "client_latest_weight", -1);
                    bson_append_int(query, "modify_date", -1);
                    LOGD("[GWJ] %s: client-latest-w order", __FUNCTION__);
                    break;

                case CLIENT_DOWNLOAD_WEIGHT_TYPE:
                    bson_append_int(query, "ptype_5", -1);
                    bson_append_int(query, "client_download_weight", -1);
                    LOGD("[GWJ] %s: client-download-w order", __FUNCTION__);
                    break;

                case CLIENT_POP_WEIGHT_TYPE:
                    bson_append_int(query, "client_pop_weight", -1);
                    bson_append_int(query, "star", -1);
                    LOGD("[GWJ] %s: client-pop-w order", __FUNCTION__);
                    break;

                case CLIENT_WEEKLY_DOWNLOAD_TYPE:
                    bson_append_int(query, "ptype_2", -1);
                    LOGD("[GWJ] %s: client-weekly-download-w order", __FUNCTION__);
                    break;

                default:
                    //never come here
                    break;
            }

            bson_append_finish_object(query);
        }
    }

    return 0;
}
