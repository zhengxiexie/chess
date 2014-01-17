/*
 * content_input.h
 *
 *  Created on: 2013-5-14
 *      Author: guoweijiang
 */

#ifndef CONTENT_INPUT_H_
#define CONTENT_INPUT_H_

/*
 *  输入参数为结构体，且有多项数据用逗号分隔时
 *  将分割后的数据存入此处
 */
struct content_input_params
{
    vector<string> lable_ids;
    vector<string> img_types;
    vector<string> column_ids;
    vector<string> content_ids;

    void clear()
    {
        int size = lable_ids.size();
        for(int i = 0; i < size; i++)
        {
            lable_ids[i].clear();
        }
        lable_ids.clear();

        size = img_types.size();
        for(int i = 0; i < size; i++)
        {
            img_types[i].clear();
        }
        img_types.clear();

        size = column_ids.size();
        for(int i = 0; i < size; i++)
        {
            column_ids[i].clear();
        }
        column_ids.clear();

        size = content_ids.size();
        for(int i = 0; i < size; i++)
        {
            content_ids[i].clear();
        }
        content_ids.clear();
    }
};

#endif /* CONTENT_INPUT_H_ */
