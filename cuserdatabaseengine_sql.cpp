#include "cuserdatabaseengine_sql.h"

//====================================================================================================
//конструктор класса
//====================================================================================================
CUserDatabaseEngine_SQL::CUserDatabaseEngine_SQL(const CString &user_list_table_name) 
{
 //настраиваем подключение к базам данных
 UserListTableName=user_list_table_name;
 UserListBaseInitString="ODBC;DRIVER=Microsoft Paradox Driver (*.db );FIL=Paradox 5.X;DBQ="+UserListTableName;
}
//====================================================================================================
//деструктор класса
//====================================================================================================
CUserDatabaseEngine_SQL::~CUserDatabaseEngine_SQL() 
{
}
//====================================================================================================
//функции класса
//====================================================================================================

//----------------------------------------------------------------------------------------------------
//добавить пользователя
//----------------------------------------------------------------------------------------------------
bool CUserDatabaseEngine_SQL::AddUser(SUser& sUser)
{
 CRAIICDatabase cRAIICDatabase(&cDatabase_UserList,UserListBaseInitString);
 {
  if (cRAIICDatabase.IsOpen()==false) return(false);
  CRAIICRecordset_UserList cRAIICRecordset_UserList(&cDatabase_UserList,UserListTableName);
  if (cRAIICRecordset_UserList.IsOk()==false) return(false);
  if (cRAIICRecordset_UserList.GetMainObject().CanAppend()==TRUE)
  {
   if (cRAIICRecordset_UserList.GetMainObject().GetRecordCount()!=0) cRAIICRecordset_UserList.GetMainObject().MoveLast();
   cRAIICRecordset_UserList.GetMainObject().AddNew();
   cRAIICRecordset_UserList.GetMainObject().SetRecord(sUser); 
   cRAIICRecordset_UserList.GetMainObject().Update();
  }
 }
 return(true);
}
//----------------------------------------------------------------------------------------------------
//изменить данные пользователя
//----------------------------------------------------------------------------------------------------
bool CUserDatabaseEngine_SQL::ChangeUser(long index,const SUser& sUser)
{
 CRAIICDatabase cRAIICDatabase(&cDatabase_UserList,UserListBaseInitString);
 {
  if (cRAIICDatabase.IsOpen()==false) return(false);
  CRAIICRecordset_UserList cRAIICRecordset_UserList(&cDatabase_UserList,UserListTableName);
  if (cRAIICRecordset_UserList.IsOk()==false) return(false);
  if (cRAIICRecordset_UserList.GetMainObject().GetRecordCount()==0) return(false);
  cRAIICRecordset_UserList.GetMainObject().Move(index+1,SQL_FETCH_ABSOLUTE);
  cRAIICRecordset_UserList.GetMainObject().Edit();
  cRAIICRecordset_UserList.GetMainObject().SetRecord(sUser);   
  cRAIICRecordset_UserList.GetMainObject().Update();
 }
 return(true);
}
//----------------------------------------------------------------------------------------------------
//получить пользователя по индексу
//----------------------------------------------------------------------------------------------------
bool CUserDatabaseEngine_SQL::GetUser(long index,SUser &sUser)
{
 CRAIICDatabase cRAIICDatabase(&cDatabase_UserList,UserListBaseInitString);
 {
  if (cRAIICDatabase.IsOpen()==false) return(false);
  CRAIICRecordset_UserList cRAIICRecordset_UserList(&cDatabase_UserList,UserListTableName);
  if (cRAIICRecordset_UserList.IsOk()==false) return(false);
  if (cRAIICRecordset_UserList.GetMainObject().GetRecordCount()==0) return(false);
  cRAIICRecordset_UserList.GetMainObject().Move(index+1,SQL_FETCH_ABSOLUTE);
  cRAIICRecordset_UserList.GetMainObject().GetRecord(sUser);
 }
 return(true);
}
//----------------------------------------------------------------------------------------------------
//удалить пользователя по индексу
//----------------------------------------------------------------------------------------------------
bool CUserDatabaseEngine_SQL::DeleteUser(long index)
{
 SUser sUser_Deleted; 
 if (GetUser(index,sUser_Deleted)==false) return(false);
 CRAIICDatabase cRAIICDatabase(&cDatabase_UserList,UserListBaseInitString);
 {
  if (cRAIICDatabase.IsOpen()==false) return(false);
  CRAIICRecordset_UserList cRAIICRecordset_UserList(&cDatabase_UserList,UserListTableName);
  if (cRAIICRecordset_UserList.IsOk()==false) return(false);
  if (cRAIICRecordset_UserList.GetMainObject().GetRecordCount()==0) return(false);  
  cRAIICRecordset_UserList.GetMainObject().Move(index+1,SQL_FETCH_ABSOLUTE);
  cRAIICRecordset_UserList.GetMainObject().Delete();
 }
 return(true);
}
//----------------------------------------------------------------------------------------------------
//получить список всех пользователей
//----------------------------------------------------------------------------------------------------
list<SUser> CUserDatabaseEngine_SQL::GetAllUser(void)
{ 
 list<SUser> list_SUser_Local; 
 CRAIICDatabase cRAIICDatabase(&cDatabase_UserList,UserListBaseInitString);
 {
  if (cRAIICDatabase.IsOpen()==false) return(false);
  CString sql_request="";
  sql_request+="SELECT * FROM ";
  sql_request+=UserListTableName;
  sql_request+=" ORDER BY Name";
  CRAIICRecordset_UserList cRAIICRecordset_UserList(&cDatabase_UserList,sql_request);
  if (cRAIICRecordset_UserList.IsOk()==false) return(list_SUser_Local);
  if (cRAIICRecordset_UserList.GetMainObject().GetRecordCount()==0) return(list_SUser_Local);
  cRAIICRecordset_UserList.GetMainObject().MoveFirst();
  while(cRAIICRecordset_UserList.GetMainObject().IsEOF()==FALSE)
  {
   SUser sUser;
   cRAIICRecordset_UserList.GetMainObject().GetRecord(sUser);
   list_SUser_Local.push_back(sUser);
   cRAIICRecordset_UserList.GetMainObject().MoveNext();
  }
 } 
 return(list_SUser_Local);
}
//----------------------------------------------------------------------------------------------------
//найти пользователя
//----------------------------------------------------------------------------------------------------
bool CUserDatabaseEngine_SQL::FindUserByLoginAndPassword(const CString& login,const CString& password,SUser& sUser)
{ 
 CRAIICDatabase cRAIICDatabase(&cDatabase_UserList,UserListBaseInitString);
 {
  if (cRAIICDatabase.IsOpen()==false) return(false);
  CString sql_request="";
  sql_request+="SELECT * FROM ";
  sql_request+=UserListTableName;
  sql_request+=" WHERE (Login='"+login+"') AND (Password='"+password+"')";
  CRAIICRecordset_UserList cRAIICRecordset_UserList(&cDatabase_UserList,sql_request);
  if (cRAIICRecordset_UserList.IsOk()==false) return(false);
  if (cRAIICRecordset_UserList.GetMainObject().GetRecordCount()==0) return(false);
  cRAIICRecordset_UserList.GetMainObject().MoveFirst();
  if (cRAIICRecordset_UserList.GetMainObject().IsEOF()==TRUE) return(false);
  cRAIICRecordset_UserList.GetMainObject().GetRecord(sUser);
  return(true);
 } 
 return(false);
}
//----------------------------------------------------------------------------------------------------
//найти пользователя по GUID
//----------------------------------------------------------------------------------------------------
bool CUserDatabaseEngine_SQL::FindUserByGUID(const CString& guid,SUser& sUser)
{ 
 CRAIICDatabase cRAIICDatabase(&cDatabase_UserList,UserListBaseInitString);
 {
  if (cRAIICDatabase.IsOpen()==false) return(false);
  CString sql_request="";
  sql_request+="SELECT * FROM ";
  sql_request+=UserListTableName;
  sql_request+=" WHERE (GUID='"+guid+"')";
  CRAIICRecordset_UserList cRAIICRecordset_UserList(&cDatabase_UserList,sql_request);
  if (cRAIICRecordset_UserList.IsOk()==false) return(false);
  if (cRAIICRecordset_UserList.GetMainObject().GetRecordCount()==0) return(false);
  cRAIICRecordset_UserList.GetMainObject().MoveFirst();
  if (cRAIICRecordset_UserList.GetMainObject().IsEOF()==TRUE) return(false);
  cRAIICRecordset_UserList.GetMainObject().GetRecord(sUser);
  return(true);
 } 
 return(false);
}
