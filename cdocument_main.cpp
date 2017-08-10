#include "cdocument_main.h"
#include "cthreadserver.h"

extern CThreadServer cThreadServer;//��������� �����

//���� ������� ��-���������
#define THREAD_SERVER_PORT_DEFAULT 9090

//====================================================================================================
//����������� ������
//====================================================================================================
CDocument_Main::CDocument_Main(void) 
{
 {
  CRAIICCriticalSection cRAIICCriticalSection(&sProtectedVariables.cCriticalSection);
  {
   //������� ��������� �������
   FILE *file=fopen("server_settings.bin","rb");
   if (file!=NULL)
   {
    fread(&sProtectedVariables.sServerSettings,sizeof(SServerSettings),1,file);
    fclose(file);
   }
   else
   {
    sProtectedVariables.sServerSettings.Port=THREAD_SERVER_PORT_DEFAULT;
   }
/*   sProtectedVariables.cIUserDatabaseEngine_Ptr=new CUserDatabaseEngine_Software("UserBase");
   sProtectedVariables.cIProjectDatabaseEngine_Ptr=new CProjectDatabaseEngine_Software("ProjBase");
   sProtectedVariables.cITaskDatabaseEngine_Ptr=new CTaskDatabaseEngine_Software("TaskBase");
   */

   sProtectedVariables.cIUserDatabaseEngine_Ptr=new CUserDatabaseEngine_SQL("UserBase");
   sProtectedVariables.cIProjectDatabaseEngine_Ptr=new CProjectDatabaseEngine_SQL("ProjBase");
   sProtectedVariables.cITaskDatabaseEngine_Ptr=new CTaskDatabaseEngine_SQL("TaskBase");

   cThreadServer.SetDocument(this);
  }
 }
 cThreadServer.Start();
}
//====================================================================================================
//���������� ������
//====================================================================================================
CDocument_Main::~CDocument_Main() 
{
 cThreadServer.Stop();
 SaveState();
 delete(sProtectedVariables.cIUserDatabaseEngine_Ptr);
 delete(sProtectedVariables.cIProjectDatabaseEngine_Ptr);
 delete(sProtectedVariables.cITaskDatabaseEngine_Ptr);
}
//====================================================================================================
//������� ������
//====================================================================================================

//----------------------------------------------------------------------------------------------------
//���������� ���������
//----------------------------------------------------------------------------------------------------
void CDocument_Main::SaveState(void)
{
 {
  CRAIICCriticalSection cRAIICCriticalSection(&sProtectedVariables.cCriticalSection);
  {
   //���������� ��������� �������
   FILE *file=fopen("server_settings.bin","wb");
   if (file!=NULL)
   {
    fwrite(&sProtectedVariables.sServerSettings,sizeof(SServerSettings),1,file);
    fclose(file);
   }
  }
 }
}

