#include "cuserdatabaseengine_sql.h"

//====================================================================================================
//конструктор класса
//====================================================================================================
CUserDatabaseEngine_SQL::CUserDatabaseEngine_SQL(const CString &user_list_table_name) 
{
 //настраиваем подключение к базам данных
 UserListTableName=user_list_table_name;
 UserListBaseInitString="ODBC;DRIVER=Microsoft Paradox Driver (*.db );FIL=Paradox 5.X;DBQ="+UserListTableName;
 cDatabaseEngine_SQL_Ptr=new CDatabaseEngine_SQL<CRAIICRecordset<CRecordset_UserList>,CUser>(UserListTableName);
}
//====================================================================================================
//деструктор класса
//====================================================================================================
CUserDatabaseEngine_SQL::~CUserDatabaseEngine_SQL() 
{
 delete(cDatabaseEngine_SQL_Ptr);
}
//====================================================================================================
//функции класса
//====================================================================================================

//----------------------------------------------------------------------------------------------------
//добавить пользователя
//----------------------------------------------------------------------------------------------------
bool CUserDatabaseEngine_SQL::AddUser(CUser& cUser)
{
 return(cDatabaseEngine_SQL_Ptr->Add(cUser));
}
//----------------------------------------------------------------------------------------------------
//изменить данные пользователя
//----------------------------------------------------------------------------------------------------
bool CUserDatabaseEngine_SQL::ChangeUser(long index,const CUser& cUser)
{
 CRAIICDatabase cRAIICDatabase(&cDatabase_UserList,UserListBaseInitString);
 {
  if (cRAIICDatabase.IsOpen()==false) return(false);
  CRAIICRecordset<CRecordset_UserList> cRAIICRecordset_UserList(&cDatabase_UserList,UserListTableName);
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
  CRAIICRecordset<CRecordset_UserList> cRAIICRecordset_UserList(&cDatabase_UserList,UserListTableName);
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
  CRAIICRecordset<CRecordset_UserList> cRAIICRecordset_UserList(&cDatabase_UserList,UserListTableName);
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
  CRAIICRecordset<CRecordset_UserList> cRAIICRecordset_UserList(&cDatabase_UserList,sql_request);
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
 //чтобы избежать SQL-инъекции, ищем пользователя самостоятельно
 CRAIICDatabase cRAIICDatabase(&cDatabase_UserList,UserListBaseInitString);
 {
  if (cRAIICDatabase.IsOpen()==false) return(false);
  CRAIICRecordset<CRecordset_UserList> cRAIICRecordset_UserList(&cDatabase_UserList,UserListTableName);
  if (cRAIICRecordset_UserList.IsOk()==false) return(false);
  if (cRAIICRecordset_UserList.GetMainObject().GetRecordCount()==0) return(false);
  cRAIICRecordset_UserList.GetMainObject().MoveFirst();
  while(cRAIICRecordset_UserList.GetMainObject().IsEOF()==FALSE)
  {
   cRAIICRecordset_UserList.GetMainObject().GetRecord(cUser);
   if (cUser.IsLoginAndPassword(login,password)==true) return(true);
   cRAIICRecordset_UserList.GetMainObject().MoveNext();
  }
 } 
 return(false);
}
//----------------------------------------------------------------------------------------------------
//найти пользователя по GUID
//----------------------------------------------------------------------------------------------------
bool CUserDatabaseEngine_SQL::FindUserByGUID(const CSafeString& guid,CUser& cUser)
{ 
 CString sql_condition="GUID='"+guid+"'"; 
 return(cDatabaseEngine_SQL_Ptr->FindOne(sql_condition,cUser));
}
//----------------------------------------------------------------------------------------------------
//изменить пользователя по GUID
//----------------------------------------------------------------------------------------------------
bool CUserDatabaseEngine_SQL::ChangeUserByGUID(const CSafeString& guid,const CUser& cUser)
{
 CString sql_condition="GUID='"+guid+"'";
 return(cDatabaseEngine_SQL_Ptr->Change(sql_condition,cUser));
}
//----------------------------------------------------------------------------------------------------
//удалить пользователя по GUID
//----------------------------------------------------------------------------------------------------
bool CUserDatabaseEngine_SQL::DeleteUserByGUID(const CSafeString& guid)
{
 CString sql_condition="GUID='"+guid+"'";
 return(cDatabaseEngine_SQL_Ptr->Delete(sql_condition));
}
//----------------------------------------------------------------------------------------------------
//очистить базу
//----------------------------------------------------------------------------------------------------
void CUserDatabaseEngine_SQL::ResetBase(void)
{ 
 cDatabaseEngine_SQL_Ptr->ResetBase();
}
