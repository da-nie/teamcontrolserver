#include "ctaskdatabaseengine_software.h"

//====================================================================================================
//конструктор класса
//====================================================================================================
CTaskDatabaseEngine_Software::CTaskDatabaseEngine_Software(const CString &task_list_table_name) 
{
 //настраиваем подключение к базам данных
 TaskListTableName=task_list_table_name;
 TaskListBaseInitString="ODBC;DRIVER=Microsoft Paradox Driver (*.db );FIL=Paradox 5.X;DBQ="+TaskListTableName;

 cRAIICDatabase_Ptr.reset(new CRAIICDatabase(&cDatabase_TaskList,TaskListBaseInitString));
}
//====================================================================================================
//деструктор класса
//====================================================================================================
CTaskDatabaseEngine_Software::~CTaskDatabaseEngine_Software() 
{
}
//====================================================================================================
//функции класса
//====================================================================================================

//----------------------------------------------------------------------------------------------------
//найти задание по GUID
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
//получить все задания для и от пользователя с заданным GUID
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
    if (cTask.IsFromOrForUserGUID(guid)==true) list_CTask_Local.push_back(cTask);//это нам или наше задание
   }
  }
 }
 return(list_CTask_Local);
}
//----------------------------------------------------------------------------------------------------
//получить все задания
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
//получить общие задания
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
    //общие задания - это задания с отметкой
	if (cTask.IsCommon()==true) list_CTask_Local.push_back(cTask);
   }
  }
 }
 return(list_CTask_Local);
}

//----------------------------------------------------------------------------------------------------
//добавить задание
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
 //считаем задание заново, так как там есть автоинкрементное поле
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
//удалить задание
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
//изменить задание
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
      //если у задания поменялась общность
     if (cTask.IsCommon()!=cTask_Local.IsCommon()) common_change=true;
     //если у задания поменялся адресат, то отправляем старому адресату сообщение об удалении задания,
     //а новому о новом задании
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
//очистить базу
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
