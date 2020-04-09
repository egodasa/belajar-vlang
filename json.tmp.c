 

#include <stdio.h>  // TODO remove all these includes, define all function signatures and types manually
#include <stdlib.h>
#include <signal.h>
#include <stdarg.h> // for va_list
#include <inttypes.h>  // int64_t etc
#include <string.h> // memcpy

#ifndef _WIN32
#include <ctype.h>
#include <locale.h> // tolower
#include <sys/time.h>
#include <unistd.h> // sleep	
#endif


#ifdef __APPLE__
#include <libproc.h> // proc_pidpath
#include <execinfo.h> // backtrace and backtrace_symbols_fd
#endif

#ifdef __linux__
#ifndef __BIONIC__
#include <execinfo.h> // backtrace and backtrace_symbols_fd
#endif
#pragma weak backtrace
#pragma weak backtrace_symbols_fd
#endif


#ifdef __linux__
#include <sys/types.h>
#include <sys/wait.h> // os__wait uses wait on nix
#endif

#define EMPTY_STRUCT_DECLARATION
#define EMPTY_STRUCT_INITIALIZATION 0
#ifdef __TINYC__
#undef EMPTY_STRUCT_INITIALIZATION
#define EMPTY_STRUCT_INITIALIZATION
#endif

#define OPTION_CAST(x) (x)

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

// must be included after <windows.h>
#ifndef __TINYC__
#include <shellapi.h>
#endif

#include <io.h> // _waccess
#include <fcntl.h> // _O_U8TEXT
#include <direct.h> // _wgetcwd
//#include <WinSock2.h>
#ifdef _MSC_VER
// On MSVC these are the same (as long as /volatile:ms is passed)
#define _Atomic volatile

// MSVC cannot parse some things properly
#undef EMPTY_STRUCT_DECLARATION
#undef OPTION_CAST

#define EMPTY_STRUCT_DECLARATION int ____dummy_variable
#define OPTION_CAST(x)
#endif

void pthread_mutex_lock(HANDLE *m) {
	WaitForSingleObject(*m, INFINITE);
}

void pthread_mutex_unlock(HANDLE *m) {
	ReleaseMutex(*m);
}
#else
#include <pthread.h>
#endif

//================================== TYPEDEFS ================================*/

typedef int64_t i64;
typedef int16_t i16;
typedef int8_t i8;
typedef uint64_t u64;
typedef uint32_t u32;
typedef uint16_t u16;
typedef uint8_t byte;
typedef uint32_t rune;
typedef float f32;
typedef double f64;
typedef unsigned char* byteptr;
typedef int* intptr;
typedef void* voidptr;
typedef struct array array;
typedef struct map map;
typedef array array_string;
typedef array array_int;
typedef array array_byte;
typedef array array_f32;
typedef array array_f64;
typedef map map_int;
typedef map map_string;
#ifndef bool
	typedef int bool;
	#define true 1
	#define false 0
#endif

//============================== HELPER C MACROS =============================*/

#define _PUSH(arr, val, tmp, tmp_typ) {tmp_typ tmp = (val); array__push(arr, &tmp);}
#define _PUSH_MANY(arr, val, tmp, tmp_typ) {tmp_typ tmp = (val); array__push_many(arr, tmp.data, tmp.len);}
#define _IN(typ, val, arr) array_##typ##_contains(arr, val)
#define _IN_MAP(val, m) map__exists(m, val)
//#define ALLOC_INIT(type, ...) (type *)memdup((type[]){ __VA_ARGS__ }, sizeof(type))

//================================== GLOBALS =================================*/
byteptr g_str_buf;
int load_so(byteptr);
void reload_so();
void init_consts();


 int g_test_ok = 1; 
 
#define js_get(object, key) cJSON_GetObjectItemCaseSensitive((object), (key))

#include <float.h>
#include <math.h>
#include "cJSON.h"
typedef struct array array;
typedef array array_string;
typedef array array_byte;
typedef array array_int;
typedef struct string string;
typedef struct ustring ustring;
typedef struct map map;
typedef struct mapnode mapnode;
typedef struct Option Option;
typedef struct strings__Builder strings__Builder;
typedef struct User User;
typedef array array_User;
typedef Option Option_array_User;
struct string {
	byte* str;
	int len;
};

struct array {
	void* data;
	int len;
	int cap;
	int element_size;
};

struct map {
	int element_size;
	mapnode* root;
	int size;
};

struct Option {
	byte data  [255 ];
	string error;
	bool ok;
};


//----
struct User {
	string name;
	int age;
};

struct mapnode {
	mapnode* left;
	mapnode* right;
	bool is_empty;
	string key;
	void* val;
};

struct strings__Builder {
	array_byte buf;
	int len;
};

struct ustring {
	string s;
	array_int runes;
	int len;
};



string _STR(const char*, ...);


string _STR_TMP(const char*, ...);

array new_array(int mylen, int cap, int elm_size);
array _make(int len, int cap, int elm_size);
array new_array_from_c_array(int len, int cap, int elm_size, void* c_array);
array new_array_from_c_array_no_alloc(int len, int cap, int elm_size, void* c_array);
array array_repeat_old(void* val, int nr_repeats, int elm_size);
array array_repeat(array a, int nr_repeats);
array array_repeat2(array a, int nr_repeats);
void array_sort_with_compare(array* a, void* compare);
void array_insert(array* a, int i, void* val);
void array_prepend(array* a, void* val);
void v_array_delete(array* a, int idx);
void* array__get(array a, int i);
void* array_first(array a);
void* array_last(array a);
array array_left(array s, int n);
array array_right(array s, int n);
array array_slice(array s, int start, int _end);
void array_set(array* a, int idx, void* val);
void array__push(array* arr, void* val);
void array__push_many(array* arr, void* val, int size);
array array_reverse(array a);
array array_clone(array a);
void v_array_free(array a);
string array_string_str(array_string a);
string array_byte_hex(array_byte b);
int copy(array_byte dst, array_byte src);
int compare_ints(int* a, int* b);
void array_int_sort(array_int* a);
int vstrlen(byte* s);
void todo();
string tos(byte* s, int len);
string tos_clone(byte* s);
string tos2(byte* s);
string string_clone(string a);
string string_replace(string s, string rep, string with);
int v_string_int(string s);
i64 string_i64(string s);
f32 string_f32(string s);
f64 string_f64(string s);
u32 string_u32(string s);
u64 string_u64(string s);
bool string_eq(string s, string a);
bool string_ne(string s, string a);
bool string_lt(string s, string a);
bool string_le(string s, string a);
bool string_gt(string s, string a);
bool string_ge(string s, string a);
string string_add(string s, string a);
array_string string_split(string s, string delim);
array_string string_split_single(string s, byte delim);
array_string string_split_into_lines(string s);
string string_left(string s, int n);
string string_right(string s, int n);
string string_substr(string s, int start, int end);
int string_index_old(string s, string p);
int string_index(string s, string p);
int string_index_any(string s, string chars);
int string_last_index(string s, string p);
int string_index_after(string s, string p, int start);
int string_count(string s, string substr);
bool string_contains(string s, string p);
bool string_starts_with(string s, string p);
bool string_ends_with(string s, string p);
string string_to_lower(string s);
string string_to_upper(string s);
string string_capitalize(string s);
string string_title(string s);
string string_find_between(string s, string start, string end);
bool array_string_contains(array_string ar, string val);
bool array_int_contains(array_int ar, int val);
bool is_space(byte c);
bool byte_is_space(byte c);
string string_trim_space(string s);
string string_trim(string s, string cutset);
string string_trim_left(string s, string cutset);
string string_trim_right(string s, string cutset);
int compare_strings(string* a, string* b);
int compare_strings_by_len(string* a, string* b);
int compare_lower_strings(string* a, string* b);
void array_string_sort(array_string* s);
void array_string_sort_ignore_case(array_string* s);
void array_string_sort_by_len(array_string* s);
ustring string_ustring(string s);
ustring string_ustring_tmp(string s);
string ustring_substr(ustring u, int _start, int _end);
string ustring_left(ustring u, int pos);
string ustring_right(ustring u, int pos);
byte string_at(string s, int idx);
string ustring_at(ustring u, int idx);
void v_ustring_free(ustring u);
bool byte_is_digit(byte c);
bool byte_is_hex_digit(byte c);
bool byte_is_oct_digit(byte c);
bool byte_is_letter(byte c);
void v_string_free(string s);
string string_all_before(string s, string dot);
string string_all_before_last(string s, string dot);
string string_all_after(string s, string dot);
string array_string_join(array_string a, string del);
string array_string_join_lines(array_string s);
string string_reverse(string s);
string string_limit(string s, int max);
bool byte_is_white(byte c);
int string_hash(string s);
array_byte string_bytes(string s);
void v_exit(int code);
bool isnil(void* v);
void on_panic(int (*f)( int  /*FFF*/ ));
void print_backtrace_skipping_top_frames(int skipframes);
void print_backtrace();
void _panic_debug(int line_no, string file, string mod, string fn_name, string s);
void v_panic(string s);
void println(string s);
void eprintln(string s);
void print(string s);
byte* v_malloc(int n);
byte* v_calloc(int n);
void v_free(void* ptr);
void* memdup(void* src, int sz);
void v_ptr_free(void* ptr);
string f64_str(f64 d);
string f32_str(f32 d);
string ptr_str(void* ptr);
bool f64_eq(f64 a, f64 b);
string int_str(int nn);
string u32_str(u32 nn);
string i64_str(i64 nn);
string u64_str(u64 nn);
string bool_str(bool b);
string int_hex(int n);
string i64_hex(i64 n);
bool array_byte_contains(array_byte a, byte val);
string rune_str(rune c);
string byte_str(byte c);
bool byte_is_capital(byte c);
array_byte array_byte_clone(array_byte b);
int utf8_char_len(byte b);
string utf32_to_str(u32 code);
string utf32_to_str_no_malloc(u32 code, void* buf);
int string_utf32_code(string _rune);
u16* string_to_wide(string _str);
string string_from_wide(u16* _wstr);
string string_from_wide2(u16* _wstr, int len);
int utf8_len(byte c);
int utf8_getchar();
map new_map(int cap, int elm_size);
map new_map_init(int cap, int elm_size, string* keys, void* vals);
mapnode* new_node(string key, void* val, int element_size);
void map_insert(map* m, mapnode* n, string key, void* val);
bool mapnode_find(mapnode* n, string key, void* out, int element_size);
bool mapnode_find2(mapnode* n, string key, int element_size);
void map__set(map* m, string key, void* val);
int preorder_keys(mapnode* node, array_string* keys, int key_i);
array_string map_keys(map* m);
bool map_get(map m, string key, void* out);
void v_mapnode_delete(mapnode* n, string key, int element_size);
void v_map_delete(map* m, string key);
void map_exists(map m, string key);
bool map__exists(map m, string key);
void map_print(map m);
void v_mapnode_free(mapnode* n);
void v_map_free(map* m);
string map_string_str(map_string m);
Option opt_ok(void* data, int size);
Option v_error(string s);
int json__jsdecode_int(cJSON* root);
i8 json__jsdecode_i8(cJSON* root);
i16 json__jsdecode_i16(cJSON* root);
i64 json__jsdecode_i64(cJSON* root);
byte json__jsdecode_byte(cJSON* root);
u16 json__jsdecode_u16(cJSON* root);
u32 json__jsdecode_u32(cJSON* root);
u64 json__jsdecode_u64(cJSON* root);
f32 json__jsdecode_f32(cJSON* root);
f64 json__jsdecode_f64(cJSON* root);
string json__jsdecode_string(cJSON* root);
bool json__jsdecode_bool(cJSON* root);
cJSON* json__jsencode_int(int val);
cJSON* json__jsencode_i8(i8 val);
cJSON* json__jsencode_i16(i16 val);
cJSON* json__jsencode_i64(i64 val);
cJSON* json__jsencode_byte(byte val);
cJSON* json__jsencode_u16(u16 val);
cJSON* json__jsencode_u32(u32 val);
cJSON* json__jsencode_u64(u64 val);
cJSON* json__jsencode_f32(f32 val);
cJSON* json__jsencode_f64(f64 val);
cJSON* json__jsencode_bool(bool val);
cJSON* json__jsencode_string(string val);
cJSON* json__json_parse(string s);
string json__json_print(cJSON* json);
strings__Builder strings__new_builder(int initial_size);
void strings__Builder_write(strings__Builder* b, string s);
void strings__Builder_writeln(strings__Builder* b, string s);
string strings__Builder_str(strings__Builder b);
void strings__Builder_cut(strings__Builder* b, int n);
void strings__Builder_free(strings__Builder* b);
int strings__levenshtein_distance(string a, string b);
f32 strings__levenshtein_distance_percentage(string a, string b);
f32 strings__dice_coefficient(string s1, string s2);
string strings__repeat(byte c, int n);

