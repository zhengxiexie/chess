#ifndef pt_game_QandA_H_
#define pt_game_QandA_H_


#include "abstract_mcp_content.h"


class PTGameQandARecord : public AbstractMCPRecord
{
public:


    static const char* ClassName;

    string enable;
    string answer;
    string status;
    string app_id;
    string question_type;
    string phone;
    string question;
    string date_q;
    string remarks;
    string id;
    string date_a;

    void clear()
    {
        LOGG("[GWJ] %s:  start ", __FUNCTION__);
        enable.clear();
        answer.clear();
        status.clear();
        app_id.clear();
        question_type.clear();
        phone.clear();
        question.clear();
        date_q.clear();
        remarks.clear();
        id.clear();
        date_a.clear();
        LOGG("[GWJ] %s:  end ", __FUNCTION__);
    }

    PTGameQandARecord():AbstractMCPRecord()
    {
    }


    virtual ~PTGameQandARecord()
    {
        this->clear();
    }
};




class PTGameQandA : public AbstractMCPContent
{
protected:
    int set_basic_info_from_bson_result(bson* bson_out, const char* aggregate_name);


public:
    PTGameQandA(const char* key_name);
    virtual ~PTGameQandA();
};


#endif /* pt_game_QandA_H_ */