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
 cITaskExport_Ptr=new CTaskExportHTML;
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
 delete(cITaskExport_Ptr);
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
bool CDocument_Main::AddUser(CUser& cUser)
{
 CSafeString guid;
 if (CreateGUID(guid)==false)
 {
  MessageBox(NULL,"�� ���� ������� GUID (���������-���������� �������������) ������ ������������!","������",MB_OK);
  return(false);
 }
 cUser.SetUserGUID(guid);
 {
  CRAIICCriticalSection cRAIICCriticalSection(&sProtectedVariables.cCriticalSection);
  {
   if (sProtectedVariables.cIUserDatabaseEngine_Ptr->AddUser(cUser)==false) return(false);
  }
 }
 CUser cUser_Added=cUser;
 cThreadServer.OnUserAdded(cUser_Added);//��������� ������, ��� ��������� ���������� ������������
 return(true);
}
//----------------------------------------------------------------------------------------------------
//�������� ������ ������������
//----------------------------------------------------------------------------------------------------
bool CDocument_Main::ChangeUser(long index,const CUser& cUser)
{
 {
  CRAIICCriticalSection cRAIICCriticalSection(&sProtectedVariables.cCriticalSection);
  {
   if (sProtectedVariables.cIUserDatabaseEngine_Ptr->ChangeUser(index,cUser)==false) return(false);
  }
 }
 CUser cUser_Changed=cUser;
 cThreadServer.OnUserChanged(cUser_Changed);//��������� ������, ��� ��������� ��������� ������ ������������
 return(true);
}
//----------------------------------------------------------------------------------------------------
//�������� ������������ �� �������
//----------------------------------------------------------------------------------------------------
bool CDocument_Main::GetUser(long index,CUser &cUser)
{
 {
  CRAIICCriticalSection cRAIICCriticalSection(&sProtectedVariables.cCriticalSection);
  {
   if (sProtectedVariables.cIUserDatabaseEngine_Ptr->GetUser(index,cUser)==false) return(false);
  }
 }
 return(true);
}
//----------------------------------------------------------------------------------------------------
//������� ������������ �� �������
//----------------------------------------------------------------------------------------------------
void CDocument_Main::DeleteUser(long index)
{
 CUser cUser_Deleted; 
 if (GetUser(index,cUser_Deleted)==false) return;
 {
  CRAIICCriticalSection cRAIICCriticalSection(&sProtectedVariables.cCriticalSection);
  {
   if (sProtectedVariables.cIUserDatabaseEngine_Ptr->DeleteUser(index)==false) return;
  }
 }
 cThreadServer.OnUserDeleted(cUser_Deleted);//��������� ������, ��� ��������� �������� ������������
}
//----------------------------------------------------------------------------------------------------
//�������� ������ ���� �������������
//----------------------------------------------------------------------------------------------------
list<CUser> CDocument_Main::GetAllUser(void)
{ 
 list<CUser> list_CUser_Local; 
 {
  CRAIICCriticalSection cRAIICCriticalSection(&sProtectedVariables.cCriticalSection);
  {
   return(sProtectedVariables.cIUserDatabaseEngine_Ptr->GetAllUser());
  }
 }
 return(list_CUser_Local);
}
//----------------------------------------------------------------------------------------------------
//����� ������������
//----------------------------------------------------------------------------------------------------
bool CDocument_Main::FindUserByLoginAndPassword(const CSafeString& login,const CSafeString& password,CUser& cUser)
{ 
 {
  CRAIICCriticalSection cRAIICCriticalSection(&sProtectedVariables.cCriticalSection);
  {
   return(sProtectedVariables.cIUserDatabaseEngine_Ptr->FindUserByLoginAndPassword(login,password,cUser));
  }
 }
 return(false);
}
//----------------------------------------------------------------------------------------------------
//����� ������������ �� GUID
//----------------------------------------------------------------------------------------------------
bool CDocument_Main::FindUserByGUID(const CSafeString& guid,CUser& cUser)
{ 
 {
  CRAIICCriticalSection cRAIICCriticalSection(&sProtectedVariables.cCriticalSection);
  {
   return(sProtectedVariables.cIUserDatabaseEngine_Ptr->FindUserByGUID(guid,cUser));
  }
 }
 return(false);
}
//----------------------------------------------------------------------------------------------------
//�������� ������������ �� GUID
//----------------------------------------------------------------------------------------------------
bool CDocument_Main::ChangeUserByGUID(const CSafeString& guid,const CUser& cUser)
{
 {
  CRAIICCriticalSection cRAIICCriticalSection(&sProtectedVariables.cCriticalSection);
  {
   if (sProtectedVariables.cIUserDatabaseEngine_Ptr->ChangeUserByGUID(guid,cUser)==false) return(false);
  }
 }
 CUser cUser_Changed=cUser;
 cThreadServer.OnUserChanged(cUser_Changed);//��������� ������, ��� ��������� ��������� ������ ������������
 return(true);
}
//----------------------------------------------------------------------------------------------------
//������� ������������ �� GUID
//----------------------------------------------------------------------------------------------------
bool CDocument_Main::DeleteUserByGUID(const CSafeString& guid)
{
 CUser cUser_Deleted; 
 if (FindUserByGUID(guid,cUser_Deleted)==false) return(false);
 {
  CRAIICCriticalSection cRAIICCriticalSection(&sProtectedVariables.cCriticalSection);
  {
   if (sProtectedVariables.cIUserDatabaseEngine_Ptr->DeleteUserByGUID(guid)==false) return(false);
  }
 }
 cThreadServer.OnUserDeleted(cUser_Deleted);//��������� ������, ��� ��������� �������� ������������
 return(true);
}

