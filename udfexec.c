#include <mysql/mysql.h>
#include <mysql/m_string.h>

my_bool exec_init(UDF_INIT *initid, UDF_ARGS *args, char *message);
my_ulonglong exec(UDF_INIT *initid, UDF_ARGS *args, char *is_null, char *error);
void exec_deinit(UDF_INIT *initid);

// exec関数実行前に呼ばれる
my_bool exec_init(UDF_INIT *initid, UDF_ARGS *args, char *message){
  // 引数が文字列1つであるかどうかを調べる
  if(args->arg_count == 1 && args->arg_type[0]==STRING_RESULT){
    return 0;
  } else {
    strcpy(message, "Expected exactly one string type parameter" );
    return 1;
  }
}

// exec関数呼び出し終了後に呼ばれる
void exec_deinit(UDF_INIT *initid){
  // 今回は後処理は何もなし
}

// exec関数本体
my_ulonglong exec(UDF_INIT *initid, UDF_ARGS *args, char *is_null, char *error){
  // 引数をそのまま system 関数に渡す
  return system(args->args[0]);
}
