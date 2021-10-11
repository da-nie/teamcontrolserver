#include "cuserdatabaseengine_software.h"

//====================================================================================================
//����������� ������
//====================================================================================================
CUserDatabaseEngine_Software::CUserDatabaseEngine_Software(const CString &user_list_table_name) 
{
 //����������� ����������� � ����� ������
 UserListTableName=user_list_table_name;
 UserListBaseInitString="ODBC;DRIVER=Microsoft Paradox Driver (*.db );FIL=Paradox 5.X;DBQ="+UserListTableName;

 cRAIICDatabase_Ptr.reset(new CRAIICDatabase(&cDatabase_UserList,UserListBaseInitString));
}
//====================================================================================================
//���������� ������
//====================================================================================================
CUserDatabaseEngine_Software::~CUserDatabaseEngine_Software() 
{
}
//====================================================================================================
//������� ������
//====================================================================================================

//----------------------------------------------------------------------------------------------------
//�������� ������������
//----------------------------------------------------------------------------------------------------
bool CUserDatabaseEngine_Software::AddUser(CUser& cUser)
{
 {
  if (cRAIICDatabase_Ptr->IsOpen()==false) return(false);
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
//�������� ������ ������������
//----------------------------------------------------------------------------------------------------
bool CUserDatabaseEngine_Software::ChangeUser(long index,const CUser& cUser)
{
 {
  if (cRAIICDatabase_Ptr->IsOpen()==false) return(false);
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
//�������� ������������ �� �������
//----------------------------------------------------------------------------------------------------
bool CUserDatabaseEngine_Software::GetUser(long index,CUser &cUser)
{
 {
  if (cRAIICDatabase_Ptr->IsOpen()==false) return(false);
  CRAIICRecordset<CRecordset_UserList> cRAIICRecordset_UserList(&cDatabase_UserList,UserListTableName);
  if (cRAIICRecordset_UserList.IsOk()==false) return(false);
  if (cRAIICRecordset_UserList.GetMainObject().GetRecordCount()==0) return(false);
  cRAIICRecordset_UserList.GetMainObject().Move(index+1,SQL_FETCH_ABSOLUTE);
  cRAIICRecordset_UserList.GetMainObject().GetRecord(cUser);
 }
 return(true);
}
//----------------------------------------------------------------------------------------------------
//������� ������������ �� �������
//----------------------------------------------------------------------------------------------------
bool CUserDatabaseEngine_Software::DeleteUser(long index)
{
 CUser cUser_Deleted; 
 if (GetUser(index,cUser_Deleted)==false) return(false);
 {
  if (cRAIICDatabase_Ptr->IsOpen()==false) return(false);
  CRAIICRecordset<CRecordset_UserList> cRAIICRecordset_UserList(&cDatabase_UserList,UserListTableName);
  if (cRAIICRecordset_UserList.IsOk()==false) return(false);
  if (cRAIICRecordset_UserList.GetMainObject().GetRecordCount()==0) return(false);  
  cRAIICRecordset_UserList.GetMainObject().Move(index+1,SQL_FETCH_ABSOLUTE);
  cRAIICRecordset_UserList.GetMainObject().Delete();
 }
 return(true);
}
//----------------------------------------------------------------------------------------------------
//�������� ������ ���� �������������
//----------------------------------------------------------------------------------------------------
std::list<CUser> CUserDatabaseEngine_Software::GetAllUser(void)
{ 
 std::list<CUser> list_CUser_Local; 
 {
  if (cRAIICDatabase_Ptr->IsOpen()==false) return(list_CUser_Local);
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
//����� ������������
//----------------------------------------------------------------------------------------------------
bool CUserDatabaseEngine_Software::FindUserByLoginAndPassword(const CSafeString& login,const CSafeString& password,CUser& cUser)
{ 
 {
  if (cRAIICDatabase_Ptr->IsOpen()==false) return(false);
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
//����� ������������ �� GUID
//----------------------------------------------------------------------------------------------------
bool CUserDatabaseEngine_Software::FindUserByGUID(const CSafeString& guid,CUser& cUser)
{ 
 {
  if (cRAIICDatabase_Ptr->IsOpen()==false) return(false);
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
//�������� ������������ �� GUID
//----------------------------------------------------------------------------------------------------
bool CUserDatabaseEngine_Software::ChangeUserByGUID(const CSafeString& guid,const CUser& cUser)
{
 {
  if (cRAIICDatabase_Ptr->IsOpen()==false) return(false);
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
//������� ������������ �� GUID
//----------------------------------------------------------------------------------------------------
bool CUserDatabaseEngine_Software::DeleteUserByGUID(const CSafeString& guid)
{
 {
  if (cRAIICDatabase_Ptr->IsOpen()==false) return(false);
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
//�������� ����
//----------------------------------------------------------------------------------------------------
void CUserDatabaseEngine_Software::ResetBase(void)
{ 
 {
  if (cRAIICDatabase_Ptr->IsOpen()==false) return;
  CSafeString sql_request="";
  sql_request+="DELETE * FROM ";
  sql_request+=UserListTableName;
  cDatabase_UserList.ExecuteSQL(sql_request);
 }
}
