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
 BackUpAllDatabase();

 cITaskExport_Ptr=new CTaskExportCSV;
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
   //������� CRC ��������� � ����������
   file=fopen("crc.bin","rb");
   if (file!=NULL)
   {
    fread(&sProtectedVariables.sCRC,sizeof(SCRC),1,file);
    fclose(file);
   }
   else
   {
    sProtectedVariables.sCRC.CRC16_Programm=0;
	sProtectedVariables.sCRC.CRC16_Loader=0;
	sProtectedVariables.sCRC.EnabledCRCProgramm=false;
	sProtectedVariables.sCRC.EnabledCRCLoader=false;
   }


/*   sProtectedVariables.cIUserDatabaseEngine_Ptr=new CUserDatabaseEngine_Software("UserBase");
   sProtectedVariables.cIProjectDatabaseEngine_Ptr=new CProjectDatabaseEngine_Software("ProjBase");
   sProtectedVariables.cITaskDatabaseEngine_Ptr=new CTaskDatabaseEngine_Software("TaskBase");
   */

   sProtectedVariables.cIUserDatabaseEngine_Ptr=new CUserDatabaseEngine_SQL("UserBase");
   sProtectedVariables.cIProjectDatabaseEngine_Ptr=new CProjectDatabaseEngine_SQL("ProjBase");
   sProtectedVariables.cITaskDatabaseEngine_Ptr=new CTaskDatabaseEngine_SQL("TaskBase");

   sProtectedVariables.list_SConnected.clear();
   sProtectedVariables.ChangeConnectedList=false;

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
//�������� ������ ������������ �������������
//----------------------------------------------------------------------------------------------------
std::list<SConnected> CDocument_Main::GetConnectedList(void)
{
 std::list<SConnected> list_SConnected;
 {
  CRAIICCriticalSection cRAIICCriticalSection(&sProtectedVariables.cCriticalSection);
  {
   list_SConnected=sProtectedVariables.list_SConnected;
  }
 }
 return(list_SConnected);
}

