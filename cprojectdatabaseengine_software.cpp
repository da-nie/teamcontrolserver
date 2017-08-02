#include "cprojectdatabaseengine_software.h"

//====================================================================================================
//конструктор класса
//====================================================================================================
CProjectDatabaseEngine_Software::CProjectDatabaseEngine_Software(const CString &project_list_table_name) 
{
 //настраиваем подключение к базе данных
 ProjectListTableName=project_list_table_name;
 ProjectListBaseInitString="ODBC;DRIVER=Microsoft Paradox Driver (*.db );FIL=Paradox 5.X;DBQ="+ProjectListTableName;
/*
 //сохранение базы в файл
 FILE *file=fopen("base.csv","wb");
 list<STask> list_STask=GetAllTask();
 list<SUser> list_SUser=GetAllUser();

 list<STask>::iterator iterator=list_STask.begin();
 list<STask>::iterator iterator_end=list_STask.end();  
 while(iterator!=iterator_end)
 {
  STask &sTask=*iterator;  
  SUser sUser_From;
  SUser sUser_For; 
  
  list<SUser>::iterator iterator_user=list_SUser.begin();
  list<SUser>::iterator iterator_user_end=list_SUser.end();  
  while(iterator_user!=iterator_user_end)
  {
   SUser &sUser=*iterator_user;
   if (sUser.UserGUID.Compare(sTask.ForUserGUID)==0) sUser_For=sUser;
   if (sUser.UserGUID.Compare(sTask.FromUserGUID)==0) sUser_From=sUser;
   iterator_user++;
  }
  fprintf(file,"ќт: %s;ƒл€:%s;«адание:%s\r\n",sUser_From.Name,sUser_For.Name,sTask.Task);
  iterator++;
 }
 fclose(file);
*/
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
bool CProjectDatabaseEngine_Software::FindProjectByGUID(const CString &guid,SProject &sProject)
{
 CRAIICDatabase cRAIICDatabase(&cDatabase_ProjectList,ProjectListBaseInitString);
 {
  if (cRAIICDatabase.IsOpen()==false) return(false);
  {
   CRAIICRecordset_ProjectList cRAIICRecordset_ProjectList(&cDatabase_ProjectList,ProjectListTableName);
   if (cRAIICRecordset_ProjectList.IsOk()==false) return(false);
   if (cRAIICRecordset_ProjectList.GetMainObject().GetRecordCount()==0) return(false);
   cRAIICRecordset_ProjectList.GetMainObject().MoveFirst();
   while(cRAIICRecordset_ProjectList.GetMainObject().IsEOF()==FALSE)
   {
    cRAIICRecordset_ProjectList.GetMainObject().GetRecord(sProject);
    if (sProject.ProjectGUID.Compare(guid)==0) return(true);    
    cRAIICRecordset_ProjectList.GetMainObject().MoveNext();
   }
  }
 } 
 return(false);
}

//----------------------------------------------------------------------------------------------------
//получить все проекты
//----------------------------------------------------------------------------------------------------
list<SProject> CProjectDatabaseEngine_Software::GetAllProject(void)
{
 list<SProject> list_SProject_Local; 
 CRAIICDatabase cRAIICDatabase(&cDatabase_ProjectList,ProjectListBaseInitString);
 {
  if (cRAIICDatabase.IsOpen()==false) return(list_SProject_Local);
  {
   CRAIICRecordset_ProjectList cRAIICRecordset_ProjectList(&cDatabase_ProjectList,ProjectListTableName);
   if (cRAIICRecordset_ProjectList.IsOk()==false) return(list_SProject_Local);
   if (cRAIICRecordset_ProjectList.GetMainObject().GetRecordCount()==0) return(list_SProject_Local);
   cRAIICRecordset_ProjectList.GetMainObject().MoveFirst();
   while(cRAIICRecordset_ProjectList.GetMainObject().IsEOF()==FALSE)
   {
    SProject sProject;
    cRAIICRecordset_ProjectList.GetMainObject().GetRecord(sProject);
    cRAIICRecordset_ProjectList.GetMainObject().MoveNext();
    list_SProject_Local.push_back(sProject);
   }
  }
 } 
 return(list_SProject_Local);
}

//----------------------------------------------------------------------------------------------------
//добавить проект
//----------------------------------------------------------------------------------------------------
bool CProjectDatabaseEngine_Software::AddProject(SProject &sProject)
{
 CRAIICDatabase cRAIICDatabase(&cDatabase_ProjectList,ProjectListBaseInitString);
 {
  if (cRAIICDatabase.IsOpen()==false) return(false);
  {
   CRAIICRecordset_ProjectList cRAIICRecordset_ProjectList(&cDatabase_ProjectList,ProjectListTableName);
   if (cRAIICRecordset_ProjectList.IsOk()==false) return(false);
   if (cRAIICRecordset_ProjectList.GetMainObject().CanAppend()==TRUE)
   {
    if (cRAIICRecordset_ProjectList.GetMainObject().GetRecordCount()!=0) cRAIICRecordset_ProjectList.GetMainObject().MoveLast();
    cRAIICRecordset_ProjectList.GetMainObject().AddNew();
    cRAIICRecordset_ProjectList.GetMainObject().SetRecord(sProject);
    cRAIICRecordset_ProjectList.GetMainObject().Update();
   }
  }
 }
 //автоинкрементное поле не используетс€, так что считывать заново не требуетс€
 return(true);
}
//----------------------------------------------------------------------------------------------------
//удалить проект
//----------------------------------------------------------------------------------------------------
bool CProjectDatabaseEngine_Software::DeleteProject(const SProject &sProject)
{
 CRAIICDatabase cRAIICDatabase(&cDatabase_ProjectList,ProjectListBaseInitString);
 {
  if (cRAIICDatabase.IsOpen()==false) return(false);
  {
   CRAIICRecordset_ProjectList cRAIICRecordset_ProjectList(&cDatabase_ProjectList,ProjectListTableName);
   if (cRAIICRecordset_ProjectList.IsOk()==false) return(false);
   if (cRAIICRecordset_ProjectList.GetMainObject().GetRecordCount()==0) return(false);
   cRAIICRecordset_ProjectList.GetMainObject().MoveFirst();   
   while(cRAIICRecordset_ProjectList.GetMainObject().IsEOF()==FALSE)
   {
    SProject sProject_Local;
    cRAIICRecordset_ProjectList.GetMainObject().GetRecord(sProject_Local);
    if (sProject_Local.ProjectGUID.Compare(sProject.ProjectGUID)==0)
	{
     cRAIICRecordset_ProjectList.GetMainObject().Delete();
	 return(true);
	}
    cRAIICRecordset_ProjectList.GetMainObject().MoveNext();
   }
  }
 } 
 return(false);
}
//----------------------------------------------------------------------------------------------------
//изменить проект
//----------------------------------------------------------------------------------------------------
bool CProjectDatabaseEngine_Software::ChangeProject(const SProject &sProject)
{
 CRAIICDatabase cRAIICDatabase(&cDatabase_ProjectList,ProjectListBaseInitString);
 {
  if (cRAIICDatabase.IsOpen()==false) return(false);

  CRAIICRecordset_ProjectList cRAIICRecordset_ProjectList(&cDatabase_ProjectList,ProjectListTableName);
  if (cRAIICRecordset_ProjectList.IsOk()==false) return(false);
  if (cRAIICRecordset_ProjectList.GetMainObject().GetRecordCount()==0) return(false);
  cRAIICRecordset_ProjectList.GetMainObject().MoveFirst();
  while(cRAIICRecordset_ProjectList.GetMainObject().IsEOF()==FALSE)
  {
   SProject sProject_Local;
   cRAIICRecordset_ProjectList.GetMainObject().GetRecord(sProject_Local);
   if (sProject_Local.ProjectGUID.Compare(sProject.ProjectGUID)==0)
   {
    cRAIICRecordset_ProjectList.GetMainObject().Edit();
    cRAIICRecordset_ProjectList.GetMainObject().SetRecord(sProject);   
    cRAIICRecordset_ProjectList.GetMainObject().Update();
	return(true);
   }
   cRAIICRecordset_ProjectList.GetMainObject().MoveNext();
  }
 }
 return(false);
}

