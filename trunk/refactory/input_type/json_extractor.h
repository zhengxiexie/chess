/*
 * json_extractor.h
 *
 *  Created on: 2013-6-25
 *      Author: guoweijiang
 */

#ifndef JSON_EXTRACTOR_H_
#define JSON_EXTRACTOR_H_

#include "../common.h"
using namespace std;

class JsonExtractor
{
private:

    map<string, string> extracted_params;

    void extract_parameters(string& key_name,
                            string& value_str,
                            string input_param);
public:
    ~JsonExtractor();

    int parse_input_params(const char* input_param);
    const char* get_filter_condition(const char* key_name);

    int set_key_value_tuple(const char* key, const char* value);
    const char* generate_json_string();
};

#endif /* JSON_EXTRACTOR_H_ */