//User json__jsdecode_User(cJSON* root) {
Option json__jsdecode_User(cJSON* root, User* res) {
//  User res;
  if (!root) {
    const char *error_ptr = cJSON_GetErrorPtr();
    if (error_ptr != NULL)	{
      fprintf(stderr, "Error in decode() for User error_ptr=: %%s\n", error_ptr);
//      printf("\nbad js=%%s\n", js.str);
      return v_error(tos2(error_ptr));
    }
  }
 res->name = json__jsdecode_string(js_get(root, "name"));
 res->age = json__jsdecode_int(js_get(root, "age"));
 return opt_ok(res, sizeof(*res)); 
}
/*enc start*/ 
cJSON* json__jsencode_User(User val) {
cJSON *o = cJSON_CreateObject();
string res = tos2("");
  cJSON_AddItemToObject(o,  "name",json__jsencode_string(val.name)); 
  cJSON_AddItemToObject(o,  "age",json__jsencode_int(val.age)); 
 return o;}

//array_User json__jsdecode_array_User(cJSON* root) {
Option json__jsdecode_array_User(cJSON* root, array_User* res) {
//  array_User res;
  if (!root) {
    const char *error_ptr = cJSON_GetErrorPtr();
    if (error_ptr != NULL)	{
      fprintf(stderr, "Error in decode() for array_User error_ptr=: %%s\n", error_ptr);
//      printf("\nbad js=%%s\n", js.str);
      return v_error(tos2(error_ptr));
    }
  }

*res = new_array(0, 0, sizeof(User));
const cJSON *jsval = NULL;
cJSON_ArrayForEach(jsval, root)
{
  User val; json__jsdecode_User(jsval, &val); 
  array__push(res, &val);
}
 return opt_ok(res, sizeof(*res)); 
}
/*enc start*/ 
cJSON* json__jsencode_array_User(array_User val) {
cJSON *o = cJSON_CreateObject();
string res = tos2("");

o = cJSON_CreateArray();
for (int i = 0; i < val.len; i++){
  cJSON_AddItemToArray(o, json__jsencode_User(  ((User*)val.data)[i]  ));
}
 return o;}
