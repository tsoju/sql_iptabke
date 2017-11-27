#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mysql/mysql.h>

static char *tbl_name = "ip";

void finish_with_error(MYSQL *con){
  fprintf(stderr,"%s\n",mysql_error(con));
  mysql_close(con);
  exit(1);
}

int create_tbl(MYSQL *con, char *argv[]){
  char sql_str[255];

  snprintf(sql_str,sizeof(sql_str)-1,"CREATE TABLE %s(IP varchar(256),MAC varchar(256))",tbl_name);
  if(mysql_query(con,sql_str))
    return -1;

  return 0;
}

int add_ip(MYSQL *con, char *argv[]){
  char sql_str[255];

  if(argv[0] == NULL)
    return -1;

  snprintf(sql_str,sizeof(sql_str)-1,"DELETE from %s where ip=\"%s\"",tbl_name,argv[0]);
  if(mysql_query(con,sql_str))
    return -1;
  
  snprintf(sql_str,sizeof(sql_str)-1,"INSERT %s(IP,MAC) value(\"%s\",\"%s\")",tbl_name,argv[0],argv[1]);
  if(mysql_query(con,sql_str))
    return -1;
  
  return 0;
}

int del_ip(MYSQL *con, char *argv[]){
  char sql_str[255];

  if(argv[0] == NULL)
    return -1;

  if(strcmp(argv[0],"all") == 0)
    snprintf(sql_str,sizeof(sql_str)-1,"DELETE from %s",tbl_name);
  else
    snprintf(sql_str,sizeof(sql_str)-1,"DELETE from %s where ip=\"%s\"",tbl_name,argv[0]);
  if(mysql_query(con,sql_str))
    return -1;
  
  return 0;
}

int get_ip(MYSQL *con, char *argv[]){
  MYSQL_RES *resp;
  MYSQL_ROW row;
  char sql_str[255];
  int i;
  int num_field;
  
  snprintf(sql_str,sizeof(sql_str)-1,"SELECT * FROM %s",tbl_name);
  if(mysql_query(con,sql_str))
    return -1;

  resp = mysql_store_result(con);
  if(resp == NULL)
    return -1;

  num_field = mysql_num_fields(resp);

  while((row = mysql_fetch_row(resp)) != NULL){
    for(i=0;i<num_field;i++){
      printf("%s ", row[i] ? row[i]:"(null)");
    }
    printf("\n");
  }

  return 0;
}

int main(int argc, char *argv[]){
  MYSQL *con     = NULL;
  MYSQL_RES *resp = NULL;
  char *sql_serv  = "localhost";
  char *user      = "root";
  char *passwd    = "2fG6mg#iL";
  char *db_name   = "ip_lease";
  int i;
  int idx;

  struct cmd_tbl{
    char *cmd;
    int (*func)(MYSQL *con, char *argv[]);
  }tbl[]={
    {"add", add_ip},
    {"del", del_ip},
    {"get", get_ip},
    {"create", create_tbl},
  };

  for(i=0;i<sizeof(tbl)/sizeof(tbl[0]);i++){
    if(strcmp(argv[1],tbl[i].cmd) == 0){
      idx = i;
      break;
    }
  }
  if(i >= sizeof(tbl)/sizeof(tbl[0])){
    printf("Invalid command\n");
    return -1;
  }
  
  con = mysql_init(NULL);
  if(con == NULL){
    fprintf(stderr,"mysql_init() failed\n");
    exit(1);
  }
  
  if(mysql_real_connect(con,sql_serv,user,passwd,db_name,0,NULL,0) == NULL)
    finish_with_error(con);

  if(tbl[idx].func(con,&argv[2]) < 0)
    finish_with_error(con);
  
  mysql_free_result(resp);
  mysql_close(con);
  return 0;
}
