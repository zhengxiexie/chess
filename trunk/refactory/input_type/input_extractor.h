/*
 * input_extractor.h
 *
 *  Created on: 2013-6-18
 *      Author: guoweijiang
 */

#ifndef INPUT_EXTRACTOR_H_
#define INPUT_EXTRACTOR_H_

#include "../common.h"
using namespace std;

/*
 *  输入参数的书写规格：
 *      KEY_1[XXX,XXX,XXX];KEY_2[XXX,XXX,XXX];
 *
 *  简介：
 *      其中 KEY_X 为筛选条件的名字
 *      XXX 为筛选的值，同一个 KEY_X 内多个值之间是 OR 逻辑
 *      不同 KEY_X 之间是 AND 逻辑
 */
class InputExtractor
{
private:

    /*
     *  分离出来的输入参数:
     *
     *  KEY   : 就是输入参数中的 KEY_X
     *  VALUE : 是[]中的字符串，即  XXX,XXX,XXX
     */
    map<string, string> extracted_params;

    void extract_parameters(string& key_name,
                            string& value_str,
                            string input_param);
public:
    ~InputExtractor();

    /*
     *  将输入参数 ：KEY_1[XXX,XXX,XXX];KEY_2[XXX,XXX,XXX];
     *  规格化保存到  extracted_params 中
     */
    int parse_input_params(const char* input_param);

    /*
     *  获取指定 key_name 的 XXX,XXX,XXX 值
     */
    const char* get_filter_condition(const char* key_name);
};

#endif /* INPUT_EXTRACTOR_H_ */
