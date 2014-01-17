#ifndef pt_game_user_QA_list_H_
#define pt_game_user_QA_list_H_


#include "abstract_mcp_content.h"


class PTGameUserQARecord : public AbstractMCPRecord
{
public:


    static const char* ClassName;

    string question_id;
    string user_id;

    void clear()
    {
        LOGG("[GWJ] %s:  start ", __FUNCTION__);
        question_id.clear();
        user_id.clear();
        LOGG("[GWJ] %s:  end ", __FUNCTION__);
    }

    PTGameUserQARecord():AbstractMCPRecord()
    {
    }


    virtual ~PTGameUserQARecord()
    {
        this->clear();
    }
};




class PTGameUserQandA : public AbstractMCPContent
{
protected:
    int set_basic_info_from_bson_result(bson* bson_out, const char* aggregate_name);


public:
    PTGameUserQandA(const char* key_name);
    virtual ~PTGameUserQandA();
};


#endif /* pt_game_user_QA_list_H_ */