//----------------------------------------------------------------------------------------------------
//����� ������� �� GUID
//----------------------------------------------------------------------------------------------------
bool CDocument_Main::FindTaskByGUID(const CSafeString &guid,CTask &cTask)
{
 {
  CRAIICCriticalSection cRAIICCriticalSection(&sProtectedVariables.cCriticalSection);
  {
   return(sProtectedVariables.cITaskDatabaseEngine_Ptr->FindTaskByGUID(guid,cTask));
  }
 }
 return(false);
}
//----------------------------------------------------------------------------------------------------
//����� ������ �� GUID
//----------------------------------------------------------------------------------------------------
bool CDocument_Main::FindProjectByGUID(const CSafeString &guid,CProject &cProject)
{
 {
  CRAIICCriticalSection cRAIICCriticalSection(&sProtectedVariables.cCriticalSection);
  {
   return(sProtectedVariables.cIProjectDatabaseEngine_Ptr->FindProjectByGUID(guid,cProject));
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
list<CTask> CDocument_Main::GetAllTaskForUserGUID(const CSafeString &guid)
{ 
 list<CTask> list_CTask_Local;  
 {
  CRAIICCriticalSection cRAIICCriticalSection(&sProtectedVariables.cCriticalSection);
  {
   return(sProtectedVariables.cITaskDatabaseEngine_Ptr->GetAllTaskForUserGUID(guid));
  }
 }
 return(list_CTask_Local);
}
//----------------------------------------------------------------------------------------------------
//�������� ��� �������
//----------------------------------------------------------------------------------------------------
list<CTask> CDocument_Main::GetAllTask(void)
{
 list<CTask> list_CTask_Local; 
 {
  CRAIICCriticalSection cRAIICCriticalSection(&sProtectedVariables.cCriticalSection);
  {
   return(sProtectedVariables.cITaskDatabaseEngine_Ptr->GetAllTask());
  }
 }
 return(list_CTask_Local);
}

//----------------------------------------------------------------------------------------------------
//�������� ��� �������
//----------------------------------------------------------------------------------------------------
list<CProject> CDocument_Main::GetAllProject(void)
{
 list<CProject> list_CProject_Local; 
 {
  CRAIICCriticalSection cRAIICCriticalSection(&sProtectedVariables.cCriticalSection);
  {
   return(sProtectedVariables.cIProjectDatabaseEngine_Ptr->GetAllProject());
  }
 }
 return(list_CProject_Local);
}
//----------------------------------------------------------------------------------------------------
//�������� �������
//----------------------------------------------------------------------------------------------------
bool CDocument_Main::AddTask(CTask &cTask)
{
 CSafeString guid; 
 if (CreateGUID(guid)==false) return(false);
 cTask.SetTaskGUID(guid);
 cTask.SetState(TASK_STATE_NO_READ);
 {
  CRAIICCriticalSection cRAIICCriticalSection(&sProtectedVariables.cCriticalSection);
  {
   if (sProtectedVariables.cITaskDatabaseEngine_Ptr->AddTask(cTask)==false) return(false);
  }
 }
 CTask cTask_Added=cTask;
 cThreadServer.OnTaskAdded(cTask_Added);//��������� ������, ��� ��������� ���������� �������
 return(true);
}
//----------------------------------------------------------------------------------------------------
//������� �������
//----------------------------------------------------------------------------------------------------
bool CDocument_Main::DeleteTask(const CTask &cTask)
{
 {
  CRAIICCriticalSection cRAIICCriticalSection(&sProtectedVariables.cCriticalSection);
  {
   if (sProtectedVariables.cITaskDatabaseEngine_Ptr->DeleteTask(cTask)==false) return(false);
  }
 }
 CTask cTask_Deleted=cTask;
 cThreadServer.OnTaskDeleted(cTask_Deleted);//��������� ������, ��� ��������� �������� �������
 return(true);
}
//----------------------------------------------------------------------------------------------------
//�������� �������
//----------------------------------------------------------------------------------------------------
bool CDocument_Main::ChangeTask(const CTask &cTask)
{
 CTask cTask_Deleted;
 CTask cTask_Added;
 bool for_user_change=false;
 {
  CRAIICCriticalSection cRAIICCriticalSection(&sProtectedVariables.cCriticalSection);
  {
   if (sProtectedVariables.cITaskDatabaseEngine_Ptr->ChangeTask(cTask,for_user_change,cTask_Deleted,cTask_Added)==false) return(false);
  }
 }
 if (for_user_change==true)//������� � ������� ��������
 {
  cThreadServer.OnTaskDeleted(cTask_Deleted);//��������� ������, ��� ��������� �������� �������  
  cThreadServer.OnTaskAdded(cTask_Added);//��������� ������, ��� ��������� ���������� �������
  return(true);
 } 
 //��������� ��� ������ ��������
 CTask cTask_Changed=cTask;
 cThreadServer.OnTaskChanged(cTask_Changed);//��������� ������, ��� ��������� ��������� ������ �������
 return(true);
}

//----------------------------------------------------------------------------------------------------
//�������� ������
//----------------------------------------------------------------------------------------------------
bool CDocument_Main::AddProject(CProject &cProject)
{
 CSafeString guid;
 if (CreateGUID(guid)==false) return(false);
 cProject.SetProjectGUID(guid);
 {
  CRAIICCriticalSection cRAIICCriticalSection(&sProtectedVariables.cCriticalSection);
  {
   if (sProtectedVariables.cIProjectDatabaseEngine_Ptr->AddProject(cProject)==false) return(false);
  }
 }
 CProject cProject_Added=cProject;
 cThreadServer.OnProjectAdded(cProject_Added);//��������� ������, ��� ��������� ���������� �������
 return(true);
}
//----------------------------------------------------------------------------------------------------
//������� ������
//----------------------------------------------------------------------------------------------------
bool CDocument_Main::DeleteProject(const CProject &cProject)
{
 {
  CRAIICCriticalSection cRAIICCriticalSection(&sProtectedVariables.cCriticalSection);
  {
   if (sProtectedVariables.cIProjectDatabaseEngine_Ptr->DeleteProject(cProject)==false) return(false);
  }
 }
 CProject cProject_Deleted=cProject;
 cThreadServer.OnProjectDeleted(cProject_Deleted);//��������� ������, ��� ��������� �������� �������
 return(true);
}
//----------------------------------------------------------------------------------------------------
//�������� ������
//----------------------------------------------------------------------------------------------------
bool CDocument_Main::ChangeProject(const CProject &cProject)
{
 {
  CRAIICCriticalSection cRAIICCriticalSection(&sProtectedVariables.cCriticalSection);
  {
   if (sProtectedVariables.cIProjectDatabaseEngine_Ptr->ChangeProject(cProject)==false) return(false);
  }
 }
 CProject cProject_Changed=cProject;
 cThreadServer.OnProjectChanged(cProject_Changed);//��������� ������, ��� ��������� ��������� ������ �������
 return(true);
}
//----------------------------------------------------------------------------------------------------
//������� ���� �������
//----------------------------------------------------------------------------------------------------
void CDocument_Main::ExportTaskBase(const CString &file_name)
{
 list<CTask> list_CTask;
 list<CUser> list_CUser;
 {
  CRAIICCriticalSection cRAIICCriticalSection(&sProtectedVariables.cCriticalSection);
  {
   list_CTask=sProtectedVariables.cITaskDatabaseEngine_Ptr->GetAllTask();
   list_CUser=sProtectedVariables.cIUserDatabaseEngine_Ptr->GetAllUser();
  }
 }
 if (cITaskExport_Ptr->Export(file_name,list_CTask,list_CUser)==false) MessageBox(NULL,"�� ���� ��������� ����!","������",MB_OK);
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
