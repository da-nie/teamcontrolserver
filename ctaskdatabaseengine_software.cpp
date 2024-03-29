#include "ctaskdatabaseengine_software.h"

//====================================================================================================
//����������� ������
//====================================================================================================
CTaskDatabaseEngine_Software::CTaskDatabaseEngine_Software(const CString &task_list_table_name) 
{
 //����������� ����������� � ����� ������
 TaskListTableName=task_list_table_name;
 TaskListBaseInitString="ODBC;DRIVER=Microsoft Paradox Driver (*.db );FIL=Paradox 5.X;DBQ="+TaskListTableName;

 cRAIICDatabase_Ptr.reset(new CRAIICDatabase(&cDatabase_TaskList,TaskListBaseInitString));
}
//====================================================================================================
//���������� ������
//====================================================================================================
CTaskDatabaseEngine_Software::~CTaskDatabaseEngine_Software() 
{
}
//====================================================================================================
//������� ������
//====================================================================================================

//----------------------------------------------------------------------------------------------------
//����� ������� �� GUID
//----------------------------------------------------------------------------------------------------
bool CTaskDatabaseEngine_Software::FindTaskByGUID(const CSafeString &guid,CTask &cTask)
{
 {
  if (cRAIICDatabase_Ptr->IsOpen()==false) return(false);
  {
   CRAIICRecordset<CRecordset_TaskList> cRAIICRecordset_TaskList(&cDatabase_TaskList,TaskListTableName);
   if (cRAIICRecordset_TaskList.IsOk()==false) return(false);
   if (cRAIICRecordset_TaskList.GetMainObject().GetRecordCount()==0) return(false);
   cRAIICRecordset_TaskList.GetMainObject().MoveFirst();
   while(cRAIICRecordset_TaskList.GetMainObject().IsEOF()==FALSE)
   {
    cRAIICRecordset_TaskList.GetMainObject().GetRecord(cTask);
    if (cTask.IsTaskGUID(guid)==true) return(true);
    cRAIICRecordset_TaskList.GetMainObject().MoveNext();
   }
  }
 } 
 return(false);
}
//----------------------------------------------------------------------------------------------------
//�������� ��� ������� ��� � �� ������������ � �������� GUID
//----------------------------------------------------------------------------------------------------
std::list<CTask> CTaskDatabaseEngine_Software::GetAllTaskForUserGUID(const CSafeString &guid)
{ 
 std::list<CTask> list_CTask_Local;  
 {
  if (cRAIICDatabase_Ptr->IsOpen()==false) return(list_CTask_Local);
  {
   CRAIICRecordset<CRecordset_TaskList> cRAIICRecordset_TaskList(&cDatabase_TaskList,TaskListTableName);
   if (cRAIICRecordset_TaskList.IsOk()==false) return(list_CTask_Local);
   if (cRAIICRecordset_TaskList.GetMainObject().GetRecordCount()==0) return(list_CTask_Local);
   cRAIICRecordset_TaskList.GetMainObject().MoveFirst();   
   while(cRAIICRecordset_TaskList.GetMainObject().IsEOF()==FALSE)
   {
    CTask cTask;
    cRAIICRecordset_TaskList.GetMainObject().GetRecord(cTask);
    cRAIICRecordset_TaskList.GetMainObject().MoveNext();
    if (cTask.IsFromOrForUserGUID(guid)==true) list_CTask_Local.push_back(cTask);//��� ��� ��� ���� �������
   }
  }
 }
 return(list_CTask_Local);
}
//----------------------------------------------------------------------------------------------------
//�������� ��� �������
//----------------------------------------------------------------------------------------------------
std::list<CTask> CTaskDatabaseEngine_Software::GetAllTask(void)
{
 std::list<CTask> list_CTask_Local; 
 {
  if (cRAIICDatabase_Ptr->IsOpen()==false) return(list_CTask_Local);
  {
   CRAIICRecordset<CRecordset_TaskList> cRAIICRecordset_TaskList(&cDatabase_TaskList,TaskListTableName);
   if (cRAIICRecordset_TaskList.IsOk()==false) return(list_CTask_Local);
   if (cRAIICRecordset_TaskList.GetMainObject().GetRecordCount()==0) return(list_CTask_Local);
   cRAIICRecordset_TaskList.GetMainObject().MoveFirst();
   while(cRAIICRecordset_TaskList.GetMainObject().IsEOF()==FALSE)
   {
    CTask cTask;
    cRAIICRecordset_TaskList.GetMainObject().GetRecord(cTask);
    cRAIICRecordset_TaskList.GetMainObject().MoveNext();
    list_CTask_Local.push_back(cTask);
   }
  }
 } 
 return(list_CTask_Local);
}
//----------------------------------------------------------------------------------------------------
//�������� ����� �������
//----------------------------------------------------------------------------------------------------
std::list<CTask> CTaskDatabaseEngine_Software::GetCommonTask(void)
{ 
 std::list<CTask> list_CTask_Local;  
 {
  if (cRAIICDatabase_Ptr->IsOpen()==false) return(list_CTask_Local);
  {
   CRAIICRecordset<CRecordset_TaskList> cRAIICRecordset_TaskList(&cDatabase_TaskList,TaskListTableName);
   if (cRAIICRecordset_TaskList.IsOk()==false) return(list_CTask_Local);
   if (cRAIICRecordset_TaskList.GetMainObject().GetRecordCount()==0) return(list_CTask_Local);
   cRAIICRecordset_TaskList.GetMainObject().MoveFirst();   
   while(cRAIICRecordset_TaskList.GetMainObject().IsEOF()==FALSE)
   {
    CTask cTask;
    cRAIICRecordset_TaskList.GetMainObject().GetRecord(cTask);
    cRAIICRecordset_TaskList.GetMainObject().MoveNext();
    //����� ������� - ��� ������� � ��������
	if (cTask.IsCommon()==true) list_CTask_Local.push_back(cTask);
   }
  }
 }
 return(list_CTask_Local);
}

