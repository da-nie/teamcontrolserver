#include "ctaskdatabaseengine_software.h"

//====================================================================================================
//конструктор класса
//====================================================================================================
CTaskDatabaseEngine_Software::CTaskDatabaseEngine_Software(const CString &task_list_table_name) 
{
 //настраиваем подключение к базам данных
 TaskListTableName=task_list_table_name;
 TaskListBaseInitString="ODBC;DRIVER=Microsoft Paradox Driver (*.db );FIL=Paradox 5.X;DBQ="+TaskListTableName;
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
bool CTaskDatabaseEngine_Software::FindTaskByGUID(const CString &guid,STask &sTask)
{
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
    cRAIICRecordset_TaskList.GetMainObject().GetRecord(sTask);
    if (sTask.TaskGUID.Compare(guid)==0) return(true);    
    cRAIICRecordset_TaskList.GetMainObject().MoveNext();
   }
  }
 } 
 return(false);
}
//----------------------------------------------------------------------------------------------------
//получить все задания для и от пользователя с заданным GUID
//----------------------------------------------------------------------------------------------------
list<STask> CTaskDatabaseEngine_Software::GetAllTaskForUserGUID(const CString &guid)
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
    if (sTask.ForUserGUID.Compare(guid)==0 || sTask.FromUserGUID.Compare(guid)==0) list_STask_Local.push_back(sTask);//это нам или наше задание
   }
  }
 }
 return(list_STask_Local);
}
//----------------------------------------------------------------------------------------------------
//получить все задания
//----------------------------------------------------------------------------------------------------
list<STask> CTaskDatabaseEngine_Software::GetAllTask(void)
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
//добавить задание
//----------------------------------------------------------------------------------------------------
bool CTaskDatabaseEngine_Software::AddTask(STask &sTask)
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
 //считаем задание заново, так как там есть автоинкрементное поле
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
//удалить задание
//----------------------------------------------------------------------------------------------------
bool CTaskDatabaseEngine_Software::DeleteTask(const STask &sTask)
{
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
    STask sTask_Local;
    cRAIICRecordset_TaskList.GetMainObject().GetRecord(sTask_Local);
    if (sTask_Local.TaskGUID.Compare(sTask.TaskGUID)==0)
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
bool CTaskDatabaseEngine_Software::ChangeTask(const STask &sTask,bool &for_user_change,STask &sTask_Deleted,STask &sTask_Added)
{
 for_user_change=false;
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
    STask sTask_Local;
    cRAIICRecordset_TaskList.GetMainObject().GetRecord(sTask_Local);
    if (sTask_Local.TaskGUID.Compare(sTask.TaskGUID)==0)
	{
     //если у задания поменялся адресат, то отправляем старому адресату сообщение об удалении задания,
     //а новому о новом задании
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
 CRAIICDatabase cRAIICDatabase(&cDatabase_TaskList,TaskListBaseInitString);
 {
  CString sql_request="";
  sql_request+="DELETE * FROM ";
  sql_request+=TaskListTableName;
  cDatabase_TaskList.ExecuteSQL(sql_request);
 }
}