array_int g_ustring_runes; // global
i64 total_m =  0; // global
#define builtin__CP_UTF8  65001


 array new_array(int mylen, int cap, int elm_size) {
 
array arr= (array) { .len =  mylen , .cap =  cap , .element_size =  elm_size , .data =  v_calloc ( cap * elm_size ) } ;
 
return  arr ;
 
 }
 array _make(int len, int cap, int elm_size) {
 
return  new_array ( len , cap , elm_size ) ;
 
 }
 array new_array_from_c_array(int len, int cap, int elm_size, void* c_array) {
 
array arr= (array) { .len =  len , .cap =  cap , .element_size =  elm_size , .data =  v_malloc ( cap * elm_size ) } ;
 
 memcpy ( arr .data ,  c_array ,  len * elm_size ) ;
 
return  arr ;
 
 }
 array new_array_from_c_array_no_alloc(int len, int cap, int elm_size, void* c_array) {
 
array arr= (array) { .len =  len , .cap =  cap , .element_size =  elm_size , .data =  c_array } ;
 
return  arr ;
 
 }
 array array_repeat_old(void* val, int nr_repeats, int elm_size) {
 
array arr= (array) { .len =  nr_repeats , .cap =  nr_repeats , .element_size =  elm_size , .data =  v_malloc ( nr_repeats * elm_size ) } ;
 
 for (
int i= 0  ;  i < nr_repeats  ;  i ++ ) { 
 
 
 memcpy ((byte*) arr .data + i * elm_size ,  val ,  elm_size ) ;
 
 }
 ;
 
return  arr ;
 
 }
 array array_repeat(array a, int nr_repeats) {
 
array arr= (array) { .len =  nr_repeats , .cap =  nr_repeats , .element_size =  a .element_size , .data =  v_malloc ( nr_repeats * a .element_size ) } ;
 
void* val=(byte*) a .data + 0 ;
 
 for (
int i= 0  ;  i < nr_repeats  ;  i ++ ) { 
 
 
 memcpy ((byte*) arr .data + i * a .element_size ,  val ,  a .element_size ) ;
 
 }
 ;
 
return  arr ;
 
 }
 array array_repeat2(array a, int nr_repeats) {
 
array arr= (array) { .len =  nr_repeats , .cap =  nr_repeats , .element_size =  a .element_size , .data =  v_malloc ( nr_repeats * a .element_size ) } ;
 
void* val=(byte*) a .data + 0 ;
 
 for (
int i= 0  ;  i < nr_repeats  ;  i ++ ) { 
 
 
 memcpy ((byte*) arr .data + i * a .element_size ,  val ,  a .element_size ) ;
 
 }
 ;
 
return  arr ;
 
 }
 void array_sort_with_compare(array* a, void* compare) {
 
 qsort ( a ->data ,  a ->len ,  a ->element_size ,  compare ) ;
 
 }
 void array_insert(array* a, int i, void* val) {
 
 if ( i >= a ->len ) {
 
 v_panic ( tos2((byte*)"array.insert: index larger than length") ) ;
 
 }
 ;
 
 array__push( a , val ) ;
 
int size= a ->element_size ;
 
 memmove ((byte*) a ->data + ( i + 1 ) * size , (byte*) a ->data + i * size ,  ( a ->len - i ) * size ) ;
 
 array_set( a , i , val ) ;
 
 }
 void array_prepend(array* a, void* val) {
 
 array_insert( a , 0 , val ) ;
 
 }
 void v_array_delete(array* a, int idx) {
 
int size= a ->element_size ;
 
 memmove ((byte*) a ->data + idx * size , (byte*) a ->data + ( idx + 1 ) * size ,  ( a ->len - idx ) * size ) ;
 
 a ->len -- ;
 
 a ->cap -- ;
 
 }
 void* array__get(array a, int i) {
 
 if ( i < 0  ||  i >= a .len ) {
 
 v_panic ( _STR("array index out of range: %d/%d", i, a .len) ) ;
 
 }
 ;
 
return (byte*) a .data + i * a .element_size ;
 
 }
 void* array_first(array a) {
 
 if ( a .len == 0 ) {
 
 v_panic ( tos2((byte*)"array.first: empty array") ) ;
 
 }
 ;
 
return (byte*) a .data + 0 ;
 
 }
 void* array_last(array a) {
 
 if ( a .len == 0 ) {
 
 v_panic ( tos2((byte*)"array.last: empty array") ) ;
 
 }
 ;
 
return (byte*) a .data + ( a .len - 1 ) * a .element_size ;
 
 }
 array array_left(array s, int n) {
 
 if ( n >= s .len ) {
 
return  s ;
 
 }
 ;
 
return  array_slice( s , 0 , n ) ;
 
 }
 array array_right(array s, int n) {
 
 if ( n >= s .len ) {
 
return  s ;
 
 }
 ;
 
return  array_slice( s , n , s .len ) ;
 
 }
 array array_slice(array s, int start, int _end) {
 
int end= _end ;
 
 if ( start > end ) {
 
 v_panic ( _STR("invalid slice index: %d > %d", start, end) ) ;
 
 }
 ;
 
 if ( end > s .len ) {
 
 v_panic ( _STR("runtime error: slice bounds out of range (%d >= %d)", end, s .len) ) ;
 
 }
 ;
 
 if ( start < 0 ) {
 
 v_panic ( _STR("runtime error: slice bounds out of range (%d < 0)", start) ) ;
 
 }
 ;
 
int l= end - start ;
 
array res= (array) { .element_size =  s .element_size , .data = (byte*) s .data + start * s .element_size , .len =  l , .cap =  l } ;
 
return  res ;
 
 }
 void array_set(array* a, int idx, void* val) {
 
 if ( idx < 0  ||  idx >= a ->len ) {
 
 v_panic ( _STR("array index out of range: %d / %d", idx, a ->len) ) ;
 
 }
 ;
 
 memcpy ((byte*) a ->data + a ->element_size * idx ,  val ,  a ->element_size ) ;
 
 }
 void array__push(array* arr, void* val) {
 
 if ( arr ->len >= arr ->cap - 1 ) {
 
int cap= ( arr ->len + 1 ) * 2 ;
 
 if ( arr ->cap == 0 ) {
 
 arr ->data  =  v_malloc ( cap * arr ->element_size ) ;
 
 }
  else { 
 
 arr ->data  =  realloc ( arr ->data ,  cap * arr ->element_size ) ;
 
 }
 ;
 
 arr ->cap  =  cap ;
 
 }
 ;
 
 memcpy ((byte*) arr ->data + arr ->element_size * arr ->len ,  val ,  arr ->element_size ) ;
 
 arr ->len ++ ;
 
 }
 void array__push_many(array* arr, void* val, int size) {
 
 if ( arr ->len >= arr ->cap - size ) {
 
int cap= ( arr ->len + size ) * 2 ;
 
 if ( arr ->cap == 0 ) {
 
 arr ->data  =  v_malloc ( cap * arr ->element_size ) ;
 
 }
  else { 
 
 arr ->data  =  realloc ( arr ->data ,  cap * arr ->element_size ) ;
 
 }
 ;
 
 arr ->cap  =  cap ;
 
 }
 ;
 
 memcpy ((byte*) arr ->data + arr ->element_size * arr ->len ,  val ,  arr ->element_size * size ) ;
 
 arr ->len  +=  size ;
 
 }
 array array_reverse(array a) {
 
array arr= (array) { .len =  a .len , .cap =  a .cap , .element_size =  a .element_size , .data =  v_malloc ( a .cap * a .element_size ) } ;
 
 for (
int i= 0  ;  i < a .len  ;  i ++ ) { 
 
 
 memcpy ((byte*) arr .data + i * arr .element_size ,  & /*vvar*/  ( *(array*) array__get( a , a .len - 1 - i) ) ,  arr .element_size ) ;
 
 }
 ;
 
return  arr ;
 
 }
 array array_clone(array a) {
 
array arr= (array) { .len =  a .len , .cap =  a .cap , .element_size =  a .element_size , .data =  v_malloc ( a .cap * a .element_size ) } ;
 
 memcpy ( arr .data ,  a .data ,  a .cap * a .element_size ) ;
 
return  arr ;
 
 }
 void v_array_free(array a) {
 
 free ( a .data ) ;
 
 }
 string array_string_str(array_string a) {
 
strings__Builder sb= strings__new_builder ( a .len * 3 ) ;
 
 strings__Builder_write(& /* ? */ sb , tos2((byte*)"[") ) ;
 
 for (
int i= 0  ;  i < a .len  ;  i ++ ) { 
 
 
string val= ( *(string*) array__get( a , i) ) ;
 
 strings__Builder_write(& /* ? */ sb , tos2((byte*)"\"") ) ;
 
 strings__Builder_write(& /* ? */ sb , val ) ;
 
 strings__Builder_write(& /* ? */ sb , tos2((byte*)"\"") ) ;
 
 if ( i < a .len - 1 ) {
 
 strings__Builder_write(& /* ? */ sb , tos2((byte*)", ") ) ;
 
 }
 ;
 
 }
 ;
 
 strings__Builder_write(& /* ? */ sb , tos2((byte*)"]") ) ;
 
return  strings__Builder_str( sb ) ;
 
 }
 string array_byte_hex(array_byte b) {
 
byte* hex= v_malloc ( b .len * 2 + 1 ) ;
 
byte* ptr= & /*vvar*/  hex [/*ptr*/ 0 ]/*rbyte 1*/ ;
 
 for (
int i= 0  ;  i < b .len  ;  i ++ ) { 
 
 
 ptr  +=  sprintf ( ((char*)( ptr ) ) ,  "%02x" ,  ( *(byte*) array__get( b , i) ) ) ;
 
 }
 ;
 
return  (tos2((byte *) hex ) ) ;
 
 }
 int copy(array_byte dst, array_byte src) {
 
 if ( dst .len > 0  &&  src .len > 0 ) {
 
int min= ( dst .len < src .len ) ? ( dst .len ) : ( src .len ) ;
 
 memcpy ( dst .data ,  array_left( src , min ) .data ,  dst .element_size * min ) ;
 
return  min ;
 
 }
 ;
 
return  0 ;
 
 }
 int compare_ints(int* a, int* b) {
 
 if ( a < b ) {
 
return  - 1 ;
 
 }
 ;
 
 if ( a > b ) {
 
return  1 ;
 
 }
 ;
 
return  0 ;
 
 }
 void array_int_sort(array_int* a) {
 
 array_sort_with_compare( a , compare_ints ) ;
 
 }
 int vstrlen(byte* s) {
 
return  strlen ( ((char*)( s ) ) ) ;
 
 }
 void todo() {
 
 }
 string tos(byte* s, int len) {
 
 if ( isnil ( s ) ) {
 
 v_panic ( tos2((byte*)"tos(): nil string") ) ;
 
 }
 ;
 
return  (string) { .str =  s , .len =  len } ;
 
 }
 string tos_clone(byte* s) {
 
 if ( isnil ( s ) ) {
 
 v_panic ( tos2((byte*)"tos: nil string") ) ;
 
 }
 ;
 
return  string_clone( tos2 ( s ) ) ;
 
 }
 string tos2(byte* s) {
 
 if ( isnil ( s ) ) {
 
 v_panic ( tos2((byte*)"tos2: nil string") ) ;
 
 }
 ;
 
int len= vstrlen ( s ) ;
 
string res= tos ( s , len ) ;
 
return  res ;
 
 }
 string string_clone(string a) {
 
string b= (string) { .len =  a .len , .str =  v_malloc ( a .len + 1 ) } ;
 
 for (
int i= 0  ;  i < a .len  ;  i ++ ) { 
 
 
 b .str[ i ]/*rbyte 1*/  =  a .str[ i ]/*rbyte 0*/ ;
 
 }
 ;
 
 b .str[ a .len ]/*rbyte 1*/  =  '\0' ;
 
return  b ;
 
 }
 string string_replace(string s, string rep, string with) {
 
 if ( s .len == 0  ||  rep .len == 0 ) {
 
return  s ;
 
 }
 ;
 
array_int idxs=new_array_from_c_array(0, 0, sizeof(int), (int[]) {   0 }) ;
 
string rem= s ;
 
int rstart= 0 ;
 
 while (1) { 
 
int i= string_index( rem , rep ) ;
 
 if ( i < 0 ) {
 
 break
 ;
 
 }
 ;
 
_PUSH(& idxs , ( rstart + i ), tmp9, int) ;
 
 i  +=  rep .len ;
 
 rstart  +=  i ;
 
 rem  =  string_substr( rem , i , rem .len ) ;
 
 }
 ;
 
 if ( idxs .len == 0 ) {
 
return  s ;
 
 }
 ;
 
int new_len= s .len + idxs .len * ( with .len - rep .len ) ;
 
byte* b= v_malloc ( new_len + 1 ) ;
 
int idx_pos= 0 ;
 
int cur_idx= ( *(int*) array__get( idxs , idx_pos) ) ;
 
int b_i= 0 ;
 
 for (
int i= 0  ;  i < s .len  ;  i ++ ) { 
 
 
 if ( i == cur_idx ) {
 
 for (
int j= 0  ;  j < with .len  ;  j ++ ) { 
 
 
 b [/*ptr*/ b_i ]/*rbyte 1*/  =  with .str[ j ]/*rbyte 0*/ ;
 
 b_i ++ ;
 
 }
 ;
 
 i  +=  rep .len - 1 ;
 
 idx_pos ++ ;
 
 if ( idx_pos < idxs .len ) {
 
 cur_idx  =  ( *(int*) array__get( idxs , idx_pos) ) ;
 
 }
 ;
 
 }
  else { 
 
 b [/*ptr*/ b_i ]/*rbyte 1*/  =  s .str[ i ]/*rbyte 0*/ ;
 
 b_i ++ ;
 
 }
 ;
 
 }
 ;
 
 b [/*ptr*/ new_len ]/*rbyte 1*/  =  '\0' ;
 
return  tos ( b , new_len ) ;
 
 }
 int v_string_int(string s) {
 
return  atoi ( ((char*)( s .str ) ) ) ;
 
 }
 i64 string_i64(string s) {
 
return  atoll ( ((char*)( s .str ) ) ) ;
 
 }
 f32 string_f32(string s) {
 
return  atof ( ((char*)( s .str ) ) ) ;
 
 }
 f64 string_f64(string s) {
 
return  atof ( ((char*)( s .str ) ) ) ;
 
 }
 u32 string_u32(string s) {
 
return  strtoul ( ((char*)( s .str ) ) ,  0 ,  0 ) ;
 
 }
 u64 string_u64(string s) {
 
return  strtoull ( ((char*)( s .str ) ) ,  0 ,  0 ) ;
 
 }
 bool string_eq(string s, string a) {
 
 if ( isnil ( s .str ) ) {
 
 v_panic ( tos2((byte*)"string.eq(): nil string") ) ;
 
 }
 ;
 
 if ( s .len != a .len ) {
 
return  0 ;
 
 }
 ;
 
 for (
int i= 0  ;  i < s .len  ;  i ++ ) { 
 
 
 if ( s .str[ i ]/*rbyte 0*/ != a .str[ i ]/*rbyte 0*/ ) {
 
return  0 ;
 
 }
 ;
 
 }
 ;
 
return  1 ;
 
 }
 bool string_ne(string s, string a) {
 
return  ! string_eq( s , a ) ;
 
 }
 bool string_lt(string s, string a) {
 
 for (
int i= 0  ;  i < s .len  ;  i ++ ) { 
 
 
 if ( i >= a .len  ||  s .str[ i ]/*rbyte 0*/ > a .str[ i ]/*rbyte 0*/ ) {
 
return  0 ;
 
 }
  else  if ( s .str[ i ]/*rbyte 0*/ < a .str[ i ]/*rbyte 0*/ ) {
 
return  1 ;
 
 }
 ;
 
 }
 ;
 
 if ( s .len < a .len ) {
 
return  1 ;
 
 }
 ;
 
return  0 ;
 
 }
 bool string_le(string s, string a) {
 
return  string_lt( s , a )  ||  string_eq( s , a ) ;
 
 }
 bool string_gt(string s, string a) {
 
return  ! string_le( s , a ) ;
 
 }
 bool string_ge(string s, string a) {
 
return  ! string_lt( s , a ) ;
 
 }
 string string_add(string s, string a) {
 
int new_len= a .len + s .len ;
 
string res= (string) { .len =  new_len , .str =  v_malloc ( new_len + 1 ) } ;
 
 for (
int j= 0  ;  j < s .len  ;  j ++ ) { 
 
 
 res .str[ j ]/*rbyte 1*/  =  s .str[ j ]/*rbyte 0*/ ;
 
 }
 ;
 
 for (
int j= 0  ;  j < a .len  ;  j ++ ) { 
 
 
 res .str[ s .len + j ]/*rbyte 1*/  =  a .str[ j ]/*rbyte 0*/ ;
 
 }
 ;
 
 res .str[ new_len ]/*rbyte 1*/  =  '\0' ;
 
return  res ;
 
 }
 array_string string_split(string s, string delim) {
 
array_string res=new_array_from_c_array(0, 0, sizeof(string), (string[]) {   0 }) ;
 
 if ( delim .len == 0 ) {
 
_PUSH(& res , ( s ), tmp28, string) ;
 
return  res ;
 
 }
 ;
 
 if ( delim .len == 1 ) {
 
return  string_split_single( s , delim .str[ 0 ]/*rbyte 0*/ ) ;
 
 }
 ;
 
int i= 0 ;
 
int start= 0 ;
 
 while ( i < s .len ) {
 
 
bool a= s .str[ i ]/*rbyte 0*/ == delim .str[ 0 ]/*rbyte 0*/ ;
 
int j= 1 ;
 
 while ( j < delim .len  &&  a ) {
 
 
 a  =  a  &&  s .str[ i + j ]/*rbyte 0*/ == delim .str[ j ]/*rbyte 0*/ ;
 
 j ++ ;
 
 }
 ;
 
bool last= i == s .len - 1 ;
 
 if ( a  ||  last ) {
 
 if ( last ) {
 
 i ++ ;
 
 }
 ;
 
string val= string_substr( s , start , i ) ;
 
 if ( val .len > 0 ) {
 
 if ( string_starts_with( val , delim ) ) {
 
 val  =  string_right( val , delim .len ) ;
 
 }
 ;
 
_PUSH(& res , ( string_trim_space( val ) ), tmp35, string) ;
 
 }
 ;
 
 start  =  i ;
 
 }
 ;
 
 i ++ ;
 
 }
 ;
 
return  res ;
 
 }
 array_string string_split_single(string s, byte delim) {
 
array_string res=new_array_from_c_array(0, 0, sizeof(string), (string[]) {   0 }) ;
 
 if ( ((int)( delim ) ) == 0 ) {
 
_PUSH(& res , ( s ), tmp37, string) ;
 
return  res ;
 
 }
 ;
 
int i= 0 ;
 
int start= 0 ;
 
 while ( i < s .len ) {
 
 
bool is_delim= s .str[ i ]/*rbyte 0*/ == delim ;
 
bool last= i == s .len - 1 ;
 
 if ( is_delim  ||  last ) {
 
 if ( ! is_delim  &&  i == s .len - 1 ) {
 
 i ++ ;
 
 }
 ;
 
string val= string_substr( s , start , i ) ;
 
 if ( val .len > 0 ) {
 
_PUSH(& res , ( val ), tmp43, string) ;
 
 }
 ;
 
 start  =  i + 1 ;
 
 }
 ;
 
 i ++ ;
 
 }
 ;
 
return  res ;
 
 }
 array_string string_split_into_lines(string s) {
 
array_string res=new_array_from_c_array(0, 0, sizeof(string), (string[]) {   0 }) ;
 
 if ( s .len == 0 ) {
 
return  res ;
 
 }
 ;
 
int start= 0 ;
 
 for (
int i= 0  ;  i < s .len  ;  i ++ ) { 
 
 
bool last= i == s .len - 1 ;
 
 if ( ((int)( s .str[ i ]/*rbyte 0*/ ) ) == 10  ||  last ) {
 
 if ( last ) {
 
 i ++ ;
 
 }
 ;
 
string line= string_substr( s , start , i ) ;
 
_PUSH(& res , ( line ), tmp49, string) ;
 
 start  =  i + 1 ;
 
 }
 ;
 
 }
 ;
 
return  res ;
 
 }
 string string_left(string s, int n) {
 
 if ( n >= s .len ) {
 
return  s ;
 
 }
 ;
 
return  string_substr( s , 0 , n ) ;
 
 }
 string string_right(string s, int n) {
 
 if ( n >= s .len ) {
 
return  tos2((byte*)"") ;
 
 }
 ;
 
return  string_substr( s , n , s .len ) ;
 
 }
 string string_substr(string s, int start, int end) {
 
 if ( start > end  ||  start > s .len  ||  end > s .len  ||  start < 0  ||  end < 0 ) {
 
 v_panic ( _STR("substr(%d, %d) out of bounds (len=%d)", start, end, s .len) ) ;
 
 }
 ;
 
int len= end - start ;
 
string res= (string) { .len =  len , .str =  v_malloc ( len + 1 ) } ;
 
 for (
int i= 0  ;  i < len  ;  i ++ ) { 
 
 
 res .str [/*ptr*/ i ]/*rbyte 1*/  =  s .str [/*ptr*/ start + i ]/*rbyte 0*/ ;
 
 }
 ;
 
 res .str [/*ptr*/ len ]/*rbyte 1*/  =  '\0' ;
 
return  res ;
 
 }
 int string_index_old(string s, string p) {
 
 if ( p .len > s .len ) {
 
return  - 1 ;
 
 }
 ;
 
int i= 0 ;
 
 while ( i < s .len ) {
 
 
int j= 0 ;
 
int ii= i ;
 
 while ( j < p .len  &&  s .str[ ii ]/*rbyte 0*/ == p .str[ j ]/*rbyte 0*/ ) {
 
 
 j ++ ;
 
 ii ++ ;
 
 }
 ;
 
 if ( j == p .len ) {
 
return  i - p .len + 1 ;
 
 }
 ;
 
 i ++ ;
 
 }
 ;
 
return  - 1 ;
 
 }
 int string_index(string s, string p) {
 
 if ( p .len > s .len ) {
 
return  - 1 ;
 
 }
 ;
 
array_int prefix= array_repeat2(new_array_from_c_array(1, 1, sizeof(int), (int[]) {  0  }) , p .len ) ;
 
int j= 0 ;
 
 for (
int i= 1  ;  i < p .len  ;  i ++ ) { 
 
 
 while ( p .str[ j ]/*rbyte 0*/ != p .str[ i ]/*rbyte 0*/  &&  j > 0 ) {
 
 
 j  =  ( *(int*) array__get( prefix , j - 1) ) ;
 
 }
 ;
 
 if ( p .str[ j ]/*rbyte 0*/ == p .str[ i ]/*rbyte 0*/ ) {
 
 j ++ ;
 
 }
 ;
 
array_set(&/*q*/ prefix , i , & (int []) {  j }) ;
 
 }
 ;
 
 j  =  0 ;
 
 for (
int i= 0  ;  i < s .len  ;  i ++ ) { 
 
 
 while ( p .str[ j ]/*rbyte 0*/ != s .str[ i ]/*rbyte 0*/  &&  j > 0 ) {
 
 
 j  =  ( *(int*) array__get( prefix , j - 1) ) ;
 
 }
 ;
 
 if ( p .str[ j ]/*rbyte 0*/ == s .str[ i ]/*rbyte 0*/ ) {
 
 j ++ ;
 
 }
 ;
 
 if ( j == p .len ) {
 
return  i - p .len + 1 ;
 
 }
 ;
 
 }
 ;
 
return  - 1 ;
 
 }
 int string_index_any(string s, string chars) {
 
 string tmp64 =  chars;
 array_byte bytes_tmp64 = string_bytes( tmp64 );
 ;
for (int tmp65 = 0; tmp65 < tmp64 .len; tmp65 ++) {
 byte c = ((byte *) bytes_tmp64 . data)[tmp65];
 
 
int index= string_index( s , byte_str( c ) ) ;
 
 if ( index != - 1 ) {
 
return  index ;
 
 }
 ;
 
 }
 ;
 
return  - 1 ;
 
 }
 int string_last_index(string s, string p) {
 
 if ( p .len > s .len ) {
 
return  - 1 ;
 
 }
 ;
 
int i= s .len - p .len ;
 
 while ( i >= 0 ) {
 
 
int j= 0 ;
 
 while ( j < p .len  &&  s .str[ i + j ]/*rbyte 0*/ == p .str[ j ]/*rbyte 0*/ ) {
 
 
 j ++ ;
 
 }
 ;
 
 if ( j == p .len ) {
 
return  i ;
 
 }
 ;
 
 i -- ;
 
 }
 ;
 
return  - 1 ;
 
 }
 int string_index_after(string s, string p, int start) {
 
 if ( p .len > s .len ) {
 
return  - 1 ;
 
 }
 ;
 
int strt= start ;
 
 if ( start < 0 ) {
 
 strt  =  0 ;
 
 }
 ;
 
 if ( start >= s .len ) {
 
return  - 1 ;
 
 }
 ;
 
int i= strt ;
 
 while ( i < s .len ) {
 
 
int j= 0 ;
 
int ii= i ;
 
 while ( j < p .len  &&  s .str[ ii ]/*rbyte 0*/ == p .str[ j ]/*rbyte 0*/ ) {
 
 
 j ++ ;
 
 ii ++ ;
 
 }
 ;
 
 if ( j == p .len ) {
 
return  i ;
 
 }
 ;
 
 i ++ ;
 
 }
 ;
 
return  - 1 ;
 
 }
 int string_count(string s, string substr) {
 
 if ( s .len == 0  ||  substr .len == 0 ) {
 
return  0 ;
 
 }
 ;
 
 if ( substr .len > s .len ) {
 
return  0 ;
 
 }
 ;
 
int n= 0 ;
 
int i= 0 ;
 
 while (1) { 
 
 i  =  string_index_after( s , substr , i ) ;
 
 if ( i == - 1 ) {
 
return  n ;
 
 }
 ;
 
 i  +=  substr .len ;
 
 n ++ ;
 
 }
 ;
 
return  0 ;
 
 }
 bool string_contains(string s, string p) {
 
bool res= string_index( s , p ) > 0 - 1 ;
 
return  res ;
 
 }
 bool string_starts_with(string s, string p) {
 
bool res= string_index( s , p ) == 0 ;
 
return  res ;
 
 }
 bool string_ends_with(string s, string p) {
 
 if ( p .len > s .len ) {
 
return  0 ;
 
 }
 ;
 
bool res= string_last_index( s , p ) == s .len - p .len ;
 
return  res ;
 
 }
 string string_to_lower(string s) {
 
byte* b= v_malloc ( s .len + 1 ) ;
 
 for (
int i= 0  ;  i < s .len  ;  i ++ ) { 
 
 
 b [/*ptr*/ i ]/*rbyte 1*/  =  tolower ( s .str [/*ptr*/ i ]/*rbyte 0*/ ) ;
 
 }
 ;
 
return  tos ( b , s .len ) ;
 
 }
 string string_to_upper(string s) {
 
byte* b= v_malloc ( s .len + 1 ) ;
 
 for (
int i= 0  ;  i < s .len  ;  i ++ ) { 
 
 
 b [/*ptr*/ i ]/*rbyte 1*/  =  toupper ( s .str [/*ptr*/ i ]/*rbyte 0*/ ) ;
 
 }
 ;
 
return  tos ( b , s .len ) ;
 
 }
 string string_capitalize(string s) {
 
string sl= string_to_lower( s ) ;
 
string cap=string_add( string_to_upper( byte_str( sl .str[ 0 ]/*rbyte 0*/ ) ) , string_right( sl , 1 ) ) ;
 
return  cap ;
 
 }
 string string_title(string s) {
 
array_string words= string_split( s , tos2((byte*)" ") ) ;
 
array_string tit=new_array_from_c_array(0, 0, sizeof(string), (string[]) {   0 }) ;
 
 array_string tmp86 =  words;
 for (int tmp87 = 0; tmp87 < tmp86.len; tmp87++) {
 string word = ((string *) tmp86 . data)[tmp87];
 
 
_PUSH(& tit , ( string_capitalize( word ) ), tmp88, string) ;
 
 }
 ;
 
string title= array_string_join( tit , tos2((byte*)" ") ) ;
 
return  title ;
 
 }
 string string_find_between(string s, string start, string end) {
 
int start_pos= string_index( s , start ) ;
 
 if ( start_pos == - 1 ) {
 
return  tos2((byte*)"") ;
 
 }
 ;
 
string val= string_right( s , start_pos + start .len ) ;
 
int end_pos= string_index( val , end ) ;
 
 if ( end_pos == - 1 ) {
 
return  val ;
 
 }
 ;
 
return  string_left( val , end_pos ) ;
 
 }
 bool array_string_contains(array_string ar, string val) {
 
 array_string tmp93 =  ar;
 for (int tmp94 = 0; tmp94 < tmp93.len; tmp94++) {
 string s = ((string *) tmp93 . data)[tmp94];
 
 
 if (string_eq( s , val ) ) {
 
return  1 ;
 
 }
 ;
 
 }
 ;
 
return  0 ;
 
 }
 bool array_int_contains(array_int ar, int val) {
 
 array_int tmp95 =  ar;
 for (int i = 0; i < tmp95.len; i++) {
 int s = ((int *) tmp95 . data)[i];
 
 
 if ( s == val ) {
 
return  1 ;
 
 }
 ;
 
 }
 ;
 
return  0 ;
 
 }
 bool is_space(byte c) {
 
return _IN(byte,  c , new_array_from_c_array(6, 6, sizeof(byte), (byte[]) {  ' ' ,  '\n' ,  '\t' ,  '\v' ,  '\f' ,  '\r'  }) ) ;
 
 }
 bool byte_is_space(byte c) {
 
return  is_space ( c ) ;
 
 }
 string string_trim_space(string s) {
 
return  string_trim( s , tos2((byte*)" \n\t\v\f\r") ) ;
 
 }
 string string_trim(string s, string cutset) {
 
 if ( s .len < 1  ||  cutset .len < 1 ) {
 
return  s ;
 
 }
 ;
 
array_byte cs_arr= string_bytes( cutset ) ;
 
int pos_left= 0 ;
 
int pos_right= s .len - 1 ;
 
bool cs_match= 1 ;
 
 while ( pos_left <= s .len  &&  pos_right >= - 1  &&  cs_match ) {
 
 
 cs_match  =  0 ;
 
 if (_IN(byte,  s .str[ pos_left ]/*rbyte 0*/ ,  cs_arr ) ) {
 
 pos_left ++ ;
 
 cs_match  =  1 ;
 
 }
 ;
 
 if (_IN(byte,  s .str[ pos_right ]/*rbyte 0*/ ,  cs_arr ) ) {
 
 pos_right -- ;
 
 cs_match  =  1 ;
 
 }
 ;
 
 if ( pos_left > pos_right ) {
 
return  tos2((byte*)"") ;
 
 }
 ;
 
 }
 ;
 
return  string_substr( s , pos_left , pos_right + 1 ) ;
 
 }
 string string_trim_left(string s, string cutset) {
 
 if ( s .len < 1  ||  cutset .len < 1 ) {
 
return  s ;
 
 }
 ;
 
array_byte cs_arr= string_bytes( cutset ) ;
 
int pos= 0 ;
 
 while ( pos <= s .len  && _IN(byte,  s .str[ pos ]/*rbyte 0*/ ,  cs_arr ) ) {
 
 
 pos ++ ;
 
 }
 ;
 
return  string_right( s , pos ) ;
 
 }
 string string_trim_right(string s, string cutset) {
 
 if ( s .len < 1  ||  cutset .len < 1 ) {
 
return  s ;
 
 }
 ;
 
array_byte cs_arr= string_bytes( cutset ) ;
 
int pos= s .len - 1 ;
 
 while ( pos >= - 1  && _IN(byte,  s .str[ pos ]/*rbyte 0*/ ,  cs_arr ) ) {
 
 
 pos -- ;
 
 }
 ;
 
return  string_left( s , pos + 1 ) ;
 
 }
 int compare_strings(string* a, string* b) {
 
 if ( string_lt(* a ,* b ) ) {
 
return  - 1 ;
 
 }
 ;
 
 if ( string_gt(* a ,* b ) ) {
 
return  1 ;
 
 }
 ;
 
return  0 ;
 
 }
 int compare_strings_by_len(string* a, string* b) {
 
 if ( a ->len < b ->len ) {
 
return  - 1 ;
 
 }
 ;
 
 if ( a ->len > b ->len ) {
 
return  1 ;
 
 }
 ;
 
return  0 ;
 
 }
 int compare_lower_strings(string* a, string* b) {
 
string aa= string_to_lower(* a ) ;
 
string bb= string_to_lower(* b ) ;
 
return  compare_strings (& /*112 EXP:"string*" GOT:"string" */ aa ,& /*112 EXP:"string*" GOT:"string" */ bb ) ;
 
 }
 void array_string_sort(array_string* s) {
 
 array_sort_with_compare( s , compare_strings ) ;
 
 }
 void array_string_sort_ignore_case(array_string* s) {
 
 array_sort_with_compare( s , compare_lower_strings ) ;
 
 }
 void array_string_sort_by_len(array_string* s) {
 
 array_sort_with_compare( s , compare_strings_by_len ) ;
 
 }
 ustring string_ustring(string s) {
 
ustring res= (ustring) { .s =  s , .runes =  new_array ( 0 , s .len , sizeof( int) ) , .len =  0 } ;
 
 for (
int i= 0  ;  i < s .len  ;  i ++ ) { 
 
 
int char_len= utf8_char_len ( s .str [/*ptr*/ i ]/*rbyte 0*/ ) ;
 
_PUSH(& res .runes , ( i ), tmp109, int) ;
 
 i  +=  char_len - 1 ;
 
 res .len ++ ;
 
 }
 ;
 
return  res ;
 
 }
 ustring string_ustring_tmp(string s) {
 
 if ( g_ustring_runes .len == 0 ) {
 
 g_ustring_runes  =  new_array ( 0 , 128 , sizeof( int) ) ;
 
 }
 ;
 
ustring res= (ustring) { .s =  s , .runes =  new_array(0, 1, sizeof( int )) , .len =  0 } ;
 
 res .runes  =  g_ustring_runes ;
 
 res .runes .len  =  s .len ;
 
int j= 0 ;
 
 for (
int i= 0  ;  i < s .len  ;  i ++ ) { 
 
 
int char_len= utf8_char_len ( s .str [/*ptr*/ i ]/*rbyte 0*/ ) ;
 
array_set(&/*q*/ res .runes , j , & (int []) {  i }) ;
 
 j ++ ;
 
 i  +=  char_len - 1 ;
 
 res .len ++ ;
 
 }
 ;
 
return  res ;
 
 }
 string ustring_substr(ustring u, int _start, int _end) {
 
int start= ( *(int*) array__get( u .runes , _start) ) ;
 
int end= ( _end >= u .runes .len ) ? ( u .s .len ) : ( ( *(int*) array__get( u .runes , _end) ) ) ;
 
return  string_substr( u .s , start , end ) ;
 
 }
 string ustring_left(ustring u, int pos) {
 
return  ustring_substr( u , 0 , pos ) ;
 
 }
 string ustring_right(ustring u, int pos) {
 
return  ustring_substr( u , pos , u .len ) ;
 
 }
 byte string_at(string s, int idx) {
 
 if ( idx < 0  ||  idx >= s .len ) {
 
 v_panic ( _STR("string index out of range: %d / %d", idx, s .len) ) ;
 
 }
 ;
 
return  s .str [/*ptr*/ idx ]/*rbyte 0*/ ;
 
 }
 string ustring_at(ustring u, int idx) {
 
return  ustring_substr( u , idx , idx + 1 ) ;
 
 }
 void v_ustring_free(ustring u) {
 
 v_array_free( u .runes ) ;
 
 }
 bool byte_is_digit(byte c) {
 
return  c >= '0'  &&  c <= '9' ;
 
 }
 bool byte_is_hex_digit(byte c) {
 
return  byte_is_digit( c )  ||  ( c >= 'a'  &&  c <= 'f' )  ||  ( c >= 'A'  &&  c <= 'F' ) ;
 
 }
 bool byte_is_oct_digit(byte c) {
 
return  c >= '0'  &&  c <= '7' ;
 
 }
 bool byte_is_letter(byte c) {
 
return  ( c >= 'a'  &&  c <= 'z' )  ||  ( c >= 'A'  &&  c <= 'Z' ) ;
 
 }
 void v_string_free(string s) {
 
 v_free ( s .str ) ;
 
 }
 string string_all_before(string s, string dot) {
 
int pos= string_index( s , dot ) ;
 
 if ( pos == - 1 ) {
 
return  s ;
 
 }
 ;
 
return  string_left( s , pos ) ;
 
 }
 string string_all_before_last(string s, string dot) {
 
int pos= string_last_index( s , dot ) ;
 
 if ( pos == - 1 ) {
 
return  s ;
 
 }
 ;
 
return  string_left( s , pos ) ;
 
 }
 string string_all_after(string s, string dot) {
 
int pos= string_last_index( s , dot ) ;
 
 if ( pos == - 1 ) {
 
return  s ;
 
 }
 ;
 
return  string_right( s , pos + dot .len ) ;
 
 }
 string array_string_join(array_string a, string del) {
 
 if ( a .len == 0 ) {
 
return  tos2((byte*)"") ;
 
 }
 ;
 
int len= 0 ;
 
 array_string tmp124 =  a;
 for (int i = 0; i < tmp124.len; i++) {
 string val = ((string *) tmp124 . data)[i];
 
 
 len  +=  val .len + del .len ;
 
 }
 ;
 
 len  -=  del .len ;
 
string res= tos2((byte*)"") ;
 
 res .len  =  len ;
 
 res .str  =  v_malloc ( res .len + 1 ) ;
 
int idx= 0 ;
 
 array_string tmp127 =  a;
 for (int i = 0; i < tmp127.len; i++) {
 string val = ((string *) tmp127 . data)[i];
 
 
 for (
int j= 0  ;  j < val .len  ;  j ++ ) { 
 
 
byte c= val .str[ j ]/*rbyte 0*/ ;
 
 res .str [/*ptr*/ idx ]/*rbyte 1*/  =  val .str [/*ptr*/ j ]/*rbyte 0*/ ;
 
 idx ++ ;
 
 }
 ;
 
 if ( i != a .len - 1 ) {
 
 for (
int k= 0  ;  k < del .len  ;  k ++ ) { 
 
 
 res .str [/*ptr*/ idx ]/*rbyte 1*/  =  del .str [/*ptr*/ k ]/*rbyte 0*/ ;
 
 idx ++ ;
 
 }
 ;
 
 }
 ;
 
 }
 ;
 
 res .str [/*ptr*/ res .len ]/*rbyte 1*/  =  '\0' ;
 
return  res ;
 
 }
 string array_string_join_lines(array_string s) {
 
return  array_string_join( s , tos2((byte*)"\n") ) ;
 
 }
 string string_reverse(string s) {
 
string res= (string) { .len =  s .len , .str =  v_malloc ( s .len ) } ;
 
 for (
int i= s .len - 1  ;  i >= 0  ;  i -- ) { 
 
 
 res .str[ s .len - i - 1 ]/*rbyte 1*/  =  s .str[ i ]/*rbyte 0*/ ;
 
 }
 ;
 
return  res ;
 
 }
 string string_limit(string s, int max) {
 
ustring u= string_ustring( s ) ;
 
 if ( u .len <= max ) {
 
return  s ;
 
 }
 ;
 
return  ustring_substr( u , 0 , max ) ;
 
 }
 bool byte_is_white(byte c) {
 
int i= ((int)( c ) ) ;
 
return  i == 10  ||  i == 32  ||  i == 9  ||  i == 13  ||  c == '\r' ;
 
 }
 int string_hash(string s) {
 
int h= 0 ;
 
 if ( h == 0  &&  s .len > 0 ) {
 
 string tmp136 =  s;
 array_byte bytes_tmp136 = string_bytes( tmp136 );
 ;
for (int tmp137 = 0; tmp137 < tmp136 .len; tmp137 ++) {
 byte c = ((byte *) bytes_tmp136 . data)[tmp137];
 
 
 h  =  h * 31 + ((int)( c ) ) ;
 
 }
 ;
 
 }
 ;
 
return  h ;
 
 }
 array_byte string_bytes(string s) {
 
 if ( s .len == 0 ) {
 
return new_array_from_c_array(0, 0, sizeof(byte), (byte[]) {   0 }) ;
 
 }
 ;
 
array_byte buf= array_repeat2(new_array_from_c_array(1, 1, sizeof(byte), (byte[]) {  ((byte)( 0 ) )  }) , s .len ) ;
 
 memcpy ( buf .data , (char*) s .str ,  s .len ) ;
 
return  buf ;
 
 }
 void v_exit(int code) {
 
 exit ( code ) ;
 
 }
 bool isnil(void* v) {
 
return  v == 0 ;
 
 }
 void on_panic(int (*f)( int  /*FFF*/ )) {
 
 }
 void print_backtrace_skipping_top_frames(int skipframes) {
 
 #ifdef __APPLE__
 
byte* buffer  [100 ] ;
 
int nr_ptrs= backtrace ( ((voidptr*)( buffer ) ) ,  100 ) ;
 
 backtrace_symbols_fd ( ((voidptr*)( & /*vvar*/  buffer [ skipframes ]/*rbyte* 0*/ ) ) ,  nr_ptrs - skipframes ,  1 ) ;
 
 return ;
 
 #endif
 ;
 
 #ifdef __linux__
 
 #ifndef __BIONIC__
 
 if ( backtrace_symbols_fd != 0 ) {
 
byte* buffer  [100 ] ;
 
int nr_ptrs= backtrace ( ((voidptr*)( buffer ) ) ,  100 ) ;
 
 backtrace_symbols_fd ( & /*vvar*/  buffer [ skipframes ]/*rbyte* 0*/ ,  nr_ptrs - skipframes ,  1 ) ;
 
 return ;
 
 }
  else { 
 
 printf ( "backtrace_symbols_fd is missing, so printing backtraces is not available.\n" ) ;
 
 printf ( "Some libc implementations like musl simply do not provide it.\n" ) ;
 
 }
 ;
 
 #endif
 ;
 
 #endif
 ;
 
 println ( tos2((byte*)"print_backtrace_skipping_top_frames is not implemented on this platform for now...\n") ) ;
 
 }
 void print_backtrace() {
 
 print_backtrace_skipping_top_frames ( 2 ) ;
 
 }
 void _panic_debug(int line_no, string file, string mod, string fn_name, string s) {
 
 println ( tos2((byte*)"================ V panic ================") ) ;
 
printf( "   module: %.*s\n", mod.len, mod.str ) ;
 
printf( " function: %.*s()\n", fn_name.len, fn_name.str ) ;
 
printf( "     file: %.*s\n", file.len, file.str ) ;
 
 println (string_add( tos2((byte*)"     line: ") , int_str( line_no ) ) ) ;
 
printf( "  message: %.*s\n", s.len, s.str ) ;
 
 println ( tos2((byte*)"=========================================") ) ;
 
 print_backtrace_skipping_top_frames ( 1 ) ;
 
 exit ( 1 ) ;
 
 }
 void v_panic(string s) {
 
printf( "V panic: %.*s\n", s.len, s.str ) ;
 
 print_backtrace ( ) ;
 
 exit ( 1 ) ;
 
 }
 void println(string s) {
 
 if ( isnil ( s .str ) ) {
 
 v_panic ( tos2((byte*)"println(NIL)") ) ;
 
 }
 ;
 
 #ifdef _WIN32
 
 _putws ( string_to_wide( s ) ) ;
 
 #else
 
 printf ( "%.*s\n" ,  s .len , (char*) s .str ) ;
 
 #endif
 ;
 
 }
 void eprintln(string s) {
 
 if ( isnil ( s .str ) ) {
 
 v_panic ( tos2((byte*)"eprintln(NIL)") ) ;
 
 }
 ;
 
 #ifdef __APPLE__
 
 fprintf ( stderr ,  "%.*s\n" ,  s .len , (char*) s .str ) ;
 
 #else
 
 println ( s ) ;
 
 #endif
 ;
 
 }
 void print(string s) {
 
 #ifdef _WIN32
 
 wprintf ( string_to_wide( s ) ) ;
 
 #else
 
 printf ( "%.*s" ,  s .len , (char*) s .str ) ;
 
 #endif
 ;
 
 }
 byte* v_malloc(int n) {
 
 if ( n < 0 ) {
 
 v_panic ( tos2((byte*)"malloc(<0)") ) ;
 
 }
 ;
 
byte* ptr= malloc ( n ) ;
 
 if ( isnil ( ptr ) ) {
 
 v_panic ( _STR("malloc(%d) failed", n) ) ;
 
 }
 ;
 
return  ptr ;
 
 }
 byte* v_calloc(int n) {
 
 if ( n < 0 ) {
 
 v_panic ( tos2((byte*)"calloc(<0)") ) ;
 
 }
 ;
 
return  calloc ( n ,  1 ) ;
 
 }
 void v_free(void* ptr) {
 
 free ( ptr ) ;
 
 }
 void* memdup(void* src, int sz) {
 
byte* mem= v_malloc ( sz ) ;
 
return  memcpy ((char*) mem ,  src ,  sz ) ;
 
 }
 void v_ptr_free(void* ptr) {
 
 free ( ptr ) ;
 
 }
 string f64_str(f64 d) {
 
byte* buf= v_malloc ( sizeof( double) * 5 + 1 ) ;
 
 sprintf ( ((char*)( buf ) ) ,  "%f" ,  d ) ;
 
return  tos ( buf , vstrlen ( buf ) ) ;
 
 }
 string f32_str(f32 d) {
 
byte* buf= v_malloc ( sizeof( double) * 5 + 1 ) ;
 
 sprintf ( ((char*)( buf ) ) ,  "%f" ,  d ) ;
 
return  tos ( buf , vstrlen ( buf ) ) ;
 
 }
 string ptr_str(void* ptr) {
 
byte* buf= v_malloc ( sizeof( double) * 5 + 1 ) ;
 
 sprintf ( ((char*)( buf ) ) ,  "%p" ,  ptr ) ;
 
return  tos ( buf , vstrlen ( buf ) ) ;
 
 }
 bool f64_eq(f64 a, f64 b) {
 
return  ( a - b ) <= DBL_EPSILON ;
 
 }
 string int_str(int nn) {
 
int n= nn ;
 
 if ( n == 0 ) {
 
return  tos2((byte*)"0") ;
 
 }
 ;
 
int max= 16 ;
 
byte* buf= v_calloc ( max ) ;
 
int len= 0 ;
 
bool is_neg= 0 ;
 
 if ( n < 0 ) {
 
 n  =  - n ;
 
 is_neg  =  1 ;
 
 }
 ;
 
 while ( n > 0 ) {
 
 
int d= n % 10 ;
 
 buf [/*ptr*/ max - len - 1 ]/*rbyte 1*/  =  d + ((int)( '0' ) ) ;
 
 len ++ ;
 
 n  =  n / 10 ;
 
 }
 ;
 
 if ( is_neg ) {
 
 buf [/*ptr*/ max - len - 1 ]/*rbyte 1*/  =  '-' ;
 
 len ++ ;
 
 }
 ;
 
return  tos ( buf + max - len , len ) ;
 
 }
 string u32_str(u32 nn) {
 
u32 n= nn ;
 
 if ( n == ((u32)( 0 ) ) ) {
 
return  tos2((byte*)"0") ;
 
 }
 ;
 
int max= 16 ;
 
byte* buf= v_malloc ( max ) ;
 
int len= 0 ;
 
 while ( n > ((u32)( 0 ) ) ) {
 
 
u32 d= n % ((u32)( 10 ) ) ;
 
 buf [/*ptr*/ max - len - 1 ]/*rbyte 1*/  =  d + ((u32)( '0' ) ) ;
 
 len ++ ;
 
 n  =  n / ((u32)( 10 ) ) ;
 
 }
 ;
 
return  tos ( buf + max - len , len ) ;
 
 }
 string i64_str(i64 nn) {
 
i64 n= nn ;
 
 if ( n == ((i64)( 0 ) ) ) {
 
return  tos2((byte*)"0") ;
 
 }
 ;
 
int max= 32 ;
 
byte* buf= v_malloc ( max ) ;
 
int len= 0 ;
 
bool is_neg= 0 ;
 
 if ( n < ((i64)( 0 ) ) ) {
 
 n  =  - n ;
 
 is_neg  =  1 ;
 
 }
 ;
 
 while ( n > ((i64)( 0 ) ) ) {
 
 
int d= ((int)( n % ((i64)( 10 ) ) ) ) ;
 
 buf [/*ptr*/ max - len - 1 ]/*rbyte 1*/  =  d + ((int)( '0' ) ) ;
 
 len ++ ;
 
 n  =  n / ((i64)( 10 ) ) ;
 
 }
 ;
 
 if ( is_neg ) {
 
 buf [/*ptr*/ max - len - 1 ]/*rbyte 1*/  =  '-' ;
 
 len ++ ;
 
 }
 ;
 
return  tos ( buf + max - len , len ) ;
 
 }
 string u64_str(u64 nn) {
 
u64 n= nn ;
 
 if ( n == ((u64)( 0 ) ) ) {
 
return  tos2((byte*)"0") ;
 
 }
 ;
 
int max= 32 ;
 
byte* buf= v_malloc ( max ) ;
 
int len= 0 ;
 
 while ( n > ((u64)( 0 ) ) ) {
 
 
u64 d= n % ((u64)( 10 ) ) ;
 
 buf [/*ptr*/ max - len - 1 ]/*rbyte 1*/  =  d + ((u64)( '0' ) ) ;
 
 len ++ ;
 
 n  =  n / ((u64)( 10 ) ) ;
 
 }
 ;
 
return  tos ( buf + max - len , len ) ;
 
 }
 string bool_str(bool b) {
 
 if ( b ) {
 
return  tos2((byte*)"true") ;
 
 }
 ;
 
return  tos2((byte*)"false") ;
 
 }
 string int_hex(int n) {
 
int len= ( n >= 0 ) ? ( int_str( n ) .len + 3 ) : ( 11 ) ;
 
byte* hex= v_malloc ( len ) ;
 
int count= ((int)( sprintf ( ((char*)( hex ) ) ,  "0x%x" ,  n ) ) ) ;
 
return  tos ( hex , count ) ;
 
 }
 string i64_hex(i64 n) {
 
int len= ( n >= ((i64)( 0 ) ) ) ? ( i64_str( n ) .len + 3 ) : ( 19 ) ;
 
byte* hex= v_malloc ( len ) ;
 
int count= ((int)( sprintf ( ((char*)( hex ) ) ,  "0x%llx" ,  n ) ) ) ;
 
return  tos ( hex , count ) ;
 
 }
 bool array_byte_contains(array_byte a, byte val) {
 
 array_byte tmp32 =  a;
 for (int tmp33 = 0; tmp33 < tmp32.len; tmp33++) {
 byte aa = ((byte *) tmp32 . data)[tmp33];
 
 
 if ( aa == val ) {
 
return  1 ;
 
 }
 ;
 
 }
 ;
 
return  0 ;
 
 }
 string rune_str(rune c) {
 
int fst_byte= ((int)( c ) )  >>  8 * 3 & 0xff ;
 
int len= utf8_char_len ( fst_byte ) ;
 
string str= (string) { .len =  len , .str =  v_malloc ( len + 1 ) } ;
 
 for (
int i= 0  ;  i < len  ;  i ++ ) { 
 
 
 str .str [/*ptr*/ i ]/*rbyte 1*/  =  ((int)( c ) )  >>  8 * ( 3 - i ) & 0xff ;
 
 }
 ;
 
 str .str[ len ]/*rbyte 1*/  =  '\0' ;
 
return  str ;
 
 }
 string byte_str(byte c) {
 
string str= (string) { .len =  1 , .str =  v_malloc ( 2 ) } ;
 
 str .str [/*ptr*/ 0 ]/*rbyte 1*/  =  c ;
 
 str .str [/*ptr*/ 1 ]/*rbyte 1*/  =  '\0' ;
 
return  str ;
 
 }
 bool byte_is_capital(byte c) {
 
return  c >= 'A'  &&  c <= 'Z' ;
 
 }
 array_byte array_byte_clone(array_byte b) {
 
array_byte res= array_repeat2(new_array_from_c_array(1, 1, sizeof(byte), (byte[]) {  ((byte)( 0 ) )  }) , b .len ) ;
 
 for (
int i= 0  ;  i < b .len  ;  i ++ ) { 
 
 
array_set(&/*q*/ res , i , & (byte []) {  ( *(byte*) array__get( b , i) ) }) ;
 
 }
 ;
 
return  res ;
 
 }
 int utf8_char_len(byte b) {
 
return  ( ( 0xe5000000  >>  ( ( b  >>  3 ) & 0x1e ) ) & 3 ) + 1 ;
 
 }
 string utf32_to_str(u32 code) {
 
int icode= ((int)( code ) ) ;
 
byte* buffer= v_malloc ( 5 ) ;
 
 if ( icode <= 127 ) {
 
 buffer [/*ptr*/ 0 ]/*rbyte 1*/  =  icode ;
 
return  tos ( buffer , 1 ) ;
 
 }
 ;
 
 if ( ( icode <= 2047 ) ) {
 
 buffer [/*ptr*/ 0 ]/*rbyte 1*/  =  192 | ( icode  >>  6 ) ;
 
 buffer [/*ptr*/ 1 ]/*rbyte 1*/  =  128 | ( icode & 63 ) ;
 
return  tos ( buffer , 2 ) ;
 
 }
 ;
 
 if ( ( icode <= 65535 ) ) {
 
 buffer [/*ptr*/ 0 ]/*rbyte 1*/  =  224 | ( icode  >>  12 ) ;
 
 buffer [/*ptr*/ 1 ]/*rbyte 1*/  =  128 | ( ( icode  >>  6 ) & 63 ) ;
 
 buffer [/*ptr*/ 2 ]/*rbyte 1*/  =  128 | ( icode & 63 ) ;
 
return  tos ( buffer , 3 ) ;
 
 }
 ;
 
 if ( ( icode <= 1114111 ) ) {
 
 buffer [/*ptr*/ 0 ]/*rbyte 1*/  =  240 | ( icode  >>  18 ) ;
 
 buffer [/*ptr*/ 1 ]/*rbyte 1*/  =  128 | ( ( icode  >>  12 ) & 63 ) ;
 
 buffer [/*ptr*/ 2 ]/*rbyte 1*/  =  128 | ( ( icode  >>  6 ) & 63 ) ;
 
 buffer [/*ptr*/ 3 ]/*rbyte 1*/  =  128 | ( icode & 63 ) ;
 
return  tos ( buffer , 4 ) ;
 
 }
 ;
 
return  tos2((byte*)"") ;
 
 }
 string utf32_to_str_no_malloc(u32 code, void* buf) {
 
int icode= ((int)( code ) ) ;
 
byteptr buffer= ((byteptr)( buf ) ) ;
 
 if ( icode <= 127 ) {
 
 buffer [/*ptr*/ 0 ]/*rbyteptr 1*/  =  icode ;
 
return  tos ( buffer , 1 ) ;
 
 }
 ;
 
 if ( ( icode <= 2047 ) ) {
 
 buffer [/*ptr*/ 0 ]/*rbyteptr 1*/  =  192 | ( icode  >>  6 ) ;
 
 buffer [/*ptr*/ 1 ]/*rbyteptr 1*/  =  128 | ( icode & 63 ) ;
 
return  tos ( buffer , 2 ) ;
 
 }
 ;
 
 if ( ( icode <= 65535 ) ) {
 
 buffer [/*ptr*/ 0 ]/*rbyteptr 1*/  =  224 | ( icode  >>  12 ) ;
 
 buffer [/*ptr*/ 1 ]/*rbyteptr 1*/  =  128 | ( ( icode  >>  6 ) & 63 ) ;
 
 buffer [/*ptr*/ 2 ]/*rbyteptr 1*/  =  128 | ( icode & 63 ) ;
 
return  tos ( buffer , 3 ) ;
 
 }
 ;
 
 if ( ( icode <= 1114111 ) ) {
 
 buffer [/*ptr*/ 0 ]/*rbyteptr 1*/  =  240 | ( icode  >>  18 ) ;
 
 buffer [/*ptr*/ 1 ]/*rbyteptr 1*/  =  128 | ( ( icode  >>  12 ) & 63 ) ;
 
 buffer [/*ptr*/ 2 ]/*rbyteptr 1*/  =  128 | ( ( icode  >>  6 ) & 63 ) ;
 
 buffer [/*ptr*/ 3 ]/*rbyteptr 1*/  =  128 | ( icode & 63 ) ;
 
return  tos ( buffer , 4 ) ;
 
 }
 ;
 
return  tos2((byte*)"") ;
 
 }
 int string_utf32_code(string _rune) {
 
 if ( _rune .len == 0 ) {
 
return  0 ;
 
 }
 ;
 
 if ( _rune .len == 1 ) {
 
return  ((int)( _rune .str[ 0 ]/*rbyte 0*/ ) ) ;
 
 }
 ;
 
byte b= ((byte)( ((int)( _rune .str[ 0 ]/*rbyte 0*/ ) ) ) ) ;
 
 b  =  b  <<  _rune .len ;
 
int res= ((int)( b ) ) ;
 
int shift= 6 - _rune .len ;
 
 for (
int i= 1  ;  i < _rune .len  ;  i ++ ) { 
 
 
int c= ((int)( _rune .str[ i ]/*rbyte 0*/ ) ) ;
 
 res  =  res  <<  shift ;
 
 res  |=  c & 63 ;
 
 shift  =  6 ;
 
 }
 ;
 
return  res ;
 
 }
 u16* string_to_wide(string _str) {
 
 #ifdef _WIN32
 
int num_chars= ((int)( MultiByteToWideChar ( builtin__CP_UTF8 ,  0 , (char*) _str .str ,  _str .len ,  0 ,  0 ) ) ) ;
 
u16* wstr= ((u16*)( v_malloc ( ( num_chars + 1 ) * 2 ) ) ) ;
 
 if ( wstr > 0 ) {
 
 MultiByteToWideChar ( builtin__CP_UTF8 ,  0 , (char*) _str .str ,  _str .len ,  wstr ,  num_chars ) ;
 
 memset ((char*) ((byte*)( wstr ) ) + num_chars * 2 ,  0 ,  2 ) ;
 
 }
 ;
 
return  wstr ;
 
 #else
 
return  0 ;
 
 #endif
 ;
 
 }
 string string_from_wide(u16* _wstr) {
 
 #ifdef _WIN32
 
int wstr_len= ((int)( wcslen ( _wstr ) ) ) ;
 
return  string_from_wide2 ( _wstr , wstr_len ) ;
 
 #else
 
return  tos2((byte*)"") ;
 
 #endif
 ;
 
 }
 string string_from_wide2(u16* _wstr, int len) {
 
 #ifdef _WIN32
 
int num_chars= ((int)( WideCharToMultiByte ( builtin__CP_UTF8 ,  0 ,  _wstr ,  len ,  0 ,  0 ,  0 ,  0 ) ) ) ;
 
byte* str_to= ((byte*)( v_malloc ( num_chars + 1 ) ) ) ;
 
 if ( str_to > 0 ) {
 
 WideCharToMultiByte ( builtin__CP_UTF8 ,  0 ,  _wstr ,  len , (char*) str_to ,  num_chars ,  0 ,  0 ) ;
 
 memset ((char*) ((byte*)( str_to ) ) + num_chars ,  0 ,  1 ) ;
 
 }
 ;
 
return  tos2 ( str_to ) ;
 
 #else
 
return  tos2((byte*)"") ;
 
 #endif
 ;
 
 }
 int utf8_len(byte c) {
 
int b= 0 ;
 
byte x= c ;
 
 if ( ( ( x & 240 ) != 0 ) ) {
 
 x  >>=  4 ;
 
 }
  else { 
 
 b  +=  4 ;
 
 }
 ;
 
 if ( ( ( x & 12 ) != 0 ) ) {
 
 x  >>=  2 ;
 
 }
  else { 
 
 b  +=  2 ;
 
 }
 ;
 
 if ( ( ( x & 2 ) == 0 ) ) {
 
 b ++ ;
 
 }
 ;
 
return  b ;
 
 }
 int utf8_getchar() {
 
int c= ((int)( getchar ( ) ) ) ;
 
int len= utf8_len ( ~ c ) ;
 
 if ( c < 0 ) {
 
return  0 ;
 
 }
  else  if ( len == 0 ) {
 
return  c ;
 
 }
  else  if ( len == 1 ) {
 
return  - 1 ;
 
 }
  else { 
 
int uc= ((int)( c & ( ( 1  <<  ( 7 - len ) ) - 1 ) ) ) ;
 
 for (
int i= 0  ;  i + 1 < len  ;  i ++ ) { 
 
 
int c2= ((int)( getchar ( ) ) ) ;
 
 if ( c2 != - 1  &&  ( c2  >>  6 ) == 2 ) {
 
 uc  <<=  6 ;
 
 uc  |=  ((int)( ( c2 & 63 ) ) ) ;
 
 }
  else  if ( c2 == - 1 ) {
 
return  0 ;
 
 }
  else { 
 
return  - 1 ;
 
 }
 ;
 
 }
 ;
 
return  uc ;
 
 }
 ;
 
 }
 map new_map(int cap, int elm_size) {
 
map res= (map) { .element_size =  elm_size , .root =  0 , .size =  0 } ;
 
return  res ;
 
 }
 map new_map_init(int cap, int elm_size, string* keys, void* vals) {
 
map res= (map) { .element_size =  elm_size , .root =  0 , .size =  0 } ;
 
 int tmp3 =  0;
 ;
for (int tmp4 = tmp3; tmp4 <  cap; tmp4++) {
 int i = tmp4;
 
 
 map__set(& /* ? */ res , keys [/*ptr*/ i ]/*rstring 0*/ ,(byte*) vals + i * elm_size ) ;
 
 }
 ;
 
return  res ;
 
 }
 mapnode* new_node(string key, void* val, int element_size) {
 
mapnode* new_e= (mapnode*)memdup(&(mapnode)  { .key =  key , .val =  v_malloc ( element_size ) , .left =  0 , .right =  0 , .is_empty =  0 , } , sizeof(mapnode)) ;
 
 memcpy ( new_e ->val ,  val ,  element_size ) ;
 
return  new_e ;
 
 }
 void map_insert(map* m, mapnode* n, string key, void* val) {
 
 if (string_eq( n ->key , key ) ) {
 
 memcpy ( n ->val ,  val ,  m ->element_size ) ;
 
 return ;
 
 }
 ;
 
 if (string_gt( n ->key , key ) ) {
 
 if ( n ->left == 0 ) {
 
 n ->left  =  new_node ( key , val , m ->element_size ) ;
 
 m ->size ++ ;
 
 }
  else { 
 
 map_insert( m , n ->left , key , val ) ;
 
 }
 ;
 
 return ;
 
 }
 ;
 
 if ( n ->right == 0 ) {
 
 n ->right  =  new_node ( key , val , m ->element_size ) ;
 
 m ->size ++ ;
 
 }
  else { 
 
 map_insert( m , n ->right , key , val ) ;
 
 }
 ;
 
 }
 bool mapnode_find(mapnode* n, string key, void* out, int element_size) {
 
 if (string_eq( n ->key , key ) ) {
 
 memcpy ( out ,  n ->val ,  element_size ) ;
 
return  1 ;
 
 }
  else  if (string_gt( n ->key , key ) ) {
 
 if ( n ->left == 0 ) {
 
return  0 ;
 
 }
  else { 
 
return  mapnode_find(& /* ? */* n ->left , key , out , element_size ) ;
 
 }
 ;
 
 }
  else { 
 
 if ( n ->right == 0 ) {
 
return  0 ;
 
 }
  else { 
 
return  mapnode_find(& /* ? */* n ->right , key , out , element_size ) ;
 
 }
 ;
 
 }
 ;
 
 }
 bool mapnode_find2(mapnode* n, string key, int element_size) {
 
 if (string_eq( n ->key , key ) ) {
 
return  1 ;
 
 }
  else  if (string_gt( n ->key , key ) ) {
 
 if ( isnil ( n ->left ) ) {
 
return  0 ;
 
 }
  else { 
 
return  mapnode_find2(& /* ? */* n ->left , key , element_size ) ;
 
 }
 ;
 
 }
  else { 
 
 if ( isnil ( n ->right ) ) {
 
return  0 ;
 
 }
  else { 
 
return  mapnode_find2(& /* ? */* n ->right , key , element_size ) ;
 
 }
 ;
 
 }
 ;
 
 }
 void map__set(map* m, string key, void* val) {
 
 if ( isnil ( m ->root ) ) {
 
 m ->root  =  new_node ( key , val , m ->element_size ) ;
 
 m ->size ++ ;
 
 return ;
 
 }
 ;
 
 map_insert( m , m ->root , key , val ) ;
 
 }
 int preorder_keys(mapnode* node, array_string* keys, int key_i) {
 
int i= key_i ;
 
 if ( ! node ->is_empty ) {
 
array_string a= * keys ;
 
array_set(&/*q*/ a , i , & (string []) {  node ->key }) ;
 
 i ++ ;
 
 }
 ;
 
 if ( ! isnil ( node ->left ) ) {
 
 i  =  preorder_keys ( node ->left , keys , i ) ;
 
 }
 ;
 
 if ( ! isnil ( node ->right ) ) {
 
 i  =  preorder_keys ( node ->right , keys , i ) ;
 
 }
 ;
 
return  i ;
 
 }
 array_string map_keys(map* m) {
 
array_string keys= array_repeat2(new_array_from_c_array(1, 1, sizeof(string), (string[]) {  tos2((byte*)"")  }) , m ->size ) ;
 
 if ( isnil ( m ->root ) ) {
 
return  keys ;
 
 }
 ;
 
 preorder_keys ( m ->root ,& /*111*/ (array[]){ keys }[0]  , 0 ) ;
 
return  keys ;
 
 }
 bool map_get(map m, string key, void* out) {
 
 if ( m .root == 0 ) {
 
return  0 ;
 
 }
 ;
 
return  mapnode_find(& /* ? */* m .root , key , out , m .element_size ) ;
 
 }
 void v_mapnode_delete(mapnode* n, string key, int element_size) {
 
 if (string_eq( n ->key , key ) ) {
 
 memset ( n ->val ,  0 ,  element_size ) ;
 
 n ->is_empty  =  1 ;
 
 return ;
 
 }
  else  if (string_gt( n ->key , key ) ) {
 
 if ( isnil ( n ->left ) ) {
 
 return ;
 
 }
  else { 
 
 v_mapnode_delete( n ->left , key , element_size ) ;
 
 }
 ;
 
 }
  else { 
 
 if ( isnil ( n ->right ) ) {
 
 return ;
 
 }
  else { 
 
 v_mapnode_delete( n ->right , key , element_size ) ;
 
 }
 ;
 
 }
 ;
 
 }
 void v_map_delete(map* m, string key) {
 
 v_mapnode_delete( m ->root , key , m ->element_size ) ;
 
 m ->size -- ;
 
 }
 void map_exists(map m, string key) {
 
 v_panic ( tos2((byte*)"map.exists(key) was removed from the language. Use `key in map` instead.") ) ;
 
 }
 bool map__exists(map m, string key) {
 
return  ! isnil ( m .root )  &&  mapnode_find2(& /* ? */* m .root , key , m .element_size ) ;
 
 }
 void map_print(map m) {
 
 println ( tos2((byte*)"<<<<<<<<") ) ;
 
 println ( tos2((byte*)">>>>>>>>>>") ) ;
 
 }
 void v_mapnode_free(mapnode* n) {
 
 if ( n ->val != 0 ) {
 
 v_free ( n ->val ) ;
 
 }
 ;
 
 if ( n ->left != 0 ) {
 
 v_mapnode_free( n ->left ) ;
 
 }
 ;
 
 if ( n ->right != 0 ) {
 
 v_mapnode_free( n ->right ) ;
 
 }
 ;
 
 v_free ( n ) ;
 
 }
 void v_map_free(map* m) {
 
 if ( m ->root == 0 ) {
 
 return ;
 
 }
 ;
 
 v_mapnode_free( m ->root ) ;
 
 }
 string map_string_str(map_string m) {
 
 if ( m .size == 0 ) {
 
return  tos2((byte*)"{}") ;
 
 }
 ;
 
strings__Builder sb= strings__new_builder ( 50 ) ;
 
 strings__Builder_writeln(& /* ? */ sb , tos2((byte*)"{") ) ;
 
 map_string tmp10 =  m;
 array_string keys_tmp10 = map_keys(& tmp10 ); 
 for (int l = 0; l < keys_tmp10 .len; l++) {
 string key = ((string*)keys_tmp10 .data)[l];
 string val = {0}; map_get(tmp10, key, & val);
 
 
 strings__Builder_writeln(& /* ? */ sb , _STR("  \"%.*s\" => \"%.*s\"", key.len, key.str, val.len, val.str) ) ;
 
 }
 ;
 
 strings__Builder_writeln(& /* ? */ sb , tos2((byte*)"}") ) ;
 
return  strings__Builder_str( sb ) ;
 
 }
 Option opt_ok(void* data, int size) {
 
 if ( size >= 255 ) {
 
 v_panic ( _STR("option size too big: %d (max is 255), this is a temporary limit", size) ) ;
 
 }
 ;
 
Option res= (Option) { .ok =  1 , .error =  tos((byte *)"", 0) , } ;
 
 memcpy ( res .data ,  data ,  size ) ;
 
return  res ;
 
 }
 Option v_error(string s) {
 
return  (Option) { .error =  s , .ok =  0 } ;
 
 }
 int json__jsdecode_int(cJSON* root) {
 
 if ( isnil ( root ) ) {
 
return  0 ;
 
 }
 ;
 
return  root ->valueint ;
 
 }
 i8 json__jsdecode_i8(cJSON* root) {
 
 if ( isnil ( root ) ) {
 
return  ((i8)( 0 ) ) ;
 
 }
 ;
 
return  ((i8)( root ->valueint ) ) ;
 
 }
 i16 json__jsdecode_i16(cJSON* root) {
 
 if ( isnil ( root ) ) {
 
return  ((i16)( 0 ) ) ;
 
 }
 ;
 
return  ((i16)( root ->valueint ) ) ;
 
 }
 i64 json__jsdecode_i64(cJSON* root) {
 
 if ( isnil ( root ) ) {
 
return  ((i64)( 0 ) ) ;
 
 }
 ;
 
return  ((i64)( root ->valuedouble ) ) ;
 
 }
 byte json__jsdecode_byte(cJSON* root) {
 
 if ( isnil ( root ) ) {
 
return  ((byte)( 0 ) ) ;
 
 }
 ;
 
return  ((byte)( root ->valueint ) ) ;
 
 }
 u16 json__jsdecode_u16(cJSON* root) {
 
 if ( isnil ( root ) ) {
 
return  ((u16)( 0 ) ) ;
 
 }
 ;
 
return  ((u16)( root ->valueint ) ) ;
 
 }
 u32 json__jsdecode_u32(cJSON* root) {
 
 if ( isnil ( root ) ) {
 
return  ((u32)( 0 ) ) ;
 
 }
 ;
 
return  ((u32)( root ->valueint ) ) ;
 
 }
 u64 json__jsdecode_u64(cJSON* root) {
 
 if ( isnil ( root ) ) {
 
return  ((u64)( 0 ) ) ;
 
 }
 ;
 
return  ((u64)( root ->valueint ) ) ;
 
 }
 f32 json__jsdecode_f32(cJSON* root) {
 
 if ( isnil ( root ) ) {
 
return  ((f32)( 0 ) ) ;
 
 }
 ;
 
return  ((f32)( root ->valuedouble ) ) ;
 
 }
 f64 json__jsdecode_f64(cJSON* root) {
 
 if ( isnil ( root ) ) {
 
return  ((f64)( 0 ) ) ;
 
 }
 ;
 
return  ((f64)( root ->valuedouble ) ) ;
 
 }
 string json__jsdecode_string(cJSON* root) {
 
 if ( isnil ( root ) ) {
 
return  tos2((byte*)"") ;
 
 }
 ;
 
 if ( isnil ( root ->valuestring ) ) {
 
return  tos2((byte*)"") ;
 
 }
 ;
 
return  tos_clone ( root ->valuestring ) ;
 
 }
 bool json__jsdecode_bool(cJSON* root) {
 
 if ( isnil ( root ) ) {
 
return  0 ;
 
 }
 ;
 
return  cJSON_IsTrue ( root ) ;
 
 }
 cJSON* json__jsencode_int(int val) {
 
return  cJSON_CreateNumber ( val ) ;
 
 }
 cJSON* json__jsencode_i8(i8 val) {
 
return  cJSON_CreateNumber ( val ) ;
 
 }
 cJSON* json__jsencode_i16(i16 val) {
 
return  cJSON_CreateNumber ( val ) ;
 
 }
 cJSON* json__jsencode_i64(i64 val) {
 
return  cJSON_CreateNumber ( val ) ;
 
 }
 cJSON* json__jsencode_byte(byte val) {
 
return  cJSON_CreateNumber ( val ) ;
 
 }
 cJSON* json__jsencode_u16(u16 val) {
 
return  cJSON_CreateNumber ( val ) ;
 
 }
 cJSON* json__jsencode_u32(u32 val) {
 
return  cJSON_CreateNumber ( val ) ;
 
 }
 cJSON* json__jsencode_u64(u64 val) {
 
return  cJSON_CreateNumber ( val ) ;
 
 }
 cJSON* json__jsencode_f32(f32 val) {
 
return  cJSON_CreateNumber ( val ) ;
 
 }
 cJSON* json__jsencode_f64(f64 val) {
 
return  cJSON_CreateNumber ( val ) ;
 
 }
 cJSON* json__jsencode_bool(bool val) {
 
return  cJSON_CreateBool ( val ) ;
 
 }
 cJSON* json__jsencode_string(string val) {
 
string clone= string_clone( val ) ;
 
return  cJSON_CreateString ((char*) clone .str ) ;
 
 }
 cJSON* json__json_parse(string s) {
 
return  cJSON_Parse ((char*) s .str ) ;
 
 }
 string json__json_print(cJSON* json) {
 
void* s= cJSON_PrintUnformatted ( json ) ;
 
return  tos ( s , strlen ( s ) ) ;
 
 }
 strings__Builder strings__new_builder(int initial_size) {
 
return  (strings__Builder) { .buf =  new_array(0, 1, sizeof( byte )) , .len =  0 } ;
 
 }
 void strings__Builder_write(strings__Builder* b, string s) {
 
 array__push_many(& /* ? */ b ->buf , s .str , s .len ) ;
 
 b ->len  +=  s .len ;
 
 }
 void strings__Builder_writeln(strings__Builder* b, string s) {
 
 array__push_many(& /* ? */ b ->buf , s .str , s .len ) ;
 
_PUSH(& b ->buf , ( '\n' ), tmp1, byte) ;
 
 b ->len  +=  s .len + 1 ;
 
 }
 string strings__Builder_str(strings__Builder b) {
 
return  (tos((byte *) b .buf .data ,  b .len ) ) ;
 
 }
 void strings__Builder_cut(strings__Builder* b, int n) {
 
 b ->len  -=  n ;
 
 }
 void strings__Builder_free(strings__Builder* b) {
 
 v_free ( b ->buf .data ) ;
 
 }
 int strings__levenshtein_distance(string a, string b) {
 
array_int f= array_repeat2(new_array_from_c_array(1, 1, sizeof(int), (int[]) {  0  }) , b .len + 1 ) ;
 
 string tmp2 =  a;
 array_byte bytes_tmp2 = string_bytes( tmp2 );
 ;
for (int tmp3 = 0; tmp3 < tmp2 .len; tmp3 ++) {
 byte ca = ((byte *) bytes_tmp2 . data)[tmp3];
 
 
int j= 1 ;
 
int fj1= ( *(int*) array__get( f , 0) ) ;
 
 ( *(int*) array__get( f , 0) ) ++ ;
 
 string tmp10 =  b;
 array_byte bytes_tmp10 = string_bytes( tmp10 );
 ;
for (int tmp11 = 0; tmp11 < tmp10 .len; tmp11 ++) {
 byte cb = ((byte *) bytes_tmp10 . data)[tmp11];
 
 
int mn= ( ( *(int*) array__get( f , j) ) + 1 <= ( *(int*) array__get( f , j - 1) ) + 1 ) ? ( ( *(int*) array__get( f , j) ) + 1 ) : ( ( *(int*) array__get( f , j - 1) ) + 1 ) ;
 
 if ( cb != ca ) {
 
 mn  =  ( mn <= fj1 + 1 ) ? ( mn ) : ( fj1 + 1 ) ;
 
 }
  else { 
 
 mn  =  ( mn <= fj1 ) ? ( mn ) : ( fj1 ) ;
 
 }
 ;
 
 fj1  =  ( *(int*) array__get( f , j) ) ;
 
array_set(&/*q*/ f , j , & (int []) {  mn }) ;
 
 j ++ ;
 
 }
 ;
 
 }
 ;
 
return  ( *(int*) array__get( f , f .len - 1) ) ;
 
 }
 f32 strings__levenshtein_distance_percentage(string a, string b) {
 
int d= strings__levenshtein_distance ( a , b ) ;
 
int l= ( a .len >= b .len ) ? ( a .len ) : ( b .len ) ;
 
return  ( 1.00 - ((f32)( d ) ) / ((f32)( l ) ) ) * 100.00 ;
 
 }
 f32 strings__dice_coefficient(string s1, string s2) {
 
 if ( s1 .len == 0  ||  s2 .len == 0 ) {
 
return  0.0 ;
 
 }
 ;
 
 if (string_eq( s1 , s2 ) ) {
 
return  1.0 ;
 
 }
 ;
 
 if ( s1 .len < 2  ||  s2 .len < 2 ) {
 
return  0.0 ;
 
 }
 ;
 
string a= ( s1 .len > s2 .len ) ? ( s1 ) : ( s2 ) ;
 
string b= (string_eq( a , s1 ) ) ? ( s2 ) : ( s1 ) ;
 
map_int first_bigrams= new_map(1, sizeof(int)) ;
 
 for (
int i= 0  ;  i < a .len - 1  ;  i ++ ) { 
 
 
string bigram= string_substr( a , i , i + 2 ) ;
  
 int tmp32 = 0; bool tmp33 = map_get( first_bigrams , bigram, & tmp32); 

map__set(& first_bigrams , bigram , & (int []) {  (_IN_MAP(  bigram ,  first_bigrams ) ) ? ( tmp32 + 1 ) : ( 1 ) }) ;
 
 }
 ;
 
int intersection_size= 0 ;
 
 for (
int i= 0  ;  i < b .len - 1  ;  i ++ ) { 
 
 
string bigram= string_substr( b , i , i + 2 ) ;
  
 int tmp37 = 0; bool tmp38 = map_get( first_bigrams , bigram, & tmp37); 

int count= (_IN_MAP(  bigram ,  first_bigrams ) ) ? ( tmp37 ) : ( 0 ) ;
 
 if ( count > 0 ) {
 
map__set(& first_bigrams , bigram , & (int []) {  count - 1 }) ;
 
 intersection_size ++ ;
 
 }
 ;
 
 }
 ;
 
return  ( 2.0 * intersection_size ) / ( ((f32)( a .len ) ) + ((f32)( b .len ) ) - 2 ) ;
 
 }
 string strings__repeat(byte c, int n) {
 
 if ( n <= 0 ) {
 
return  tos2((byte*)"") ;
 
 }
 ;
 
array_byte arr= array_repeat2(new_array_from_c_array(1, 1, sizeof(byte), (byte[]) {  ((byte)( 0 ) )  }) , n + 1 ) ;
 
 for (
int i= 0  ;  i < n  ;  i ++ ) { 
 
 
array_set(&/*q*/ arr , i , & (byte []) {  c }) ;
 
 }
 ;
 
array_set(&/*q*/ arr , n , & (byte []) {  '\0' }) ;
 
return  (tos((byte *) arr .data ,  n ) ) ;
 
 }
 int main(int argc, char** argv) {
 init_consts();
 
string s= tos2((byte*)"[{\"name\":\"Frodo\", \"age\":25}, {\"name\":\"Bobby\", \"age\":10}]") ;
  
 array_User tmp2; cJSON* tmp3 = json__json_parse( s); 

Option_array_User tmp4 =  json__jsdecode_array_User(tmp3, &tmp2); cJSON_Delete(tmp3); ;
 if (!tmp4 .ok) {
 string err = tmp4 . error;
 
 eprintln ( tos2((byte*)"Failed to parse json") ) ;
 
 return 0 ;
 
 }
 array_User users = *(array_User*) tmp4 . data;
 ;
 
 array_User tmp5 =  users;
 for (int tmp6 = 0; tmp6 < tmp5.len; tmp6++) {
 User user = ((User *) tmp5 . data)[tmp6];
 
 
printf( "%.*s: %d\n", user .name.len, user .name.str, user .age ) ;
 
 }
 ;
 
 }
 void init_consts() {
#ifdef _WIN32
#ifndef _BOOTSTRAP_NO_UNICODE_STREAM
_setmode(_fileno(stdout), _O_U8TEXT);
SetConsoleMode(GetStdHandle(STD_OUTPUT_HANDLE), ENABLE_PROCESSED_OUTPUT | 0x0004);
// ENABLE_VIRTUAL_TERMINAL_PROCESSING
#endif
#endif
g_str_buf=malloc(1000);

}
 
string _STR(const char *fmt, ...) {
	va_list argptr;
	va_start(argptr, fmt);
	size_t len = vsnprintf(0, 0, fmt, argptr) + 1;
	va_end(argptr);
	byte* buf = malloc(len);
	va_start(argptr, fmt);
	vsprintf((char *)buf, fmt, argptr);
	va_end(argptr);
#ifdef DEBUG_ALLOC
	puts("_STR:");
	puts(buf);
#endif
	return tos2(buf);
}

string _STR_TMP(const char *fmt, ...) {
	va_list argptr;
	va_start(argptr, fmt);
	//size_t len = vsnprintf(0, 0, fmt, argptr) + 1;
	va_end(argptr);
	va_start(argptr, fmt);
	vsprintf((char *)g_str_buf, fmt, argptr);
	va_end(argptr);
#ifdef DEBUG_ALLOC
	//puts("_STR_TMP:");
	//puts(g_str_buf);
#endif
	return tos2(g_str_buf);
}


