#include "chess_hall_props.idl.h"
__attribute__((unused)) extern bsl::syspool _common_pool;
namespace idl {
// @@class exchange_code_info
static void _mcpack2_load_exchange_code_info_user_id(const mc_pack_item_t*,exchange_code_info*);
static void _mcpack2_load_exchange_code_info_number(const mc_pack_item_t*,exchange_code_info*);
static void _mcpack2_load_exchange_code_info_commodity_id(const mc_pack_item_t*,exchange_code_info*);
static void _mcpack2_load_exchange_code_info_expired_time(const mc_pack_item_t*,exchange_code_info*);
static void _mcpack2_load_exchange_code_info_obtain_time(const mc_pack_item_t*,exchange_code_info*);
static void _mcpack2_load_exchange_code_info_expired_type(const mc_pack_item_t*,exchange_code_info*);
static void _mcpack2_load_exchange_code_info_game_id(const mc_pack_item_t*,exchange_code_info*);
static bsl::readmap<const char*,void (*)(const mc_pack_item_t*,exchange_code_info*),::idl::string_hash,::idl::string_equal> _mcpack2_load_exchange_code_info_readmap;
// static initialization.
static bool _mcpack2_load_exchange_code_info_static_init(){
std::vector< std::pair<const char*, void (*)(const mc_pack_item_t*,exchange_code_info*)> >_vec;
_vec.push_back(std::pair<const char*,void (*)(const mc_pack_item_t*,exchange_code_info*)>("user_id",_mcpack2_load_exchange_code_info_user_id));
_vec.push_back(std::pair<const char*,void (*)(const mc_pack_item_t*,exchange_code_info*)>("number",_mcpack2_load_exchange_code_info_number));
_vec.push_back(std::pair<const char*,void (*)(const mc_pack_item_t*,exchange_code_info*)>("commodity_id",_mcpack2_load_exchange_code_info_commodity_id));
_vec.push_back(std::pair<const char*,void (*)(const mc_pack_item_t*,exchange_code_info*)>("expired_time",_mcpack2_load_exchange_code_info_expired_time));
_vec.push_back(std::pair<const char*,void (*)(const mc_pack_item_t*,exchange_code_info*)>("obtain_time",_mcpack2_load_exchange_code_info_obtain_time));
_vec.push_back(std::pair<const char*,void (*)(const mc_pack_item_t*,exchange_code_info*)>("expired_type",_mcpack2_load_exchange_code_info_expired_type));
_vec.push_back(std::pair<const char*,void (*)(const mc_pack_item_t*,exchange_code_info*)>("game_id",_mcpack2_load_exchange_code_info_game_id));
_mcpack2_load_exchange_code_info_readmap.create(16*7);
_mcpack2_load_exchange_code_info_readmap.assign(_vec.begin(),_vec.end());
return true;
}
__attribute__((unused)) static bool _mcpack2_load_exchange_code_info_static_inited=_mcpack2_load_exchange_code_info_static_init();
// @@int32_t user_id;
static void _mcpack2_load_exchange_code_info_user_id(__attribute__((unused)) const mc_pack_item_t* item,exchange_code_info* self){
self->_flag[0 >> 3] |= (1 << (0 & 0x7));
 
__attribute__((unused)) int res=0;
__attribute__((unused)) size_t size=0;
res=mc_pack_get_int32_from_item(item,&(self->_m_user_id));
if(res){
throw ::idl::McpackWrongTypeException() << BSL_EARG << "user_id";
}
}
// @@string number;
static void _mcpack2_load_exchange_code_info_number(__attribute__((unused)) const mc_pack_item_t* item,exchange_code_info* self){
self->_flag[1 >> 3] |= (1 << (1 & 0x7));
 
__attribute__((unused)) int res=0;
__attribute__((unused)) size_t size=0;
if(item->type!=MC_IT_TXT){
throw ::idl::McpackWrongTypeException() << BSL_EARG << "number";
}
self->_m_number.set_ptr(item->value,item->value_size-1);
}
// @@int32_t commodity_id;
static void _mcpack2_load_exchange_code_info_commodity_id(__attribute__((unused)) const mc_pack_item_t* item,exchange_code_info* self){
self->_flag[2 >> 3] |= (1 << (2 & 0x7));
 
__attribute__((unused)) int res=0;
__attribute__((unused)) size_t size=0;
res=mc_pack_get_int32_from_item(item,&(self->_m_commodity_id));
if(res){
throw ::idl::McpackWrongTypeException() << BSL_EARG << "commodity_id";
}
}
// @@string expired_time;
static void _mcpack2_load_exchange_code_info_expired_time(__attribute__((unused)) const mc_pack_item_t* item,exchange_code_info* self){
self->_flag[3 >> 3] |= (1 << (3 & 0x7));
 
__attribute__((unused)) int res=0;
__attribute__((unused)) size_t size=0;
if(item->type!=MC_IT_TXT){
throw ::idl::McpackWrongTypeException() << BSL_EARG << "expired_time";
}
self->_m_expired_time.set_ptr(item->value,item->value_size-1);
}
// @@string obtain_time;
static void _mcpack2_load_exchange_code_info_obtain_time(__attribute__((unused)) const mc_pack_item_t* item,exchange_code_info* self){
self->_flag[4 >> 3] |= (1 << (4 & 0x7));
 
__attribute__((unused)) int res=0;
__attribute__((unused)) size_t size=0;
if(item->type!=MC_IT_TXT){
throw ::idl::McpackWrongTypeException() << BSL_EARG << "obtain_time";
}
self->_m_obtain_time.set_ptr(item->value,item->value_size-1);
}
// @@int32_t expired_type;
static void _mcpack2_load_exchange_code_info_expired_type(__attribute__((unused)) const mc_pack_item_t* item,exchange_code_info* self){
self->_flag[5 >> 3] |= (1 << (5 & 0x7));
 
__attribute__((unused)) int res=0;
__attribute__((unused)) size_t size=0;
res=mc_pack_get_int32_from_item(item,&(self->_m_expired_type));
if(res){
throw ::idl::McpackWrongTypeException() << BSL_EARG << "expired_type";
}
}
// @@string game_id;
static void _mcpack2_load_exchange_code_info_game_id(__attribute__((unused)) const mc_pack_item_t* item,exchange_code_info* self){
self->_flag[6 >> 3] |= (1 << (6 & 0x7));
 
__attribute__((unused)) int res=0;
__attribute__((unused)) size_t size=0;
if(item->type!=MC_IT_TXT){
throw ::idl::McpackWrongTypeException() << BSL_EARG << "game_id";
}
self->_m_game_id.set_ptr(item->value,item->value_size-1);
}
void exchange_code_info::LoadWithoutCheck(const mc_pack_t* pack){
if(MC_PACK_PTR_ERR(pack)){
throw ::idl::McpackException(pack) << BSL_EARG;
}
clear();
mc_pack_item_t item;
int res=mc_pack_first_item(pack,&item);
if(res && res!=MC_PE_NOT_FOUND){
throw ::idl::McpackException(res) << BSL_EARG;
}
if(res!=MC_PE_NOT_FOUND){
void (*func)(const mc_pack_item_t*,exchange_code_info*)=0;
do{
if(_mcpack2_load_exchange_code_info_readmap.get(mc_pack_get_subkey(item.key),&func)==bsl::HASH_EXIST){
func(&item,this);
}else{
bool push=true;
for(size_t i=0;i<_ext.size();i++){
if(_ext[i]->load(item)>=0){
push=false;
}
}
if(push){
unknown().push_back<mc_pack_item_t>(item);
}
}
}while(!mc_pack_next_item(&item,&item));
}
}
//for compatiblity.return 0 when it accepts item,else return -1
int exchange_code_info::load(const mc_pack_item_t& item){
void (*func)(const mc_pack_item_t*,exchange_code_info*)=0;
if(_mcpack2_load_exchange_code_info_readmap.get(mc_pack_get_subkey(item.key),&func)==bsl::HASH_EXIST){
func(&item,this);
return 0;
}
return -1;
}
void exchange_code_info::load(const mc_pack_t* pack){
LoadWithoutCheck(pack);
check();
}
size_t exchange_code_info::save(mc_pack_t* pack) const{
if(MC_PACK_PTR_ERR(pack)){
throw ::idl::McpackException(pack) << BSL_EARG;
}
size_t orig_size=mc_pack_get_size(pack);
// @@int32_t user_id;
{
int res=mc_pack_put_int32(pack,"user_id",_m_user_id);
if(res){
throw ::idl::McpackException(res) << BSL_EARG;
}
}
// @@string number;
{
int res=mc_pack_put_str(pack,"number",_m_number.c_str());
if(res){
throw ::idl::McpackException(res) << BSL_EARG;
}
}
// @@int32_t commodity_id;
{
int res=mc_pack_put_int32(pack,"commodity_id",_m_commodity_id);
if(res){
throw ::idl::McpackException(res) << BSL_EARG;
}
}
// @@string expired_time;
{
int res=mc_pack_put_str(pack,"expired_time",_m_expired_time.c_str());
if(res){
throw ::idl::McpackException(res) << BSL_EARG;
}
}
// @@string obtain_time;
{
int res=mc_pack_put_str(pack,"obtain_time",_m_obtain_time.c_str());
if(res){
throw ::idl::McpackException(res) << BSL_EARG;
}
}
// @@int32_t expired_type;
{
int res=mc_pack_put_int32(pack,"expired_type",_m_expired_type);
if(res){
throw ::idl::McpackException(res) << BSL_EARG;
}
}
// @@string game_id;
{
int res=mc_pack_put_str(pack,"game_id",_m_game_id.c_str());
if(res){
throw ::idl::McpackException(res) << BSL_EARG;
}
}
// save '_unknown' field.
for(size_t i=0;i<_unknown.size();i++){
const mc_pack_item_t& item=_unknown.GetWithoutCheck< mc_pack_item_t >(i);
if(item.type==MC_PT_OBJ){
mc_pack_t* tmp=mc_pack_put_object(pack,mc_pack_get_subkey(item.key));
if(MC_PACK_PTR_ERR(tmp)){
throw ::idl::McpackException(tmp) << BSL_EARG;
}
const mc_pack_t* tmppack=0;
int res=mc_pack_get_pack_from_item(&item,&tmppack);
if(res<0){
throw ::idl::McpackException(res) << BSL_EARG;
}
res=mc_pack_copy_item(tmppack,tmp,0);
if(res<0){
throw ::idl::McpackException(res) << BSL_EARG;
}
mc_pack_finish(tmp);
}else if(item.type==MC_PT_ARR){
mc_pack_t* tmp=mc_pack_put_array(pack,mc_pack_get_subkey(item.key));
if(MC_PACK_PTR_ERR(tmp)){
throw ::idl::McpackException(tmp) << BSL_EARG;
}
const mc_pack_t* tmppack=0;
int res=mc_pack_get_array_from_item(&item,&tmppack);
if(res<0){
throw ::idl::McpackException(res) << BSL_EARG;
}
res=mc_pack_copy_item(tmppack,tmp,0);
if(res<0){
throw ::idl::McpackException(res) << BSL_EARG;
}
mc_pack_finish(tmp);
}else{
int res=mc_pack_put_item(pack,&item);
if(res<0){
throw ::idl::McpackException(res) << BSL_EARG;
}
}
}
// save '_ext' field.
for(size_t i=0;i<_ext.size();i++){
_ext[i]->save(pack);
}
return mc_pack_get_size(pack)-orig_size;
}
// @@class result_cp
static void _mcpack2_load_result_cp_is_success(const mc_pack_item_t*,result_cp*);
static void _mcpack2_load_result_cp_code(const mc_pack_item_t*,result_cp*);
static void _mcpack2_load_result_cp_message(const mc_pack_item_t*,result_cp*);
static bsl::readmap<const char*,void (*)(const mc_pack_item_t*,result_cp*),::idl::string_hash,::idl::string_equal> _mcpack2_load_result_cp_readmap;
// static initialization.
static bool _mcpack2_load_result_cp_static_init(){
std::vector< std::pair<const char*, void (*)(const mc_pack_item_t*,result_cp*)> >_vec;
_vec.push_back(std::pair<const char*,void (*)(const mc_pack_item_t*,result_cp*)>("is_success",_mcpack2_load_result_cp_is_success));
_vec.push_back(std::pair<const char*,void (*)(const mc_pack_item_t*,result_cp*)>("code",_mcpack2_load_result_cp_code));
_vec.push_back(std::pair<const char*,void (*)(const mc_pack_item_t*,result_cp*)>("message",_mcpack2_load_result_cp_message));
_mcpack2_load_result_cp_readmap.create(16*3);
_mcpack2_load_result_cp_readmap.assign(_vec.begin(),_vec.end());
return true;
}
__attribute__((unused)) static bool _mcpack2_load_result_cp_static_inited=_mcpack2_load_result_cp_static_init();
// @@bool is_success;
static void _mcpack2_load_result_cp_is_success(__attribute__((unused)) const mc_pack_item_t* item,result_cp* self){
self->_flag[0 >> 3] |= (1 << (0 & 0x7));
 
__attribute__((unused)) int res=0;
__attribute__((unused)) size_t size=0;
res=mc_pack_get_bool_from_item(item,&(self->_m_is_success));
if(res){
throw ::idl::McpackWrongTypeException() << BSL_EARG << "is_success";
}
}
// @@int32_t code;
static void _mcpack2_load_result_cp_code(__attribute__((unused)) const mc_pack_item_t* item,result_cp* self){
self->_flag[1 >> 3] |= (1 << (1 & 0x7));
 
__attribute__((unused)) int res=0;
__attribute__((unused)) size_t size=0;
res=mc_pack_get_int32_from_item(item,&(self->_m_code));
if(res){
throw ::idl::McpackWrongTypeException() << BSL_EARG << "code";
}
}
// @@string message;
static void _mcpack2_load_result_cp_message(__attribute__((unused)) const mc_pack_item_t* item,result_cp* self){
self->_flag[2 >> 3] |= (1 << (2 & 0x7));
 
__attribute__((unused)) int res=0;
__attribute__((unused)) size_t size=0;
if(item->type!=MC_IT_TXT){
throw ::idl::McpackWrongTypeException() << BSL_EARG << "message";
}
self->_m_message.set_ptr(item->value,item->value_size-1);
}
void result_cp::LoadWithoutCheck(const mc_pack_t* pack){
if(MC_PACK_PTR_ERR(pack)){
throw ::idl::McpackException(pack) << BSL_EARG;
}
clear();
mc_pack_item_t item;
int res=mc_pack_first_item(pack,&item);
if(res && res!=MC_PE_NOT_FOUND){
throw ::idl::McpackException(res) << BSL_EARG;
}
if(res!=MC_PE_NOT_FOUND){
void (*func)(const mc_pack_item_t*,result_cp*)=0;
do{
if(_mcpack2_load_result_cp_readmap.get(mc_pack_get_subkey(item.key),&func)==bsl::HASH_EXIST){
func(&item,this);
}else{
bool push=true;
for(size_t i=0;i<_ext.size();i++){
if(_ext[i]->load(item)>=0){
push=false;
}
}
if(push){
unknown().push_back<mc_pack_item_t>(item);
}
}
}while(!mc_pack_next_item(&item,&item));
}
}
//for compatiblity.return 0 when it accepts item,else return -1
int result_cp::load(const mc_pack_item_t& item){
void (*func)(const mc_pack_item_t*,result_cp*)=0;
if(_mcpack2_load_result_cp_readmap.get(mc_pack_get_subkey(item.key),&func)==bsl::HASH_EXIST){
func(&item,this);
return 0;
}
return -1;
}
void result_cp::load(const mc_pack_t* pack){
LoadWithoutCheck(pack);
check();
}
size_t result_cp::save(mc_pack_t* pack) const{
if(MC_PACK_PTR_ERR(pack)){
throw ::idl::McpackException(pack) << BSL_EARG;
}
size_t orig_size=mc_pack_get_size(pack);
// @@bool is_success;
{
int res=mc_pack_put_bool(pack,"is_success",_m_is_success);
if(res){
throw ::idl::McpackException(res) << BSL_EARG;
}
}
// @@int32_t code;
{
int res=mc_pack_put_int32(pack,"code",_m_code);
if(res){
throw ::idl::McpackException(res) << BSL_EARG;
}
}
// @@string message;
{
int res=mc_pack_put_str(pack,"message",_m_message.c_str());
if(res){
throw ::idl::McpackException(res) << BSL_EARG;
}
}
// save '_unknown' field.
for(size_t i=0;i<_unknown.size();i++){
const mc_pack_item_t& item=_unknown.GetWithoutCheck< mc_pack_item_t >(i);
if(item.type==MC_PT_OBJ){
mc_pack_t* tmp=mc_pack_put_object(pack,mc_pack_get_subkey(item.key));
if(MC_PACK_PTR_ERR(tmp)){
throw ::idl::McpackException(tmp) << BSL_EARG;
}
const mc_pack_t* tmppack=0;
int res=mc_pack_get_pack_from_item(&item,&tmppack);
if(res<0){
throw ::idl::McpackException(res) << BSL_EARG;
}
res=mc_pack_copy_item(tmppack,tmp,0);
if(res<0){
throw ::idl::McpackException(res) << BSL_EARG;
}
mc_pack_finish(tmp);
}else if(item.type==MC_PT_ARR){
mc_pack_t* tmp=mc_pack_put_array(pack,mc_pack_get_subkey(item.key));
if(MC_PACK_PTR_ERR(tmp)){
throw ::idl::McpackException(tmp) << BSL_EARG;
}
const mc_pack_t* tmppack=0;
int res=mc_pack_get_array_from_item(&item,&tmppack);
if(res<0){
throw ::idl::McpackException(res) << BSL_EARG;
}
res=mc_pack_copy_item(tmppack,tmp,0);
if(res<0){
throw ::idl::McpackException(res) << BSL_EARG;
}
mc_pack_finish(tmp);
}else{
int res=mc_pack_put_item(pack,&item);
if(res<0){
throw ::idl::McpackException(res) << BSL_EARG;
}
}
}
// save '_ext' field.
for(size_t i=0;i<_ext.size();i++){
_ext[i]->save(pack);
}
return mc_pack_get_size(pack)-orig_size;
}
// @@class chesshallprops_obtain_commodity_params
static void _mcpack2_load_chesshallprops_obtain_commodity_params_userId(const mc_pack_item_t*,chesshallprops_obtain_commodity_params*);
static void _mcpack2_load_chesshallprops_obtain_commodity_params_commodity_id(const mc_pack_item_t*,chesshallprops_obtain_commodity_params*);
static void _mcpack2_load_chesshallprops_obtain_commodity_params_num(const mc_pack_item_t*,chesshallprops_obtain_commodity_params*);
static void _mcpack2_load_chesshallprops_obtain_commodity_params_gameId(const mc_pack_item_t*,chesshallprops_obtain_commodity_params*);
static bsl::readmap<const char*,void (*)(const mc_pack_item_t*,chesshallprops_obtain_commodity_params*),::idl::string_hash,::idl::string_equal> _mcpack2_load_chesshallprops_obtain_commodity_params_readmap;
// static initialization.
static bool _mcpack2_load_chesshallprops_obtain_commodity_params_static_init(){
std::vector< std::pair<const char*, void (*)(const mc_pack_item_t*,chesshallprops_obtain_commodity_params*)> >_vec;
_vec.push_back(std::pair<const char*,void (*)(const mc_pack_item_t*,chesshallprops_obtain_commodity_params*)>("userId",_mcpack2_load_chesshallprops_obtain_commodity_params_userId));
_vec.push_back(std::pair<const char*,void (*)(const mc_pack_item_t*,chesshallprops_obtain_commodity_params*)>("commodity_id",_mcpack2_load_chesshallprops_obtain_commodity_params_commodity_id));
_vec.push_back(std::pair<const char*,void (*)(const mc_pack_item_t*,chesshallprops_obtain_commodity_params*)>("num",_mcpack2_load_chesshallprops_obtain_commodity_params_num));
_vec.push_back(std::pair<const char*,void (*)(const mc_pack_item_t*,chesshallprops_obtain_commodity_params*)>("gameId",_mcpack2_load_chesshallprops_obtain_commodity_params_gameId));
_mcpack2_load_chesshallprops_obtain_commodity_params_readmap.create(16*4);
_mcpack2_load_chesshallprops_obtain_commodity_params_readmap.assign(_vec.begin(),_vec.end());
return true;
}
__attribute__((unused)) static bool _mcpack2_load_chesshallprops_obtain_commodity_params_static_inited=_mcpack2_load_chesshallprops_obtain_commodity_params_static_init();
// @@int32_t userId=in();
static void _mcpack2_load_chesshallprops_obtain_commodity_params_userId(__attribute__((unused)) const mc_pack_item_t* item,chesshallprops_obtain_commodity_params* self){
self->_flag[0 >> 3] |= (1 << (0 & 0x7));
 
__attribute__((unused)) int res=0;
__attribute__((unused)) size_t size=0;
res=mc_pack_get_int32_from_item(item,&(self->_m_userId));
if(res){
throw ::idl::McpackWrongTypeException() << BSL_EARG << "userId";
}
}
// @@int32_t commodity_id=in();
static void _mcpack2_load_chesshallprops_obtain_commodity_params_commodity_id(__attribute__((unused)) const mc_pack_item_t* item,chesshallprops_obtain_commodity_params* self){
self->_flag[1 >> 3] |= (1 << (1 & 0x7));
 
__attribute__((unused)) int res=0;
__attribute__((unused)) size_t size=0;
res=mc_pack_get_int32_from_item(item,&(self->_m_commodity_id));
if(res){
throw ::idl::McpackWrongTypeException() << BSL_EARG << "commodity_id";
}
}
// @@int32_t num=in();
static void _mcpack2_load_chesshallprops_obtain_commodity_params_num(__attribute__((unused)) const mc_pack_item_t* item,chesshallprops_obtain_commodity_params* self){
self->_flag[2 >> 3] |= (1 << (2 & 0x7));
 
__attribute__((unused)) int res=0;
__attribute__((unused)) size_t size=0;
res=mc_pack_get_int32_from_item(item,&(self->_m_num));
if(res){
throw ::idl::McpackWrongTypeException() << BSL_EARG << "num";
}
}
// @@string gameId=in();
static void _mcpack2_load_chesshallprops_obtain_commodity_params_gameId(__attribute__((unused)) const mc_pack_item_t* item,chesshallprops_obtain_commodity_params* self){
self->_flag[3 >> 3] |= (1 << (3 & 0x7));
 
__attribute__((unused)) int res=0;
__attribute__((unused)) size_t size=0;
if(item->type!=MC_IT_TXT){
throw ::idl::McpackWrongTypeException() << BSL_EARG << "gameId";
}
self->_m_gameId.set_ptr(item->value,item->value_size-1);
}
void chesshallprops_obtain_commodity_params::LoadWithoutCheck(const mc_pack_t* pack){
if(MC_PACK_PTR_ERR(pack)){
throw ::idl::McpackException(pack) << BSL_EARG;
}
clear();
mc_pack_item_t item;
int res=mc_pack_first_item(pack,&item);
if(res && res!=MC_PE_NOT_FOUND){
throw ::idl::McpackException(res) << BSL_EARG;
}
if(res!=MC_PE_NOT_FOUND){
void (*func)(const mc_pack_item_t*,chesshallprops_obtain_commodity_params*)=0;
do{
if(_mcpack2_load_chesshallprops_obtain_commodity_params_readmap.get(mc_pack_get_subkey(item.key),&func)==bsl::HASH_EXIST){
func(&item,this);
}else{
bool push=true;
for(size_t i=0;i<_ext.size();i++){
if(_ext[i]->load(item)>=0){
push=false;
}
}
if(push){
unknown().push_back<mc_pack_item_t>(item);
}
}
}while(!mc_pack_next_item(&item,&item));
}
}
//for compatiblity.return 0 when it accepts item,else return -1
int chesshallprops_obtain_commodity_params::load(const mc_pack_item_t& item){
void (*func)(const mc_pack_item_t*,chesshallprops_obtain_commodity_params*)=0;
if(_mcpack2_load_chesshallprops_obtain_commodity_params_readmap.get(mc_pack_get_subkey(item.key),&func)==bsl::HASH_EXIST){
func(&item,this);
return 0;
}
return -1;
}
void chesshallprops_obtain_commodity_params::load(const mc_pack_t* pack){
LoadWithoutCheck(pack);
check();
}
size_t chesshallprops_obtain_commodity_params::save(mc_pack_t* pack) const{
if(MC_PACK_PTR_ERR(pack)){
throw ::idl::McpackException(pack) << BSL_EARG;
}
size_t orig_size=mc_pack_get_size(pack);
// @@int32_t userId=in();
{
int res=mc_pack_put_int32(pack,"userId",_m_userId);
if(res){
throw ::idl::McpackException(res) << BSL_EARG;
}
}
// @@int32_t commodity_id=in();
{
int res=mc_pack_put_int32(pack,"commodity_id",_m_commodity_id);
if(res){
throw ::idl::McpackException(res) << BSL_EARG;
}
}
// @@int32_t num=in();
{
int res=mc_pack_put_int32(pack,"num",_m_num);
if(res){
throw ::idl::McpackException(res) << BSL_EARG;
}
}
// @@string gameId=in();
{
int res=mc_pack_put_str(pack,"gameId",_m_gameId.c_str());
if(res){
throw ::idl::McpackException(res) << BSL_EARG;
}
}
// save '_unknown' field.
for(size_t i=0;i<_unknown.size();i++){
const mc_pack_item_t& item=_unknown.GetWithoutCheck< mc_pack_item_t >(i);
if(item.type==MC_PT_OBJ){
mc_pack_t* tmp=mc_pack_put_object(pack,mc_pack_get_subkey(item.key));
if(MC_PACK_PTR_ERR(tmp)){
throw ::idl::McpackException(tmp) << BSL_EARG;
}
const mc_pack_t* tmppack=0;
int res=mc_pack_get_pack_from_item(&item,&tmppack);
if(res<0){
throw ::idl::McpackException(res) << BSL_EARG;
}
res=mc_pack_copy_item(tmppack,tmp,0);
if(res<0){
throw ::idl::McpackException(res) << BSL_EARG;
}
mc_pack_finish(tmp);
}else if(item.type==MC_PT_ARR){
mc_pack_t* tmp=mc_pack_put_array(pack,mc_pack_get_subkey(item.key));
if(MC_PACK_PTR_ERR(tmp)){
throw ::idl::McpackException(tmp) << BSL_EARG;
}
const mc_pack_t* tmppack=0;
int res=mc_pack_get_array_from_item(&item,&tmppack);
if(res<0){
throw ::idl::McpackException(res) << BSL_EARG;
}
res=mc_pack_copy_item(tmppack,tmp,0);
if(res<0){
throw ::idl::McpackException(res) << BSL_EARG;
}
mc_pack_finish(tmp);
}else{
int res=mc_pack_put_item(pack,&item);
if(res<0){
throw ::idl::McpackException(res) << BSL_EARG;
}
}
}
// save '_ext' field.
for(size_t i=0;i<_ext.size();i++){
_ext[i]->save(pack);
}
return mc_pack_get_size(pack)-orig_size;
}
// @@class chesshallprops_obtain_commodity_result_params
static void _mcpack2_load_chesshallprops_obtain_commodity_result_params_success(const mc_pack_item_t*,chesshallprops_obtain_commodity_result_params*);
static bsl::readmap<const char*,void (*)(const mc_pack_item_t*,chesshallprops_obtain_commodity_result_params*),::idl::string_hash,::idl::string_equal> _mcpack2_load_chesshallprops_obtain_commodity_result_params_readmap;
// static initialization.
static bool _mcpack2_load_chesshallprops_obtain_commodity_result_params_static_init(){
std::vector< std::pair<const char*, void (*)(const mc_pack_item_t*,chesshallprops_obtain_commodity_result_params*)> >_vec;
_vec.push_back(std::pair<const char*,void (*)(const mc_pack_item_t*,chesshallprops_obtain_commodity_result_params*)>("success",_mcpack2_load_chesshallprops_obtain_commodity_result_params_success));
_mcpack2_load_chesshallprops_obtain_commodity_result_params_readmap.create(16*1);
_mcpack2_load_chesshallprops_obtain_commodity_result_params_readmap.assign(_vec.begin(),_vec.end());
return true;
}
__attribute__((unused)) static bool _mcpack2_load_chesshallprops_obtain_commodity_result_params_static_inited=_mcpack2_load_chesshallprops_obtain_commodity_result_params_static_init();
// @@bool success=out();
static void _mcpack2_load_chesshallprops_obtain_commodity_result_params_success(__attribute__((unused)) const mc_pack_item_t* item,chesshallprops_obtain_commodity_result_params* self){
self->_flag[0 >> 3] |= (1 << (0 & 0x7));
 
__attribute__((unused)) int res=0;
__attribute__((unused)) size_t size=0;
res=mc_pack_get_bool_from_item(item,&(self->_m_success));
if(res){
throw ::idl::McpackWrongTypeException() << BSL_EARG << "success";
}
}
void chesshallprops_obtain_commodity_result_params::LoadWithoutCheck(const mc_pack_t* pack){
if(MC_PACK_PTR_ERR(pack)){
throw ::idl::McpackException(pack) << BSL_EARG;
}
clear();
mc_pack_item_t item;
int res=mc_pack_first_item(pack,&item);
if(res && res!=MC_PE_NOT_FOUND){
throw ::idl::McpackException(res) << BSL_EARG;
}
if(res!=MC_PE_NOT_FOUND){
void (*func)(const mc_pack_item_t*,chesshallprops_obtain_commodity_result_params*)=0;
do{
if(_mcpack2_load_chesshallprops_obtain_commodity_result_params_readmap.get(mc_pack_get_subkey(item.key),&func)==bsl::HASH_EXIST){
func(&item,this);
}else{
bool push=true;
for(size_t i=0;i<_ext.size();i++){
if(_ext[i]->load(item)>=0){
push=false;
}
}
if(push){
unknown().push_back<mc_pack_item_t>(item);
}
}
}while(!mc_pack_next_item(&item,&item));
}
}
//for compatiblity.return 0 when it accepts item,else return -1
int chesshallprops_obtain_commodity_result_params::load(const mc_pack_item_t& item){
void (*func)(const mc_pack_item_t*,chesshallprops_obtain_commodity_result_params*)=0;
if(_mcpack2_load_chesshallprops_obtain_commodity_result_params_readmap.get(mc_pack_get_subkey(item.key),&func)==bsl::HASH_EXIST){
func(&item,this);
return 0;
}
return -1;
}
void chesshallprops_obtain_commodity_result_params::load(const mc_pack_t* pack){
LoadWithoutCheck(pack);
check();
}
size_t chesshallprops_obtain_commodity_result_params::save(mc_pack_t* pack) const{
if(MC_PACK_PTR_ERR(pack)){
throw ::idl::McpackException(pack) << BSL_EARG;
}
size_t orig_size=mc_pack_get_size(pack);
// @@bool success=out();
{
int res=mc_pack_put_bool(pack,"success",_m_success);
if(res){
throw ::idl::McpackException(res) << BSL_EARG;
}
}
// save '_unknown' field.
for(size_t i=0;i<_unknown.size();i++){
const mc_pack_item_t& item=_unknown.GetWithoutCheck< mc_pack_item_t >(i);
if(item.type==MC_PT_OBJ){
mc_pack_t* tmp=mc_pack_put_object(pack,mc_pack_get_subkey(item.key));
if(MC_PACK_PTR_ERR(tmp)){
throw ::idl::McpackException(tmp) << BSL_EARG;
}
const mc_pack_t* tmppack=0;
int res=mc_pack_get_pack_from_item(&item,&tmppack);
if(res<0){
throw ::idl::McpackException(res) << BSL_EARG;
}
res=mc_pack_copy_item(tmppack,tmp,0);
if(res<0){
throw ::idl::McpackException(res) << BSL_EARG;
}
mc_pack_finish(tmp);
}else if(item.type==MC_PT_ARR){
mc_pack_t* tmp=mc_pack_put_array(pack,mc_pack_get_subkey(item.key));
if(MC_PACK_PTR_ERR(tmp)){
throw ::idl::McpackException(tmp) << BSL_EARG;
}
const mc_pack_t* tmppack=0;
int res=mc_pack_get_array_from_item(&item,&tmppack);
if(res<0){
throw ::idl::McpackException(res) << BSL_EARG;
}
res=mc_pack_copy_item(tmppack,tmp,0);
if(res<0){
throw ::idl::McpackException(res) << BSL_EARG;
}
mc_pack_finish(tmp);
}else{
int res=mc_pack_put_item(pack,&item);
if(res<0){
throw ::idl::McpackException(res) << BSL_EARG;
}
}
}
// save '_ext' field.
for(size_t i=0;i<_ext.size();i++){
_ext[i]->save(pack);
}
return mc_pack_get_size(pack)-orig_size;
}
// @@class chesshallprops_obtain_commodity_response
static void _mcpack2_load_chesshallprops_obtain_commodity_response_result_params(const mc_pack_item_t*,chesshallprops_obtain_commodity_response*);
static bsl::readmap<const char*,void (*)(const mc_pack_item_t*,chesshallprops_obtain_commodity_response*),::idl::string_hash,::idl::string_equal> _mcpack2_load_chesshallprops_obtain_commodity_response_readmap;
// static initialization.
static bool _mcpack2_load_chesshallprops_obtain_commodity_response_static_init(){
std::vector< std::pair<const char*, void (*)(const mc_pack_item_t*,chesshallprops_obtain_commodity_response*)> >_vec;
_vec.push_back(std::pair<const char*,void (*)(const mc_pack_item_t*,chesshallprops_obtain_commodity_response*)>("result_params",_mcpack2_load_chesshallprops_obtain_commodity_response_result_params));
_mcpack2_load_chesshallprops_obtain_commodity_response_readmap.create(16*1);
_mcpack2_load_chesshallprops_obtain_commodity_response_readmap.assign(_vec.begin(),_vec.end());
return true;
}
__attribute__((unused)) static bool _mcpack2_load_chesshallprops_obtain_commodity_response_static_inited=_mcpack2_load_chesshallprops_obtain_commodity_response_static_init();
// @@chesshallprops_obtain_commodity_result_params result_params;
static void _mcpack2_load_chesshallprops_obtain_commodity_response_result_params(__attribute__((unused)) const mc_pack_item_t* item,chesshallprops_obtain_commodity_response* self){
self->_flag[0 >> 3] |= (1 << (0 & 0x7));
 
__attribute__((unused)) int res=0;
__attribute__((unused)) size_t size=0;
const mc_pack_t *pack=0;
res=mc_pack_get_pack_from_item(item,&pack);
if(res<0){
throw ::idl::McpackException(res) << BSL_EARG;
}
if(!self->_m_result_params){
self->_m_result_params=::idl::chesshallprops_obtain_commodity_result_params::create(self->_pool);
}
self->_m_result_params->LoadWithoutCheck(pack);
}
void chesshallprops_obtain_commodity_response::LoadWithoutCheck(const mc_pack_t* pack){
if(MC_PACK_PTR_ERR(pack)){
throw ::idl::McpackException(pack) << BSL_EARG;
}
clear();
mc_pack_item_t item;
int res=mc_pack_first_item(pack,&item);
if(res && res!=MC_PE_NOT_FOUND){
throw ::idl::McpackException(res) << BSL_EARG;
}
if(res!=MC_PE_NOT_FOUND){
void (*func)(const mc_pack_item_t*,chesshallprops_obtain_commodity_response*)=0;
do{
if(_mcpack2_load_chesshallprops_obtain_commodity_response_readmap.get(mc_pack_get_subkey(item.key),&func)==bsl::HASH_EXIST){
func(&item,this);
}else{
bool push=true;
for(size_t i=0;i<_ext.size();i++){
if(_ext[i]->load(item)>=0){
push=false;
}
}
if(push){
unknown().push_back<mc_pack_item_t>(item);
}
}
}while(!mc_pack_next_item(&item,&item));
}
}
//for compatiblity.return 0 when it accepts item,else return -1
int chesshallprops_obtain_commodity_response::load(const mc_pack_item_t& item){
void (*func)(const mc_pack_item_t*,chesshallprops_obtain_commodity_response*)=0;
if(_mcpack2_load_chesshallprops_obtain_commodity_response_readmap.get(mc_pack_get_subkey(item.key),&func)==bsl::HASH_EXIST){
func(&item,this);
return 0;
}
return -1;
}
void chesshallprops_obtain_commodity_response::load(const mc_pack_t* pack){
LoadWithoutCheck(pack);
check();
}
size_t chesshallprops_obtain_commodity_response::save(mc_pack_t* pack) const{
if(MC_PACK_PTR_ERR(pack)){
throw ::idl::McpackException(pack) << BSL_EARG;
}
size_t orig_size=mc_pack_get_size(pack);
// @@chesshallprops_obtain_commodity_result_params result_params;
{
mc_pack_t* subpack=mc_pack_put_object(pack,"result_params");
if(MC_PACK_PTR_ERR(subpack)){
throw ::idl::McpackException(subpack) << BSL_EARG;
}
_m_result_params->save(subpack);
mc_pack_finish(subpack);
}
// save '_unknown' field.
for(size_t i=0;i<_unknown.size();i++){
const mc_pack_item_t& item=_unknown.GetWithoutCheck< mc_pack_item_t >(i);
if(item.type==MC_PT_OBJ){
mc_pack_t* tmp=mc_pack_put_object(pack,mc_pack_get_subkey(item.key));
if(MC_PACK_PTR_ERR(tmp)){
throw ::idl::McpackException(tmp) << BSL_EARG;
}
const mc_pack_t* tmppack=0;
int res=mc_pack_get_pack_from_item(&item,&tmppack);
if(res<0){
throw ::idl::McpackException(res) << BSL_EARG;
}
res=mc_pack_copy_item(tmppack,tmp,0);
if(res<0){
throw ::idl::McpackException(res) << BSL_EARG;
}
mc_pack_finish(tmp);
}else if(item.type==MC_PT_ARR){
mc_pack_t* tmp=mc_pack_put_array(pack,mc_pack_get_subkey(item.key));
if(MC_PACK_PTR_ERR(tmp)){
throw ::idl::McpackException(tmp) << BSL_EARG;
}
const mc_pack_t* tmppack=0;
int res=mc_pack_get_array_from_item(&item,&tmppack);
if(res<0){
throw ::idl::McpackException(res) << BSL_EARG;
}
res=mc_pack_copy_item(tmppack,tmp,0);
if(res<0){
throw ::idl::McpackException(res) << BSL_EARG;
}
mc_pack_finish(tmp);
}else{
int res=mc_pack_put_item(pack,&item);
if(res<0){
throw ::idl::McpackException(res) << BSL_EARG;
}
}
}
// save '_ext' field.
for(size_t i=0;i<_ext.size();i++){
_ext[i]->save(pack);
}
return mc_pack_get_size(pack)-orig_size;
}
// @@class chesshallprops_obtain_exchange_code_params
static void _mcpack2_load_chesshallprops_obtain_exchange_code_params_user_id(const mc_pack_item_t*,chesshallprops_obtain_exchange_code_params*);
static void _mcpack2_load_chesshallprops_obtain_exchange_code_params_number(const mc_pack_item_t*,chesshallprops_obtain_exchange_code_params*);
static void _mcpack2_load_chesshallprops_obtain_exchange_code_params_commodity_id(const mc_pack_item_t*,chesshallprops_obtain_exchange_code_params*);
static void _mcpack2_load_chesshallprops_obtain_exchange_code_params_expired_time(const mc_pack_item_t*,chesshallprops_obtain_exchange_code_params*);
static void _mcpack2_load_chesshallprops_obtain_exchange_code_params_expired_type(const mc_pack_item_t*,chesshallprops_obtain_exchange_code_params*);
static void _mcpack2_load_chesshallprops_obtain_exchange_code_params_game_id(const mc_pack_item_t*,chesshallprops_obtain_exchange_code_params*);
static bsl::readmap<const char*,void (*)(const mc_pack_item_t*,chesshallprops_obtain_exchange_code_params*),::idl::string_hash,::idl::string_equal> _mcpack2_load_chesshallprops_obtain_exchange_code_params_readmap;
// static initialization.
static bool _mcpack2_load_chesshallprops_obtain_exchange_code_params_static_init(){
std::vector< std::pair<const char*, void (*)(const mc_pack_item_t*,chesshallprops_obtain_exchange_code_params*)> >_vec;
_vec.push_back(std::pair<const char*,void (*)(const mc_pack_item_t*,chesshallprops_obtain_exchange_code_params*)>("user_id",_mcpack2_load_chesshallprops_obtain_exchange_code_params_user_id));
_vec.push_back(std::pair<const char*,void (*)(const mc_pack_item_t*,chesshallprops_obtain_exchange_code_params*)>("number",_mcpack2_load_chesshallprops_obtain_exchange_code_params_number));
_vec.push_back(std::pair<const char*,void (*)(const mc_pack_item_t*,chesshallprops_obtain_exchange_code_params*)>("commodity_id",_mcpack2_load_chesshallprops_obtain_exchange_code_params_commodity_id));
_vec.push_back(std::pair<const char*,void (*)(const mc_pack_item_t*,chesshallprops_obtain_exchange_code_params*)>("expired_time",_mcpack2_load_chesshallprops_obtain_exchange_code_params_expired_time));
_vec.push_back(std::pair<const char*,void (*)(const mc_pack_item_t*,chesshallprops_obtain_exchange_code_params*)>("expired_type",_mcpack2_load_chesshallprops_obtain_exchange_code_params_expired_type));
_vec.push_back(std::pair<const char*,void (*)(const mc_pack_item_t*,chesshallprops_obtain_exchange_code_params*)>("game_id",_mcpack2_load_chesshallprops_obtain_exchange_code_params_game_id));
_mcpack2_load_chesshallprops_obtain_exchange_code_params_readmap.create(16*6);
_mcpack2_load_chesshallprops_obtain_exchange_code_params_readmap.assign(_vec.begin(),_vec.end());
return true;
}
__attribute__((unused)) static bool _mcpack2_load_chesshallprops_obtain_exchange_code_params_static_inited=_mcpack2_load_chesshallprops_obtain_exchange_code_params_static_init();
// @@int32_t user_id=in();
static void _mcpack2_load_chesshallprops_obtain_exchange_code_params_user_id(__attribute__((unused)) const mc_pack_item_t* item,chesshallprops_obtain_exchange_code_params* self){
self->_flag[0 >> 3] |= (1 << (0 & 0x7));
 
__attribute__((unused)) int res=0;
__attribute__((unused)) size_t size=0;
res=mc_pack_get_int32_from_item(item,&(self->_m_user_id));
if(res){
throw ::idl::McpackWrongTypeException() << BSL_EARG << "user_id";
}
}
// @@string number=in();
static void _mcpack2_load_chesshallprops_obtain_exchange_code_params_number(__attribute__((unused)) const mc_pack_item_t* item,chesshallprops_obtain_exchange_code_params* self){
self->_flag[1 >> 3] |= (1 << (1 & 0x7));
 
__attribute__((unused)) int res=0;
__attribute__((unused)) size_t size=0;
if(item->type!=MC_IT_TXT){
throw ::idl::McpackWrongTypeException() << BSL_EARG << "number";
}
self->_m_number.set_ptr(item->value,item->value_size-1);
}
// @@int32_t commodity_id=in();
static void _mcpack2_load_chesshallprops_obtain_exchange_code_params_commodity_id(__attribute__((unused)) const mc_pack_item_t* item,chesshallprops_obtain_exchange_code_params* self){
self->_flag[2 >> 3] |= (1 << (2 & 0x7));
 
__attribute__((unused)) int res=0;
__attribute__((unused)) size_t size=0;
res=mc_pack_get_int32_from_item(item,&(self->_m_commodity_id));
if(res){
throw ::idl::McpackWrongTypeException() << BSL_EARG << "commodity_id";
}
}
// @@string expired_time=in();
static void _mcpack2_load_chesshallprops_obtain_exchange_code_params_expired_time(__attribute__((unused)) const mc_pack_item_t* item,chesshallprops_obtain_exchange_code_params* self){
self->_flag[3 >> 3] |= (1 << (3 & 0x7));
 
__attribute__((unused)) int res=0;
__attribute__((unused)) size_t size=0;
if(item->type!=MC_IT_TXT){
throw ::idl::McpackWrongTypeException() << BSL_EARG << "expired_time";
}
self->_m_expired_time.set_ptr(item->value,item->value_size-1);
}
// @@int32_t expired_type=in();
static void _mcpack2_load_chesshallprops_obtain_exchange_code_params_expired_type(__attribute__((unused)) const mc_pack_item_t* item,chesshallprops_obtain_exchange_code_params* self){
self->_flag[4 >> 3] |= (1 << (4 & 0x7));
 
__attribute__((unused)) int res=0;
__attribute__((unused)) size_t size=0;
res=mc_pack_get_int32_from_item(item,&(self->_m_expired_type));
if(res){
throw ::idl::McpackWrongTypeException() << BSL_EARG << "expired_type";
}
}
// @@string game_id=in();
static void _mcpack2_load_chesshallprops_obtain_exchange_code_params_game_id(__attribute__((unused)) const mc_pack_item_t* item,chesshallprops_obtain_exchange_code_params* self){
self->_flag[5 >> 3] |= (1 << (5 & 0x7));
 
__attribute__((unused)) int res=0;
__attribute__((unused)) size_t size=0;
if(item->type!=MC_IT_TXT){
throw ::idl::McpackWrongTypeException() << BSL_EARG << "game_id";
}
self->_m_game_id.set_ptr(item->value,item->value_size-1);
}
void chesshallprops_obtain_exchange_code_params::LoadWithoutCheck(const mc_pack_t* pack){
if(MC_PACK_PTR_ERR(pack)){
throw ::idl::McpackException(pack) << BSL_EARG;
}
clear();
mc_pack_item_t item;
int res=mc_pack_first_item(pack,&item);
if(res && res!=MC_PE_NOT_FOUND){
throw ::idl::McpackException(res) << BSL_EARG;
}
if(res!=MC_PE_NOT_FOUND){
void (*func)(const mc_pack_item_t*,chesshallprops_obtain_exchange_code_params*)=0;
do{
if(_mcpack2_load_chesshallprops_obtain_exchange_code_params_readmap.get(mc_pack_get_subkey(item.key),&func)==bsl::HASH_EXIST){
func(&item,this);
}else{
bool push=true;
for(size_t i=0;i<_ext.size();i++){
if(_ext[i]->load(item)>=0){
push=false;
}
}
if(push){
unknown().push_back<mc_pack_item_t>(item);
}
}
}while(!mc_pack_next_item(&item,&item));
}
}
//for compatiblity.return 0 when it accepts item,else return -1
int chesshallprops_obtain_exchange_code_params::load(const mc_pack_item_t& item){
void (*func)(const mc_pack_item_t*,chesshallprops_obtain_exchange_code_params*)=0;
if(_mcpack2_load_chesshallprops_obtain_exchange_code_params_readmap.get(mc_pack_get_subkey(item.key),&func)==bsl::HASH_EXIST){
func(&item,this);
return 0;
}
return -1;
}
void chesshallprops_obtain_exchange_code_params::load(const mc_pack_t* pack){
LoadWithoutCheck(pack);
check();
}
size_t chesshallprops_obtain_exchange_code_params::save(mc_pack_t* pack) const{
if(MC_PACK_PTR_ERR(pack)){
throw ::idl::McpackException(pack) << BSL_EARG;
}
size_t orig_size=mc_pack_get_size(pack);
// @@int32_t user_id=in();
{
int res=mc_pack_put_int32(pack,"user_id",_m_user_id);
if(res){
throw ::idl::McpackException(res) << BSL_EARG;
}
}
// @@string number=in();
{
int res=mc_pack_put_str(pack,"number",_m_number.c_str());
if(res){
throw ::idl::McpackException(res) << BSL_EARG;
}
}
// @@int32_t commodity_id=in();
{
int res=mc_pack_put_int32(pack,"commodity_id",_m_commodity_id);
if(res){
throw ::idl::McpackException(res) << BSL_EARG;
}
}
// @@string expired_time=in();
{
int res=mc_pack_put_str(pack,"expired_time",_m_expired_time.c_str());
if(res){
throw ::idl::McpackException(res) << BSL_EARG;
}
}
// @@int32_t expired_type=in();
{
int res=mc_pack_put_int32(pack,"expired_type",_m_expired_type);
if(res){
throw ::idl::McpackException(res) << BSL_EARG;
}
}
// @@string game_id=in();
{
int res=mc_pack_put_str(pack,"game_id",_m_game_id.c_str());
if(res){
throw ::idl::McpackException(res) << BSL_EARG;
}
}
// save '_unknown' field.
for(size_t i=0;i<_unknown.size();i++){
const mc_pack_item_t& item=_unknown.GetWithoutCheck< mc_pack_item_t >(i);
if(item.type==MC_PT_OBJ){
mc_pack_t* tmp=mc_pack_put_object(pack,mc_pack_get_subkey(item.key));
if(MC_PACK_PTR_ERR(tmp)){
throw ::idl::McpackException(tmp) << BSL_EARG;
}
const mc_pack_t* tmppack=0;
int res=mc_pack_get_pack_from_item(&item,&tmppack);
if(res<0){
throw ::idl::McpackException(res) << BSL_EARG;
}
res=mc_pack_copy_item(tmppack,tmp,0);
if(res<0){
throw ::idl::McpackException(res) << BSL_EARG;
}
mc_pack_finish(tmp);
}else if(item.type==MC_PT_ARR){
mc_pack_t* tmp=mc_pack_put_array(pack,mc_pack_get_subkey(item.key));
if(MC_PACK_PTR_ERR(tmp)){
throw ::idl::McpackException(tmp) << BSL_EARG;
}
const mc_pack_t* tmppack=0;
int res=mc_pack_get_array_from_item(&item,&tmppack);
if(res<0){
throw ::idl::McpackException(res) << BSL_EARG;
}
res=mc_pack_copy_item(tmppack,tmp,0);
if(res<0){
throw ::idl::McpackException(res) << BSL_EARG;
}
mc_pack_finish(tmp);
}else{
int res=mc_pack_put_item(pack,&item);
if(res<0){
throw ::idl::McpackException(res) << BSL_EARG;
}
}
}
// save '_ext' field.
for(size_t i=0;i<_ext.size();i++){
_ext[i]->save(pack);
}
return mc_pack_get_size(pack)-orig_size;
}
// @@class chesshallprops_obtain_exchange_code_result_params
static void _mcpack2_load_chesshallprops_obtain_exchange_code_result_params_success(const mc_pack_item_t*,chesshallprops_obtain_exchange_code_result_params*);
static bsl::readmap<const char*,void (*)(const mc_pack_item_t*,chesshallprops_obtain_exchange_code_result_params*),::idl::string_hash,::idl::string_equal> _mcpack2_load_chesshallprops_obtain_exchange_code_result_params_readmap;
// static initialization.
static bool _mcpack2_load_chesshallprops_obtain_exchange_code_result_params_static_init(){
std::vector< std::pair<const char*, void (*)(const mc_pack_item_t*,chesshallprops_obtain_exchange_code_result_params*)> >_vec;
_vec.push_back(std::pair<const char*,void (*)(const mc_pack_item_t*,chesshallprops_obtain_exchange_code_result_params*)>("success",_mcpack2_load_chesshallprops_obtain_exchange_code_result_params_success));
_mcpack2_load_chesshallprops_obtain_exchange_code_result_params_readmap.create(16*1);
_mcpack2_load_chesshallprops_obtain_exchange_code_result_params_readmap.assign(_vec.begin(),_vec.end());
return true;
}
__attribute__((unused)) static bool _mcpack2_load_chesshallprops_obtain_exchange_code_result_params_static_inited=_mcpack2_load_chesshallprops_obtain_exchange_code_result_params_static_init();
// @@bool success=out();
static void _mcpack2_load_chesshallprops_obtain_exchange_code_result_params_success(__attribute__((unused)) const mc_pack_item_t* item,chesshallprops_obtain_exchange_code_result_params* self){
self->_flag[0 >> 3] |= (1 << (0 & 0x7));
 
__attribute__((unused)) int res=0;
__attribute__((unused)) size_t size=0;
res=mc_pack_get_bool_from_item(item,&(self->_m_success));
if(res){
throw ::idl::McpackWrongTypeException() << BSL_EARG << "success";
}
}
void chesshallprops_obtain_exchange_code_result_params::LoadWithoutCheck(const mc_pack_t* pack){
if(MC_PACK_PTR_ERR(pack)){
throw ::idl::McpackException(pack) << BSL_EARG;
}
clear();
mc_pack_item_t item;
int res=mc_pack_first_item(pack,&item);
if(res && res!=MC_PE_NOT_FOUND){
throw ::idl::McpackException(res) << BSL_EARG;
}
if(res!=MC_PE_NOT_FOUND){
void (*func)(const mc_pack_item_t*,chesshallprops_obtain_exchange_code_result_params*)=0;
do{
if(_mcpack2_load_chesshallprops_obtain_exchange_code_result_params_readmap.get(mc_pack_get_subkey(item.key),&func)==bsl::HASH_EXIST){
func(&item,this);
}else{
bool push=true;
for(size_t i=0;i<_ext.size();i++){
if(_ext[i]->load(item)>=0){
push=false;
}
}
if(push){
unknown().push_back<mc_pack_item_t>(item);
}
}
}while(!mc_pack_next_item(&item,&item));
}
}
//for compatiblity.return 0 when it accepts item,else return -1
int chesshallprops_obtain_exchange_code_result_params::load(const mc_pack_item_t& item){
void (*func)(const mc_pack_item_t*,chesshallprops_obtain_exchange_code_result_params*)=0;
if(_mcpack2_load_chesshallprops_obtain_exchange_code_result_params_readmap.get(mc_pack_get_subkey(item.key),&func)==bsl::HASH_EXIST){
func(&item,this);
return 0;
}
return -1;
}
void chesshallprops_obtain_exchange_code_result_params::load(const mc_pack_t* pack){
LoadWithoutCheck(pack);
check();
}
size_t chesshallprops_obtain_exchange_code_result_params::save(mc_pack_t* pack) const{
if(MC_PACK_PTR_ERR(pack)){
throw ::idl::McpackException(pack) << BSL_EARG;
}
size_t orig_size=mc_pack_get_size(pack);
// @@bool success=out();
{
int res=mc_pack_put_bool(pack,"success",_m_success);
if(res){
throw ::idl::McpackException(res) << BSL_EARG;
}
}
// save '_unknown' field.
for(size_t i=0;i<_unknown.size();i++){
const mc_pack_item_t& item=_unknown.GetWithoutCheck< mc_pack_item_t >(i);
if(item.type==MC_PT_OBJ){
mc_pack_t* tmp=mc_pack_put_object(pack,mc_pack_get_subkey(item.key));
if(MC_PACK_PTR_ERR(tmp)){
throw ::idl::McpackException(tmp) << BSL_EARG;
}
const mc_pack_t* tmppack=0;
int res=mc_pack_get_pack_from_item(&item,&tmppack);
if(res<0){
throw ::idl::McpackException(res) << BSL_EARG;
}
res=mc_pack_copy_item(tmppack,tmp,0);
if(res<0){
throw ::idl::McpackException(res) << BSL_EARG;
}
mc_pack_finish(tmp);
}else if(item.type==MC_PT_ARR){
mc_pack_t* tmp=mc_pack_put_array(pack,mc_pack_get_subkey(item.key));
if(MC_PACK_PTR_ERR(tmp)){
throw ::idl::McpackException(tmp) << BSL_EARG;
}
const mc_pack_t* tmppack=0;
int res=mc_pack_get_array_from_item(&item,&tmppack);
if(res<0){
throw ::idl::McpackException(res) << BSL_EARG;
}
res=mc_pack_copy_item(tmppack,tmp,0);
if(res<0){
throw ::idl::McpackException(res) << BSL_EARG;
}
mc_pack_finish(tmp);
}else{
int res=mc_pack_put_item(pack,&item);
if(res<0){
throw ::idl::McpackException(res) << BSL_EARG;
}
}
}
// save '_ext' field.
for(size_t i=0;i<_ext.size();i++){
_ext[i]->save(pack);
}
return mc_pack_get_size(pack)-orig_size;
}
// @@class chesshallprops_obtain_exchange_code_response
static void _mcpack2_load_chesshallprops_obtain_exchange_code_response_result_params(const mc_pack_item_t*,chesshallprops_obtain_exchange_code_response*);
static bsl::readmap<const char*,void (*)(const mc_pack_item_t*,chesshallprops_obtain_exchange_code_response*),::idl::string_hash,::idl::string_equal> _mcpack2_load_chesshallprops_obtain_exchange_code_response_readmap;
// static initialization.
static bool _mcpack2_load_chesshallprops_obtain_exchange_code_response_static_init(){
std::vector< std::pair<const char*, void (*)(const mc_pack_item_t*,chesshallprops_obtain_exchange_code_response*)> >_vec;
_vec.push_back(std::pair<const char*,void (*)(const mc_pack_item_t*,chesshallprops_obtain_exchange_code_response*)>("result_params",_mcpack2_load_chesshallprops_obtain_exchange_code_response_result_params));
_mcpack2_load_chesshallprops_obtain_exchange_code_response_readmap.create(16*1);
_mcpack2_load_chesshallprops_obtain_exchange_code_response_readmap.assign(_vec.begin(),_vec.end());
return true;
}
__attribute__((unused)) static bool _mcpack2_load_chesshallprops_obtain_exchange_code_response_static_inited=_mcpack2_load_chesshallprops_obtain_exchange_code_response_static_init();
// @@chesshallprops_obtain_exchange_code_result_params result_params;
static void _mcpack2_load_chesshallprops_obtain_exchange_code_response_result_params(__attribute__((unused)) const mc_pack_item_t* item,chesshallprops_obtain_exchange_code_response* self){
self->_flag[0 >> 3] |= (1 << (0 & 0x7));
 
__attribute__((unused)) int res=0;
__attribute__((unused)) size_t size=0;
const mc_pack_t *pack=0;
res=mc_pack_get_pack_from_item(item,&pack);
if(res<0){
throw ::idl::McpackException(res) << BSL_EARG;
}
if(!self->_m_result_params){
self->_m_result_params=::idl::chesshallprops_obtain_exchange_code_result_params::create(self->_pool);
}
self->_m_result_params->LoadWithoutCheck(pack);
}
void chesshallprops_obtain_exchange_code_response::LoadWithoutCheck(const mc_pack_t* pack){
if(MC_PACK_PTR_ERR(pack)){
throw ::idl::McpackException(pack) << BSL_EARG;
}
clear();
mc_pack_item_t item;
int res=mc_pack_first_item(pack,&item);
if(res && res!=MC_PE_NOT_FOUND){
throw ::idl::McpackException(res) << BSL_EARG;
}
if(res!=MC_PE_NOT_FOUND){
void (*func)(const mc_pack_item_t*,chesshallprops_obtain_exchange_code_response*)=0;
do{
if(_mcpack2_load_chesshallprops_obtain_exchange_code_response_readmap.get(mc_pack_get_subkey(item.key),&func)==bsl::HASH_EXIST){
func(&item,this);
}else{
bool push=true;
for(size_t i=0;i<_ext.size();i++){
if(_ext[i]->load(item)>=0){
push=false;
}
}
if(push){
unknown().push_back<mc_pack_item_t>(item);
}
}
}while(!mc_pack_next_item(&item,&item));
}
}
//for compatiblity.return 0 when it accepts item,else return -1
int chesshallprops_obtain_exchange_code_response::load(const mc_pack_item_t& item){
void (*func)(const mc_pack_item_t*,chesshallprops_obtain_exchange_code_response*)=0;
if(_mcpack2_load_chesshallprops_obtain_exchange_code_response_readmap.get(mc_pack_get_subkey(item.key),&func)==bsl::HASH_EXIST){
func(&item,this);
return 0;
}
return -1;
}
void chesshallprops_obtain_exchange_code_response::load(const mc_pack_t* pack){
LoadWithoutCheck(pack);
check();
}
size_t chesshallprops_obtain_exchange_code_response::save(mc_pack_t* pack) const{
if(MC_PACK_PTR_ERR(pack)){
throw ::idl::McpackException(pack) << BSL_EARG;
}
size_t orig_size=mc_pack_get_size(pack);
// @@chesshallprops_obtain_exchange_code_result_params result_params;
{
mc_pack_t* subpack=mc_pack_put_object(pack,"result_params");
if(MC_PACK_PTR_ERR(subpack)){
throw ::idl::McpackException(subpack) << BSL_EARG;
}
_m_result_params->save(subpack);
mc_pack_finish(subpack);
}
// save '_unknown' field.
for(size_t i=0;i<_unknown.size();i++){
const mc_pack_item_t& item=_unknown.GetWithoutCheck< mc_pack_item_t >(i);
if(item.type==MC_PT_OBJ){
mc_pack_t* tmp=mc_pack_put_object(pack,mc_pack_get_subkey(item.key));
if(MC_PACK_PTR_ERR(tmp)){
throw ::idl::McpackException(tmp) << BSL_EARG;
}
const mc_pack_t* tmppack=0;
int res=mc_pack_get_pack_from_item(&item,&tmppack);
if(res<0){
throw ::idl::McpackException(res) << BSL_EARG;
}
res=mc_pack_copy_item(tmppack,tmp,0);
if(res<0){
throw ::idl::McpackException(res) << BSL_EARG;
}
mc_pack_finish(tmp);
}else if(item.type==MC_PT_ARR){
mc_pack_t* tmp=mc_pack_put_array(pack,mc_pack_get_subkey(item.key));
if(MC_PACK_PTR_ERR(tmp)){
throw ::idl::McpackException(tmp) << BSL_EARG;
}
const mc_pack_t* tmppack=0;
int res=mc_pack_get_array_from_item(&item,&tmppack);
if(res<0){
throw ::idl::McpackException(res) << BSL_EARG;
}
res=mc_pack_copy_item(tmppack,tmp,0);
if(res<0){
throw ::idl::McpackException(res) << BSL_EARG;
}
mc_pack_finish(tmp);
}else{
int res=mc_pack_put_item(pack,&item);
if(res<0){
throw ::idl::McpackException(res) << BSL_EARG;
}
}
}
// save '_ext' field.
for(size_t i=0;i<_ext.size();i++){
_ext[i]->save(pack);
}
return mc_pack_get_size(pack)-orig_size;
}
// @@class chesshallprops_use_exchange_code_params
static void _mcpack2_load_chesshallprops_use_exchange_code_params_userId(const mc_pack_item_t*,chesshallprops_use_exchange_code_params*);
static void _mcpack2_load_chesshallprops_use_exchange_code_params_gameId(const mc_pack_item_t*,chesshallprops_use_exchange_code_params*);
static void _mcpack2_load_chesshallprops_use_exchange_code_params_exchangeCodeId(const mc_pack_item_t*,chesshallprops_use_exchange_code_params*);
static bsl::readmap<const char*,void (*)(const mc_pack_item_t*,chesshallprops_use_exchange_code_params*),::idl::string_hash,::idl::string_equal> _mcpack2_load_chesshallprops_use_exchange_code_params_readmap;
// static initialization.
static bool _mcpack2_load_chesshallprops_use_exchange_code_params_static_init(){
std::vector< std::pair<const char*, void (*)(const mc_pack_item_t*,chesshallprops_use_exchange_code_params*)> >_vec;
_vec.push_back(std::pair<const char*,void (*)(const mc_pack_item_t*,chesshallprops_use_exchange_code_params*)>("userId",_mcpack2_load_chesshallprops_use_exchange_code_params_userId));
_vec.push_back(std::pair<const char*,void (*)(const mc_pack_item_t*,chesshallprops_use_exchange_code_params*)>("gameId",_mcpack2_load_chesshallprops_use_exchange_code_params_gameId));
_vec.push_back(std::pair<const char*,void (*)(const mc_pack_item_t*,chesshallprops_use_exchange_code_params*)>("exchangeCodeId",_mcpack2_load_chesshallprops_use_exchange_code_params_exchangeCodeId));
_mcpack2_load_chesshallprops_use_exchange_code_params_readmap.create(16*3);
_mcpack2_load_chesshallprops_use_exchange_code_params_readmap.assign(_vec.begin(),_vec.end());
return true;
}
__attribute__((unused)) static bool _mcpack2_load_chesshallprops_use_exchange_code_params_static_inited=_mcpack2_load_chesshallprops_use_exchange_code_params_static_init();
// @@int32_t userId=in();
static void _mcpack2_load_chesshallprops_use_exchange_code_params_userId(__attribute__((unused)) const mc_pack_item_t* item,chesshallprops_use_exchange_code_params* self){
self->_flag[0 >> 3] |= (1 << (0 & 0x7));
 
__attribute__((unused)) int res=0;
__attribute__((unused)) size_t size=0;
res=mc_pack_get_int32_from_item(item,&(self->_m_userId));
if(res){
throw ::idl::McpackWrongTypeException() << BSL_EARG << "userId";
}
}
// @@string gameId=in();
static void _mcpack2_load_chesshallprops_use_exchange_code_params_gameId(__attribute__((unused)) const mc_pack_item_t* item,chesshallprops_use_exchange_code_params* self){
self->_flag[1 >> 3] |= (1 << (1 & 0x7));
 
__attribute__((unused)) int res=0;
__attribute__((unused)) size_t size=0;
if(item->type!=MC_IT_TXT){
throw ::idl::McpackWrongTypeException() << BSL_EARG << "gameId";
}
self->_m_gameId.set_ptr(item->value,item->value_size-1);
}
// @@string exchangeCodeId=in();
static void _mcpack2_load_chesshallprops_use_exchange_code_params_exchangeCodeId(__attribute__((unused)) const mc_pack_item_t* item,chesshallprops_use_exchange_code_params* self){
self->_flag[2 >> 3] |= (1 << (2 & 0x7));
 
__attribute__((unused)) int res=0;
__attribute__((unused)) size_t size=0;
if(item->type!=MC_IT_TXT){
throw ::idl::McpackWrongTypeException() << BSL_EARG << "exchangeCodeId";
}
self->_m_exchangeCodeId.set_ptr(item->value,item->value_size-1);
}
void chesshallprops_use_exchange_code_params::LoadWithoutCheck(const mc_pack_t* pack){
if(MC_PACK_PTR_ERR(pack)){
throw ::idl::McpackException(pack) << BSL_EARG;
}
clear();
mc_pack_item_t item;
int res=mc_pack_first_item(pack,&item);
if(res && res!=MC_PE_NOT_FOUND){
throw ::idl::McpackException(res) << BSL_EARG;
}
if(res!=MC_PE_NOT_FOUND){
void (*func)(const mc_pack_item_t*,chesshallprops_use_exchange_code_params*)=0;
do{
if(_mcpack2_load_chesshallprops_use_exchange_code_params_readmap.get(mc_pack_get_subkey(item.key),&func)==bsl::HASH_EXIST){
func(&item,this);
}else{
bool push=true;
for(size_t i=0;i<_ext.size();i++){
if(_ext[i]->load(item)>=0){
push=false;
}
}
if(push){
unknown().push_back<mc_pack_item_t>(item);
}
}
}while(!mc_pack_next_item(&item,&item));
}
}
//for compatiblity.return 0 when it accepts item,else return -1
int chesshallprops_use_exchange_code_params::load(const mc_pack_item_t& item){
void (*func)(const mc_pack_item_t*,chesshallprops_use_exchange_code_params*)=0;
if(_mcpack2_load_chesshallprops_use_exchange_code_params_readmap.get(mc_pack_get_subkey(item.key),&func)==bsl::HASH_EXIST){
func(&item,this);
return 0;
}
return -1;
}
void chesshallprops_use_exchange_code_params::load(const mc_pack_t* pack){
LoadWithoutCheck(pack);
check();
}
size_t chesshallprops_use_exchange_code_params::save(mc_pack_t* pack) const{
if(MC_PACK_PTR_ERR(pack)){
throw ::idl::McpackException(pack) << BSL_EARG;
}
size_t orig_size=mc_pack_get_size(pack);
// @@int32_t userId=in();
{
int res=mc_pack_put_int32(pack,"userId",_m_userId);
if(res){
throw ::idl::McpackException(res) << BSL_EARG;
}
}
// @@string gameId=in();
{
int res=mc_pack_put_str(pack,"gameId",_m_gameId.c_str());
if(res){
throw ::idl::McpackException(res) << BSL_EARG;
}
}
// @@string exchangeCodeId=in();
{
int res=mc_pack_put_str(pack,"exchangeCodeId",_m_exchangeCodeId.c_str());
if(res){
throw ::idl::McpackException(res) << BSL_EARG;
}
}
// save '_unknown' field.
for(size_t i=0;i<_unknown.size();i++){
const mc_pack_item_t& item=_unknown.GetWithoutCheck< mc_pack_item_t >(i);
if(item.type==MC_PT_OBJ){
mc_pack_t* tmp=mc_pack_put_object(pack,mc_pack_get_subkey(item.key));
if(MC_PACK_PTR_ERR(tmp)){
throw ::idl::McpackException(tmp) << BSL_EARG;
}
const mc_pack_t* tmppack=0;
int res=mc_pack_get_pack_from_item(&item,&tmppack);
if(res<0){
throw ::idl::McpackException(res) << BSL_EARG;
}
res=mc_pack_copy_item(tmppack,tmp,0);
if(res<0){
throw ::idl::McpackException(res) << BSL_EARG;
}
mc_pack_finish(tmp);
}else if(item.type==MC_PT_ARR){
mc_pack_t* tmp=mc_pack_put_array(pack,mc_pack_get_subkey(item.key));
if(MC_PACK_PTR_ERR(tmp)){
throw ::idl::McpackException(tmp) << BSL_EARG;
}
const mc_pack_t* tmppack=0;
int res=mc_pack_get_array_from_item(&item,&tmppack);
if(res<0){
throw ::idl::McpackException(res) << BSL_EARG;
}
res=mc_pack_copy_item(tmppack,tmp,0);
if(res<0){
throw ::idl::McpackException(res) << BSL_EARG;
}
mc_pack_finish(tmp);
}else{
int res=mc_pack_put_item(pack,&item);
if(res<0){
throw ::idl::McpackException(res) << BSL_EARG;
}
}
}
// save '_ext' field.
for(size_t i=0;i<_ext.size();i++){
_ext[i]->save(pack);
}
return mc_pack_get_size(pack)-orig_size;
}
// @@class chesshallprops_use_exchange_code_result_params
static void _mcpack2_load_chesshallprops_use_exchange_code_result_params_resultCP(const mc_pack_item_t*,chesshallprops_use_exchange_code_result_params*);
static bsl::readmap<const char*,void (*)(const mc_pack_item_t*,chesshallprops_use_exchange_code_result_params*),::idl::string_hash,::idl::string_equal> _mcpack2_load_chesshallprops_use_exchange_code_result_params_readmap;
// static initialization.
static bool _mcpack2_load_chesshallprops_use_exchange_code_result_params_static_init(){
std::vector< std::pair<const char*, void (*)(const mc_pack_item_t*,chesshallprops_use_exchange_code_result_params*)> >_vec;
_vec.push_back(std::pair<const char*,void (*)(const mc_pack_item_t*,chesshallprops_use_exchange_code_result_params*)>("resultCP",_mcpack2_load_chesshallprops_use_exchange_code_result_params_resultCP));
_mcpack2_load_chesshallprops_use_exchange_code_result_params_readmap.create(16*1);
_mcpack2_load_chesshallprops_use_exchange_code_result_params_readmap.assign(_vec.begin(),_vec.end());
return true;
}
__attribute__((unused)) static bool _mcpack2_load_chesshallprops_use_exchange_code_result_params_static_inited=_mcpack2_load_chesshallprops_use_exchange_code_result_params_static_init();
// @@result_cp resultCP=out();
static void _mcpack2_load_chesshallprops_use_exchange_code_result_params_resultCP(__attribute__((unused)) const mc_pack_item_t* item,chesshallprops_use_exchange_code_result_params* self){
self->_flag[0 >> 3] |= (1 << (0 & 0x7));
 
__attribute__((unused)) int res=0;
__attribute__((unused)) size_t size=0;
const mc_pack_t *pack=0;
res=mc_pack_get_pack_from_item(item,&pack);
if(res<0){
throw ::idl::McpackException(res) << BSL_EARG;
}
if(!self->_m_resultCP){
self->_m_resultCP=::idl::result_cp::create(self->_pool);
}
self->_m_resultCP->LoadWithoutCheck(pack);
}
void chesshallprops_use_exchange_code_result_params::LoadWithoutCheck(const mc_pack_t* pack){
if(MC_PACK_PTR_ERR(pack)){
throw ::idl::McpackException(pack) << BSL_EARG;
}
clear();
mc_pack_item_t item;
int res=mc_pack_first_item(pack,&item);
if(res && res!=MC_PE_NOT_FOUND){
throw ::idl::McpackException(res) << BSL_EARG;
}
if(res!=MC_PE_NOT_FOUND){
void (*func)(const mc_pack_item_t*,chesshallprops_use_exchange_code_result_params*)=0;
do{
if(_mcpack2_load_chesshallprops_use_exchange_code_result_params_readmap.get(mc_pack_get_subkey(item.key),&func)==bsl::HASH_EXIST){
func(&item,this);
}else{
bool push=true;
for(size_t i=0;i<_ext.size();i++){
if(_ext[i]->load(item)>=0){
push=false;
}
}
if(push){
unknown().push_back<mc_pack_item_t>(item);
}
}
}while(!mc_pack_next_item(&item,&item));
}
}
//for compatiblity.return 0 when it accepts item,else return -1
int chesshallprops_use_exchange_code_result_params::load(const mc_pack_item_t& item){
void (*func)(const mc_pack_item_t*,chesshallprops_use_exchange_code_result_params*)=0;
if(_mcpack2_load_chesshallprops_use_exchange_code_result_params_readmap.get(mc_pack_get_subkey(item.key),&func)==bsl::HASH_EXIST){
func(&item,this);
return 0;
}
return -1;
}
void chesshallprops_use_exchange_code_result_params::load(const mc_pack_t* pack){
LoadWithoutCheck(pack);
check();
}
size_t chesshallprops_use_exchange_code_result_params::save(mc_pack_t* pack) const{
if(MC_PACK_PTR_ERR(pack)){
throw ::idl::McpackException(pack) << BSL_EARG;
}
size_t orig_size=mc_pack_get_size(pack);
// @@result_cp resultCP=out();
{
mc_pack_t* subpack=mc_pack_put_object(pack,"resultCP");
if(MC_PACK_PTR_ERR(subpack)){
throw ::idl::McpackException(subpack) << BSL_EARG;
}
_m_resultCP->save(subpack);
mc_pack_finish(subpack);
}
// save '_unknown' field.
for(size_t i=0;i<_unknown.size();i++){
const mc_pack_item_t& item=_unknown.GetWithoutCheck< mc_pack_item_t >(i);
if(item.type==MC_PT_OBJ){
mc_pack_t* tmp=mc_pack_put_object(pack,mc_pack_get_subkey(item.key));
if(MC_PACK_PTR_ERR(tmp)){
throw ::idl::McpackException(tmp) << BSL_EARG;
}
const mc_pack_t* tmppack=0;
int res=mc_pack_get_pack_from_item(&item,&tmppack);
if(res<0){
throw ::idl::McpackException(res) << BSL_EARG;
}
res=mc_pack_copy_item(tmppack,tmp,0);
if(res<0){
throw ::idl::McpackException(res) << BSL_EARG;
}
mc_pack_finish(tmp);
}else if(item.type==MC_PT_ARR){
mc_pack_t* tmp=mc_pack_put_array(pack,mc_pack_get_subkey(item.key));
if(MC_PACK_PTR_ERR(tmp)){
throw ::idl::McpackException(tmp) << BSL_EARG;
}
const mc_pack_t* tmppack=0;
int res=mc_pack_get_array_from_item(&item,&tmppack);
if(res<0){
throw ::idl::McpackException(res) << BSL_EARG;
}
res=mc_pack_copy_item(tmppack,tmp,0);
if(res<0){
throw ::idl::McpackException(res) << BSL_EARG;
}
mc_pack_finish(tmp);
}else{
int res=mc_pack_put_item(pack,&item);
if(res<0){
throw ::idl::McpackException(res) << BSL_EARG;
}
}
}
// save '_ext' field.
for(size_t i=0;i<_ext.size();i++){
_ext[i]->save(pack);
}
return mc_pack_get_size(pack)-orig_size;
}
// @@class chesshallprops_use_exchange_code_response
static void _mcpack2_load_chesshallprops_use_exchange_code_response_result_params(const mc_pack_item_t*,chesshallprops_use_exchange_code_response*);
static bsl::readmap<const char*,void (*)(const mc_pack_item_t*,chesshallprops_use_exchange_code_response*),::idl::string_hash,::idl::string_equal> _mcpack2_load_chesshallprops_use_exchange_code_response_readmap;
// static initialization.
static bool _mcpack2_load_chesshallprops_use_exchange_code_response_static_init(){
std::vector< std::pair<const char*, void (*)(const mc_pack_item_t*,chesshallprops_use_exchange_code_response*)> >_vec;
_vec.push_back(std::pair<const char*,void (*)(const mc_pack_item_t*,chesshallprops_use_exchange_code_response*)>("result_params",_mcpack2_load_chesshallprops_use_exchange_code_response_result_params));
_mcpack2_load_chesshallprops_use_exchange_code_response_readmap.create(16*1);
_mcpack2_load_chesshallprops_use_exchange_code_response_readmap.assign(_vec.begin(),_vec.end());
return true;
}
__attribute__((unused)) static bool _mcpack2_load_chesshallprops_use_exchange_code_response_static_inited=_mcpack2_load_chesshallprops_use_exchange_code_response_static_init();
// @@chesshallprops_use_exchange_code_result_params result_params;
static void _mcpack2_load_chesshallprops_use_exchange_code_response_result_params(__attribute__((unused)) const mc_pack_item_t* item,chesshallprops_use_exchange_code_response* self){
self->_flag[0 >> 3] |= (1 << (0 & 0x7));
 
__attribute__((unused)) int res=0;
__attribute__((unused)) size_t size=0;
const mc_pack_t *pack=0;
res=mc_pack_get_pack_from_item(item,&pack);
if(res<0){
throw ::idl::McpackException(res) << BSL_EARG;
}
if(!self->_m_result_params){
self->_m_result_params=::idl::chesshallprops_use_exchange_code_result_params::create(self->_pool);
}
self->_m_result_params->LoadWithoutCheck(pack);
}
void chesshallprops_use_exchange_code_response::LoadWithoutCheck(const mc_pack_t* pack){
if(MC_PACK_PTR_ERR(pack)){
throw ::idl::McpackException(pack) << BSL_EARG;
}
clear();
mc_pack_item_t item;
int res=mc_pack_first_item(pack,&item);
if(res && res!=MC_PE_NOT_FOUND){
throw ::idl::McpackException(res) << BSL_EARG;
}
if(res!=MC_PE_NOT_FOUND){
void (*func)(const mc_pack_item_t*,chesshallprops_use_exchange_code_response*)=0;
do{
if(_mcpack2_load_chesshallprops_use_exchange_code_response_readmap.get(mc_pack_get_subkey(item.key),&func)==bsl::HASH_EXIST){
func(&item,this);
}else{
bool push=true;
for(size_t i=0;i<_ext.size();i++){
if(_ext[i]->load(item)>=0){
push=false;
}
}
if(push){
unknown().push_back<mc_pack_item_t>(item);
}
}
}while(!mc_pack_next_item(&item,&item));
}
}
//for compatiblity.return 0 when it accepts item,else return -1
int chesshallprops_use_exchange_code_response::load(const mc_pack_item_t& item){
void (*func)(const mc_pack_item_t*,chesshallprops_use_exchange_code_response*)=0;
if(_mcpack2_load_chesshallprops_use_exchange_code_response_readmap.get(mc_pack_get_subkey(item.key),&func)==bsl::HASH_EXIST){
func(&item,this);
return 0;
}
return -1;
}
void chesshallprops_use_exchange_code_response::load(const mc_pack_t* pack){
LoadWithoutCheck(pack);
check();
}
size_t chesshallprops_use_exchange_code_response::save(mc_pack_t* pack) const{
if(MC_PACK_PTR_ERR(pack)){
throw ::idl::McpackException(pack) << BSL_EARG;
}
size_t orig_size=mc_pack_get_size(pack);
// @@chesshallprops_use_exchange_code_result_params result_params;
{
mc_pack_t* subpack=mc_pack_put_object(pack,"result_params");
if(MC_PACK_PTR_ERR(subpack)){
throw ::idl::McpackException(subpack) << BSL_EARG;
}
_m_result_params->save(subpack);
mc_pack_finish(subpack);
}
// save '_unknown' field.
for(size_t i=0;i<_unknown.size();i++){
const mc_pack_item_t& item=_unknown.GetWithoutCheck< mc_pack_item_t >(i);
if(item.type==MC_PT_OBJ){
mc_pack_t* tmp=mc_pack_put_object(pack,mc_pack_get_subkey(item.key));
if(MC_PACK_PTR_ERR(tmp)){
throw ::idl::McpackException(tmp) << BSL_EARG;
}
const mc_pack_t* tmppack=0;
int res=mc_pack_get_pack_from_item(&item,&tmppack);
if(res<0){
throw ::idl::McpackException(res) << BSL_EARG;
}
res=mc_pack_copy_item(tmppack,tmp,0);
if(res<0){
throw ::idl::McpackException(res) << BSL_EARG;
}
mc_pack_finish(tmp);
}else if(item.type==MC_PT_ARR){
mc_pack_t* tmp=mc_pack_put_array(pack,mc_pack_get_subkey(item.key));
if(MC_PACK_PTR_ERR(tmp)){
throw ::idl::McpackException(tmp) << BSL_EARG;
}
const mc_pack_t* tmppack=0;
int res=mc_pack_get_array_from_item(&item,&tmppack);
if(res<0){
throw ::idl::McpackException(res) << BSL_EARG;
}
res=mc_pack_copy_item(tmppack,tmp,0);
if(res<0){
throw ::idl::McpackException(res) << BSL_EARG;
}
mc_pack_finish(tmp);
}else{
int res=mc_pack_put_item(pack,&item);
if(res<0){
throw ::idl::McpackException(res) << BSL_EARG;
}
}
}
// save '_ext' field.
for(size_t i=0;i<_ext.size();i++){
_ext[i]->save(pack);
}
return mc_pack_get_size(pack)-orig_size;
}
} // namespace idl