//----------------------------------------------------------------------------------------------------
//�������� ������������
//----------------------------------------------------------------------------------------------------
bool CDocument_Main::AddUser(SUser& sUser)
{
 if (CreateGUID(sUser.UserGUID)==false)
 {
  MessageBox(NULL,"�� ���� ������� GUID (���������-���������� �������������) ������ ������������!","������",MB_OK);
  return(false);
 }
 {
  CRAIICCriticalSection cRAIICCriticalSection(&sProtectedVariables.cCriticalSection);
  {
   if (sProtectedVariables.cIUserDatabaseEngine_Ptr->AddUser(sUser)==false) return(false);
  }
 }
 SUser sUser_Added=sUser;
 cThreadServer.OnUserAdded(sUser_Added);//��������� ������, ��� ��������� ���������� ������������
 return(true);
}
//----------------------------------------------------------------------------------------------------
//�������� ������ ������������
//----------------------------------------------------------------------------------------------------
bool CDocument_Main::ChangeUser(long index,const SUser& sUser)
{
 {
  CRAIICCriticalSection cRAIICCriticalSection(&sProtectedVariables.cCriticalSection);
  {
   if (sProtectedVariables.cIUserDatabaseEngine_Ptr->ChangeUser(index,sUser)==false) return(false);
  }
 }
 SUser sUser_Changed=sUser;
 cThreadServer.OnUserChanged(sUser_Changed);//��������� ������, ��� ��������� ��������� ������ ������������
 return(true);
}
//----------------------------------------------------------------------------------------------------
//�������� ������������ �� �������
//----------------------------------------------------------------------------------------------------
bool CDocument_Main::GetUser(long index,SUser &sUser)
{
 {
  CRAIICCriticalSection cRAIICCriticalSection(&sProtectedVariables.cCriticalSection);
  {
   if (sProtectedVariables.cIUserDatabaseEngine_Ptr->GetUser(index,sUser)==false) return(false);
  }
 }
 return(true);
}
//----------------------------------------------------------------------------------------------------
//������� ������������ �� �������
//----------------------------------------------------------------------------------------------------
void CDocument_Main::DeleteUser(long index)
{
 SUser sUser_Deleted; 
 if (GetUser(index,sUser_Deleted)==false) return;
 {
  CRAIICCriticalSection cRAIICCriticalSection(&sProtectedVariables.cCriticalSection);
  {
   if (sProtectedVariables.cIUserDatabaseEngine_Ptr->DeleteUser(index)==false) return;
  }
 }
 cThreadServer.OnUserDeleted(sUser_Deleted);//��������� ������, ��� ��������� �������� ������������
}
//----------------------------------------------------------------------------------------------------
//�������� ������ ���� �������������
//----------------------------------------------------------------------------------------------------
list<SUser> CDocument_Main::GetAllUser(void)
{ 
 list<SUser> list_SUser_Local; 
 {
  CRAIICCriticalSection cRAIICCriticalSection(&sProtectedVariables.cCriticalSection);
  {
   return(sProtectedVariables.cIUserDatabaseEngine_Ptr->GetAllUser());
  }
 }
 return(list_SUser_Local);
}
//----------------------------------------------------------------------------------------------------
//����� ������������
//----------------------------------------------------------------------------------------------------
bool CDocument_Main::FindUserByLoginAndPassword(const CSafeString& login,const CSafeString& password,SUser& sUser)
{ 
 {
  CRAIICCriticalSection cRAIICCriticalSection(&sProtectedVariables.cCriticalSection);
  {
   return(sProtectedVariables.cIUserDatabaseEngine_Ptr->FindUserByLoginAndPassword(login,password,sUser));
  }
 }
 return(false);
}
//----------------------------------------------------------------------------------------------------
//����� ������������ �� GUID
//----------------------------------------------------------------------------------------------------
bool CDocument_Main::FindUserByGUID(const CSafeString& guid,SUser& sUser)
{ 
 {
  CRAIICCriticalSection cRAIICCriticalSection(&sProtectedVariables.cCriticalSection);
  {
   return(sProtectedVariables.cIUserDatabaseEngine_Ptr->FindUserByGUID(guid,sUser));
  }
 }
 return(false);
}
//----------------------------------------------------------------------------------------------------
//�������� ������������ �� GUID
//----------------------------------------------------------------------------------------------------
bool CDocument_Main::ChangeUserByGUID(const CSafeString& guid,const SUser& sUser)
{
 {
  CRAIICCriticalSection cRAIICCriticalSection(&sProtectedVariables.cCriticalSection);
  {
   if (sProtectedVariables.cIUserDatabaseEngine_Ptr->ChangeUserByGUID(guid,sUser)==false) return(false);
  }
 }
 SUser sUser_Changed=sUser;
 cThreadServer.OnUserChanged(sUser_Changed);//��������� ������, ��� ��������� ��������� ������ ������������
 return(true);
}
//----------------------------------------------------------------------------------------------------
//������� ������������ �� GUID
//----------------------------------------------------------------------------------------------------
bool CDocument_Main::DeleteUserByGUID(const CSafeString& guid)
{
 SUser sUser_Deleted; 
 if (FindUserByGUID(guid,sUser_Deleted)==false) return(false);
 {
  CRAIICCriticalSection cRAIICCriticalSection(&sProtectedVariables.cCriticalSection);
  {
   if (sProtectedVariables.cIUserDatabaseEngine_Ptr->DeleteUserByGUID(guid)==false) return(false);
  }
 }
 cThreadServer.OnUserDeleted(sUser_Deleted);//��������� ������, ��� ��������� �������� ������������
 return(true);
}

