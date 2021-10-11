#include "cprojectdatabaseengine_software.h"

//====================================================================================================
//конструктор класса
//====================================================================================================
CProjectDatabaseEngine_Software::CProjectDatabaseEngine_Software(const CString &project_list_table_name) 
{
 //настраиваем подключение к базе данных
 ProjectListTableName=project_list_table_name;
 ProjectListBaseInitString="ODBC;DRIVER=Microsoft Paradox Driver (*.db );FIL=Paradox 5.X;DBQ="+ProjectListTableName;

 cRAIICDatabase_Ptr.reset(new CRAIICDatabase(&cDatabase_ProjectList,ProjectListBaseInitString));
}
//====================================================================================================
//деструктор класса
//====================================================================================================
CProjectDatabaseEngine_Software::~CProjectDatabaseEngine_Software() 
{
}
//====================================================================================================
//функции класса
//====================================================================================================

//----------------------------------------------------------------------------------------------------
//найти проект по GUID
//----------------------------------------------------------------------------------------------------
bool CProjectDatabaseEngine_Software::FindProjectByGUID(const CSafeString &guid,CProject &cProject)
{
 {
  if (cRAIICDatabase_Ptr->IsOpen()==false) return(false);
  CRAIICRecordset<CRecordset_ProjectList> cRAIICRecordset_ProjectList(&cDatabase_ProjectList,ProjectListTableName);
  if (cRAIICRecordset_ProjectList.IsOk()==false) return(false);
  if (cRAIICRecordset_ProjectList.GetMainObject().GetRecordCount()==0) return(false);
  cRAIICRecordset_ProjectList.GetMainObject().MoveFirst();
  while(cRAIICRecordset_ProjectList.GetMainObject().IsEOF()==FALSE)
  {
   cRAIICRecordset_ProjectList.GetMainObject().GetRecord(cProject);
   if (cProject.IsProjectGUID(guid)==true) return(true);
   cRAIICRecordset_ProjectList.GetMainObject().MoveNext();
  }
 } 
 return(false);
}

//----------------------------------------------------------------------------------------------------
//получить все проекты
//----------------------------------------------------------------------------------------------------
std::list<CProject> CProjectDatabaseEngine_Software::GetAllProject(void)
{
 std::list<CProject> list_CProject_Local; 
 {
  if (cRAIICDatabase_Ptr->IsOpen()==false) return(list_CProject_Local);
  CRAIICRecordset<CRecordset_ProjectList> cRAIICRecordset_ProjectList(&cDatabase_ProjectList,ProjectListTableName);
  if (cRAIICRecordset_ProjectList.IsOk()==false) return(list_CProject_Local);
  if (cRAIICRecordset_ProjectList.GetMainObject().GetRecordCount()==0) return(list_CProject_Local);
  cRAIICRecordset_ProjectList.GetMainObject().MoveFirst();
  while(cRAIICRecordset_ProjectList.GetMainObject().IsEOF()==FALSE)
  {
   CProject cProject;
   cRAIICRecordset_ProjectList.GetMainObject().GetRecord(cProject);
   cRAIICRecordset_ProjectList.GetMainObject().MoveNext();
   list_CProject_Local.push_back(cProject);
  }
 } 
 return(list_CProject_Local);
}

//----------------------------------------------------------------------------------------------------
//добавить проект
//----------------------------------------------------------------------------------------------------
bool CProjectDatabaseEngine_Software::AddProject(CProject &cProject)
{
 {
  if (cRAIICDatabase_Ptr->IsOpen()==false) return(false);
  CRAIICRecordset<CRecordset_ProjectList> cRAIICRecordset_ProjectList(&cDatabase_ProjectList,ProjectListTableName);
  if (cRAIICRecordset_ProjectList.IsOk()==false) return(false);
  if (cRAIICRecordset_ProjectList.GetMainObject().CanAppend()==TRUE)
  {
   if (cRAIICRecordset_ProjectList.GetMainObject().GetRecordCount()!=0) cRAIICRecordset_ProjectList.GetMainObject().MoveLast();
   cRAIICRecordset_ProjectList.GetMainObject().AddNew();
   cRAIICRecordset_ProjectList.GetMainObject().SetRecord(cProject);
   cRAIICRecordset_ProjectList.GetMainObject().Update();
  }
 }
 //автоинкрементное поле не используется, так что считывать заново не требуется
 return(true);
}
//----------------------------------------------------------------------------------------------------
//удалить проект
//----------------------------------------------------------------------------------------------------
bool CProjectDatabaseEngine_Software::DeleteProject(const CProject &cProject)
{
 {
  if (cRAIICDatabase_Ptr->IsOpen()==false) return(false);
  CRAIICRecordset<CRecordset_ProjectList> cRAIICRecordset_ProjectList(&cDatabase_ProjectList,ProjectListTableName);
  if (cRAIICRecordset_ProjectList.IsOk()==false) return(false);
  if (cRAIICRecordset_ProjectList.GetMainObject().GetRecordCount()==0) return(false);
  cRAIICRecordset_ProjectList.GetMainObject().MoveFirst();   
  while(cRAIICRecordset_ProjectList.GetMainObject().IsEOF()==FALSE)
  {
   CProject cProject_Local;
   cRAIICRecordset_ProjectList.GetMainObject().GetRecord(cProject_Local);
   if (cProject_Local.IsProjectGUID(cProject.GetProjectGUID())==true)
   {
    cRAIICRecordset_ProjectList.GetMainObject().Delete();
    return(true);
   }
   cRAIICRecordset_ProjectList.GetMainObject().MoveNext();
  }
 } 
 return(false);
}
//----------------------------------------------------------------------------------------------------
//изменить проект
//----------------------------------------------------------------------------------------------------
bool CProjectDatabaseEngine_Software::ChangeProject(const CProject &cProject)
{
 {
  if (cRAIICDatabase_Ptr->IsOpen()==false) return(false);
  CRAIICRecordset<CRecordset_ProjectList> cRAIICRecordset_ProjectList(&cDatabase_ProjectList,ProjectListTableName);
  if (cRAIICRecordset_ProjectList.IsOk()==false) return(false);
  if (cRAIICRecordset_ProjectList.GetMainObject().GetRecordCount()==0) return(false);
  cRAIICRecordset_ProjectList.GetMainObject().MoveFirst();
  while(cRAIICRecordset_ProjectList.GetMainObject().IsEOF()==FALSE)
  {
   CProject cProject_Local;
   cRAIICRecordset_ProjectList.GetMainObject().GetRecord(cProject_Local);
   if (cProject_Local.IsProjectGUID(cProject.GetProjectGUID())==true)
   {
    cRAIICRecordset_ProjectList.GetMainObject().Edit();
    cRAIICRecordset_ProjectList.GetMainObject().SetRecord(cProject);   
    cRAIICRecordset_ProjectList.GetMainObject().Update();
	return(true);
   }
   cRAIICRecordset_ProjectList.GetMainObject().MoveNext();
  }
 }
 return(false);
}
//----------------------------------------------------------------------------------------------------
//очистить базу
//----------------------------------------------------------------------------------------------------
void CProjectDatabaseEngine_Software::ResetBase(void)
{ 
 {
  CSafeString sql_request="";
  sql_request+="DELETE * FROM ";
  sql_request+=ProjectListTableName;
  cDatabase_ProjectList.ExecuteSQL(sql_request);
 }
}