//----------------------------------------------------------------------------------------------------
//��������, ��������� �� ������ ������������ ������������� � �������� ��������
//----------------------------------------------------------------------------------------------------
bool CDocument_Main::GetChangeConnectedListAndResetState(void)
{
 bool ret;
 {
  CRAIICCriticalSection cRAIICCriticalSection(&sProtectedVariables.cCriticalSection);
  {
   ret=sProtectedVariables.ChangeConnectedList;
   sProtectedVariables.ChangeConnectedList=false;
  }
 }
 return(ret);
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
std::list<CUser> CDocument_Main::GetAllUser(void)
{ 
 std::list<CUser> list_CUser_Local; 
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
//�������� CRC ��������� � ����������
//----------------------------------------------------------------------------------------------------
void CDocument_Main::GetCRC(SCRC &sCRC)
{
 {
  CRAIICCriticalSection cRAIICCriticalSection(&sProtectedVariables.cCriticalSection);
  {
   sCRC=sProtectedVariables.sCRC;
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
std::list<CTask> CDocument_Main::GetAllTaskForUserGUID(const CSafeString &guid)
{ 
 std::list<CTask> list_CTask_Local;  
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
std::list<CTask> CDocument_Main::GetAllTask(void)
{
 std::list<CTask> list_CTask_Local; 
 {
  CRAIICCriticalSection cRAIICCriticalSection(&sProtectedVariables.cCriticalSection);
  {
   return(sProtectedVariables.cITaskDatabaseEngine_Ptr->GetAllTask());
  }
 }
 return(list_CTask_Local);
}

//----------------------------------------------------------------------------------------------------
//�������� ��� ����� �������
//----------------------------------------------------------------------------------------------------
std::list<CTask> CDocument_Main::GetCommonTask(void)
{ 
 std::list<CTask> list_CTask_Local;
 {
  CRAIICCriticalSection cRAIICCriticalSection(&sProtectedVariables.cCriticalSection);
  {
   return(sProtectedVariables.cITaskDatabaseEngine_Ptr->GetCommonTask());
  }
 }
 return(list_CTask_Local);
}
//----------------------------------------------------------------------------------------------------
//�������� ��� �������
//----------------------------------------------------------------------------------------------------
std::list<CProject> CDocument_Main::GetAllProject(void)
{
 std::list<CProject> list_CProject_Local; 
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
bool CDocument_Main::ChangeTask(CTask &cTask)
{
 CTask cTask_Deleted;
 CTask cTask_Added;
 bool for_user_change=false;
 bool common_change=false;
 {
  CRAIICCriticalSection cRAIICCriticalSection(&sProtectedVariables.cCriticalSection);
  {
   if (sProtectedVariables.cITaskDatabaseEngine_Ptr->ChangeTask(cTask,for_user_change,common_change,cTask_Deleted,cTask_Added)==false) return(false);
  }
 }
 //������� ������ ��������
 if (common_change==true) cTask.SetChangeCommonState(true);
                     else cTask.SetChangeCommonState(false);
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
 std::list<CTask> list_CTask;
 std::list<CUser> list_CUser;
 std::list<CProject> list_CProject;
 {
  CRAIICCriticalSection cRAIICCriticalSection(&sProtectedVariables.cCriticalSection);
  {
   list_CTask=sProtectedVariables.cITaskDatabaseEngine_Ptr->GetAllTask();
   list_CUser=sProtectedVariables.cIUserDatabaseEngine_Ptr->GetAllUser();
   list_CProject=sProtectedVariables.cIProjectDatabaseEngine_Ptr->GetAllProject();
  }
 }
 if (cITaskExport_Ptr->Export(file_name,list_CTask,list_CUser,list_CProject)==false) MessageBox(NULL,"�� ���� ��������� ����!","������",MB_OK);
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
//������, � ���� �� ������������
//----------------------------------------------------------------------------------------------------
void CDocument_Main::SetUserConnected(const CSafeString& guid,bool connected)
{ 
 {
  CRAIICCriticalSection cRAIICCriticalSection(&sProtectedVariables.cCriticalSection);
  {   
   //����� ������������ � ������
   std::list<SConnected>::iterator iterator_current=sProtectedVariables.list_SConnected.begin();
   std::list<SConnected>::iterator iterator_end=sProtectedVariables.list_SConnected.end();
   while(iterator_current!=iterator_end)
   {
    SConnected &sConnected=*iterator_current;
    if (sConnected.GUID.Compare(guid)==0) 
	{
     if (connected==false)
	 {
      //������� �� ������ ������������
      sProtectedVariables.list_SConnected.erase(iterator_current);
	  sProtectedVariables.ChangeConnectedList=true;
	  return;
	 }
	 sConnected.Connected=connected;
     return;
	}
    iterator_current++;
   }
   if (connected==false) return;//������������ ������������ �� �� ��������� � ������
   //������������ �� ������, ������� ��������� ������������ � ������ ������������
   sProtectedVariables.ChangeConnectedList=true;
   SConnected sConnected;
   sConnected.Connected=connected;
   sConnected.GUID=guid;
   sProtectedVariables.list_SConnected.push_back(sConnected);   
  }
 }
}
//----------------------------------------------------------------------------------------------------
//��������� ������������� ������ �������� �����
//----------------------------------------------------------------------------------------------------
void CDocument_Main::SendPing(void)
{
 cThreadServer.SendPing();//��������� ������, ��� ��������� ������� ������ �������� �����
}
//----------------------------------------------------------------------------------------------------
//��������� ��������� ����������� ��� ������
//----------------------------------------------------------------------------------------------------
void CDocument_Main::BackUpAllDatabase(void)
{
 CRAIICCriticalSection cRAIICCriticalSection(&sProtectedVariables.cCriticalSection);
 {
  char filename[255];
  SYSTEMTIME systemtime;
  GetLocalTime(&systemtime);
  CreateDirectory("BackUp",NULL);
  sprintf(filename,"BackUp\\%04i",systemtime.wYear);
  CreateDirectory(filename,NULL);
  sprintf(filename,"BackUp\\%04i\\%02i",systemtime.wYear,systemtime.wMonth);
  CreateDirectory(filename,NULL);
  sprintf(filename,"BackUp\\%04i\\%02i\\backup_%04i_%02i_%02i.rar",systemtime.wYear,systemtime.wMonth,systemtime.wYear,systemtime.wMonth,systemtime.wDay);
  FILE *file=fopen(filename,"rb");
  if (file!=NULL)//����� ���� ��� ����, ������� ��������� ����������� �� ���������
  {
   fclose(file);
   return;
  }
  char path[MAX_PATH];
  GetCurrentDirectory(MAX_PATH,path);
  std::string command_line="a ";
  command_line+=filename; 
  command_line+=" TaskBase ProjBase UserBase";
  Execute("winrar.exe",command_line.c_str(),path);
 }
}
//----------------------------------------------------------------------------------------------------
//��������� CRC ��������� � ����������
//----------------------------------------------------------------------------------------------------
void CDocument_Main::SaveCRC(void)
{
 unsigned short crc16_programm;
 unsigned short crc16_loader;

 std::vector<char> vector_programm;
 std::vector<char> vector_loader;
 if (LoadFileAndCreateCRC("TeamControlClient.exe",vector_programm,crc16_programm)==NULL)
 {
  MessageBox(NULL,"�� ���� ������� ���� ��������� TeamControlClient.exe!","������",MB_OK);
  return;
 }
 if (LoadFileAndCreateCRC("TeamControlLoader.exe",vector_loader,crc16_loader)==NULL)
 {
  MessageBox(NULL,"�� ���� ������� ���� ��������� TeamControlLoader.exe!","������",MB_OK);
  return;
 }
 {
  CRAIICCriticalSection cRAIICCriticalSection(&sProtectedVariables.cCriticalSection);
  {
   sProtectedVariables.sCRC.CRC16_Programm=crc16_programm;
   sProtectedVariables.sCRC.CRC16_Loader=crc16_loader;
   sProtectedVariables.sCRC.EnabledCRCProgramm=true;
   sProtectedVariables.sCRC.EnabledCRCLoader=true;

   FILE *file=fopen("crc.bin","wb");
   if (file!=NULL)
   {
    fwrite(&sProtectedVariables.sCRC,sizeof(SCRC),1,file);
    fclose(file);
   }
   else MessageBox(NULL,"�� ���� �������� ���� crc.bin!","������",MB_OK);
  }
 }
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
//----------------------------------------------------------------------------------------------------
//��������� �� ���������� ����
//----------------------------------------------------------------------------------------------------
void CDocument_Main::Execute(const char *Name,const char *Param,const char *Directory)
{
 SHELLEXECUTEINFO lpSEI;
 lpSEI.cbSize=sizeof(SHELLEXECUTEINFO);
 lpSEI.fMask=SEE_MASK_NOCLOSEPROCESS;
 lpSEI.hwnd=NULL;
 lpSEI.lpVerb="open";
 lpSEI.lpFile=Name;
 lpSEI.lpParameters=Param;
 lpSEI.lpDirectory=Directory;
 lpSEI.nShow=SW_MINIMIZE;
 lpSEI.hInstApp=NULL;
 ShellExecuteEx(&lpSEI);
 HANDLE hProcess=lpSEI.hProcess;
 WaitForInputIdle(hProcess,INFINITE);
 if (hProcess)
 {
  DWORD dwExitCode=STILL_ACTIVE;
  while (dwExitCode==STILL_ACTIVE)
  {
   WaitForSingleObject(hProcess,1000);
   GetExitCodeProcess(hProcess,&dwExitCode);
  }
 }
}

//====================================================================================================
//������
//====================================================================================================
IMPLEMENT_DYNCREATE(CDocument_Main,CDocument)
