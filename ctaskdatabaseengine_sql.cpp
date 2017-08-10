#include "ctaskdatabaseengine_sql.h"

//====================================================================================================
//����������� ������
//====================================================================================================
CTaskDatabaseEngine_SQL::CTaskDatabaseEngine_SQL(const CString &task_list_table_name) 
{
 //����������� ����������� � ����� ������
 TaskListTableName=task_list_table_name;
 TaskListBaseInitString="ODBC;DRIVER=Microsoft Paradox Driver (*.db );FIL=Paradox 5.X;DBQ="+TaskListTableName;
}
//====================================================================================================
//���������� ������
//====================================================================================================
CTaskDatabaseEngine_SQL::~CTaskDatabaseEngine_SQL() 
{
}
//====================================================================================================
//������� ������
//====================================================================================================

//----------------------------------------------------------------------------------------------------
//����� ������� �� GUID
//----------------------------------------------------------------------------------------------------
bool CTaskDatabaseEngine_SQL::FindTaskByGUID(const CSafeString &guid,STask &sTask)
{
 CRAIICDatabase cRAIICDatabase(&cDatabase_TaskList,TaskListBaseInitString);
 {
  if (cRAIICDatabase.IsOpen()==false) return(false);
  CString sql_request="";
  sql_request+="SELECT * FROM ";
  sql_request+=TaskListTableName;
  sql_request+=" WHERE (TaskGUID='"+guid+"')";
  CRAIICRecordset_TaskList cRAIICRecordset_TaskList(&cDatabase_TaskList,sql_request);
  if (cRAIICRecordset_TaskList.IsOk()==false) return(false);
  if (cRAIICRecordset_TaskList.GetMainObject().GetRecordCount()==0) return(false);
  cRAIICRecordset_TaskList.GetMainObject().MoveFirst();
  if (cRAIICRecordset_TaskList.GetMainObject().IsEOF()==TRUE) return(false);
  cRAIICRecordset_TaskList.GetMainObject().GetRecord(sTask);
  return(true);
 } 
 return(false);
}
//----------------------------------------------------------------------------------------------------
//�������� ��� ������� ��� � �� ������������ � �������� GUID
//----------------------------------------------------------------------------------------------------
list<STask> CTaskDatabaseEngine_SQL::GetAllTaskForUserGUID(const CSafeString &guid)
{ 
 list<STask> list_STask_Local;  
 CRAIICDatabase cRAIICDatabase(&cDatabase_TaskList,TaskListBaseInitString);
 {
  if (cRAIICDatabase.IsOpen()==false) return(list_STask_Local);
  CString sql_request="";
  sql_request+="SELECT * FROM ";
  sql_request+=TaskListTableName;
  sql_request+=" WHERE (FromUserGUID='"+guid+"') OR (ForUserGUID='"+guid+"')";
  CRAIICRecordset_TaskList cRAIICRecordset_TaskList(&cDatabase_TaskList,sql_request);
  if (cRAIICRecordset_TaskList.IsOk()==false) return(list_STask_Local);
  if (cRAIICRecordset_TaskList.GetMainObject().GetRecordCount()==0) return(list_STask_Local);
  cRAIICRecordset_TaskList.GetMainObject().MoveFirst();
  while(cRAIICRecordset_TaskList.GetMainObject().IsEOF()==FALSE)
  {
   STask sTask;
   cRAIICRecordset_TaskList.GetMainObject().GetRecord(sTask);
   list_STask_Local.push_back(sTask);//��� ��� ��� ���� �������
   cRAIICRecordset_TaskList.GetMainObject().MoveNext();
  }
 }
 return(list_STask_Local);
}
//----------------------------------------------------------------------------------------------------
//�������� ��� �������
//----------------------------------------------------------------------------------------------------
list<STask> CTaskDatabaseEngine_SQL::GetAllTask(void)
{
 list<STask> list_STask_Local; 
 CRAIICDatabase cRAIICDatabase(&cDatabase_TaskList,TaskListBaseInitString);
 {
  if (cRAIICDatabase.IsOpen()==false) return(list_STask_Local);
  {
   CRAIICRecordset_TaskList cRAIICRecordset_TaskList(&cDatabase_TaskList,TaskListTableName);
   if (cRAIICRecordset_TaskList.IsOk()==false) return(list_STask_Local);
   if (cRAIICRecordset_TaskList.GetMainObject().GetRecordCount()==0) return(list_STask_Local);
   cRAIICRecordset_TaskList.GetMainObject().MoveFirst();
   while(cRAIICRecordset_TaskList.GetMainObject().IsEOF()==FALSE)
   {
    STask sTask;
    cRAIICRecordset_TaskList.GetMainObject().GetRecord(sTask);
    cRAIICRecordset_TaskList.GetMainObject().MoveNext();
    list_STask_Local.push_back(sTask);
   }
  }
 } 
 return(list_STask_Local);
}
//----------------------------------------------------------------------------------------------------
//�������� �������
//----------------------------------------------------------------------------------------------------
bool CTaskDatabaseEngine_SQL::AddTask(STask &sTask)
{
 sTask.State=TASK_STATE_NO_READ;
 {
  CRAIICDatabase cRAIICDatabase(&cDatabase_TaskList,TaskListBaseInitString);
  {
   if (cRAIICDatabase.IsOpen()==false) return(false);
   {
    CRAIICRecordset_TaskList cRAIICRecordset_TaskList(&cDatabase_TaskList,TaskListTableName);
    if (cRAIICRecordset_TaskList.IsOk()==false) return(false);
    if (cRAIICRecordset_TaskList.GetMainObject().CanAppend()==TRUE)
    {
     if (cRAIICRecordset_TaskList.GetMainObject().GetRecordCount()!=0) cRAIICRecordset_TaskList.GetMainObject().MoveLast();
     cRAIICRecordset_TaskList.GetMainObject().AddNew();
     cRAIICRecordset_TaskList.GetMainObject().SetRecord(sTask);
     cRAIICRecordset_TaskList.GetMainObject().Update();
    }
   }
  }
 }
 //������� ������� ������, ��� ��� ��� ���� ���������������� ����
 CRAIICDatabase cRAIICDatabase(&cDatabase_TaskList,TaskListBaseInitString);
 {
  if (cRAIICDatabase.IsOpen()==false) return(false);
  {
   CRAIICRecordset_TaskList cRAIICRecordset_TaskList(&cDatabase_TaskList,TaskListTableName);
   if (cRAIICRecordset_TaskList.IsOk()==false) return(false);
   if (cRAIICRecordset_TaskList.GetMainObject().GetRecordCount()==0) return(false);
   cRAIICRecordset_TaskList.GetMainObject().MoveFirst();
   while(cRAIICRecordset_TaskList.GetMainObject().IsEOF()==FALSE)
   {
    STask sTask_Read;
    cRAIICRecordset_TaskList.GetMainObject().GetRecord(sTask_Read);
    cRAIICRecordset_TaskList.GetMainObject().MoveNext();
    if (sTask_Read.TaskGUID.Compare(sTask.TaskGUID)==0)
	{
     sTask=sTask_Read;
     break;
	}
   }
  }
 } 
 return(true);
}
//----------------------------------------------------------------------------------------------------
//������� �������
//----------------------------------------------------------------------------------------------------
bool CTaskDatabaseEngine_SQL::DeleteTask(const STask &sTask)
{
 CRAIICDatabase cRAIICDatabase(&cDatabase_TaskList,TaskListBaseInitString);
 {
  if (cRAIICDatabase.IsOpen()==false) return(false);
  CString sql_request="";
  sql_request+="SELECT * FROM ";
  sql_request+=TaskListTableName;
  sql_request+=" WHERE (TaskGUID='"+sTask.TaskGUID+"')";
  CRAIICRecordset_TaskList cRAIICRecordset_TaskList(&cDatabase_TaskList,sql_request);
  if (cRAIICRecordset_TaskList.IsOk()==false) return(false);
  if (cRAIICRecordset_TaskList.GetMainObject().GetRecordCount()==0) return(false);
  cRAIICRecordset_TaskList.GetMainObject().MoveFirst();
  if (cRAIICRecordset_TaskList.GetMainObject().IsEOF()==TRUE) return(false);
  STask sTask_Local;
  cRAIICRecordset_TaskList.GetMainObject().GetRecord(sTask_Local);
  cRAIICRecordset_TaskList.GetMainObject().Delete();
  return(true);
 } 
 return(false);
}
//----------------------------------------------------------------------------------------------------
//�������� �������
//----------------------------------------------------------------------------------------------------
bool CTaskDatabaseEngine_SQL::ChangeTask(const STask &sTask,bool &for_user_change,STask &sTask_Deleted,STask &sTask_Added)
{
 for_user_change=false;
 CRAIICDatabase cRAIICDatabase(&cDatabase_TaskList,TaskListBaseInitString);
 {
  if (cRAIICDatabase.IsOpen()==false) return(false);
  CString sql_request="";
  sql_request+="SELECT * FROM ";
  sql_request+=TaskListTableName;
  sql_request+=" WHERE (TaskGUID='"+sTask.TaskGUID+"')";
  CRAIICRecordset_TaskList cRAIICRecordset_TaskList(&cDatabase_TaskList,sql_request);
  if (cRAIICRecordset_TaskList.IsOk()==false) return(false);
  if (cRAIICRecordset_TaskList.GetMainObject().GetRecordCount()==0) return(false);
  cRAIICRecordset_TaskList.GetMainObject().MoveFirst();
  if (cRAIICRecordset_TaskList.GetMainObject().IsEOF()==TRUE) return(false);
  STask sTask_Local;
  cRAIICRecordset_TaskList.GetMainObject().GetRecord(sTask_Local);
  //���� � ������� ��������� �������, �� ���������� ������� �������� ��������� �� �������� �������,
  //� ������ � ����� �������
  if (sTask_Local.ForUserGUID.Compare(sTask.ForUserGUID)!=0)
  {
   sTask_Deleted=sTask_Local;
   sTask_Added=sTask;
   for_user_change=true;
  }
  cRAIICRecordset_TaskList.GetMainObject().Edit();
  cRAIICRecordset_TaskList.GetMainObject().SetRecord(sTask);   
  cRAIICRecordset_TaskList.GetMainObject().Update();
  return(true);
 } 
 return(false);
}
//----------------------------------------------------------------------------------------------------
//�������� ����
//----------------------------------------------------------------------------------------------------
void CTaskDatabaseEngine_SQL::ResetBase(void)
{ 
 CRAIICDatabase cRAIICDatabase(&cDatabase_TaskList,TaskListBaseInitString);
 {
  CString sql_request="";
  sql_request+="DELETE * FROM ";
  sql_request+=TaskListTableName;
  cDatabase_TaskList.ExecuteSQL(sql_request);
 }
}
