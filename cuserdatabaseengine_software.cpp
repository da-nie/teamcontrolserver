#include "cuserdatabaseengine_software.h"

//====================================================================================================
//конструктор класса
//====================================================================================================
CUserDatabaseEngine_Software::CUserDatabaseEngine_Software(const CString &user_list_table_name) 
{
 //настраиваем подключение к базам данных
 UserListTableName=user_list_table_name;
 UserListBaseInitString="ODBC;DRIVER=Microsoft Paradox Driver (*.db );FIL=Paradox 5.X;DBQ="+UserListTableName;
}
//====================================================================================================
//деструктор класса
//====================================================================================================
CUserDatabaseEngine_Software::~CUserDatabaseEngine_Software() 
{
}
//====================================================================================================
//функции класса
//====================================================================================================

//----------------------------------------------------------------------------------------------------
//добавить пользователя
//----------------------------------------------------------------------------------------------------
bool CUserDatabaseEngine_Software::AddUser(CUser& cUser)
{
 CRAIICDatabase cRAIICDatabase(&cDatabase_UserList,UserListBaseInitString);
 {
  if (cRAIICDatabase.IsOpen()==false) return(false);
  CRAIICRecordset<CRecordset_UserList> cRAIICRecordset_UserList(&cDatabase_UserList,UserListTableName);
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
bool CUserDatabaseEngine_Software::ChangeUser(long index,const CUser& cUser)
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
bool CUserDatabaseEngine_Software::GetUser(long index,CUser &cUser)
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
bool CUserDatabaseEngine_Software::DeleteUser(long index)
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
list<CUser> CUserDatabaseEngine_Software::GetAllUser(void)
{ 
 list<CUser> list_CUser_Local; 
 CRAIICDatabase cRAIICDatabase(&cDatabase_UserList,UserListBaseInitString);
 {
  if (cRAIICDatabase.IsOpen()==false) return(false);
  CRAIICRecordset<CRecordset_UserList> cRAIICRecordset_UserList(&cDatabase_UserList,UserListTableName);
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
bool CUserDatabaseEngine_Software::FindUserByLoginAndPassword(const CSafeString& login,const CSafeString& password,CUser& cUser)
{ 
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
bool CUserDatabaseEngine_Software::FindUserByGUID(const CSafeString& guid,CUser& cUser)
{ 
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
   if (cUser.IsUserGUID(guid)==true) return(true);
   cRAIICRecordset_UserList.GetMainObject().MoveNext();
  }
 } 
 return(false);
}
//----------------------------------------------------------------------------------------------------
//изменить пользователя по GUID
//----------------------------------------------------------------------------------------------------
bool CUserDatabaseEngine_Software::ChangeUserByGUID(const CSafeString& guid,const CUser& cUser)
{
 CRAIICDatabase cRAIICDatabase(&cDatabase_UserList,UserListBaseInitString);
 {
  if (cRAIICDatabase.IsOpen()==false) return(false);
  CRAIICRecordset<CRecordset_UserList> cRAIICRecordset_UserList(&cDatabase_UserList,UserListTableName);
  if (cRAIICRecordset_UserList.IsOk()==false) return(false);
  if (cRAIICRecordset_UserList.GetMainObject().GetRecordCount()==0) return(false);
  cRAIICRecordset_UserList.GetMainObject().MoveFirst();
  while(cRAIICRecordset_UserList.GetMainObject().IsEOF()==FALSE)
  {
   CUser cUser_Local;
   cRAIICRecordset_UserList.GetMainObject().GetRecord(cUser_Local);
   if (cUser_Local.IsUserGUID(guid)==true)
   {
    cRAIICRecordset_UserList.GetMainObject().Edit();
    cRAIICRecordset_UserList.GetMainObject().SetRecord(cUser);   
    cRAIICRecordset_UserList.GetMainObject().Update();
	return(true);    
   }
   cRAIICRecordset_UserList.GetMainObject().MoveNext();
  }
 } 
 return(false);
}
//----------------------------------------------------------------------------------------------------
//удалить пользователя по GUID
//----------------------------------------------------------------------------------------------------
bool CUserDatabaseEngine_Software::DeleteUserByGUID(const CSafeString& guid)
{
 CRAIICDatabase cRAIICDatabase(&cDatabase_UserList,UserListBaseInitString);
 {
  if (cRAIICDatabase.IsOpen()==false) return(false);
  CRAIICRecordset<CRecordset_UserList> cRAIICRecordset_UserList(&cDatabase_UserList,UserListTableName);
  if (cRAIICRecordset_UserList.IsOk()==false) return(false);
  if (cRAIICRecordset_UserList.GetMainObject().GetRecordCount()==0) return(false);
  cRAIICRecordset_UserList.GetMainObject().MoveFirst();
  while(cRAIICRecordset_UserList.GetMainObject().IsEOF()==FALSE)
  {
   CUser cUser;
   cRAIICRecordset_UserList.GetMainObject().GetRecord(cUser);
   if (cUser.IsUserGUID(guid)==true)
   {
    cRAIICRecordset_UserList.GetMainObject().Delete();
	return(true);    
   }
   cRAIICRecordset_UserList.GetMainObject().MoveNext();
  }
 } 
 return(false);
}

//----------------------------------------------------------------------------------------------------
//очистить базу
//----------------------------------------------------------------------------------------------------
void CUserDatabaseEngine_Software::ResetBase(void)
{ 
 CRAIICDatabase cRAIICDatabase(&cDatabase_UserList,UserListBaseInitString);
 {
  CSafeString sql_request="";
  sql_request+="DELETE * FROM ";
  sql_request+=UserListTableName;
  cDatabase_UserList.ExecuteSQL(sql_request);
 }
}