//----------------------------------------------------------------------------------------------------
//�������� �������
//----------------------------------------------------------------------------------------------------
bool CTaskDatabaseEngine_Software::AddTask(CTask &cTask)
{
 cTask.SetState(TASK_STATE_NO_READ);
 {
  {
   if (cRAIICDatabase_Ptr->IsOpen()==false) return(false);
   {
    CRAIICRecordset<CRecordset_TaskList> cRAIICRecordset_TaskList(&cDatabase_TaskList,TaskListTableName);
    if (cRAIICRecordset_TaskList.IsOk()==false) return(false);
    if (cRAIICRecordset_TaskList.GetMainObject().CanAppend()==TRUE)
    {
     if (cRAIICRecordset_TaskList.GetMainObject().GetRecordCount()!=0) cRAIICRecordset_TaskList.GetMainObject().MoveLast();
     cRAIICRecordset_TaskList.GetMainObject().AddNew();
     cRAIICRecordset_TaskList.GetMainObject().SetRecord(cTask);
     cRAIICRecordset_TaskList.GetMainObject().Update();
    }
   }
  }
 }
 //������� ������� ������, ��� ��� ��� ���� ���������������� ����
 {
  if (cRAIICDatabase_Ptr->IsOpen()==false) return(false);
  {
   CRAIICRecordset<CRecordset_TaskList> cRAIICRecordset_TaskList(&cDatabase_TaskList,TaskListTableName);
   if (cRAIICRecordset_TaskList.IsOk()==false) return(false);
   if (cRAIICRecordset_TaskList.GetMainObject().GetRecordCount()==0) return(false);
   cRAIICRecordset_TaskList.GetMainObject().MoveFirst();
   while(cRAIICRecordset_TaskList.GetMainObject().IsEOF()==FALSE)
   {
    CTask cTask_Read;
    cRAIICRecordset_TaskList.GetMainObject().GetRecord(cTask_Read);
    cRAIICRecordset_TaskList.GetMainObject().MoveNext();
	if (cTask_Read.IsTaskGUID(cTask.GetTaskGUID())==true)
	{
     cTask=cTask_Read;
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
bool CTaskDatabaseEngine_Software::DeleteTask(const CTask &cTask)
{
 {
  if (cRAIICDatabase_Ptr->IsOpen()==false) return(false);
  {
   CRAIICRecordset<CRecordset_TaskList> cRAIICRecordset_TaskList(&cDatabase_TaskList,TaskListTableName);
   if (cRAIICRecordset_TaskList.IsOk()==false) return(false);
   if (cRAIICRecordset_TaskList.GetMainObject().GetRecordCount()==0) return(false);
   cRAIICRecordset_TaskList.GetMainObject().MoveFirst();   
   while(cRAIICRecordset_TaskList.GetMainObject().IsEOF()==FALSE)
   {
    CTask cTask_Local;
    cRAIICRecordset_TaskList.GetMainObject().GetRecord(cTask_Local);
	if (cTask_Local.IsTaskGUID(cTask.GetTaskGUID())==true)
	{
     cRAIICRecordset_TaskList.GetMainObject().Delete();
	 return(true);
	}
    cRAIICRecordset_TaskList.GetMainObject().MoveNext();
   }
  }
 } 
 return(false);
}
//----------------------------------------------------------------------------------------------------
//�������� �������
//----------------------------------------------------------------------------------------------------
bool CTaskDatabaseEngine_Software::ChangeTask(const CTask &cTask,bool &for_user_change,bool &common_change,CTask &cTask_Deleted,CTask &cTask_Added)
{
 for_user_change=false;
 common_change=false;
 {
  if (cRAIICDatabase_Ptr->IsOpen()==false) return(false);
  {
   CRAIICRecordset<CRecordset_TaskList> cRAIICRecordset_TaskList(&cDatabase_TaskList,TaskListTableName);
   if (cRAIICRecordset_TaskList.IsOk()==false) return(false);
   if (cRAIICRecordset_TaskList.GetMainObject().GetRecordCount()==0) return(false);
   cRAIICRecordset_TaskList.GetMainObject().MoveFirst();
   while(cRAIICRecordset_TaskList.GetMainObject().IsEOF()==FALSE)
   {
    CTask cTask_Local;
    cRAIICRecordset_TaskList.GetMainObject().GetRecord(cTask_Local);
	if (cTask_Local.IsTaskGUID(cTask.GetTaskGUID())==true)
	{
      //���� � ������� ���������� ��������
     if (cTask.IsCommon()!=cTask_Local.IsCommon()) common_change=true;
     //���� � ������� ��������� �������, �� ���������� ������� �������� ��������� �� �������� �������,
     //� ������ � ����� �������
     if (cTask_Local.IsForUserGUID(cTask.GetForUserGUID())==false)
	 {
      cTask_Deleted=cTask_Local;
	  cTask_Added=cTask;
	  for_user_change=true;
	 }
     cRAIICRecordset_TaskList.GetMainObject().Edit();
     cRAIICRecordset_TaskList.GetMainObject().SetRecord(cTask);   
     cRAIICRecordset_TaskList.GetMainObject().Update();
	 return(true);
	}
    cRAIICRecordset_TaskList.GetMainObject().MoveNext();
   }
  }
 } 
 return(false);
}
//----------------------------------------------------------------------------------------------------
//�������� ����
//----------------------------------------------------------------------------------------------------
void CTaskDatabaseEngine_Software::ResetBase(void)
{ 
 {
  if (cRAIICDatabase_Ptr->IsOpen()==false) return;
  CSafeString sql_request="";
  sql_request+="DELETE * FROM ";
  sql_request+=TaskListTableName;
  cDatabase_TaskList.ExecuteSQL(sql_request);
 }
}
