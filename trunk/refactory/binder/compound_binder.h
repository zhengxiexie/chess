///*
// * compound_binder.h
// *
// *  Created on: 2013-5-28
// *      Author: guoweijiang
// */

#ifndef COMPOUND_BINDER_H_
#define COMPOUND_BINDER_H_

#include "../common.h"
#include "../module/abstract_mcp_content.h"
#include "../factory/compound_binded_factory.h"

/*
 *  当涉及到多表联合时，需要用该黏合剂进行黏合
 *  每张表对应一个 module，不同 module 之间通过 key_value 连接
 */
class CompoundBindedRecorder
{

friend class CompoundBindedRecordFactory;

public:

private:
    const char* key_value;

    /*
     *  多个 module 黏合到一起，它们的 key_bucket 有相同的 key_value
     *  就是通过这个相同的 key_value 来判断不同 module 中的哪个 key_bucket 黏合到一起
     *
     *  【注意】
     *   module_records 中只有真数据的 module，没有占位 module
     */
    vector<KeyBucket*> module_records;

    /*
     *  专门记录 join 的所有 module 中哪个是占位空数据，哪个有实际真数据
     *  1: 有真数据的 module
     *  0: 没有数据的占位空 module
     *  见 CompoundBindedRecorder::add_key_bucket 中的使用
     *
     *  【注意】
     *   modules_info 中包含了真数据 module 和占位空 module
     */
    vector<char> modules_info;

    /*
     *  判断输入的所需 module 是否为空数据只占位的 module
     *
     *  out_mod_index 输出参数：
     *  当 module_index 不是空占位 module 而modules_info 中包含了空占位 module 时
     *  out_mod_index 返回所需 module 在 module_records 中真正的索引值
     */
    bool is_right_module_index(int module_index, int* out_mod_index);
public:

    static const char* ClassName;

    CompoundBindedRecorder()
    {
        this->key_value = "";
    }

    CompoundBindedRecorder(const char* key)
    {
        this->key_value = key;
    }

    ~CompoundBindedRecorder()
    {
        this->clear();
    }

    void clear()
    {
        /*
         *   module_records 中的元素
         *   也是对象池中分配出来的，所以此处不对各个元素做任何处理
         *   等待管理相应对象池自己清理即可
         *   此处只清理 vector 本身即可
         */
        this->key_value = "";
        this->module_records.clear();
        this->modules_info.clear();
    }

    /*
     *  获取黏合器中的数据：
     *
     *  module_index : 黏合剂中第几个 module
     *  vec_index    : 指定 module 中 key_bucket 的索引
     */
    AbstractMCPRecord* get_record(int module_index, int vec_index);

    /*
     *  获取黏合在一起的某一个 module 中 KeyBucket 的大小
     */
    int get_key_bucket_size(int module_index);

    /*
     *  从不同 module 中增加具有相同 key_value 的 key_bucket
     */
    int add_key_bucket(KeyBucket* records);

    /*
     *  获取 KEY 索引值
     */
    const char* get_key_value();
};

class CompoundBinder
{
protected:

    vector<CompoundBindedRecorder*> binded_result;

public:

    CompoundBinder();
    virtual ~CompoundBinder();

    /*
     *  获取黏合器中的结果总数
     */
    int get_result_count();

    /*
     *  获取黏合器中某一个 module 中 KeyBucket 的大小
     */
    int get_bucket_size(int k_index,
                        int module_index);

    /*
     *  获取黏合器中某一个 module 中 KeyBucket 里指定的一条记录
     */
    AbstractMCPRecord* get_record_value(int k_index,
                                        int module_index,
                                        int vec_index);

    /*
     *  增加黏合器中的 module
     */
    void left_join(AbstractMCPContent* joint_module);
};

#endif /* COMPOUND_BINDER_H_ */