//----------------------------------------------------------------------------------------------------
//����� ������� �� GUID
//----------------------------------------------------------------------------------------------------
bool CDocument_Main::FindTaskByGUID(const CSafeString &guid,STask &sTask)
{
 {
  CRAIICCriticalSection cRAIICCriticalSection(&sProtectedVariables.cCriticalSection);
  {
   return(sProtectedVariables.cITaskDatabaseEngine_Ptr->FindTaskByGUID(guid,sTask));
  }
 }
 return(false);
}
//----------------------------------------------------------------------------------------------------
//����� ������ �� GUID
//----------------------------------------------------------------------------------------------------
bool CDocument_Main::FindProjectByGUID(const CSafeString &guid,SProject &sProject)
{
 {
  CRAIICCriticalSection cRAIICCriticalSection(&sProtectedVariables.cCriticalSection);
  {
   return(sProtectedVariables.cIProjectDatabaseEngine_Ptr->FindProjectByGUID(guid,sProject));
  }
 }
 return(false);
}

//----------------------------------------------------------------------------------------------------
//�������� ��������� �������
//----------------------------------------------------------------------------------------------------
void CDocument_Main::GetServerSettings(SServerSettings &sServerSettings)
{
 {
  CRAIICCriticalSection cRAIICCriticalSection(&sProtectedVariables.cCriticalSection);
  {
   sServerSettings=sProtectedVariables.sServerSettings;
  }
 }
}
//----------------------------------------------------------------------------------------------------
//���������� ��������� ������� (������ ����� �����������)
//----------------------------------------------------------------------------------------------------
void CDocument_Main::SetServerSettings(const SServerSettings &sServerSettings)
{
 {
  CRAIICCriticalSection cRAIICCriticalSection(&sProtectedVariables.cCriticalSection);
  {
   sProtectedVariables.sServerSettings=sServerSettings;
  }
 }
 //������������� ������ � ������ �����������
 cThreadServer.Start();
}
//----------------------------------------------------------------------------------------------------
//�������� ��� ������� ��� � �� ������������ � �������� GUID
//----------------------------------------------------------------------------------------------------
list<STask> CDocument_Main::GetAllTaskForUserGUID(const CSafeString &guid)
{ 
 list<STask> list_STask_Local;  
 {
  CRAIICCriticalSection cRAIICCriticalSection(&sProtectedVariables.cCriticalSection);
  {
   return(sProtectedVariables.cITaskDatabaseEngine_Ptr->GetAllTaskForUserGUID(guid));
  }
 }
 return(list_STask_Local);
}
//----------------------------------------------------------------------------------------------------
//�������� ��� �������
//----------------------------------------------------------------------------------------------------
list<STask> CDocument_Main::GetAllTask(void)
{
 list<STask> list_STask_Local; 
 {
  CRAIICCriticalSection cRAIICCriticalSection(&sProtectedVariables.cCriticalSection);
  {
   return(sProtectedVariables.cITaskDatabaseEngine_Ptr->GetAllTask());
  }
 }
 return(list_STask_Local);
}

