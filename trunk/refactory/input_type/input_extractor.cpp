/*
 * input_extractor.cpp
 *
 *  Created on: 2013-6-18
 *      Author: guoweijiang
 */

#include "input_extractor.h"

InputExtractor::~InputExtractor()
{
    LOGD("[GWJ] %s: start", __FUNCTION__);

    map<string, string>::iterator iter = this->extracted_params.begin();
    for(; iter != this->extracted_params.end(); iter++)
    {
        iter->second.clear();
    }
    this->extracted_params.clear();

    LOGD("[GWJ] %s: end", __FUNCTION__);
}

void InputExtractor::extract_parameters(string& key_name, string& value_str, string input_param)
{
    LOGD("[GWJ] %s: start. input_param[%s]", __FUNCTION__, input_param.c_str());

    int i = 0;
    int str_len = strlen(input_param.c_str());

    if(str_len >= (KEY_NAME_MAX_LEN + VALUE_MAX_LEN))
    {
        LOGD("[GWJ] %s: InputParam Length Out of bound Error! strlen[%d]",
             __FUNCTION__, str_len);

        key_name = "";
        value_str = "";
        return;
    }

    char key[KEY_NAME_MAX_LEN];
    char value[VALUE_MAX_LEN];
    memset(key, 0x00, sizeof(key));
    memset(value, 0x00, sizeof(value));

    for(; i < str_len && i < KEY_NAME_MAX_LEN; i++)
    {
        if(input_param[i] != '[')
        {
            key[i] = input_param[i];
        }
        else
        {
            break;
        }
    }

    if(i >= KEY_NAME_MAX_LEN)
    {
        LOGD("[GWJ] %s: KEY_NAME Out of bound Error!", __FUNCTION__);
        key_name = "";
        value_str = "";
        return;
    }

    i++;
    for(int j = 0; i < str_len && i < VALUE_MAX_LEN; i++, j++)
    {
        if(input_param[i] != ']')
        {
            value[j] = input_param[i];
        }
        else
        {
            break;
        }
    }

    if(i >= VALUE_MAX_LEN)
    {
        LOGD("[GWJ] %s: VALUE Out of bound Error!", __FUNCTION__);
        key_name = "";
        value_str = "";
        return;
    }

    key_name = key;
    value_str = value;

    LOGD("[GWJ] %s: input_param[%s], key[%s], value[%s]",
         __FUNCTION__, input_param.c_str(), key_name.c_str(), value_str.c_str());
}

/*
 *  将输入参数 ：KEY_1[XXX,XXX,XXX];KEY_2[XXX,XXX,XXX];
 *  规格化保存到  extracted_params 中
 */
int InputExtractor::parse_input_params(const char* input_param)
{
    LOGD("[GWJ] %s start. input[%s]", __FUNCTION__, input_param);

    string input_param_str = input_param;
    vector<string> inputs;
    int size = CommonInterface::splic_string(input_param_str, inputs, ";");

    if(size == 0)
    {
        LOGD("[GWJ] %s no parameters", __FUNCTION__);
        return -1;
    }
    else
    {
        for(int i = 0; i < size; i++)
        {
            string key_name;
            string value_params;
            this->extract_parameters(key_name, value_params, inputs[i]);
            this->extracted_params[key_name] = value_params;

            LOGD("[GWJ] %s: index[%d], KEY[%s], VALUE[%s]",
                 __FUNCTION__, i, key_name.c_str(), value_params.c_str());
        }
    }

    CommonInterface::clear_vector_string(inputs);
    LOGD("[GWJ] %s: end", __FUNCTION__);
}

/*
 *  获取指定 key_name 的 XXX,XXX,XXX 值
 */
const char* InputExtractor::get_filter_condition(const char* key_name)
{
    LOGD("[GWJ] %s: start. key_name[%s]", __FUNCTION__, key_name);

    if(IS_VALUED_STRING(key_name))
    {
        string value = this->extracted_params[string(key_name)];

        LOGD("[GWJ] %s: end. value[%s]", __FUNCTION__, value.c_str());
        return value.c_str();
    }
    else
    {
        LOGD("[GWJ] %s: end. KEY NOT FOUND ERROR!!", __FUNCTION__);
        return NULL;
    }
}
