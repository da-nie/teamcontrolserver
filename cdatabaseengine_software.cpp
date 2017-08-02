#include "cdatabaseengine_software.h"

//====================================================================================================
//����������� ������
//====================================================================================================
CDatabaseEngine_Software::CDatabaseEngine_Software(const CString &user_list_table_name,const CString &task_list_table_name,const CString &project_list_table_name) 
{
 //����������� ����������� � ����� ������
 UserListTableName=user_list_table_name;
 UserListBaseInitString="ODBC;DRIVER=Microsoft Paradox Driver (*.db );FIL=Paradox 5.X;DBQ="+UserListTableName;

 TaskListTableName=task_list_table_name;
 TaskListBaseInitString="ODBC;DRIVER=Microsoft Paradox Driver (*.db );FIL=Paradox 5.X;DBQ="+TaskListTableName;

 ProjectListTableName=project_list_table_name;
 ProjectListBaseInitString="ODBC;DRIVER=Microsoft Paradox Driver (*.db );FIL=Paradox 5.X;DBQ="+ProjectListTableName;
/*
 //���������� ���� � ����
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
  fprintf(file,"��: %s;���:%s;�������:%s\r\n",sUser_From.Name,sUser_For.Name,sTask.Task);
  iterator++;
 }
 fclose(file);
*/
}
//====================================================================================================
//���������� ������
//====================================================================================================
CDatabaseEngine_Software::~CDatabaseEngine_Software() 
{
}
//====================================================================================================
//������� ������
//====================================================================================================