//----------------------------------------------------------------------------------------------------
//�������� ��� �������
//----------------------------------------------------------------------------------------------------
list<SProject> CDocument_Main::GetAllProject(void)
{
 list<SProject> list_SProject_Local; 
 {
  CRAIICCriticalSection cRAIICCriticalSection(&sProtectedVariables.cCriticalSection);
  {
   return(sProtectedVariables.cIProjectDatabaseEngine_Ptr->GetAllProject());
  }
 }
 return(list_SProject_Local);
}
//----------------------------------------------------------------------------------------------------
//�������� �������
//----------------------------------------------------------------------------------------------------
bool CDocument_Main::AddTask(STask &sTask)
{
 if (CreateGUID(sTask.TaskGUID)==false) return(false);
 sTask.State=TASK_STATE_NO_READ;
 {
  CRAIICCriticalSection cRAIICCriticalSection(&sProtectedVariables.cCriticalSection);
  {
   if (sProtectedVariables.cITaskDatabaseEngine_Ptr->AddTask(sTask)==false) return(false);
  }
 }
 STask sTask_Added=sTask;
 cThreadServer.OnTaskAdded(sTask_Added);//��������� ������, ��� ��������� ���������� �������
 return(true);
}
//----------------------------------------------------------------------------------------------------
//������� �������
//----------------------------------------------------------------------------------------------------
bool CDocument_Main::DeleteTask(const STask &sTask)
{
 {
  CRAIICCriticalSection cRAIICCriticalSection(&sProtectedVariables.cCriticalSection);
  {
   if (sProtectedVariables.cITaskDatabaseEngine_Ptr->DeleteTask(sTask)==false) return(false);
  }
 }
 STask sTask_Deleted=sTask;
 cThreadServer.OnTaskDeleted(sTask_Deleted);//��������� ������, ��� ��������� �������� �������
 return(true);
}
//----------------------------------------------------------------------------------------------------
//�������� �������
//----------------------------------------------------------------------------------------------------
bool CDocument_Main::ChangeTask(const STask &sTask)
{
 STask sTask_Deleted;
 STask sTask_Added;
 bool for_user_change=false;
 {
  CRAIICCriticalSection cRAIICCriticalSection(&sProtectedVariables.cCriticalSection);
  {
   if (sProtectedVariables.cITaskDatabaseEngine_Ptr->ChangeTask(sTask,for_user_change,sTask_Deleted,sTask_Added)==false) return(false);
  }
 }
 if (for_user_change==true)//������� � ������� ��������
 {
  cThreadServer.OnTaskDeleted(sTask_Deleted);//��������� ������, ��� ��������� �������� �������
  cThreadServer.OnTaskAdded(sTask_Added);//��������� ������, ��� ��������� ���������� �������
  return(true);
 } 
 //��������� ��� ������ ��������
 STask sTask_Changed=sTask;
 cThreadServer.OnTaskChanged(sTask_Changed);//��������� ������, ��� ��������� ��������� ������ �������
 return(true);
}

