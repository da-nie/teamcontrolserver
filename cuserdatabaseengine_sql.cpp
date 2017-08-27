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
bool CUserDatabaseEngine_SQL::AddUser(CUser& cUser)
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
   cRAIICRecordset_UserList.GetMainObject().SetRecord(cUser); 
   cRAIICRecordset_UserList.GetMainObject().Update();
  }
 }
 return(true);
}
//----------------------------------------------------------------------------------------------------
//изменить данные пользователя
//----------------------------------------------------------------------------------------------------
bool CUserDatabaseEngine_SQL::ChangeUser(long index,const CUser& cUser)
{
 CRAIICDatabase cRAIICDatabase(&cDatabase_UserList,UserListBaseInitString);
 {
  if (cRAIICDatabase.IsOpen()==false) return(false);
  CRAIICRecordset_UserList cRAIICRecordset_UserList(&cDatabase_UserList,UserListTableName);
  if (cRAIICRecordset_UserList.IsOk()==false) return(false);
  if (cRAIICRecordset_UserList.GetMainObject().GetRecordCount()==0) return(false);
  cRAIICRecordset_UserList.GetMainObject().Move(index+1,SQL_FETCH_ABSOLUTE);
  cRAIICRecordset_UserList.GetMainObject().Edit();
  cRAIICRecordset_UserList.GetMainObject().SetRecord(cUser);   
  cRAIICRecordset_UserList.GetMainObject().Update();
 }
 return(true);
}
//----------------------------------------------------------------------------------------------------
//получить пользователя по индексу
//----------------------------------------------------------------------------------------------------
bool CUserDatabaseEngine_SQL::GetUser(long index,CUser &cUser)
{
 CRAIICDatabase cRAIICDatabase(&cDatabase_UserList,UserListBaseInitString);
 {
  if (cRAIICDatabase.IsOpen()==false) return(false);
  CRAIICRecordset_UserList cRAIICRecordset_UserList(&cDatabase_UserList,UserListTableName);
  if (cRAIICRecordset_UserList.IsOk()==false) return(false);
  if (cRAIICRecordset_UserList.GetMainObject().GetRecordCount()==0) return(false);
  cRAIICRecordset_UserList.GetMainObject().Move(index+1,SQL_FETCH_ABSOLUTE);
  cRAIICRecordset_UserList.GetMainObject().GetRecord(cUser);
 }
 return(true);
}
//----------------------------------------------------------------------------------------------------
//удалить пользователя по индексу
//----------------------------------------------------------------------------------------------------
bool CUserDatabaseEngine_SQL::DeleteUser(long index)
{
 CUser cUser_Deleted; 
 if (GetUser(index,cUser_Deleted)==false) return(false);
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
list<CUser> CUserDatabaseEngine_SQL::GetAllUser(void)
{ 
 list<CUser> list_CUser_Local; 
 CRAIICDatabase cRAIICDatabase(&cDatabase_UserList,UserListBaseInitString);
 {
  if (cRAIICDatabase.IsOpen()==false) return(false);
  CString sql_request="";
  sql_request+="SELECT * FROM ";
  sql_request+=UserListTableName;
  sql_request+=" ORDER BY Name";
  CRAIICRecordset_UserList cRAIICRecordset_UserList(&cDatabase_UserList,sql_request);
  if (cRAIICRecordset_UserList.IsOk()==false) return(list_CUser_Local);
  if (cRAIICRecordset_UserList.GetMainObject().GetRecordCount()==0) return(list_CUser_Local);
  cRAIICRecordset_UserList.GetMainObject().MoveFirst();
  while(cRAIICRecordset_UserList.GetMainObject().IsEOF()==FALSE)
  {
   CUser cUser;
   cRAIICRecordset_UserList.GetMainObject().GetRecord(cUser);
   list_CUser_Local.push_back(cUser);
   cRAIICRecordset_UserList.GetMainObject().MoveNext();
  }
 } 
 return(list_CUser_Local);
}
//----------------------------------------------------------------------------------------------------
//найти пользователя
//----------------------------------------------------------------------------------------------------
bool CUserDatabaseEngine_SQL::FindUserByLoginAndPassword(const CSafeString& login,const CSafeString& password,CUser& cUser)
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
  cRAIICRecordset_UserList.GetMainObject().GetRecord(cUser);
  return(true);
 } 
 return(false);
}
//----------------------------------------------------------------------------------------------------
//найти пользователя по GUID
//----------------------------------------------------------------------------------------------------
bool CUserDatabaseEngine_SQL::FindUserByGUID(const CSafeString& guid,CUser& cUser)
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
  cRAIICRecordset_UserList.GetMainObject().GetRecord(cUser);
  return(true);
 } 
 return(false);
}
//----------------------------------------------------------------------------------------------------
//изменить пользователя по GUID
//----------------------------------------------------------------------------------------------------
bool CUserDatabaseEngine_SQL::ChangeUserByGUID(const CSafeString& guid,const CUser& cUser)
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
  cRAIICRecordset_UserList.GetMainObject().Edit();
  cRAIICRecordset_UserList.GetMainObject().SetRecord(cUser);   
  cRAIICRecordset_UserList.GetMainObject().Update();
 }
 return(true);
}
//----------------------------------------------------------------------------------------------------
//удалить пользователя по GUID
//----------------------------------------------------------------------------------------------------
bool CUserDatabaseEngine_SQL::DeleteUserByGUID(const CSafeString& guid)
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
  cRAIICRecordset_UserList.GetMainObject().Delete();
 }
 return(true);
}
//----------------------------------------------------------------------------------------------------
//очистить базу
//----------------------------------------------------------------------------------------------------
void CUserDatabaseEngine_SQL::ResetBase(void)
{ 
 CRAIICDatabase cRAIICDatabase(&cDatabase_UserList,UserListBaseInitString);
 {  
  CString sql_request="";
  sql_request+="DELETE * FROM ";
  sql_request+=UserListTableName;
  cDatabase_UserList.ExecuteSQL(sql_request);
 }
}
