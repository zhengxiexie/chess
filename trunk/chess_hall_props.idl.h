#ifndef IDL_GENERATED_FILE_CHESS_HALL_PROPS_IDL_H_
#define IDL_GENERATED_FILE_CHESS_HALL_PROPS_IDL_H_
#include "mcpack_idl.h"
namespace idl {
class exchange_code_info;
class result_cp;
class chesshallprops_obtain_commodity_params;
class chesshallprops_obtain_commodity_result_params;
class chesshallprops_obtain_commodity_response;
class chesshallprops_obtain_exchange_code_params;
class chesshallprops_obtain_exchange_code_result_params;
class chesshallprops_obtain_exchange_code_response;
class chesshallprops_use_exchange_code_params;
class chesshallprops_use_exchange_code_result_params;
class chesshallprops_use_exchange_code_response;
class exchange_code_info : public ::idl::IDLObject , public ::idl::McpackBean {
public:
static exchange_code_info* create(bsl::mempool* pool);
static void destroy(exchange_code_info* v);
explicit exchange_code_info(bsl::mempool* pool);
exchange_code_info(const exchange_code_info& v);
~exchange_code_info();
exchange_code_info& operator=(const exchange_code_info& v);
void clear();
void check() const;
void detach();
exchange_code_info& assign(const exchange_code_info& v);
// @@int32_t user_id;
inline bool has_user_id() const;
inline int32_t user_id() const;
inline exchange_code_info& set_user_id(int32_t v);
inline void clear_user_id();
// @@string number;
inline bool has_number() const;
inline const char* number(unsigned int* size=0) const;
inline exchange_code_info& set_number(const char* s,ssize_t size=-1);
inline exchange_code_info& set_ptr_number(const char* s,ssize_t size=-1);
inline void clear_number();
// @@int32_t commodity_id;
inline bool has_commodity_id() const;
inline int32_t commodity_id() const;
inline exchange_code_info& set_commodity_id(int32_t v);
inline void clear_commodity_id();
// @@string expired_time;
inline bool has_expired_time() const;
inline const char* expired_time(unsigned int* size=0) const;
inline exchange_code_info& set_expired_time(const char* s,ssize_t size=-1);
inline exchange_code_info& set_ptr_expired_time(const char* s,ssize_t size=-1);
inline void clear_expired_time();
// @@string obtain_time;
inline bool has_obtain_time() const;
inline const char* obtain_time(unsigned int* size=0) const;
inline exchange_code_info& set_obtain_time(const char* s,ssize_t size=-1);
inline exchange_code_info& set_ptr_obtain_time(const char* s,ssize_t size=-1);
inline void clear_obtain_time();
// @@int32_t expired_type;
inline bool has_expired_type() const;
inline int32_t expired_type() const;
inline exchange_code_info& set_expired_type(int32_t v);
inline void clear_expired_type();
// @@string game_id;
inline bool has_game_id() const;
inline const char* game_id(unsigned int* size=0) const;
inline exchange_code_info& set_game_id(const char* s,ssize_t size=-1);
inline exchange_code_info& set_ptr_game_id(const char* s,ssize_t size=-1);
inline void clear_game_id();
inline ::idl::Unknown* mutable_unknown();
inline const ::idl::Unknown& unknown() const;
// deprecated.use 'mutable_unknown' instead.
inline ::idl::Unknown& unknown();
// deprecated.use 'unknown().size()' instead.
inline size_t unknown_size() const;
//============================================================
// mcpack2 load/save.
void load(const mc_pack_t* pack);
void LoadWithoutCheck(const mc_pack_t* pack);
//for compatiblity.return 0 when it accepts item,else return -1
int load(const mc_pack_item_t& item);
size_t save(mc_pack_t* pack) const;
public:
bsl::mempool* _pool;
 int32_t _m_user_id;
 ::idl::string _m_number;
 int32_t _m_commodity_id;
 ::idl::string _m_expired_time;
 ::idl::string _m_obtain_time;
 int32_t _m_expired_type;
 ::idl::string _m_game_id;
mutable ::idl::Unknown _unknown;
char _flag[1];
}; // class exchange_code_info
class result_cp : public ::idl::IDLObject , public ::idl::McpackBean {
public:
static result_cp* create(bsl::mempool* pool);
static void destroy(result_cp* v);
explicit result_cp(bsl::mempool* pool);
result_cp(const result_cp& v);
~result_cp();
result_cp& operator=(const result_cp& v);
void clear();
void check() const;
void detach();
result_cp& assign(const result_cp& v);
// @@bool is_success;
inline bool has_is_success() const;
inline bool is_success() const;
inline result_cp& set_is_success(bool v);
inline void clear_is_success();
// @@int32_t code;
inline bool has_code() const;
inline int32_t code() const;
inline result_cp& set_code(int32_t v);
inline void clear_code();
// @@string message;
inline bool has_message() const;
inline const char* message(unsigned int* size=0) const;
inline result_cp& set_message(const char* s,ssize_t size=-1);
inline result_cp& set_ptr_message(const char* s,ssize_t size=-1);
inline void clear_message();
inline ::idl::Unknown* mutable_unknown();
inline const ::idl::Unknown& unknown() const;
// deprecated.use 'mutable_unknown' instead.
inline ::idl::Unknown& unknown();
// deprecated.use 'unknown().size()' instead.
inline size_t unknown_size() const;
//============================================================
// mcpack2 load/save.
void load(const mc_pack_t* pack);
void LoadWithoutCheck(const mc_pack_t* pack);
//for compatiblity.return 0 when it accepts item,else return -1
int load(const mc_pack_item_t& item);
size_t save(mc_pack_t* pack) const;
public:
bsl::mempool* _pool;
 bool _m_is_success;
 int32_t _m_code;
 ::idl::string _m_message;
mutable ::idl::Unknown _unknown;
char _flag[1];
}; // class result_cp
class chesshallprops_obtain_commodity_params : public ::idl::IDLObject , public ::idl::McpackBean {
public:
static chesshallprops_obtain_commodity_params* create(bsl::mempool* pool);
static void destroy(chesshallprops_obtain_commodity_params* v);
explicit chesshallprops_obtain_commodity_params(bsl::mempool* pool);
chesshallprops_obtain_commodity_params(const chesshallprops_obtain_commodity_params& v);
~chesshallprops_obtain_commodity_params();
chesshallprops_obtain_commodity_params& operator=(const chesshallprops_obtain_commodity_params& v);
void clear();
void check() const;
void detach();
chesshallprops_obtain_commodity_params& assign(const chesshallprops_obtain_commodity_params& v);
// @@int32_t userId=in();
inline bool has_userId() const;
inline int32_t userId() const;
inline chesshallprops_obtain_commodity_params& set_userId(int32_t v);
inline void clear_userId();
// @@int32_t commodity_id=in();
inline bool has_commodity_id() const;
inline int32_t commodity_id() const;
inline chesshallprops_obtain_commodity_params& set_commodity_id(int32_t v);
inline void clear_commodity_id();
// @@int32_t num=in();
inline bool has_num() const;
inline int32_t num() const;
inline chesshallprops_obtain_commodity_params& set_num(int32_t v);
inline void clear_num();
// @@string gameId=in();
inline bool has_gameId() const;
inline const char* gameId(unsigned int* size=0) const;
inline chesshallprops_obtain_commodity_params& set_gameId(const char* s,ssize_t size=-1);
inline chesshallprops_obtain_commodity_params& set_ptr_gameId(const char* s,ssize_t size=-1);
inline void clear_gameId();
inline ::idl::Unknown* mutable_unknown();
inline const ::idl::Unknown& unknown() const;
// deprecated.use 'mutable_unknown' instead.
inline ::idl::Unknown& unknown();
// deprecated.use 'unknown().size()' instead.
inline size_t unknown_size() const;
//============================================================
// mcpack2 load/save.
void load(const mc_pack_t* pack);
void LoadWithoutCheck(const mc_pack_t* pack);
//for compatiblity.return 0 when it accepts item,else return -1
int load(const mc_pack_item_t& item);
size_t save(mc_pack_t* pack) const;
public:
bsl::mempool* _pool;
 int32_t _m_userId;
 int32_t _m_commodity_id;
 int32_t _m_num;
 ::idl::string _m_gameId;
mutable ::idl::Unknown _unknown;
char _flag[1];
}; // class chesshallprops_obtain_commodity_params
class chesshallprops_obtain_commodity_result_params : public ::idl::IDLObject , public ::idl::McpackBean {
public:
static chesshallprops_obtain_commodity_result_params* create(bsl::mempool* pool);
static void destroy(chesshallprops_obtain_commodity_result_params* v);
explicit chesshallprops_obtain_commodity_result_params(bsl::mempool* pool);
chesshallprops_obtain_commodity_result_params(const chesshallprops_obtain_commodity_result_params& v);
~chesshallprops_obtain_commodity_result_params();
chesshallprops_obtain_commodity_result_params& operator=(const chesshallprops_obtain_commodity_result_params& v);
void clear();
void check() const;
void detach();
chesshallprops_obtain_commodity_result_params& assign(const chesshallprops_obtain_commodity_result_params& v);
// @@bool success=out();
inline bool has_success() const;
inline bool success() const;
inline chesshallprops_obtain_commodity_result_params& set_success(bool v);
inline void clear_success();
inline ::idl::Unknown* mutable_unknown();
inline const ::idl::Unknown& unknown() const;
// deprecated.use 'mutable_unknown' instead.
inline ::idl::Unknown& unknown();
// deprecated.use 'unknown().size()' instead.
inline size_t unknown_size() const;
//============================================================
// mcpack2 load/save.
void load(const mc_pack_t* pack);
void LoadWithoutCheck(const mc_pack_t* pack);
//for compatiblity.return 0 when it accepts item,else return -1
int load(const mc_pack_item_t& item);
size_t save(mc_pack_t* pack) const;
public:
bsl::mempool* _pool;
 bool _m_success;
mutable ::idl::Unknown _unknown;
char _flag[1];
}; // class chesshallprops_obtain_commodity_result_params
class chesshallprops_obtain_commodity_response : public ::idl::IDLObject , public ::idl::McpackBean {
public:
static chesshallprops_obtain_commodity_response* create(bsl::mempool* pool);
static void destroy(chesshallprops_obtain_commodity_response* v);
explicit chesshallprops_obtain_commodity_response(bsl::mempool* pool);
chesshallprops_obtain_commodity_response(const chesshallprops_obtain_commodity_response& v);
~chesshallprops_obtain_commodity_response();
chesshallprops_obtain_commodity_response& operator=(const chesshallprops_obtain_commodity_response& v);
void clear();
void check() const;
void detach();
chesshallprops_obtain_commodity_response& assign(const chesshallprops_obtain_commodity_response& v);
// @@chesshallprops_obtain_commodity_result_params result_params;
inline bool has_result_params() const;
inline const ::idl::chesshallprops_obtain_commodity_result_params& result_params() const;
inline ::idl::chesshallprops_obtain_commodity_result_params* mutable_result_params();
// deprecated.use 'mutable_result_params()' instead.
inline ::idl::chesshallprops_obtain_commodity_result_params* m_result_params();
inline void clear_result_params();
inline ::idl::Unknown* mutable_unknown();
inline const ::idl::Unknown& unknown() const;
// deprecated.use 'mutable_unknown' instead.
inline ::idl::Unknown& unknown();
// deprecated.use 'unknown().size()' instead.
inline size_t unknown_size() const;
//============================================================
// mcpack2 load/save.
void load(const mc_pack_t* pack);
void LoadWithoutCheck(const mc_pack_t* pack);
//for compatiblity.return 0 when it accepts item,else return -1
int load(const mc_pack_item_t& item);
size_t save(mc_pack_t* pack) const;
public:
bsl::mempool* _pool;
mutable ::idl::chesshallprops_obtain_commodity_result_params* _m_result_params;
mutable ::idl::Unknown _unknown;
char _flag[1];
}; // class chesshallprops_obtain_commodity_response
class chesshallprops_obtain_exchange_code_params : public ::idl::IDLObject , public ::idl::McpackBean {
public:
static chesshallprops_obtain_exchange_code_params* create(bsl::mempool* pool);
static void destroy(chesshallprops_obtain_exchange_code_params* v);
explicit chesshallprops_obtain_exchange_code_params(bsl::mempool* pool);
chesshallprops_obtain_exchange_code_params(const chesshallprops_obtain_exchange_code_params& v);
~chesshallprops_obtain_exchange_code_params();
chesshallprops_obtain_exchange_code_params& operator=(const chesshallprops_obtain_exchange_code_params& v);
void clear();
void check() const;
void detach();
chesshallprops_obtain_exchange_code_params& assign(const chesshallprops_obtain_exchange_code_params& v);
// @@int32_t user_id=in();
inline bool has_user_id() const;
inline int32_t user_id() const;
inline chesshallprops_obtain_exchange_code_params& set_user_id(int32_t v);
inline void clear_user_id();
// @@string number=in();
inline bool has_number() const;
inline const char* number(unsigned int* size=0) const;
inline chesshallprops_obtain_exchange_code_params& set_number(const char* s,ssize_t size=-1);
inline chesshallprops_obtain_exchange_code_params& set_ptr_number(const char* s,ssize_t size=-1);
inline void clear_number();
// @@int32_t commodity_id=in();
inline bool has_commodity_id() const;
inline int32_t commodity_id() const;
inline chesshallprops_obtain_exchange_code_params& set_commodity_id(int32_t v);
inline void clear_commodity_id();
// @@string expired_time=in();
inline bool has_expired_time() const;
inline const char* expired_time(unsigned int* size=0) const;
inline chesshallprops_obtain_exchange_code_params& set_expired_time(const char* s,ssize_t size=-1);
inline chesshallprops_obtain_exchange_code_params& set_ptr_expired_time(const char* s,ssize_t size=-1);
inline void clear_expired_time();
// @@int32_t expired_type=in();
inline bool has_expired_type() const;
inline int32_t expired_type() const;
inline chesshallprops_obtain_exchange_code_params& set_expired_type(int32_t v);
inline void clear_expired_type();
// @@string game_id=in();
inline bool has_game_id() const;
inline const char* game_id(unsigned int* size=0) const;
inline chesshallprops_obtain_exchange_code_params& set_game_id(const char* s,ssize_t size=-1);
inline chesshallprops_obtain_exchange_code_params& set_ptr_game_id(const char* s,ssize_t size=-1);
inline void clear_game_id();
inline ::idl::Unknown* mutable_unknown();
inline const ::idl::Unknown& unknown() const;
// deprecated.use 'mutable_unknown' instead.
inline ::idl::Unknown& unknown();
// deprecated.use 'unknown().size()' instead.
inline size_t unknown_size() const;
//============================================================
// mcpack2 load/save.
void load(const mc_pack_t* pack);
void LoadWithoutCheck(const mc_pack_t* pack);
//for compatiblity.return 0 when it accepts item,else return -1
int load(const mc_pack_item_t& item);
size_t save(mc_pack_t* pack) const;
public:
bsl::mempool* _pool;
 int32_t _m_user_id;
 ::idl::string _m_number;
 int32_t _m_commodity_id;
 ::idl::string _m_expired_time;
 int32_t _m_expired_type;
 ::idl::string _m_game_id;
mutable ::idl::Unknown _unknown;
char _flag[1];
}; // class chesshallprops_obtain_exchange_code_params
class chesshallprops_obtain_exchange_code_result_params : public ::idl::IDLObject , public ::idl::McpackBean {
public:
static chesshallprops_obtain_exchange_code_result_params* create(bsl::mempool* pool);
static void destroy(chesshallprops_obtain_exchange_code_result_params* v);
explicit chesshallprops_obtain_exchange_code_result_params(bsl::mempool* pool);
chesshallprops_obtain_exchange_code_result_params(const chesshallprops_obtain_exchange_code_result_params& v);
~chesshallprops_obtain_exchange_code_result_params();
chesshallprops_obtain_exchange_code_result_params& operator=(const chesshallprops_obtain_exchange_code_result_params& v);
void clear();
void check() const;
void detach();
chesshallprops_obtain_exchange_code_result_params& assign(const chesshallprops_obtain_exchange_code_result_params& v);
// @@bool success=out();
inline bool has_success() const;
inline bool success() const;
inline chesshallprops_obtain_exchange_code_result_params& set_success(bool v);
inline void clear_success();
inline ::idl::Unknown* mutable_unknown();
inline const ::idl::Unknown& unknown() const;
// deprecated.use 'mutable_unknown' instead.
inline ::idl::Unknown& unknown();
// deprecated.use 'unknown().size()' instead.
inline size_t unknown_size() const;
//============================================================
// mcpack2 load/save.
void load(const mc_pack_t* pack);
void LoadWithoutCheck(const mc_pack_t* pack);
//for compatiblity.return 0 when it accepts item,else return -1
int load(const mc_pack_item_t& item);
size_t save(mc_pack_t* pack) const;
public:
bsl::mempool* _pool;
 bool _m_success;
mutable ::idl::Unknown _unknown;
char _flag[1];
}; // class chesshallprops_obtain_exchange_code_result_params
class chesshallprops_obtain_exchange_code_response : public ::idl::IDLObject , public ::idl::McpackBean {
public:
static chesshallprops_obtain_exchange_code_response* create(bsl::mempool* pool);
static void destroy(chesshallprops_obtain_exchange_code_response* v);
explicit chesshallprops_obtain_exchange_code_response(bsl::mempool* pool);
chesshallprops_obtain_exchange_code_response(const chesshallprops_obtain_exchange_code_response& v);
~chesshallprops_obtain_exchange_code_response();
chesshallprops_obtain_exchange_code_response& operator=(const chesshallprops_obtain_exchange_code_response& v);
void clear();
void check() const;
void detach();
chesshallprops_obtain_exchange_code_response& assign(const chesshallprops_obtain_exchange_code_response& v);
// @@chesshallprops_obtain_exchange_code_result_params result_params;
inline bool has_result_params() const;
inline const ::idl::chesshallprops_obtain_exchange_code_result_params& result_params() const;
inline ::idl::chesshallprops_obtain_exchange_code_result_params* mutable_result_params();
// deprecated.use 'mutable_result_params()' instead.
inline ::idl::chesshallprops_obtain_exchange_code_result_params* m_result_params();
inline void clear_result_params();
inline ::idl::Unknown* mutable_unknown();
inline const ::idl::Unknown& unknown() const;
// deprecated.use 'mutable_unknown' instead.
inline ::idl::Unknown& unknown();
// deprecated.use 'unknown().size()' instead.
inline size_t unknown_size() const;
//============================================================
// mcpack2 load/save.
void load(const mc_pack_t* pack);
void LoadWithoutCheck(const mc_pack_t* pack);
//for compatiblity.return 0 when it accepts item,else return -1
int load(const mc_pack_item_t& item);
size_t save(mc_pack_t* pack) const;
public:
bsl::mempool* _pool;
mutable ::idl::chesshallprops_obtain_exchange_code_result_params* _m_result_params;
mutable ::idl::Unknown _unknown;
char _flag[1];
}; // class chesshallprops_obtain_exchange_code_response
class chesshallprops_use_exchange_code_params : public ::idl::IDLObject , public ::idl::McpackBean {
public:
static chesshallprops_use_exchange_code_params* create(bsl::mempool* pool);
static void destroy(chesshallprops_use_exchange_code_params* v);
explicit chesshallprops_use_exchange_code_params(bsl::mempool* pool);
chesshallprops_use_exchange_code_params(const chesshallprops_use_exchange_code_params& v);
~chesshallprops_use_exchange_code_params();
chesshallprops_use_exchange_code_params& operator=(const chesshallprops_use_exchange_code_params& v);
void clear();
void check() const;
void detach();
chesshallprops_use_exchange_code_params& assign(const chesshallprops_use_exchange_code_params& v);
// @@int32_t userId=in();
inline bool has_userId() const;
inline int32_t userId() const;
inline chesshallprops_use_exchange_code_params& set_userId(int32_t v);
inline void clear_userId();
// @@string gameId=in();
inline bool has_gameId() const;
inline const char* gameId(unsigned int* size=0) const;
inline chesshallprops_use_exchange_code_params& set_gameId(const char* s,ssize_t size=-1);
inline chesshallprops_use_exchange_code_params& set_ptr_gameId(const char* s,ssize_t size=-1);
inline void clear_gameId();
// @@string exchangeCodeId=in();
inline bool has_exchangeCodeId() const;
inline const char* exchangeCodeId(unsigned int* size=0) const;
inline chesshallprops_use_exchange_code_params& set_exchangeCodeId(const char* s,ssize_t size=-1);
inline chesshallprops_use_exchange_code_params& set_ptr_exchangeCodeId(const char* s,ssize_t size=-1);
inline void clear_exchangeCodeId();
inline ::idl::Unknown* mutable_unknown();
inline const ::idl::Unknown& unknown() const;
// deprecated.use 'mutable_unknown' instead.
inline ::idl::Unknown& unknown();
// deprecated.use 'unknown().size()' instead.
inline size_t unknown_size() const;
//============================================================
// mcpack2 load/save.
void load(const mc_pack_t* pack);
void LoadWithoutCheck(const mc_pack_t* pack);
//for compatiblity.return 0 when it accepts item,else return -1
int load(const mc_pack_item_t& item);
size_t save(mc_pack_t* pack) const;
public:
bsl::mempool* _pool;
 int32_t _m_userId;
 ::idl::string _m_gameId;
 ::idl::string _m_exchangeCodeId;
mutable ::idl::Unknown _unknown;
char _flag[1];
}; // class chesshallprops_use_exchange_code_params
class chesshallprops_use_exchange_code_result_params : public ::idl::IDLObject , public ::idl::McpackBean {
public:
static chesshallprops_use_exchange_code_result_params* create(bsl::mempool* pool);
static void destroy(chesshallprops_use_exchange_code_result_params* v);
explicit chesshallprops_use_exchange_code_result_params(bsl::mempool* pool);
chesshallprops_use_exchange_code_result_params(const chesshallprops_use_exchange_code_result_params& v);
~chesshallprops_use_exchange_code_result_params();
chesshallprops_use_exchange_code_result_params& operator=(const chesshallprops_use_exchange_code_result_params& v);
void clear();
void check() const;
void detach();
chesshallprops_use_exchange_code_result_params& assign(const chesshallprops_use_exchange_code_result_params& v);
// @@result_cp resultCP=out();
inline bool has_resultCP() const;
inline const ::idl::result_cp& resultCP() const;
inline ::idl::result_cp* mutable_resultCP();
// deprecated.use 'mutable_resultCP()' instead.
inline ::idl::result_cp* m_resultCP();
inline void clear_resultCP();
inline ::idl::Unknown* mutable_unknown();
inline const ::idl::Unknown& unknown() const;
// deprecated.use 'mutable_unknown' instead.
inline ::idl::Unknown& unknown();
// deprecated.use 'unknown().size()' instead.
inline size_t unknown_size() const;
//============================================================
// mcpack2 load/save.
void load(const mc_pack_t* pack);
void LoadWithoutCheck(const mc_pack_t* pack);
//for compatiblity.return 0 when it accepts item,else return -1
int load(const mc_pack_item_t& item);
size_t save(mc_pack_t* pack) const;
public:
bsl::mempool* _pool;
mutable ::idl::result_cp* _m_resultCP;
mutable ::idl::Unknown _unknown;
char _flag[1];
}; // class chesshallprops_use_exchange_code_result_params
class chesshallprops_use_exchange_code_response : public ::idl::IDLObject , public ::idl::McpackBean {
public:
static chesshallprops_use_exchange_code_response* create(bsl::mempool* pool);
static void destroy(chesshallprops_use_exchange_code_response* v);
explicit chesshallprops_use_exchange_code_response(bsl::mempool* pool);
chesshallprops_use_exchange_code_response(const chesshallprops_use_exchange_code_response& v);
~chesshallprops_use_exchange_code_response();
chesshallprops_use_exchange_code_response& operator=(const chesshallprops_use_exchange_code_response& v);
void clear();
void check() const;
void detach();
chesshallprops_use_exchange_code_response& assign(const chesshallprops_use_exchange_code_response& v);
// @@chesshallprops_use_exchange_code_result_params result_params;
inline bool has_result_params() const;
inline const ::idl::chesshallprops_use_exchange_code_result_params& result_params() const;
inline ::idl::chesshallprops_use_exchange_code_result_params* mutable_result_params();
// deprecated.use 'mutable_result_params()' instead.
inline ::idl::chesshallprops_use_exchange_code_result_params* m_result_params();
inline void clear_result_params();
inline ::idl::Unknown* mutable_unknown();
inline const ::idl::Unknown& unknown() const;
// deprecated.use 'mutable_unknown' instead.
inline ::idl::Unknown& unknown();
// deprecated.use 'unknown().size()' instead.
inline size_t unknown_size() const;
//============================================================
// mcpack2 load/save.
void load(const mc_pack_t* pack);
void LoadWithoutCheck(const mc_pack_t* pack);
//for compatiblity.return 0 when it accepts item,else return -1
int load(const mc_pack_item_t& item);
size_t save(mc_pack_t* pack) const;
public:
bsl::mempool* _pool;
mutable ::idl::chesshallprops_use_exchange_code_result_params* _m_result_params;
mutable ::idl::Unknown _unknown;
char _flag[1];
}; // class chesshallprops_use_exchange_code_response
inline exchange_code_info* exchange_code_info::create(bsl::mempool* pool){
if(!pool){
throw bsl::NullPointerException() << BSL_EARG;
}
exchange_code_info* tmp=(exchange_code_info*)pool->malloc(sizeof(*tmp));
if(!tmp){
throw bsl::BadAllocException() << BSL_EARG;
}
new(tmp)exchange_code_info(pool);
return tmp;
}
inline void exchange_code_info::destroy(exchange_code_info* v){
if(v){
bsl::mempool *pool=v->_pool;
v->~exchange_code_info();
pool->free(v,sizeof(*v));
}
}
inline exchange_code_info::exchange_code_info(bsl::mempool *pool): 
::idl::McpackBean(pool),
_pool(pool)
,_m_user_id(0)
,_m_number(_pool)
,_m_commodity_id(0)
,_m_expired_time(_pool)
,_m_obtain_time(_pool)
,_m_expired_type(0)
,_m_game_id(_pool)
,_unknown(_pool){
memset(_flag,0,sizeof(_flag));
}
inline exchange_code_info::exchange_code_info(const exchange_code_info& v): 
::idl::McpackBean(v._pool),
_pool(v._pool)
,_m_user_id(0)
,_m_number(_pool)
,_m_commodity_id(0)
,_m_expired_time(_pool)
,_m_obtain_time(_pool)
,_m_expired_type(0)
,_m_game_id(_pool)
,_unknown(_pool){
assign(v);
}
inline exchange_code_info::~exchange_code_info(){
}
inline exchange_code_info& exchange_code_info::operator=(const exchange_code_info& v){
assign(v);
return *this;
}
inline void exchange_code_info::clear(){
// _m_user_id=0;
_m_number.clear();
// _m_commodity_id=0;
_m_expired_time.clear();
_m_obtain_time.clear();
// _m_expired_type=0;
_m_game_id.clear();
memset(_flag,0,sizeof(_flag));
_unknown.clear(); 
::idl::McpackBean::clear();
}
// @@CF(constraint functions).
inline void exchange_code_info::check() const{
// @@int32_t user_id;
if(!(_flag[0 >> 3] & (1 << (0 & 0x7)))){
throw bsl::KeyNotFoundException() << BSL_EARG << "key '" << "user_id" << "' not found";
}
{
__attribute__((unused)) int32_t item=_m_user_id;

}
// @@string number;
if(!(_flag[1 >> 3] & (1 << (1 & 0x7)))){
throw bsl::KeyNotFoundException() << BSL_EARG << "key '" << "number" << "' not found";
}
{
__attribute__((unused)) const char* item=_m_number.get();

}
// @@int32_t commodity_id;
if(!(_flag[2 >> 3] & (1 << (2 & 0x7)))){
throw bsl::KeyNotFoundException() << BSL_EARG << "key '" << "commodity_id" << "' not found";
}
{
__attribute__((unused)) int32_t item=_m_commodity_id;

}
// @@string expired_time;
if(!(_flag[3 >> 3] & (1 << (3 & 0x7)))){
throw bsl::KeyNotFoundException() << BSL_EARG << "key '" << "expired_time" << "' not found";
}
{
__attribute__((unused)) const char* item=_m_expired_time.get();

}
// @@string obtain_time;
if(!(_flag[4 >> 3] & (1 << (4 & 0x7)))){
throw bsl::KeyNotFoundException() << BSL_EARG << "key '" << "obtain_time" << "' not found";
}
{
__attribute__((unused)) const char* item=_m_obtain_time.get();

}
// @@int32_t expired_type;
if(!(_flag[5 >> 3] & (1 << (5 & 0x7)))){
throw bsl::KeyNotFoundException() << BSL_EARG << "key '" << "expired_type" << "' not found";
}
{
__attribute__((unused)) int32_t item=_m_expired_type;

}
// @@string game_id;
if(!(_flag[6 >> 3] & (1 << (6 & 0x7)))){
throw bsl::KeyNotFoundException() << BSL_EARG << "key '" << "game_id" << "' not found";
}
{
__attribute__((unused)) const char* item=_m_game_id.get();

}

::idl::McpackBean::check();
}
inline void exchange_code_info::detach(){
_m_number.detach();
_m_expired_time.detach();
_m_obtain_time.detach();
_m_game_id.detach();
_unknown.clear();

::idl::McpackBean::detach();
}
inline exchange_code_info& exchange_code_info::assign(const exchange_code_info& v){
_m_user_id=v._m_user_id;
_m_number=v._m_number;
_m_commodity_id=v._m_commodity_id;
_m_expired_time=v._m_expired_time;
_m_obtain_time=v._m_obtain_time;
_m_expired_type=v._m_expired_type;
_m_game_id=v._m_game_id;
_unknown=v._unknown;
bsl::xmemcpy(_flag,v._flag,sizeof(v._flag));
return *this;
}
inline bool exchange_code_info::has_user_id() const{
return (_flag[0 >> 3] & (1 << (0 & 0x7)));
}
inline int32_t exchange_code_info::user_id() const{
return _m_user_id;
}
inline exchange_code_info& exchange_code_info::set_user_id(int32_t v){
_flag[0 >> 3] |= (1 << (0 & 0x7));
 _m_user_id=v;
return *this;
}
inline void exchange_code_info::clear_user_id(){
_flag[0 >> 3] &= ~(1 << (0 & 0x7));
 _m_user_id=0;
}
inline bool exchange_code_info::has_number() const{
return (_flag[1 >> 3] & (1 << (1 & 0x7)));
}
inline const char* exchange_code_info::number(unsigned int* size) const{
return _m_number.get(size);
}
inline exchange_code_info& exchange_code_info::set_number(const char* s,ssize_t size){
_flag[1 >> 3] |= (1 << (1 & 0x7));
 _m_number.set(s,(size==-1)?strlen(s):(size_t)size);
return *this;
}
inline exchange_code_info& exchange_code_info::set_ptr_number(const char* s,ssize_t size){
_flag[1 >> 3] |= (1 << (1 & 0x7));
 _m_number.set_ptr(s,(size==-1)?strlen(s):(size_t)size);
return *this;
}
inline void exchange_code_info::clear_number(){
_flag[1 >> 3] &= ~(1 << (1 & 0x7));
 _m_number.clear();
}
inline bool exchange_code_info::has_commodity_id() const{
return (_flag[2 >> 3] & (1 << (2 & 0x7)));
}
inline int32_t exchange_code_info::commodity_id() const{
return _m_commodity_id;
}
inline exchange_code_info& exchange_code_info::set_commodity_id(int32_t v){
_flag[2 >> 3] |= (1 << (2 & 0x7));
 _m_commodity_id=v;
return *this;
}
inline void exchange_code_info::clear_commodity_id(){
_flag[2 >> 3] &= ~(1 << (2 & 0x7));
 _m_commodity_id=0;
}
inline bool exchange_code_info::has_expired_time() const{
return (_flag[3 >> 3] & (1 << (3 & 0x7)));
}
inline const char* exchange_code_info::expired_time(unsigned int* size) const{
return _m_expired_time.get(size);
}
inline exchange_code_info& exchange_code_info::set_expired_time(const char* s,ssize_t size){
_flag[3 >> 3] |= (1 << (3 & 0x7));
 _m_expired_time.set(s,(size==-1)?strlen(s):(size_t)size);
return *this;
}
inline exchange_code_info& exchange_code_info::set_ptr_expired_time(const char* s,ssize_t size){
_flag[3 >> 3] |= (1 << (3 & 0x7));
 _m_expired_time.set_ptr(s,(size==-1)?strlen(s):(size_t)size);
return *this;
}
inline void exchange_code_info::clear_expired_time(){
_flag[3 >> 3] &= ~(1 << (3 & 0x7));
 _m_expired_time.clear();
}
inline bool exchange_code_info::has_obtain_time() const{
return (_flag[4 >> 3] & (1 << (4 & 0x7)));
}
inline const char* exchange_code_info::obtain_time(unsigned int* size) const{
return _m_obtain_time.get(size);
}
inline exchange_code_info& exchange_code_info::set_obtain_time(const char* s,ssize_t size){
_flag[4 >> 3] |= (1 << (4 & 0x7));
 _m_obtain_time.set(s,(size==-1)?strlen(s):(size_t)size);
return *this;
}
inline exchange_code_info& exchange_code_info::set_ptr_obtain_time(const char* s,ssize_t size){
_flag[4 >> 3] |= (1 << (4 & 0x7));
 _m_obtain_time.set_ptr(s,(size==-1)?strlen(s):(size_t)size);
return *this;
}
inline void exchange_code_info::clear_obtain_time(){
_flag[4 >> 3] &= ~(1 << (4 & 0x7));
 _m_obtain_time.clear();
}
inline bool exchange_code_info::has_expired_type() const{
return (_flag[5 >> 3] & (1 << (5 & 0x7)));
}
inline int32_t exchange_code_info::expired_type() const{
return _m_expired_type;
}
inline exchange_code_info& exchange_code_info::set_expired_type(int32_t v){
_flag[5 >> 3] |= (1 << (5 & 0x7));
 _m_expired_type=v;
return *this;
}
inline void exchange_code_info::clear_expired_type(){
_flag[5 >> 3] &= ~(1 << (5 & 0x7));
 _m_expired_type=0;
}
inline bool exchange_code_info::has_game_id() const{
return (_flag[6 >> 3] & (1 << (6 & 0x7)));
}
inline const char* exchange_code_info::game_id(unsigned int* size) const{
return _m_game_id.get(size);
}
inline exchange_code_info& exchange_code_info::set_game_id(const char* s,ssize_t size){
_flag[6 >> 3] |= (1 << (6 & 0x7));
 _m_game_id.set(s,(size==-1)?strlen(s):(size_t)size);
return *this;
}
inline exchange_code_info& exchange_code_info::set_ptr_game_id(const char* s,ssize_t size){
_flag[6 >> 3] |= (1 << (6 & 0x7));
 _m_game_id.set_ptr(s,(size==-1)?strlen(s):(size_t)size);
return *this;
}
inline void exchange_code_info::clear_game_id(){
_flag[6 >> 3] &= ~(1 << (6 & 0x7));
 _m_game_id.clear();
}
inline ::idl::Unknown* exchange_code_info::mutable_unknown(){
return &_unknown;
}
inline const ::idl::Unknown& exchange_code_info::unknown() const{
return _unknown;
}
inline ::idl::Unknown& exchange_code_info::unknown(){
return _unknown;
}
inline size_t exchange_code_info::unknown_size() const{
return _unknown.size();
}
inline result_cp* result_cp::create(bsl::mempool* pool){
if(!pool){
throw bsl::NullPointerException() << BSL_EARG;
}
result_cp* tmp=(result_cp*)pool->malloc(sizeof(*tmp));
if(!tmp){
throw bsl::BadAllocException() << BSL_EARG;
}
new(tmp)result_cp(pool);
return tmp;
}
inline void result_cp::destroy(result_cp* v){
if(v){
bsl::mempool *pool=v->_pool;
v->~result_cp();
pool->free(v,sizeof(*v));
}
}
inline result_cp::result_cp(bsl::mempool *pool): 
::idl::McpackBean(pool),
_pool(pool)
,_m_is_success(0)
,_m_code(0)
,_m_message(_pool)
,_unknown(_pool){
memset(_flag,0,sizeof(_flag));
}
inline result_cp::result_cp(const result_cp& v): 
::idl::McpackBean(v._pool),
_pool(v._pool)
,_m_is_success(0)
,_m_code(0)
,_m_message(_pool)
,_unknown(_pool){
assign(v);
}
inline result_cp::~result_cp(){
}
inline result_cp& result_cp::operator=(const result_cp& v){
assign(v);
return *this;
}
inline void result_cp::clear(){
// _m_is_success=0;
// _m_code=0;
_m_message.clear();
memset(_flag,0,sizeof(_flag));
_unknown.clear(); 
::idl::McpackBean::clear();
}
// @@CF(constraint functions).
inline void result_cp::check() const{
// @@bool is_success;
if(!(_flag[0 >> 3] & (1 << (0 & 0x7)))){
throw bsl::KeyNotFoundException() << BSL_EARG << "key '" << "is_success" << "' not found";
}
{
__attribute__((unused)) bool item=_m_is_success;

}
// @@int32_t code;
if(!(_flag[1 >> 3] & (1 << (1 & 0x7)))){
throw bsl::KeyNotFoundException() << BSL_EARG << "key '" << "code" << "' not found";
}
{
__attribute__((unused)) int32_t item=_m_code;

}
// @@string message;
if(!(_flag[2 >> 3] & (1 << (2 & 0x7)))){
throw bsl::KeyNotFoundException() << BSL_EARG << "key '" << "message" << "' not found";
}
{
__attribute__((unused)) const char* item=_m_message.get();

}

::idl::McpackBean::check();
}
inline void result_cp::detach(){
_m_message.detach();
_unknown.clear();

::idl::McpackBean::detach();
}
inline result_cp& result_cp::assign(const result_cp& v){
_m_is_success=v._m_is_success;
_m_code=v._m_code;
_m_message=v._m_message;
_unknown=v._unknown;
bsl::xmemcpy(_flag,v._flag,sizeof(v._flag));
return *this;
}
inline bool result_cp::has_is_success() const{
return (_flag[0 >> 3] & (1 << (0 & 0x7)));
}
inline bool result_cp::is_success() const{
return _m_is_success;
}
inline result_cp& result_cp::set_is_success(bool v){
_flag[0 >> 3] |= (1 << (0 & 0x7));
 _m_is_success=v;
return *this;
}
inline void result_cp::clear_is_success(){
_flag[0 >> 3] &= ~(1 << (0 & 0x7));
 _m_is_success=0;
}
inline bool result_cp::has_code() const{
return (_flag[1 >> 3] & (1 << (1 & 0x7)));
}
inline int32_t result_cp::code() const{
return _m_code;
}
inline result_cp& result_cp::set_code(int32_t v){
_flag[1 >> 3] |= (1 << (1 & 0x7));
 _m_code=v;
return *this;
}
inline void result_cp::clear_code(){
_flag[1 >> 3] &= ~(1 << (1 & 0x7));
 _m_code=0;
}
inline bool result_cp::has_message() const{
return (_flag[2 >> 3] & (1 << (2 & 0x7)));
}
inline const char* result_cp::message(unsigned int* size) const{
return _m_message.get(size);
}
inline result_cp& result_cp::set_message(const char* s,ssize_t size){
_flag[2 >> 3] |= (1 << (2 & 0x7));
 _m_message.set(s,(size==-1)?strlen(s):(size_t)size);
return *this;
}
inline result_cp& result_cp::set_ptr_message(const char* s,ssize_t size){
_flag[2 >> 3] |= (1 << (2 & 0x7));
 _m_message.set_ptr(s,(size==-1)?strlen(s):(size_t)size);
return *this;
}
inline void result_cp::clear_message(){
_flag[2 >> 3] &= ~(1 << (2 & 0x7));
 _m_message.clear();
}
inline ::idl::Unknown* result_cp::mutable_unknown(){
return &_unknown;
}
inline const ::idl::Unknown& result_cp::unknown() const{
return _unknown;
}
inline ::idl::Unknown& result_cp::unknown(){
return _unknown;
}
inline size_t result_cp::unknown_size() const{
return _unknown.size();
}
inline chesshallprops_obtain_commodity_params* chesshallprops_obtain_commodity_params::create(bsl::mempool* pool){
if(!pool){
throw bsl::NullPointerException() << BSL_EARG;
}
chesshallprops_obtain_commodity_params* tmp=(chesshallprops_obtain_commodity_params*)pool->malloc(sizeof(*tmp));
if(!tmp){
throw bsl::BadAllocException() << BSL_EARG;
}
new(tmp)chesshallprops_obtain_commodity_params(pool);
return tmp;
}
inline void chesshallprops_obtain_commodity_params::destroy(chesshallprops_obtain_commodity_params* v){
if(v){
bsl::mempool *pool=v->_pool;
v->~chesshallprops_obtain_commodity_params();
pool->free(v,sizeof(*v));
}
}
inline chesshallprops_obtain_commodity_params::chesshallprops_obtain_commodity_params(bsl::mempool *pool): 
::idl::McpackBean(pool),
_pool(pool)
,_m_userId(0)
,_m_commodity_id(0)
,_m_num(0)
,_m_gameId(_pool)
,_unknown(_pool){
memset(_flag,0,sizeof(_flag));
}
inline chesshallprops_obtain_commodity_params::chesshallprops_obtain_commodity_params(const chesshallprops_obtain_commodity_params& v): 
::idl::McpackBean(v._pool),
_pool(v._pool)
,_m_userId(0)
,_m_commodity_id(0)
,_m_num(0)
,_m_gameId(_pool)
,_unknown(_pool){
assign(v);
}
inline chesshallprops_obtain_commodity_params::~chesshallprops_obtain_commodity_params(){
}
inline chesshallprops_obtain_commodity_params& chesshallprops_obtain_commodity_params::operator=(const chesshallprops_obtain_commodity_params& v){
assign(v);
return *this;
}
inline void chesshallprops_obtain_commodity_params::clear(){
// _m_userId=0;
// _m_commodity_id=0;
// _m_num=0;
_m_gameId.clear();
memset(_flag,0,sizeof(_flag));
_unknown.clear(); 
::idl::McpackBean::clear();
}
// @@CF(constraint functions).
inline void chesshallprops_obtain_commodity_params::check() const{
// @@int32_t userId=in();
if(!(_flag[0 >> 3] & (1 << (0 & 0x7)))){
throw bsl::KeyNotFoundException() << BSL_EARG << "key '" << "userId" << "' not found";
}
{
__attribute__((unused)) int32_t item=_m_userId;

}
// @@int32_t commodity_id=in();
if(!(_flag[1 >> 3] & (1 << (1 & 0x7)))){
throw bsl::KeyNotFoundException() << BSL_EARG << "key '" << "commodity_id" << "' not found";
}
{
__attribute__((unused)) int32_t item=_m_commodity_id;

}
// @@int32_t num=in();
if(!(_flag[2 >> 3] & (1 << (2 & 0x7)))){
throw bsl::KeyNotFoundException() << BSL_EARG << "key '" << "num" << "' not found";
}
{
__attribute__((unused)) int32_t item=_m_num;

}
// @@string gameId=in();
if(!(_flag[3 >> 3] & (1 << (3 & 0x7)))){
throw bsl::KeyNotFoundException() << BSL_EARG << "key '" << "gameId" << "' not found";
}
{
__attribute__((unused)) const char* item=_m_gameId.get();

}

::idl::McpackBean::check();
}
inline void chesshallprops_obtain_commodity_params::detach(){
_m_gameId.detach();
_unknown.clear();

::idl::McpackBean::detach();
}
inline chesshallprops_obtain_commodity_params& chesshallprops_obtain_commodity_params::assign(const chesshallprops_obtain_commodity_params& v){
_m_userId=v._m_userId;
_m_commodity_id=v._m_commodity_id;
_m_num=v._m_num;
_m_gameId=v._m_gameId;
_unknown=v._unknown;
bsl::xmemcpy(_flag,v._flag,sizeof(v._flag));
return *this;
}
inline bool chesshallprops_obtain_commodity_params::has_userId() const{
return (_flag[0 >> 3] & (1 << (0 & 0x7)));
}
inline int32_t chesshallprops_obtain_commodity_params::userId() const{
return _m_userId;
}
inline chesshallprops_obtain_commodity_params& chesshallprops_obtain_commodity_params::set_userId(int32_t v){
_flag[0 >> 3] |= (1 << (0 & 0x7));
 _m_userId=v;
return *this;
}
inline void chesshallprops_obtain_commodity_params::clear_userId(){
_flag[0 >> 3] &= ~(1 << (0 & 0x7));
 _m_userId=0;
}
inline bool chesshallprops_obtain_commodity_params::has_commodity_id() const{
return (_flag[1 >> 3] & (1 << (1 & 0x7)));
}
inline int32_t chesshallprops_obtain_commodity_params::commodity_id() const{
return _m_commodity_id;
}
inline chesshallprops_obtain_commodity_params& chesshallprops_obtain_commodity_params::set_commodity_id(int32_t v){
_flag[1 >> 3] |= (1 << (1 & 0x7));
 _m_commodity_id=v;
return *this;
}
inline void chesshallprops_obtain_commodity_params::clear_commodity_id(){
_flag[1 >> 3] &= ~(1 << (1 & 0x7));
 _m_commodity_id=0;
}
inline bool chesshallprops_obtain_commodity_params::has_num() const{
return (_flag[2 >> 3] & (1 << (2 & 0x7)));
}
inline int32_t chesshallprops_obtain_commodity_params::num() const{
return _m_num;
}
inline chesshallprops_obtain_commodity_params& chesshallprops_obtain_commodity_params::set_num(int32_t v){
_flag[2 >> 3] |= (1 << (2 & 0x7));
 _m_num=v;
return *this;
}
inline void chesshallprops_obtain_commodity_params::clear_num(){
_flag[2 >> 3] &= ~(1 << (2 & 0x7));
 _m_num=0;
}
inline bool chesshallprops_obtain_commodity_params::has_gameId() const{
return (_flag[3 >> 3] & (1 << (3 & 0x7)));
}
inline const char* chesshallprops_obtain_commodity_params::gameId(unsigned int* size) const{
return _m_gameId.get(size);
}
inline chesshallprops_obtain_commodity_params& chesshallprops_obtain_commodity_params::set_gameId(const char* s,ssize_t size){
_flag[3 >> 3] |= (1 << (3 & 0x7));
 _m_gameId.set(s,(size==-1)?strlen(s):(size_t)size);
return *this;
}
inline chesshallprops_obtain_commodity_params& chesshallprops_obtain_commodity_params::set_ptr_gameId(const char* s,ssize_t size){
_flag[3 >> 3] |= (1 << (3 & 0x7));
 _m_gameId.set_ptr(s,(size==-1)?strlen(s):(size_t)size);
return *this;
}
inline void chesshallprops_obtain_commodity_params::clear_gameId(){
_flag[3 >> 3] &= ~(1 << (3 & 0x7));
 _m_gameId.clear();
}
inline ::idl::Unknown* chesshallprops_obtain_commodity_params::mutable_unknown(){
return &_unknown;
}
inline const ::idl::Unknown& chesshallprops_obtain_commodity_params::unknown() const{
return _unknown;
}
inline ::idl::Unknown& chesshallprops_obtain_commodity_params::unknown(){
return _unknown;
}
inline size_t chesshallprops_obtain_commodity_params::unknown_size() const{
return _unknown.size();
}
inline chesshallprops_obtain_commodity_result_params* chesshallprops_obtain_commodity_result_params::create(bsl::mempool* pool){
if(!pool){
throw bsl::NullPointerException() << BSL_EARG;
}
chesshallprops_obtain_commodity_result_params* tmp=(chesshallprops_obtain_commodity_result_params*)pool->malloc(sizeof(*tmp));
if(!tmp){
throw bsl::BadAllocException() << BSL_EARG;
}
new(tmp)chesshallprops_obtain_commodity_result_params(pool);
return tmp;
}
inline void chesshallprops_obtain_commodity_result_params::destroy(chesshallprops_obtain_commodity_result_params* v){
if(v){
bsl::mempool *pool=v->_pool;
v->~chesshallprops_obtain_commodity_result_params();
pool->free(v,sizeof(*v));
}
}
inline chesshallprops_obtain_commodity_result_params::chesshallprops_obtain_commodity_result_params(bsl::mempool *pool): 
::idl::McpackBean(pool),
_pool(pool)
,_m_success(0)
,_unknown(_pool){
memset(_flag,0,sizeof(_flag));
}
inline chesshallprops_obtain_commodity_result_params::chesshallprops_obtain_commodity_result_params(const chesshallprops_obtain_commodity_result_params& v): 
::idl::McpackBean(v._pool),
_pool(v._pool)
,_m_success(0)
,_unknown(_pool){
assign(v);
}
inline chesshallprops_obtain_commodity_result_params::~chesshallprops_obtain_commodity_result_params(){
}
inline chesshallprops_obtain_commodity_result_params& chesshallprops_obtain_commodity_result_params::operator=(const chesshallprops_obtain_commodity_result_params& v){
assign(v);
return *this;
}
inline void chesshallprops_obtain_commodity_result_params::clear(){
// _m_success=0;
memset(_flag,0,sizeof(_flag));
_unknown.clear(); 
::idl::McpackBean::clear();
}
// @@CF(constraint functions).
inline void chesshallprops_obtain_commodity_result_params::check() const{
// @@bool success=out();
if(!(_flag[0 >> 3] & (1 << (0 & 0x7)))){
throw bsl::KeyNotFoundException() << BSL_EARG << "key '" << "success" << "' not found";
}
{
__attribute__((unused)) bool item=_m_success;

}

::idl::McpackBean::check();
}
inline void chesshallprops_obtain_commodity_result_params::detach(){
_unknown.clear();

::idl::McpackBean::detach();
}
inline chesshallprops_obtain_commodity_result_params& chesshallprops_obtain_commodity_result_params::assign(const chesshallprops_obtain_commodity_result_params& v){
_m_success=v._m_success;
_unknown=v._unknown;
bsl::xmemcpy(_flag,v._flag,sizeof(v._flag));
return *this;
}
inline bool chesshallprops_obtain_commodity_result_params::has_success() const{
return (_flag[0 >> 3] & (1 << (0 & 0x7)));
}
inline bool chesshallprops_obtain_commodity_result_params::success() const{
return _m_success;
}
inline chesshallprops_obtain_commodity_result_params& chesshallprops_obtain_commodity_result_params::set_success(bool v){
_flag[0 >> 3] |= (1 << (0 & 0x7));
 _m_success=v;
return *this;
}
inline void chesshallprops_obtain_commodity_result_params::clear_success(){
_flag[0 >> 3] &= ~(1 << (0 & 0x7));
 _m_success=0;
}
inline ::idl::Unknown* chesshallprops_obtain_commodity_result_params::mutable_unknown(){
return &_unknown;
}
inline const ::idl::Unknown& chesshallprops_obtain_commodity_result_params::unknown() const{
return _unknown;
}
inline ::idl::Unknown& chesshallprops_obtain_commodity_result_params::unknown(){
return _unknown;
}
inline size_t chesshallprops_obtain_commodity_result_params::unknown_size() const{
return _unknown.size();
}
inline chesshallprops_obtain_commodity_response* chesshallprops_obtain_commodity_response::create(bsl::mempool* pool){
if(!pool){
throw bsl::NullPointerException() << BSL_EARG;
}
chesshallprops_obtain_commodity_response* tmp=(chesshallprops_obtain_commodity_response*)pool->malloc(sizeof(*tmp));
if(!tmp){
throw bsl::BadAllocException() << BSL_EARG;
}
new(tmp)chesshallprops_obtain_commodity_response(pool);
return tmp;
}
inline void chesshallprops_obtain_commodity_response::destroy(chesshallprops_obtain_commodity_response* v){
if(v){
bsl::mempool *pool=v->_pool;
v->~chesshallprops_obtain_commodity_response();
pool->free(v,sizeof(*v));
}
}
inline chesshallprops_obtain_commodity_response::chesshallprops_obtain_commodity_response(bsl::mempool *pool): 
::idl::McpackBean(pool),
_pool(pool)
,_m_result_params(0)
,_unknown(_pool){
_m_result_params=::idl::chesshallprops_obtain_commodity_result_params::create(_pool);
memset(_flag,0,sizeof(_flag));
}
inline chesshallprops_obtain_commodity_response::chesshallprops_obtain_commodity_response(const chesshallprops_obtain_commodity_response& v): 
::idl::McpackBean(v._pool),
_pool(v._pool)
,_m_result_params(0)
,_unknown(_pool){
_m_result_params=::idl::chesshallprops_obtain_commodity_result_params::create(_pool);
assign(v);
}
inline chesshallprops_obtain_commodity_response::~chesshallprops_obtain_commodity_response(){
if(_m_result_params){
::idl::chesshallprops_obtain_commodity_result_params::destroy(_m_result_params);
_m_result_params=0;
}
}
inline chesshallprops_obtain_commodity_response& chesshallprops_obtain_commodity_response::operator=(const chesshallprops_obtain_commodity_response& v){
assign(v);
return *this;
}
inline void chesshallprops_obtain_commodity_response::clear(){
if(_m_result_params){
_m_result_params->clear();
}
memset(_flag,0,sizeof(_flag));
_unknown.clear(); 
::idl::McpackBean::clear();
}
// @@CF(constraint functions).
inline void chesshallprops_obtain_commodity_response::check() const{
// @@chesshallprops_obtain_commodity_result_params result_params;
if(!(_flag[0 >> 3] & (1 << (0 & 0x7)))){
throw bsl::KeyNotFoundException() << BSL_EARG << "key '" << "result_params" << "' not found";
}
{
::idl::chesshallprops_obtain_commodity_result_params& item=const_cast< ::idl::chesshallprops_obtain_commodity_result_params& >(*_m_result_params);
item.check();

}

::idl::McpackBean::check();
}
inline void chesshallprops_obtain_commodity_response::detach(){
if(_m_result_params){
_m_result_params->detach();
}
_unknown.clear();

::idl::McpackBean::detach();
}
inline chesshallprops_obtain_commodity_response& chesshallprops_obtain_commodity_response::assign(const chesshallprops_obtain_commodity_response& v){
if(!v._m_result_params){
if(_m_result_params){
::idl::chesshallprops_obtain_commodity_result_params::destroy(_m_result_params);
}
_m_result_params=0;
}else{
if(!_m_result_params){
_m_result_params=::idl::chesshallprops_obtain_commodity_result_params::create(_pool);
}
*_m_result_params=*v._m_result_params;
}
_unknown=v._unknown;
bsl::xmemcpy(_flag,v._flag,sizeof(v._flag));
return *this;
}
inline bool chesshallprops_obtain_commodity_response::has_result_params() const{
return (_flag[0 >> 3] & (1 << (0 & 0x7)));
}
inline const ::idl::chesshallprops_obtain_commodity_result_params& chesshallprops_obtain_commodity_response::result_params() const{
 return *_m_result_params;
}
inline ::idl::chesshallprops_obtain_commodity_result_params* chesshallprops_obtain_commodity_response::mutable_result_params(){
_flag[0 >> 3] |= (1 << (0 & 0x7));
 if(!_m_result_params){
_m_result_params=::idl::chesshallprops_obtain_commodity_result_params::create(_pool);
}
return _m_result_params;
}
inline ::idl::chesshallprops_obtain_commodity_result_params* chesshallprops_obtain_commodity_response::m_result_params(){
_flag[0 >> 3] |= (1 << (0 & 0x7));
 if(!_m_result_params){
_m_result_params=::idl::chesshallprops_obtain_commodity_result_params::create(_pool);
}
return _m_result_params;
}
inline void chesshallprops_obtain_commodity_response::clear_result_params(){
if(_m_result_params){
_flag[0 >> 3] &= ~(1 << (0 & 0x7));
 _m_result_params->clear();
}
}
inline ::idl::Unknown* chesshallprops_obtain_commodity_response::mutable_unknown(){
return &_unknown;
}
inline const ::idl::Unknown& chesshallprops_obtain_commodity_response::unknown() const{
return _unknown;
}
inline ::idl::Unknown& chesshallprops_obtain_commodity_response::unknown(){
return _unknown;
}
inline size_t chesshallprops_obtain_commodity_response::unknown_size() const{
return _unknown.size();
}
inline chesshallprops_obtain_exchange_code_params* chesshallprops_obtain_exchange_code_params::create(bsl::mempool* pool){
if(!pool){
throw bsl::NullPointerException() << BSL_EARG;
}
chesshallprops_obtain_exchange_code_params* tmp=(chesshallprops_obtain_exchange_code_params*)pool->malloc(sizeof(*tmp));
if(!tmp){
throw bsl::BadAllocException() << BSL_EARG;
}
new(tmp)chesshallprops_obtain_exchange_code_params(pool);
return tmp;
}
inline void chesshallprops_obtain_exchange_code_params::destroy(chesshallprops_obtain_exchange_code_params* v){
if(v){
bsl::mempool *pool=v->_pool;
v->~chesshallprops_obtain_exchange_code_params();
pool->free(v,sizeof(*v));
}
}
inline chesshallprops_obtain_exchange_code_params::chesshallprops_obtain_exchange_code_params(bsl::mempool *pool): 
::idl::McpackBean(pool),
_pool(pool)
,_m_user_id(0)
,_m_number(_pool)
,_m_commodity_id(0)
,_m_expired_time(_pool)
,_m_expired_type(0)
,_m_game_id(_pool)
,_unknown(_pool){
memset(_flag,0,sizeof(_flag));
}
inline chesshallprops_obtain_exchange_code_params::chesshallprops_obtain_exchange_code_params(const chesshallprops_obtain_exchange_code_params& v): 
::idl::McpackBean(v._pool),
_pool(v._pool)
,_m_user_id(0)
,_m_number(_pool)
,_m_commodity_id(0)
,_m_expired_time(_pool)
,_m_expired_type(0)
,_m_game_id(_pool)
,_unknown(_pool){
assign(v);
}
inline chesshallprops_obtain_exchange_code_params::~chesshallprops_obtain_exchange_code_params(){
}
inline chesshallprops_obtain_exchange_code_params& chesshallprops_obtain_exchange_code_params::operator=(const chesshallprops_obtain_exchange_code_params& v){
assign(v);
return *this;
}
inline void chesshallprops_obtain_exchange_code_params::clear(){
// _m_user_id=0;
_m_number.clear();
// _m_commodity_id=0;
_m_expired_time.clear();
// _m_expired_type=0;
_m_game_id.clear();
memset(_flag,0,sizeof(_flag));
_unknown.clear(); 
::idl::McpackBean::clear();
}
// @@CF(constraint functions).
inline void chesshallprops_obtain_exchange_code_params::check() const{
// @@int32_t user_id=in();
if(!(_flag[0 >> 3] & (1 << (0 & 0x7)))){
throw bsl::KeyNotFoundException() << BSL_EARG << "key '" << "user_id" << "' not found";
}
{
__attribute__((unused)) int32_t item=_m_user_id;

}
// @@string number=in();
if(!(_flag[1 >> 3] & (1 << (1 & 0x7)))){
throw bsl::KeyNotFoundException() << BSL_EARG << "key '" << "number" << "' not found";
}
{
__attribute__((unused)) const char* item=_m_number.get();

}
// @@int32_t commodity_id=in();
if(!(_flag[2 >> 3] & (1 << (2 & 0x7)))){
throw bsl::KeyNotFoundException() << BSL_EARG << "key '" << "commodity_id" << "' not found";
}
{
__attribute__((unused)) int32_t item=_m_commodity_id;

}
// @@string expired_time=in();
if(!(_flag[3 >> 3] & (1 << (3 & 0x7)))){
throw bsl::KeyNotFoundException() << BSL_EARG << "key '" << "expired_time" << "' not found";
}
{
__attribute__((unused)) const char* item=_m_expired_time.get();

}
// @@int32_t expired_type=in();
if(!(_flag[4 >> 3] & (1 << (4 & 0x7)))){
throw bsl::KeyNotFoundException() << BSL_EARG << "key '" << "expired_type" << "' not found";
}
{
__attribute__((unused)) int32_t item=_m_expired_type;

}
// @@string game_id=in();
if(!(_flag[5 >> 3] & (1 << (5 & 0x7)))){
throw bsl::KeyNotFoundException() << BSL_EARG << "key '" << "game_id" << "' not found";
}
{
__attribute__((unused)) const char* item=_m_game_id.get();

}

::idl::McpackBean::check();
}
inline void chesshallprops_obtain_exchange_code_params::detach(){
_m_number.detach();
_m_expired_time.detach();
_m_game_id.detach();
_unknown.clear();

::idl::McpackBean::detach();
}
inline chesshallprops_obtain_exchange_code_params& chesshallprops_obtain_exchange_code_params::assign(const chesshallprops_obtain_exchange_code_params& v){
_m_user_id=v._m_user_id;
_m_number=v._m_number;
_m_commodity_id=v._m_commodity_id;
_m_expired_time=v._m_expired_time;
_m_expired_type=v._m_expired_type;
_m_game_id=v._m_game_id;
_unknown=v._unknown;
bsl::xmemcpy(_flag,v._flag,sizeof(v._flag));
return *this;
}
inline bool chesshallprops_obtain_exchange_code_params::has_user_id() const{
return (_flag[0 >> 3] & (1 << (0 & 0x7)));
}
inline int32_t chesshallprops_obtain_exchange_code_params::user_id() const{
return _m_user_id;
}
inline chesshallprops_obtain_exchange_code_params& chesshallprops_obtain_exchange_code_params::set_user_id(int32_t v){
_flag[0 >> 3] |= (1 << (0 & 0x7));
 _m_user_id=v;
return *this;
}
inline void chesshallprops_obtain_exchange_code_params::clear_user_id(){
_flag[0 >> 3] &= ~(1 << (0 & 0x7));
 _m_user_id=0;
}
inline bool chesshallprops_obtain_exchange_code_params::has_number() const{
return (_flag[1 >> 3] & (1 << (1 & 0x7)));
}
inline const char* chesshallprops_obtain_exchange_code_params::number(unsigned int* size) const{
return _m_number.get(size);
}
inline chesshallprops_obtain_exchange_code_params& chesshallprops_obtain_exchange_code_params::set_number(const char* s,ssize_t size){
_flag[1 >> 3] |= (1 << (1 & 0x7));
 _m_number.set(s,(size==-1)?strlen(s):(size_t)size);
return *this;
}
inline chesshallprops_obtain_exchange_code_params& chesshallprops_obtain_exchange_code_params::set_ptr_number(const char* s,ssize_t size){
_flag[1 >> 3] |= (1 << (1 & 0x7));
 _m_number.set_ptr(s,(size==-1)?strlen(s):(size_t)size);
return *this;
}
inline void chesshallprops_obtain_exchange_code_params::clear_number(){
_flag[1 >> 3] &= ~(1 << (1 & 0x7));
 _m_number.clear();
}
inline bool chesshallprops_obtain_exchange_code_params::has_commodity_id() const{
return (_flag[2 >> 3] & (1 << (2 & 0x7)));
}
inline int32_t chesshallprops_obtain_exchange_code_params::commodity_id() const{
return _m_commodity_id;
}
inline chesshallprops_obtain_exchange_code_params& chesshallprops_obtain_exchange_code_params::set_commodity_id(int32_t v){
_flag[2 >> 3] |= (1 << (2 & 0x7));
 _m_commodity_id=v;
return *this;
}
inline void chesshallprops_obtain_exchange_code_params::clear_commodity_id(){
_flag[2 >> 3] &= ~(1 << (2 & 0x7));
 _m_commodity_id=0;
}
inline bool chesshallprops_obtain_exchange_code_params::has_expired_time() const{
return (_flag[3 >> 3] & (1 << (3 & 0x7)));
}
inline const char* chesshallprops_obtain_exchange_code_params::expired_time(unsigned int* size) const{
return _m_expired_time.get(size);
}
inline chesshallprops_obtain_exchange_code_params& chesshallprops_obtain_exchange_code_params::set_expired_time(const char* s,ssize_t size){
_flag[3 >> 3] |= (1 << (3 & 0x7));
 _m_expired_time.set(s,(size==-1)?strlen(s):(size_t)size);
return *this;
}
inline chesshallprops_obtain_exchange_code_params& chesshallprops_obtain_exchange_code_params::set_ptr_expired_time(const char* s,ssize_t size){
_flag[3 >> 3] |= (1 << (3 & 0x7));
 _m_expired_time.set_ptr(s,(size==-1)?strlen(s):(size_t)size);
return *this;
}
inline void chesshallprops_obtain_exchange_code_params::clear_expired_time(){
_flag[3 >> 3] &= ~(1 << (3 & 0x7));
 _m_expired_time.clear();
}
inline bool chesshallprops_obtain_exchange_code_params::has_expired_type() const{
return (_flag[4 >> 3] & (1 << (4 & 0x7)));
}
inline int32_t chesshallprops_obtain_exchange_code_params::expired_type() const{
return _m_expired_type;
}
inline chesshallprops_obtain_exchange_code_params& chesshallprops_obtain_exchange_code_params::set_expired_type(int32_t v){
_flag[4 >> 3] |= (1 << (4 & 0x7));
 _m_expired_type=v;
return *this;
}
inline void chesshallprops_obtain_exchange_code_params::clear_expired_type(){
_flag[4 >> 3] &= ~(1 << (4 & 0x7));
 _m_expired_type=0;
}
inline bool chesshallprops_obtain_exchange_code_params::has_game_id() const{
return (_flag[5 >> 3] & (1 << (5 & 0x7)));
}
inline const char* chesshallprops_obtain_exchange_code_params::game_id(unsigned int* size) const{
return _m_game_id.get(size);
}
inline chesshallprops_obtain_exchange_code_params& chesshallprops_obtain_exchange_code_params::set_game_id(const char* s,ssize_t size){
_flag[5 >> 3] |= (1 << (5 & 0x7));
 _m_game_id.set(s,(size==-1)?strlen(s):(size_t)size);
return *this;
}
inline chesshallprops_obtain_exchange_code_params& chesshallprops_obtain_exchange_code_params::set_ptr_game_id(const char* s,ssize_t size){
_flag[5 >> 3] |= (1 << (5 & 0x7));
 _m_game_id.set_ptr(s,(size==-1)?strlen(s):(size_t)size);
return *this;
}
inline void chesshallprops_obtain_exchange_code_params::clear_game_id(){
_flag[5 >> 3] &= ~(1 << (5 & 0x7));
 _m_game_id.clear();
}
inline ::idl::Unknown* chesshallprops_obtain_exchange_code_params::mutable_unknown(){
return &_unknown;
}
inline const ::idl::Unknown& chesshallprops_obtain_exchange_code_params::unknown() const{
return _unknown;
}
inline ::idl::Unknown& chesshallprops_obtain_exchange_code_params::unknown(){
return _unknown;
}
inline size_t chesshallprops_obtain_exchange_code_params::unknown_size() const{
return _unknown.size();
}
inline chesshallprops_obtain_exchange_code_result_params* chesshallprops_obtain_exchange_code_result_params::create(bsl::mempool* pool){
if(!pool){
throw bsl::NullPointerException() << BSL_EARG;
}
chesshallprops_obtain_exchange_code_result_params* tmp=(chesshallprops_obtain_exchange_code_result_params*)pool->malloc(sizeof(*tmp));
if(!tmp){
throw bsl::BadAllocException() << BSL_EARG;
}
new(tmp)chesshallprops_obtain_exchange_code_result_params(pool);
return tmp;
}
inline void chesshallprops_obtain_exchange_code_result_params::destroy(chesshallprops_obtain_exchange_code_result_params* v){
if(v){
bsl::mempool *pool=v->_pool;
v->~chesshallprops_obtain_exchange_code_result_params();
pool->free(v,sizeof(*v));
}
}
inline chesshallprops_obtain_exchange_code_result_params::chesshallprops_obtain_exchange_code_result_params(bsl::mempool *pool): 
::idl::McpackBean(pool),
_pool(pool)
,_m_success(0)
,_unknown(_pool){
memset(_flag,0,sizeof(_flag));
}
inline chesshallprops_obtain_exchange_code_result_params::chesshallprops_obtain_exchange_code_result_params(const chesshallprops_obtain_exchange_code_result_params& v): 
::idl::McpackBean(v._pool),
_pool(v._pool)
,_m_success(0)
,_unknown(_pool){
assign(v);
}
inline chesshallprops_obtain_exchange_code_result_params::~chesshallprops_obtain_exchange_code_result_params(){
}
inline chesshallprops_obtain_exchange_code_result_params& chesshallprops_obtain_exchange_code_result_params::operator=(const chesshallprops_obtain_exchange_code_result_params& v){
assign(v);
return *this;
}
inline void chesshallprops_obtain_exchange_code_result_params::clear(){
// _m_success=0;
memset(_flag,0,sizeof(_flag));
_unknown.clear(); 
::idl::McpackBean::clear();
}
// @@CF(constraint functions).
inline void chesshallprops_obtain_exchange_code_result_params::check() const{
// @@bool success=out();
if(!(_flag[0 >> 3] & (1 << (0 & 0x7)))){
throw bsl::KeyNotFoundException() << BSL_EARG << "key '" << "success" << "' not found";
}
{
__attribute__((unused)) bool item=_m_success;

}

::idl::McpackBean::check();
}
inline void chesshallprops_obtain_exchange_code_result_params::detach(){
_unknown.clear();

::idl::McpackBean::detach();
}
inline chesshallprops_obtain_exchange_code_result_params& chesshallprops_obtain_exchange_code_result_params::assign(const chesshallprops_obtain_exchange_code_result_params& v){
_m_success=v._m_success;
_unknown=v._unknown;
bsl::xmemcpy(_flag,v._flag,sizeof(v._flag));
return *this;
}
inline bool chesshallprops_obtain_exchange_code_result_params::has_success() const{
return (_flag[0 >> 3] & (1 << (0 & 0x7)));
}
inline bool chesshallprops_obtain_exchange_code_result_params::success() const{
return _m_success;
}
inline chesshallprops_obtain_exchange_code_result_params& chesshallprops_obtain_exchange_code_result_params::set_success(bool v){
_flag[0 >> 3] |= (1 << (0 & 0x7));
 _m_success=v;
return *this;
}
inline void chesshallprops_obtain_exchange_code_result_params::clear_success(){
_flag[0 >> 3] &= ~(1 << (0 & 0x7));
 _m_success=0;
}
inline ::idl::Unknown* chesshallprops_obtain_exchange_code_result_params::mutable_unknown(){
return &_unknown;
}
inline const ::idl::Unknown& chesshallprops_obtain_exchange_code_result_params::unknown() const{
return _unknown;
}
inline ::idl::Unknown& chesshallprops_obtain_exchange_code_result_params::unknown(){
return _unknown;
}
inline size_t chesshallprops_obtain_exchange_code_result_params::unknown_size() const{
return _unknown.size();
}
inline chesshallprops_obtain_exchange_code_response* chesshallprops_obtain_exchange_code_response::create(bsl::mempool* pool){
if(!pool){
throw bsl::NullPointerException() << BSL_EARG;
}
chesshallprops_obtain_exchange_code_response* tmp=(chesshallprops_obtain_exchange_code_response*)pool->malloc(sizeof(*tmp));
if(!tmp){
throw bsl::BadAllocException() << BSL_EARG;
}
new(tmp)chesshallprops_obtain_exchange_code_response(pool);
return tmp;
}
inline void chesshallprops_obtain_exchange_code_response::destroy(chesshallprops_obtain_exchange_code_response* v){
if(v){
bsl::mempool *pool=v->_pool;
v->~chesshallprops_obtain_exchange_code_response();
pool->free(v,sizeof(*v));
}
}
inline chesshallprops_obtain_exchange_code_response::chesshallprops_obtain_exchange_code_response(bsl::mempool *pool): 
::idl::McpackBean(pool),
_pool(pool)
,_m_result_params(0)
,_unknown(_pool){
_m_result_params=::idl::chesshallprops_obtain_exchange_code_result_params::create(_pool);
memset(_flag,0,sizeof(_flag));
}
inline chesshallprops_obtain_exchange_code_response::chesshallprops_obtain_exchange_code_response(const chesshallprops_obtain_exchange_code_response& v): 
::idl::McpackBean(v._pool),
_pool(v._pool)
,_m_result_params(0)
,_unknown(_pool){
_m_result_params=::idl::chesshallprops_obtain_exchange_code_result_params::create(_pool);
assign(v);
}
inline chesshallprops_obtain_exchange_code_response::~chesshallprops_obtain_exchange_code_response(){
if(_m_result_params){
::idl::chesshallprops_obtain_exchange_code_result_params::destroy(_m_result_params);
_m_result_params=0;
}
}
inline chesshallprops_obtain_exchange_code_response& chesshallprops_obtain_exchange_code_response::operator=(const chesshallprops_obtain_exchange_code_response& v){
assign(v);
return *this;
}
inline void chesshallprops_obtain_exchange_code_response::clear(){
if(_m_result_params){
_m_result_params->clear();
}
memset(_flag,0,sizeof(_flag));
_unknown.clear(); 
::idl::McpackBean::clear();
}
// @@CF(constraint functions).
inline void chesshallprops_obtain_exchange_code_response::check() const{
// @@chesshallprops_obtain_exchange_code_result_params result_params;
if(!(_flag[0 >> 3] & (1 << (0 & 0x7)))){
throw bsl::KeyNotFoundException() << BSL_EARG << "key '" << "result_params" << "' not found";
}
{
::idl::chesshallprops_obtain_exchange_code_result_params& item=const_cast< ::idl::chesshallprops_obtain_exchange_code_result_params& >(*_m_result_params);
item.check();

}

::idl::McpackBean::check();
}
inline void chesshallprops_obtain_exchange_code_response::detach(){
if(_m_result_params){
_m_result_params->detach();
}
_unknown.clear();

::idl::McpackBean::detach();
}
inline chesshallprops_obtain_exchange_code_response& chesshallprops_obtain_exchange_code_response::assign(const chesshallprops_obtain_exchange_code_response& v){
if(!v._m_result_params){
if(_m_result_params){
::idl::chesshallprops_obtain_exchange_code_result_params::destroy(_m_result_params);
}
_m_result_params=0;
}else{
if(!_m_result_params){
_m_result_params=::idl::chesshallprops_obtain_exchange_code_result_params::create(_pool);
}
*_m_result_params=*v._m_result_params;
}
_unknown=v._unknown;
bsl::xmemcpy(_flag,v._flag,sizeof(v._flag));
return *this;
}
inline bool chesshallprops_obtain_exchange_code_response::has_result_params() const{
return (_flag[0 >> 3] & (1 << (0 & 0x7)));
}
inline const ::idl::chesshallprops_obtain_exchange_code_result_params& chesshallprops_obtain_exchange_code_response::result_params() const{
 return *_m_result_params;
}
inline ::idl::chesshallprops_obtain_exchange_code_result_params* chesshallprops_obtain_exchange_code_response::mutable_result_params(){
_flag[0 >> 3] |= (1 << (0 & 0x7));
 if(!_m_result_params){
_m_result_params=::idl::chesshallprops_obtain_exchange_code_result_params::create(_pool);
}
return _m_result_params;
}
inline ::idl::chesshallprops_obtain_exchange_code_result_params* chesshallprops_obtain_exchange_code_response::m_result_params(){
_flag[0 >> 3] |= (1 << (0 & 0x7));
 if(!_m_result_params){
_m_result_params=::idl::chesshallprops_obtain_exchange_code_result_params::create(_pool);
}
return _m_result_params;
}
inline void chesshallprops_obtain_exchange_code_response::clear_result_params(){
if(_m_result_params){
_flag[0 >> 3] &= ~(1 << (0 & 0x7));
 _m_result_params->clear();
}
}
inline ::idl::Unknown* chesshallprops_obtain_exchange_code_response::mutable_unknown(){
return &_unknown;
}
inline const ::idl::Unknown& chesshallprops_obtain_exchange_code_response::unknown() const{
return _unknown;
}
inline ::idl::Unknown& chesshallprops_obtain_exchange_code_response::unknown(){
return _unknown;
}
inline size_t chesshallprops_obtain_exchange_code_response::unknown_size() const{
return _unknown.size();
}
inline chesshallprops_use_exchange_code_params* chesshallprops_use_exchange_code_params::create(bsl::mempool* pool){
if(!pool){
throw bsl::NullPointerException() << BSL_EARG;
}
chesshallprops_use_exchange_code_params* tmp=(chesshallprops_use_exchange_code_params*)pool->malloc(sizeof(*tmp));
if(!tmp){
throw bsl::BadAllocException() << BSL_EARG;
}
new(tmp)chesshallprops_use_exchange_code_params(pool);
return tmp;
}
inline void chesshallprops_use_exchange_code_params::destroy(chesshallprops_use_exchange_code_params* v){
if(v){
bsl::mempool *pool=v->_pool;
v->~chesshallprops_use_exchange_code_params();
pool->free(v,sizeof(*v));
}
}
inline chesshallprops_use_exchange_code_params::chesshallprops_use_exchange_code_params(bsl::mempool *pool): 
::idl::McpackBean(pool),
_pool(pool)
,_m_userId(0)
,_m_gameId(_pool)
,_m_exchangeCodeId(_pool)
,_unknown(_pool){
memset(_flag,0,sizeof(_flag));
}
inline chesshallprops_use_exchange_code_params::chesshallprops_use_exchange_code_params(const chesshallprops_use_exchange_code_params& v): 
::idl::McpackBean(v._pool),
_pool(v._pool)
,_m_userId(0)
,_m_gameId(_pool)
,_m_exchangeCodeId(_pool)
,_unknown(_pool){
assign(v);
}
inline chesshallprops_use_exchange_code_params::~chesshallprops_use_exchange_code_params(){
}
inline chesshallprops_use_exchange_code_params& chesshallprops_use_exchange_code_params::operator=(const chesshallprops_use_exchange_code_params& v){
assign(v);
return *this;
}
inline void chesshallprops_use_exchange_code_params::clear(){
// _m_userId=0;
_m_gameId.clear();
_m_exchangeCodeId.clear();
memset(_flag,0,sizeof(_flag));
_unknown.clear(); 
::idl::McpackBean::clear();
}
// @@CF(constraint functions).
inline void chesshallprops_use_exchange_code_params::check() const{
// @@int32_t userId=in();
if(!(_flag[0 >> 3] & (1 << (0 & 0x7)))){
throw bsl::KeyNotFoundException() << BSL_EARG << "key '" << "userId" << "' not found";
}
{
__attribute__((unused)) int32_t item=_m_userId;

}
// @@string gameId=in();
if(!(_flag[1 >> 3] & (1 << (1 & 0x7)))){
throw bsl::KeyNotFoundException() << BSL_EARG << "key '" << "gameId" << "' not found";
}
{
__attribute__((unused)) const char* item=_m_gameId.get();

}
// @@string exchangeCodeId=in();
if(!(_flag[2 >> 3] & (1 << (2 & 0x7)))){
throw bsl::KeyNotFoundException() << BSL_EARG << "key '" << "exchangeCodeId" << "' not found";
}
{
__attribute__((unused)) const char* item=_m_exchangeCodeId.get();

}

::idl::McpackBean::check();
}
inline void chesshallprops_use_exchange_code_params::detach(){
_m_gameId.detach();
_m_exchangeCodeId.detach();
_unknown.clear();

::idl::McpackBean::detach();
}
inline chesshallprops_use_exchange_code_params& chesshallprops_use_exchange_code_params::assign(const chesshallprops_use_exchange_code_params& v){
_m_userId=v._m_userId;
_m_gameId=v._m_gameId;
_m_exchangeCodeId=v._m_exchangeCodeId;
_unknown=v._unknown;
bsl::xmemcpy(_flag,v._flag,sizeof(v._flag));
return *this;
}
inline bool chesshallprops_use_exchange_code_params::has_userId() const{
return (_flag[0 >> 3] & (1 << (0 & 0x7)));
}
inline int32_t chesshallprops_use_exchange_code_params::userId() const{
return _m_userId;
}
inline chesshallprops_use_exchange_code_params& chesshallprops_use_exchange_code_params::set_userId(int32_t v){
_flag[0 >> 3] |= (1 << (0 & 0x7));
 _m_userId=v;
return *this;
}
inline void chesshallprops_use_exchange_code_params::clear_userId(){
_flag[0 >> 3] &= ~(1 << (0 & 0x7));
 _m_userId=0;
}
inline bool chesshallprops_use_exchange_code_params::has_gameId() const{
return (_flag[1 >> 3] & (1 << (1 & 0x7)));
}
inline const char* chesshallprops_use_exchange_code_params::gameId(unsigned int* size) const{
return _m_gameId.get(size);
}
inline chesshallprops_use_exchange_code_params& chesshallprops_use_exchange_code_params::set_gameId(const char* s,ssize_t size){
_flag[1 >> 3] |= (1 << (1 & 0x7));
 _m_gameId.set(s,(size==-1)?strlen(s):(size_t)size);
return *this;
}
inline chesshallprops_use_exchange_code_params& chesshallprops_use_exchange_code_params::set_ptr_gameId(const char* s,ssize_t size){
_flag[1 >> 3] |= (1 << (1 & 0x7));
 _m_gameId.set_ptr(s,(size==-1)?strlen(s):(size_t)size);
return *this;
}
inline void chesshallprops_use_exchange_code_params::clear_gameId(){
_flag[1 >> 3] &= ~(1 << (1 & 0x7));
 _m_gameId.clear();
}
inline bool chesshallprops_use_exchange_code_params::has_exchangeCodeId() const{
return (_flag[2 >> 3] & (1 << (2 & 0x7)));
}
inline const char* chesshallprops_use_exchange_code_params::exchangeCodeId(unsigned int* size) const{
return _m_exchangeCodeId.get(size);
}
inline chesshallprops_use_exchange_code_params& chesshallprops_use_exchange_code_params::set_exchangeCodeId(const char* s,ssize_t size){
_flag[2 >> 3] |= (1 << (2 & 0x7));
 _m_exchangeCodeId.set(s,(size==-1)?strlen(s):(size_t)size);
return *this;
}
inline chesshallprops_use_exchange_code_params& chesshallprops_use_exchange_code_params::set_ptr_exchangeCodeId(const char* s,ssize_t size){
_flag[2 >> 3] |= (1 << (2 & 0x7));
 _m_exchangeCodeId.set_ptr(s,(size==-1)?strlen(s):(size_t)size);
return *this;
}
inline void chesshallprops_use_exchange_code_params::clear_exchangeCodeId(){
_flag[2 >> 3] &= ~(1 << (2 & 0x7));
 _m_exchangeCodeId.clear();
}
inline ::idl::Unknown* chesshallprops_use_exchange_code_params::mutable_unknown(){
return &_unknown;
}
inline const ::idl::Unknown& chesshallprops_use_exchange_code_params::unknown() const{
return _unknown;
}
inline ::idl::Unknown& chesshallprops_use_exchange_code_params::unknown(){
return _unknown;
}
inline size_t chesshallprops_use_exchange_code_params::unknown_size() const{
return _unknown.size();
}
inline chesshallprops_use_exchange_code_result_params* chesshallprops_use_exchange_code_result_params::create(bsl::mempool* pool){
if(!pool){
throw bsl::NullPointerException() << BSL_EARG;
}
chesshallprops_use_exchange_code_result_params* tmp=(chesshallprops_use_exchange_code_result_params*)pool->malloc(sizeof(*tmp));
if(!tmp){
throw bsl::BadAllocException() << BSL_EARG;
}
new(tmp)chesshallprops_use_exchange_code_result_params(pool);
return tmp;
}
inline void chesshallprops_use_exchange_code_result_params::destroy(chesshallprops_use_exchange_code_result_params* v){
if(v){
bsl::mempool *pool=v->_pool;
v->~chesshallprops_use_exchange_code_result_params();
pool->free(v,sizeof(*v));
}
}
inline chesshallprops_use_exchange_code_result_params::chesshallprops_use_exchange_code_result_params(bsl::mempool *pool): 
::idl::McpackBean(pool),
_pool(pool)
,_m_resultCP(0)
,_unknown(_pool){
_m_resultCP=::idl::result_cp::create(_pool);
memset(_flag,0,sizeof(_flag));
}
inline chesshallprops_use_exchange_code_result_params::chesshallprops_use_exchange_code_result_params(const chesshallprops_use_exchange_code_result_params& v): 
::idl::McpackBean(v._pool),
_pool(v._pool)
,_m_resultCP(0)
,_unknown(_pool){
_m_resultCP=::idl::result_cp::create(_pool);
assign(v);
}
inline chesshallprops_use_exchange_code_result_params::~chesshallprops_use_exchange_code_result_params(){
if(_m_resultCP){
::idl::result_cp::destroy(_m_resultCP);
_m_resultCP=0;
}
}
inline chesshallprops_use_exchange_code_result_params& chesshallprops_use_exchange_code_result_params::operator=(const chesshallprops_use_exchange_code_result_params& v){
assign(v);
return *this;
}
inline void chesshallprops_use_exchange_code_result_params::clear(){
if(_m_resultCP){
_m_resultCP->clear();
}
memset(_flag,0,sizeof(_flag));
_unknown.clear(); 
::idl::McpackBean::clear();
}
// @@CF(constraint functions).
inline void chesshallprops_use_exchange_code_result_params::check() const{
// @@result_cp resultCP=out();
if(!(_flag[0 >> 3] & (1 << (0 & 0x7)))){
throw bsl::KeyNotFoundException() << BSL_EARG << "key '" << "resultCP" << "' not found";
}
{
::idl::result_cp& item=const_cast< ::idl::result_cp& >(*_m_resultCP);
item.check();

}

::idl::McpackBean::check();
}
inline void chesshallprops_use_exchange_code_result_params::detach(){
if(_m_resultCP){
_m_resultCP->detach();
}
_unknown.clear();

::idl::McpackBean::detach();
}
inline chesshallprops_use_exchange_code_result_params& chesshallprops_use_exchange_code_result_params::assign(const chesshallprops_use_exchange_code_result_params& v){
if(!v._m_resultCP){
if(_m_resultCP){
::idl::result_cp::destroy(_m_resultCP);
}
_m_resultCP=0;
}else{
if(!_m_resultCP){
_m_resultCP=::idl::result_cp::create(_pool);
}
*_m_resultCP=*v._m_resultCP;
}
_unknown=v._unknown;
bsl::xmemcpy(_flag,v._flag,sizeof(v._flag));
return *this;
}
inline bool chesshallprops_use_exchange_code_result_params::has_resultCP() const{
return (_flag[0 >> 3] & (1 << (0 & 0x7)));
}
inline const ::idl::result_cp& chesshallprops_use_exchange_code_result_params::resultCP() const{
 return *_m_resultCP;
}
inline ::idl::result_cp* chesshallprops_use_exchange_code_result_params::mutable_resultCP(){
_flag[0 >> 3] |= (1 << (0 & 0x7));
 if(!_m_resultCP){
_m_resultCP=::idl::result_cp::create(_pool);
}
return _m_resultCP;
}
inline ::idl::result_cp* chesshallprops_use_exchange_code_result_params::m_resultCP(){
_flag[0 >> 3] |= (1 << (0 & 0x7));
 if(!_m_resultCP){
_m_resultCP=::idl::result_cp::create(_pool);
}
return _m_resultCP;
}
inline void chesshallprops_use_exchange_code_result_params::clear_resultCP(){
if(_m_resultCP){
_flag[0 >> 3] &= ~(1 << (0 & 0x7));
 _m_resultCP->clear();
}
}
inline ::idl::Unknown* chesshallprops_use_exchange_code_result_params::mutable_unknown(){
return &_unknown;
}
inline const ::idl::Unknown& chesshallprops_use_exchange_code_result_params::unknown() const{
return _unknown;
}
inline ::idl::Unknown& chesshallprops_use_exchange_code_result_params::unknown(){
return _unknown;
}
inline size_t chesshallprops_use_exchange_code_result_params::unknown_size() const{
return _unknown.size();
}
inline chesshallprops_use_exchange_code_response* chesshallprops_use_exchange_code_response::create(bsl::mempool* pool){
if(!pool){
throw bsl::NullPointerException() << BSL_EARG;
}
chesshallprops_use_exchange_code_response* tmp=(chesshallprops_use_exchange_code_response*)pool->malloc(sizeof(*tmp));
if(!tmp){
throw bsl::BadAllocException() << BSL_EARG;
}
new(tmp)chesshallprops_use_exchange_code_response(pool);
return tmp;
}
inline void chesshallprops_use_exchange_code_response::destroy(chesshallprops_use_exchange_code_response* v){
if(v){
bsl::mempool *pool=v->_pool;
v->~chesshallprops_use_exchange_code_response();
pool->free(v,sizeof(*v));
}
}
inline chesshallprops_use_exchange_code_response::chesshallprops_use_exchange_code_response(bsl::mempool *pool): 
::idl::McpackBean(pool),
_pool(pool)
,_m_result_params(0)
,_unknown(_pool){
_m_result_params=::idl::chesshallprops_use_exchange_code_result_params::create(_pool);
memset(_flag,0,sizeof(_flag));
}
inline chesshallprops_use_exchange_code_response::chesshallprops_use_exchange_code_response(const chesshallprops_use_exchange_code_response& v): 
::idl::McpackBean(v._pool),
_pool(v._pool)
,_m_result_params(0)
,_unknown(_pool){
_m_result_params=::idl::chesshallprops_use_exchange_code_result_params::create(_pool);
assign(v);
}
inline chesshallprops_use_exchange_code_response::~chesshallprops_use_exchange_code_response(){
if(_m_result_params){
::idl::chesshallprops_use_exchange_code_result_params::destroy(_m_result_params);
_m_result_params=0;
}
}
inline chesshallprops_use_exchange_code_response& chesshallprops_use_exchange_code_response::operator=(const chesshallprops_use_exchange_code_response& v){
assign(v);
return *this;
}
inline void chesshallprops_use_exchange_code_response::clear(){
if(_m_result_params){
_m_result_params->clear();
}
memset(_flag,0,sizeof(_flag));
_unknown.clear(); 
::idl::McpackBean::clear();
}
// @@CF(constraint functions).
inline void chesshallprops_use_exchange_code_response::check() const{
// @@chesshallprops_use_exchange_code_result_params result_params;
if(!(_flag[0 >> 3] & (1 << (0 & 0x7)))){
throw bsl::KeyNotFoundException() << BSL_EARG << "key '" << "result_params" << "' not found";
}
{
::idl::chesshallprops_use_exchange_code_result_params& item=const_cast< ::idl::chesshallprops_use_exchange_code_result_params& >(*_m_result_params);
item.check();

}

::idl::McpackBean::check();
}
inline void chesshallprops_use_exchange_code_response::detach(){
if(_m_result_params){
_m_result_params->detach();
}
_unknown.clear();

::idl::McpackBean::detach();
}
inline chesshallprops_use_exchange_code_response& chesshallprops_use_exchange_code_response::assign(const chesshallprops_use_exchange_code_response& v){
if(!v._m_result_params){
if(_m_result_params){
::idl::chesshallprops_use_exchange_code_result_params::destroy(_m_result_params);
}
_m_result_params=0;
}else{
if(!_m_result_params){
_m_result_params=::idl::chesshallprops_use_exchange_code_result_params::create(_pool);
}
*_m_result_params=*v._m_result_params;
}
_unknown=v._unknown;
bsl::xmemcpy(_flag,v._flag,sizeof(v._flag));
return *this;
}
inline bool chesshallprops_use_exchange_code_response::has_result_params() const{
return (_flag[0 >> 3] & (1 << (0 & 0x7)));
}
inline const ::idl::chesshallprops_use_exchange_code_result_params& chesshallprops_use_exchange_code_response::result_params() const{
 return *_m_result_params;
}
inline ::idl::chesshallprops_use_exchange_code_result_params* chesshallprops_use_exchange_code_response::mutable_result_params(){
_flag[0 >> 3] |= (1 << (0 & 0x7));
 if(!_m_result_params){
_m_result_params=::idl::chesshallprops_use_exchange_code_result_params::create(_pool);
}
return _m_result_params;
}
inline ::idl::chesshallprops_use_exchange_code_result_params* chesshallprops_use_exchange_code_response::m_result_params(){
_flag[0 >> 3] |= (1 << (0 & 0x7));
 if(!_m_result_params){
_m_result_params=::idl::chesshallprops_use_exchange_code_result_params::create(_pool);
}
return _m_result_params;
}
inline void chesshallprops_use_exchange_code_response::clear_result_params(){
if(_m_result_params){
_flag[0 >> 3] &= ~(1 << (0 & 0x7));
 _m_result_params->clear();
}
}
inline ::idl::Unknown* chesshallprops_use_exchange_code_response::mutable_unknown(){
return &_unknown;
}
inline const ::idl::Unknown& chesshallprops_use_exchange_code_response::unknown() const{
return _unknown;
}
inline ::idl::Unknown& chesshallprops_use_exchange_code_response::unknown(){
return _unknown;
}
inline size_t chesshallprops_use_exchange_code_response::unknown_size() const{
return _unknown.size();
}
} // namespace idl
#endif // IDL_GENERATED_FILE_CHESS_HALL_PROPS_IDL_H_
