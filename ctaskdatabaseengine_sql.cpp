#include "ctaskdatabaseengine_sql.h"

//====================================================================================================
//конструктор класса
//====================================================================================================
CTaskDatabaseEngine_SQL::CTaskDatabaseEngine_SQL(const CString &task_list_table_name) 
{
 //настраиваем подключение к базам данных
 TaskListTableName=task_list_table_name;
 TaskListBaseInitString="ODBC;DRIVER=Microsoft Paradox Driver (*.db );FIL=Paradox 5.X;DBQ="+TaskListTableName;
 cDatabaseEngine_SQL_Ptr=new CDatabaseEngine_SQL<CRAIICRecordset<CRecordset_TaskList>,CTask>(TaskListTableName);

 cRAIICDatabase_Ptr.reset(new CRAIICDatabase(&cDatabase_TaskList,TaskListBaseInitString));
}
//====================================================================================================
//деструктор класса
//====================================================================================================
CTaskDatabaseEngine_SQL::~CTaskDatabaseEngine_SQL() 
{
 delete(cDatabaseEngine_SQL_Ptr);
}
//====================================================================================================
//функции класса
//====================================================================================================

//----------------------------------------------------------------------------------------------------
//найти задание по GUID
//----------------------------------------------------------------------------------------------------
bool CTaskDatabaseEngine_SQL::FindTaskByGUID(const CSafeString &guid,CTask &cTask)
{
 CString sql_condition="TaskGUID='"+guid+"'"; 
 return(cDatabaseEngine_SQL_Ptr->FindOne(sql_condition,cTask));
}
//----------------------------------------------------------------------------------------------------
//получить все задания для и от пользователя с заданным GUID
//----------------------------------------------------------------------------------------------------
std::list<CTask> CTaskDatabaseEngine_SQL::GetAllTaskForUserGUID(const CSafeString &guid)
{ 
 CString sql_condition="FromUserGUID='"+guid+"') OR (ForUserGUID='"+guid+"'"; 
 return(cDatabaseEngine_SQL_Ptr->FindAll(sql_condition));
}
//----------------------------------------------------------------------------------------------------
//получить все задания
//----------------------------------------------------------------------------------------------------
std::list<CTask> CTaskDatabaseEngine_SQL::GetAllTask(void)
{
 return(cDatabaseEngine_SQL_Ptr->GetAll());
}

//----------------------------------------------------------------------------------------------------
//получить общие задания
//----------------------------------------------------------------------------------------------------
std::list<CTask> CTaskDatabaseEngine_SQL::GetCommonTask(void)
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
bool CTaskDatabaseEngine_SQL::AddTask(CTask &cTask)
{ 
 if (cDatabaseEngine_SQL_Ptr->Add(cTask)==false) return(false);
 //считаем задание заново, так как там есть автоинкрементное поле
 {
  if (cRAIICDatabase_Ptr->IsOpen()==false) return(false);
  {
   CRAIICRecordset<CRecordset_TaskList> cRAIICRecordset(&cDatabase_TaskList,TaskListTableName);
   if (cRAIICRecordset.IsOk()==false) return(false);
   if (cRAIICRecordset.GetMainObject().GetRecordCount()==0) return(false);
   cRAIICRecordset.GetMainObject().MoveFirst();
   while(cRAIICRecordset.GetMainObject().IsEOF()==FALSE)
   {
    CTask cTask_Read;
    cRAIICRecordset.GetMainObject().GetRecord(cTask_Read);
    cRAIICRecordset.GetMainObject().MoveNext();
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
bool CTaskDatabaseEngine_SQL::DeleteTask(const CTask &cTask)
{
 CString sql_condition="TaskGUID='"+cTask.GetTaskGUID()+"'";
 return(cDatabaseEngine_SQL_Ptr->Delete(sql_condition));
}
//----------------------------------------------------------------------------------------------------
//изменить задание
//----------------------------------------------------------------------------------------------------
bool CTaskDatabaseEngine_SQL::ChangeTask(const CTask &cTask,bool &for_user_change,bool &common_change,CTask &cTask_Deleted,CTask &cTask_Added)
{
 for_user_change=false;
 common_change=false;
 {
  if (cRAIICDatabase_Ptr->IsOpen()==false) return(false);
  CString sql_request="";
  sql_request+="SELECT * FROM ";
  sql_request+=TaskListTableName;
  sql_request+=" WHERE (TaskGUID='"+cTask.GetTaskGUID()+"')";
  CRAIICRecordset<CRecordset_TaskList> cRAIICRecordset_TaskList(&cDatabase_TaskList,sql_request);
  if (cRAIICRecordset_TaskList.IsOk()==false) return(false);
  if (cRAIICRecordset_TaskList.GetMainObject().GetRecordCount()==0) return(false);
  cRAIICRecordset_TaskList.GetMainObject().MoveFirst();
  if (cRAIICRecordset_TaskList.GetMainObject().IsEOF()==TRUE) return(false);
  CTask cTask_Local;
  cRAIICRecordset_TaskList.GetMainObject().GetRecord(cTask_Local);
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
 return(false);
}
//----------------------------------------------------------------------------------------------------
//очистить базу
//----------------------------------------------------------------------------------------------------
void CTaskDatabaseEngine_SQL::ResetBase(void)
{ 
 cDatabaseEngine_SQL_Ptr->ResetBase();
}
