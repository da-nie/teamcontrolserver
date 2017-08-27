#include "ctaskdatabaseengine_sql.h"

//====================================================================================================
//конструктор класса
//====================================================================================================
CTaskDatabaseEngine_SQL::CTaskDatabaseEngine_SQL(const CString &task_list_table_name) 
{
 //настраиваем подключение к базам данных
 TaskListTableName=task_list_table_name;
 TaskListBaseInitString="ODBC;DRIVER=Microsoft Paradox Driver (*.db );FIL=Paradox 5.X;DBQ="+TaskListTableName;
}
//====================================================================================================
//деструктор класса
//====================================================================================================
CTaskDatabaseEngine_SQL::~CTaskDatabaseEngine_SQL() 
{
}
//====================================================================================================
//функции класса
//====================================================================================================

//----------------------------------------------------------------------------------------------------
//найти задание по GUID
//----------------------------------------------------------------------------------------------------
bool CTaskDatabaseEngine_SQL::FindTaskByGUID(const CSafeString &guid,CTask &cTask)
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
  cRAIICRecordset_TaskList.GetMainObject().GetRecord(cTask);
  return(true);
 } 
 return(false);
}
//----------------------------------------------------------------------------------------------------
//получить все задания для и от пользователя с заданным GUID
//----------------------------------------------------------------------------------------------------
list<CTask> CTaskDatabaseEngine_SQL::GetAllTaskForUserGUID(const CSafeString &guid)
{ 
 list<CTask> list_CTask_Local;  
 CRAIICDatabase cRAIICDatabase(&cDatabase_TaskList,TaskListBaseInitString);
 {
  if (cRAIICDatabase.IsOpen()==false) return(list_CTask_Local);
  CString sql_request="";
  sql_request+="SELECT * FROM ";
  sql_request+=TaskListTableName;
  sql_request+=" WHERE (FromUserGUID='"+guid+"') OR (ForUserGUID='"+guid+"')";
  CRAIICRecordset_TaskList cRAIICRecordset_TaskList(&cDatabase_TaskList,sql_request);
  if (cRAIICRecordset_TaskList.IsOk()==false) return(list_CTask_Local);
  if (cRAIICRecordset_TaskList.GetMainObject().GetRecordCount()==0) return(list_CTask_Local);
  cRAIICRecordset_TaskList.GetMainObject().MoveFirst();
  while(cRAIICRecordset_TaskList.GetMainObject().IsEOF()==FALSE)
  {
   CTask cTask;
   cRAIICRecordset_TaskList.GetMainObject().GetRecord(cTask);
   list_CTask_Local.push_back(cTask);//это нам или наше задание
   cRAIICRecordset_TaskList.GetMainObject().MoveNext();
  }
 }
 return(list_CTask_Local);
}
//----------------------------------------------------------------------------------------------------
//получить все задания
//----------------------------------------------------------------------------------------------------
list<CTask> CTaskDatabaseEngine_SQL::GetAllTask(void)
{
 list<CTask> list_CTask_Local; 
 CRAIICDatabase cRAIICDatabase(&cDatabase_TaskList,TaskListBaseInitString);
 {
  if (cRAIICDatabase.IsOpen()==false) return(list_CTask_Local);
  {
   CRAIICRecordset_TaskList cRAIICRecordset_TaskList(&cDatabase_TaskList,TaskListTableName);
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
//добавить задание
//----------------------------------------------------------------------------------------------------
bool CTaskDatabaseEngine_SQL::AddTask(CTask &cTask)
{
 cTask.SetState(TASK_STATE_NO_READ);
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
     cRAIICRecordset_TaskList.GetMainObject().SetRecord(cTask);
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
    CTask cTask_Read;
    cRAIICRecordset_TaskList.GetMainObject().GetRecord(cTask_Read);
    cRAIICRecordset_TaskList.GetMainObject().MoveNext();

    if (cTask_Read.IsTaskGUIDCorrect(cTask.GetTaskGUID())==true)
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
bool CTaskDatabaseEngine_SQL::DeleteTask(const CTask &cTask)
{
 CRAIICDatabase cRAIICDatabase(&cDatabase_TaskList,TaskListBaseInitString);
 {
  if (cRAIICDatabase.IsOpen()==false) return(false);
  CString sql_request="";
  sql_request+="SELECT * FROM ";
  sql_request+=TaskListTableName;
  sql_request+=" WHERE (TaskGUID='"+cTask.GetTaskGUID()+"')";
  CRAIICRecordset_TaskList cRAIICRecordset_TaskList(&cDatabase_TaskList,sql_request);
  if (cRAIICRecordset_TaskList.IsOk()==false) return(false);
  if (cRAIICRecordset_TaskList.GetMainObject().GetRecordCount()==0) return(false);
  cRAIICRecordset_TaskList.GetMainObject().MoveFirst();
  if (cRAIICRecordset_TaskList.GetMainObject().IsEOF()==TRUE) return(false);
  CTask cTask_Local;
  cRAIICRecordset_TaskList.GetMainObject().GetRecord(cTask_Local);
  cRAIICRecordset_TaskList.GetMainObject().Delete();
  return(true);
 } 
 return(false);
}
//----------------------------------------------------------------------------------------------------
//изменить задание
//----------------------------------------------------------------------------------------------------
bool CTaskDatabaseEngine_SQL::ChangeTask(const CTask &cTask,bool &for_user_change,CTask &cTask_Deleted,CTask &cTask_Added)
{
 for_user_change=false;
 CRAIICDatabase cRAIICDatabase(&cDatabase_TaskList,TaskListBaseInitString);
 {
  if (cRAIICDatabase.IsOpen()==false) return(false);
  CString sql_request="";
  sql_request+="SELECT * FROM ";
  sql_request+=TaskListTableName;
  sql_request+=" WHERE (TaskGUID='"+cTask.GetTaskGUID()+"')";
  CRAIICRecordset_TaskList cRAIICRecordset_TaskList(&cDatabase_TaskList,sql_request);
  if (cRAIICRecordset_TaskList.IsOk()==false) return(false);
  if (cRAIICRecordset_TaskList.GetMainObject().GetRecordCount()==0) return(false);
  cRAIICRecordset_TaskList.GetMainObject().MoveFirst();
  if (cRAIICRecordset_TaskList.GetMainObject().IsEOF()==TRUE) return(false);
  CTask cTask_Local;
  cRAIICRecordset_TaskList.GetMainObject().GetRecord(cTask_Local);
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
 return(false);
}
//----------------------------------------------------------------------------------------------------
//очистить базу
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