//----------------------------------------------------------------------------------------------------
//�������� ������
//----------------------------------------------------------------------------------------------------
bool CDocument_Main::AddProject(SProject &sProject)
{
 if (CreateGUID(sProject.ProjectGUID)==false) return(false);
 {
  CRAIICCriticalSection cRAIICCriticalSection(&sProtectedVariables.cCriticalSection);
  {
   if (sProtectedVariables.cIProjectDatabaseEngine_Ptr->AddProject(sProject)==false) return(false);
  }
 }
 SProject sProject_Added=sProject;
 cThreadServer.OnProjectAdded(sProject_Added);//��������� ������, ��� ��������� ���������� �������
 return(true);
}
//----------------------------------------------------------------------------------------------------
//������� ������
//----------------------------------------------------------------------------------------------------
bool CDocument_Main::DeleteProject(const SProject &sProject)
{
 {
  CRAIICCriticalSection cRAIICCriticalSection(&sProtectedVariables.cCriticalSection);
  {
   if (sProtectedVariables.cIProjectDatabaseEngine_Ptr->DeleteProject(sProject)==false) return(false);
  }
 }
 SProject sProject_Deleted=sProject;
 cThreadServer.OnProjectDeleted(sProject_Deleted);//��������� ������, ��� ��������� �������� �������
 return(true);
}
//----------------------------------------------------------------------------------------------------
//�������� ������
//----------------------------------------------------------------------------------------------------
bool CDocument_Main::ChangeProject(const SProject &sProject)
{
 {
  CRAIICCriticalSection cRAIICCriticalSection(&sProtectedVariables.cCriticalSection);
  {
   if (sProtectedVariables.cIProjectDatabaseEngine_Ptr->ChangeProject(sProject)==false) return(false);
  }
 }
 SProject sProject_Changed=sProject;
 cThreadServer.OnProjectChanged(sProject_Changed);//��������� ������, ��� ��������� ��������� ������ �������
 return(true);
}
//----------------------------------------------------------------------------------------------------
//������� ���� �������
//----------------------------------------------------------------------------------------------------
void CDocument_Main::ExportTaskBase(const CString &file_name)
{
 FILE *file=fopen(file_name,"wb");
 if (file==NULL)
 {
  MessageBox(NULL,"�� ���� ��������� ����!","������",MB_OK);
  return;
 }
 list<STask> list_STask;
 list<SUser> list_SUser;
 {
  CRAIICCriticalSection cRAIICCriticalSection(&sProtectedVariables.cCriticalSection);
  {
   list_STask=sProtectedVariables.cITaskDatabaseEngine_Ptr->GetAllTask();
   list_SUser=sProtectedVariables.cIUserDatabaseEngine_Ptr->GetAllUser();
  }
 }

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
  fprintf(file,"��: %s\t���:%s\t�������:%s\r\n",sUser_From.Name,sUser_For.Name,sTask.Task);
  iterator++;
 }
 fclose(file);
}
//----------------------------------------------------------------------------------------------------
//�������� ���� �������������
//----------------------------------------------------------------------------------------------------
void CDocument_Main::ResetUserListBase(void)
{
 {
  CRAIICCriticalSection cRAIICCriticalSection(&sProtectedVariables.cCriticalSection);
  {
   sProtectedVariables.cIUserDatabaseEngine_Ptr->ResetBase();
  }
 }
 UpdateAllViews(NULL);
}
//----------------------------------------------------------------------------------------------------
//�������� ���� �������
//----------------------------------------------------------------------------------------------------
void CDocument_Main::ResetTaskListBase(void)
{
 {
  CRAIICCriticalSection cRAIICCriticalSection(&sProtectedVariables.cCriticalSection);
  {
   sProtectedVariables.cITaskDatabaseEngine_Ptr->ResetBase();
  }
 }
 UpdateAllViews(NULL);
}
//----------------------------------------------------------------------------------------------------
//�������� ���� ��������
//----------------------------------------------------------------------------------------------------
void CDocument_Main::ResetProjectListBase(void)
{
 {
  CRAIICCriticalSection cRAIICCriticalSection(&sProtectedVariables.cCriticalSection);
  {
   sProtectedVariables.cIProjectDatabaseEngine_Ptr->ResetBase();
  }
 }
 UpdateAllViews(NULL);
}

//----------------------------------------------------------------------------------------------------
//������� GUID
//----------------------------------------------------------------------------------------------------
bool CDocument_Main::CreateGUID(CSafeString &cSafeString_GUID)
{
 cSafeString_GUID="";
 //��������� GUID
 GUID guid;
 HRESULT hr=CoCreateGuid(&guid);
 if (FAILED(hr)) return(false);
 LPOLESTR szGUID=new WCHAR[39];
 StringFromGUID2(guid,szGUID,39);
 long length=WideCharToMultiByte(CP_UTF8,0,szGUID,-1,0,0,0,0);
 char *guid_name=new char[length+1];
 WideCharToMultiByte(CP_ACP,0,szGUID,-1,guid_name,length,0,0);
 cSafeString_GUID=guid_name;
 delete[](guid_name);
 delete[](szGUID);
 return(true);
}

//====================================================================================================
//������
//====================================================================================================
IMPLEMENT_DYNCREATE(CDocument_Main,CDocument)
