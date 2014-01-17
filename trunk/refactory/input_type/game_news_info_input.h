/*
 * open_server_input.h
 *
 *  Created on: 2013-6-4
 *      Author: guoweijiang
 */

#ifndef OPEN_SERVER_INPUT_H_
#define OPEN_SERVER_INPUT_H_

struct game_news_info_input_params
{
    vector<string> game_img_types;
    vector<string> activity_img_types;

    void clear()
    {
        int size = game_img_types.size();
        for(int i = 0; i < size; i++)
        {
            game_img_types[i].clear();
        }
        game_img_types.clear();

        size = activity_img_types.size();
        for(int i = 0; i < size; i++)
        {
            activity_img_types[i].clear();
        }
        activity_img_types.clear();
    }
};

#endif /* OPEN_SERVER_INPUT_H_ */