//----------------------------------------------------------------------------------------------------
//�������� ������������
//----------------------------------------------------------------------------------------------------
bool CDatabaseEngine_Software::AddUser(SUser& sUser)
{
 CRAIICDatabase cRAIICDatabase(&cDatabase_UserList,UserListBaseInitString);
 {
  if (cRAIICDatabase.IsOpen()==false) return(false);
  {
   CRAIICRecordset_UserList cRAIICRecordset_UserList(&cDatabase_UserList,UserListTableName);
   if (cRAIICRecordset_UserList.IsOk()==false) return(false);
   if (cRAIICRecordset_UserList.GetMainObject().CanAppend()==TRUE)
   {
    if (cRAIICRecordset_UserList.GetMainObject().GetRecordCount()!=0) cRAIICRecordset_UserList.GetMainObject().MoveLast();
    cRAIICRecordset_UserList.GetMainObject().AddNew();
    cRAIICRecordset_UserList.GetMainObject().SetRecord(sUser); 
    cRAIICRecordset_UserList.GetMainObject().Update();
   }
  }
 }
 return(true);
}
//----------------------------------------------------------------------------------------------------
//�������� ������ ������������
//----------------------------------------------------------------------------------------------------
bool CDatabaseEngine_Software::ChangeUser(long index,const SUser& sUser)
{
 CRAIICDatabase cRAIICDatabase(&cDatabase_UserList,UserListBaseInitString);
 {
  if (cRAIICDatabase.IsOpen()==false) return(false);
  {
   CRAIICRecordset_UserList cRAIICRecordset_UserList(&cDatabase_UserList,UserListTableName);
   if (cRAIICRecordset_UserList.IsOk()==false) return(false);
   if (cRAIICRecordset_UserList.GetMainObject().GetRecordCount()==0) return(false);
   cRAIICRecordset_UserList.GetMainObject().Move(index+1,SQL_FETCH_ABSOLUTE);
   cRAIICRecordset_UserList.GetMainObject().Edit();
   cRAIICRecordset_UserList.GetMainObject().SetRecord(sUser);   
   cRAIICRecordset_UserList.GetMainObject().Update();
  }
 }
 return(true);
}
//----------------------------------------------------------------------------------------------------
//�������� ������������ �� �������
//----------------------------------------------------------------------------------------------------
bool CDatabaseEngine_Software::GetUser(long index,SUser &sUser)
{
 CRAIICDatabase cRAIICDatabase(&cDatabase_UserList,UserListBaseInitString);
 {
  if (cRAIICDatabase.IsOpen()==false) return(false);
  {
   CRAIICRecordset_UserList cRAIICRecordset_UserList(&cDatabase_UserList,UserListTableName);
   if (cRAIICRecordset_UserList.IsOk()==false) return(false);
   if (cRAIICRecordset_UserList.GetMainObject().GetRecordCount()==0) return(false);
   cRAIICRecordset_UserList.GetMainObject().Move(index+1,SQL_FETCH_ABSOLUTE);
   cRAIICRecordset_UserList.GetMainObject().GetRecord(sUser);
  }
 }
 return(true);
}
//----------------------------------------------------------------------------------------------------
//������� ������������ �� �������
//----------------------------------------------------------------------------------------------------
bool CDatabaseEngine_Software::DeleteUser(long index)
{
 SUser sUser_Deleted; 
 if (GetUser(index,sUser_Deleted)==false) return(false);
 CRAIICDatabase cRAIICDatabase(&cDatabase_UserList,UserListBaseInitString);
 {
  if (cRAIICDatabase.IsOpen()==false) return(false);
  {
   CRAIICRecordset_UserList cRAIICRecordset_UserList(&cDatabase_UserList,UserListTableName);
   if (cRAIICRecordset_UserList.IsOk()==false) return(false);
   if (cRAIICRecordset_UserList.GetMainObject().GetRecordCount()==0) return(false);  
   cRAIICRecordset_UserList.GetMainObject().Move(index+1,SQL_FETCH_ABSOLUTE);
   cRAIICRecordset_UserList.GetMainObject().Delete();
  }
 }
 return(true);
}
//----------------------------------------------------------------------------------------------------
//�������� ������ ���� �������������
//----------------------------------------------------------------------------------------------------
list<SUser> CDatabaseEngine_Software::GetAllUser(void)
{ 
 list<SUser> list_SUser_Local; 
 CRAIICDatabase cRAIICDatabase(&cDatabase_UserList,UserListBaseInitString);
 {
  if (cRAIICDatabase.IsOpen()==false) return(false);
  {
   CRAIICRecordset_UserList cRAIICRecordset_UserList(&cDatabase_UserList,UserListTableName);
   if (cRAIICRecordset_UserList.IsOk()==false) return(list_SUser_Local);
   if (cRAIICRecordset_UserList.GetMainObject().GetRecordCount()==0) return(list_SUser_Local);
   cRAIICRecordset_UserList.GetMainObject().MoveFirst();
   while(cRAIICRecordset_UserList.GetMainObject().IsEOF()==FALSE)
   {
    SUser sUser;
    cRAIICRecordset_UserList.GetMainObject().GetRecord(sUser);
    list_SUser_Local.push_back(sUser);
    cRAIICRecordset_UserList.GetMainObject().MoveNext();
   }
  }
 } 
 return(list_SUser_Local);
}
//----------------------------------------------------------------------------------------------------
//����� ������������
//----------------------------------------------------------------------------------------------------
bool CDatabaseEngine_Software::FindUserByLoginAndPassword(const CString& login,const CString& password,SUser& sUser)
{ 
 CRAIICDatabase cRAIICDatabase(&cDatabase_UserList,UserListBaseInitString);
 {
  if (cRAIICDatabase.IsOpen()==false) return(false);
  {
   CRAIICRecordset_UserList cRAIICRecordset_UserList(&cDatabase_UserList,UserListTableName);
   if (cRAIICRecordset_UserList.IsOk()==false) return(false);
   if (cRAIICRecordset_UserList.GetMainObject().GetRecordCount()==0) return(false);
   cRAIICRecordset_UserList.GetMainObject().MoveFirst();
   while(cRAIICRecordset_UserList.GetMainObject().IsEOF()==FALSE)
   {
    cRAIICRecordset_UserList.GetMainObject().GetRecord(sUser);
  	if (sUser.Login.Compare(login)==0 && sUser.Password.Compare(password)==0) return(true);    
    cRAIICRecordset_UserList.GetMainObject().MoveNext();
   }
  }
 } 
 return(false);
}
//----------------------------------------------------------------------------------------------------
//����� ������������ �� GUID
//----------------------------------------------------------------------------------------------------
bool CDatabaseEngine_Software::FindUserByGUID(const CString& guid,SUser& sUser)
{ 
 CRAIICDatabase cRAIICDatabase(&cDatabase_UserList,UserListBaseInitString);
 {
  if (cRAIICDatabase.IsOpen()==false) return(false);
  {
   CRAIICRecordset_UserList cRAIICRecordset_UserList(&cDatabase_UserList,UserListTableName);
   if (cRAIICRecordset_UserList.IsOk()==false) return(false);
   if (cRAIICRecordset_UserList.GetMainObject().GetRecordCount()==0) return(false);
   cRAIICRecordset_UserList.GetMainObject().MoveFirst();
   while(cRAIICRecordset_UserList.GetMainObject().IsEOF()==FALSE)
   {
    cRAIICRecordset_UserList.GetMainObject().GetRecord(sUser);
    if (sUser.UserGUID.Compare(guid)==0) return(true);    
    cRAIICRecordset_UserList.GetMainObject().MoveNext();
   }
  }
 } 
 return(false);
}
//----------------------------------------------------------------------------------------------------
//����� ������� �� GUID
//----------------------------------------------------------------------------------------------------
bool CDatabaseEngine_Software::FindTaskByGUID(const CString &guid,STask &sTask)
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
//����� ������ �� GUID
//----------------------------------------------------------------------------------------------------
bool CDatabaseEngine_Software::FindProjectByGUID(const CString &guid,SProject &sProject)
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
//�������� ��� ������� ��� � �� ������������ � �������� GUID
//----------------------------------------------------------------------------------------------------
list<STask> CDatabaseEngine_Software::GetAllTaskForUserGUID(const CString &guid)
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
    if (sTask.ForUserGUID.Compare(guid)==0 || sTask.FromUserGUID.Compare(guid)==0) list_STask_Local.push_back(sTask);//��� ��� ��� ���� �������
   }
  }
 }
 return(list_STask_Local);
}
//----------------------------------------------------------------------------------------------------
//�������� ��� �������
//----------------------------------------------------------------------------------------------------
list<STask> CDatabaseEngine_Software::GetAllTask(void)
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
//�������� ��� �������
//----------------------------------------------------------------------------------------------------
list<SProject> CDatabaseEngine_Software::GetAllProject(void)
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
//�������� �������
//----------------------------------------------------------------------------------------------------
bool CDatabaseEngine_Software::AddTask(STask &sTask)
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
bool CDatabaseEngine_Software::DeleteTask(const STask &sTask)
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
//�������� �������
//----------------------------------------------------------------------------------------------------
bool CDatabaseEngine_Software::ChangeTask(const STask &sTask,bool &for_user_change,STask &sTask_Deleted,STask &sTask_Added)
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
    cRAIICRecordset_TaskList.GetMainObject().MoveNext();
   }
  }
 } 
 return(false);
}

//----------------------------------------------------------------------------------------------------
//�������� ������
//----------------------------------------------------------------------------------------------------
bool CDatabaseEngine_Software::AddProject(SProject &sProject)
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
 //���������������� ���� �� ������������, ��� ��� ��������� ������ �� ���������
 return(true);
}
//----------------------------------------------------------------------------------------------------
//������� ������
//----------------------------------------------------------------------------------------------------
bool CDatabaseEngine_Software::DeleteProject(const SProject &sProject)
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
//�������� ������
//----------------------------------------------------------------------------------------------------
bool CDatabaseEngine_Software::ChangeProject(const SProject &sProject)
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

