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
   sProtectedVariables.cIDatabaseEngine_Ptr=new CDatabaseEngine_Software("UserBase","TaskBase","ProjBase");
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
 if (sProtectedVariables.cIDatabaseEngine_Ptr=NULL) delete(sProtectedVariables.cIDatabaseEngine_Ptr);
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
   if (sProtectedVariables.cIDatabaseEngine_Ptr->AddUser(sUser)==false) return(false);
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
   if (sProtectedVariables.cIDatabaseEngine_Ptr->ChangeUser(index,sUser)==false) return(false);
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
   if (sProtectedVariables.cIDatabaseEngine_Ptr->GetUser(index,sUser)==false) return(false);
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
   if (sProtectedVariables.cIDatabaseEngine_Ptr->DeleteUser(index)==false) return;
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
   return(list_SUser_Local=sProtectedVariables.cIDatabaseEngine_Ptr->GetAllUser());
  }
 }
 return(list_SUser_Local);
}
//----------------------------------------------------------------------------------------------------
//����� ������������
//----------------------------------------------------------------------------------------------------
bool CDocument_Main::FindUserByLoginAndPassword(const CString& login,const CString& password,SUser& sUser)
{ 
 {
  CRAIICCriticalSection cRAIICCriticalSection(&sProtectedVariables.cCriticalSection);
  {
   return(sProtectedVariables.cIDatabaseEngine_Ptr->FindUserByLoginAndPassword(login,password,sUser));
  }
 }
 return(false);
}
//----------------------------------------------------------------------------------------------------
//����� ������������ �� GUID
//----------------------------------------------------------------------------------------------------
bool CDocument_Main::FindUserByGUID(const CString& guid,SUser& sUser)
{ 
 {
  CRAIICCriticalSection cRAIICCriticalSection(&sProtectedVariables.cCriticalSection);
  {
   return(sProtectedVariables.cIDatabaseEngine_Ptr->FindUserByGUID(guid,sUser));
  }
 }
 return(false);
}
//----------------------------------------------------------------------------------------------------
//����� ������� �� GUID
//----------------------------------------------------------------------------------------------------
bool CDocument_Main::FindTaskByGUID(const CString &guid,STask &sTask)
{
 {
  CRAIICCriticalSection cRAIICCriticalSection(&sProtectedVariables.cCriticalSection);
  {
   return(sProtectedVariables.cIDatabaseEngine_Ptr->FindTaskByGUID(guid,sTask));
  }
 }
 return(false);
}
//----------------------------------------------------------------------------------------------------
//����� ������ �� GUID
//----------------------------------------------------------------------------------------------------
bool CDocument_Main::FindProjectByGUID(const CString &guid,SProject &sProject)
{
 {
  CRAIICCriticalSection cRAIICCriticalSection(&sProtectedVariables.cCriticalSection);
  {
   return(sProtectedVariables.cIDatabaseEngine_Ptr->FindProjectByGUID(guid,sProject));
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
list<STask> CDocument_Main::GetAllTaskForUserGUID(const CString &guid)
{ 
 list<STask> list_STask_Local;  
 {
  CRAIICCriticalSection cRAIICCriticalSection(&sProtectedVariables.cCriticalSection);
  {
   return(sProtectedVariables.cIDatabaseEngine_Ptr->GetAllTaskForUserGUID(guid));
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
   return(sProtectedVariables.cIDatabaseEngine_Ptr->GetAllTask());
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
   return(sProtectedVariables.cIDatabaseEngine_Ptr->GetAllProject());
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
   if (sProtectedVariables.cIDatabaseEngine_Ptr->AddTask(sTask)==false) return(false);
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
   if (sProtectedVariables.cIDatabaseEngine_Ptr->DeleteTask(sTask)==false) return(false);
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
   if (sProtectedVariables.cIDatabaseEngine_Ptr->ChangeTask(sTask,for_user_change,sTask_Deleted,sTask_Added)==false) return(false);
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
   if (sProtectedVariables.cIDatabaseEngine_Ptr->AddProject(sProject)==false) return(false);
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
   if (sProtectedVariables.cIDatabaseEngine_Ptr->DeleteProject(sProject)==false) return(false);
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
   if (sProtectedVariables.cIDatabaseEngine_Ptr->ChangeProject(sProject)==false) return(false);
  }
 }
 SProject sProject_Changed=sProject;
 cThreadServer.OnProjectChanged(sProject_Changed);//��������� ������, ��� ��������� ��������� ������ �������
 return(true);
}

//----------------------------------------------------------------------------------------------------
//������� GUID
//----------------------------------------------------------------------------------------------------
bool CDocument_Main::CreateGUID(CString &cString_GUID)
{
 cString_GUID="";
 //��������� GUID
 GUID guid;
 HRESULT hr=CoCreateGuid(&guid);
 if (FAILED(hr)) return(false);
 LPOLESTR szGUID=new WCHAR[39];
 StringFromGUID2(guid,szGUID,39);
 long length=WideCharToMultiByte(CP_UTF8,0,szGUID,-1,0,0,0,0);
 char *guid_name=new char[length+1];
 WideCharToMultiByte(CP_ACP,0,szGUID,-1,guid_name,length,0,0);
 cString_GUID=guid_name;
 delete[](guid_name);
 delete[](szGUID);
 return(true);
}

//====================================================================================================
//������
//====================================================================================================
IMPLEMENT_DYNCREATE(CDocument_Main,CDocument)
