#include "cthreadserverunit.h"

//====================================================================================================
//����������������
//====================================================================================================

//������ ������ �����
#define BUFFER_SIZE 16384

//====================================================================================================
//������� ������
//====================================================================================================

UINT ThreadServerUnit(LPVOID pParam)
{
 CThreadServerUnit *cThreadServer_Ptr=reinterpret_cast<CThreadServerUnit*>(pParam);
 if (cThreadServer_Ptr==NULL) return(0);
 cThreadServer_Ptr->Processing();
 return(0);
}
//====================================================================================================
//����������� ������
//====================================================================================================
CThreadServerUnit::CThreadServerUnit(void)
{
 sClient_My.UserGUID="";
 sClient_My.Login="";
 sClient_My.Password="";
 sClient_My.vector_Data.clear();
 sClient_My.vector_Data.reserve(MAX_PACKAGE_LENGTH); 
 sClient_My.StuffingEnabled=false;

 Buffer=new char[BUFFER_SIZE];
 cWinThread_Thread=NULL;
 cDocument_Main_Ptr=NULL;
}
//====================================================================================================
//���������� ������
//====================================================================================================
CThreadServerUnit::~CThreadServerUnit()
{ 
 Stop();
 delete[](Buffer);
}
//====================================================================================================
//������� ������
//====================================================================================================

//----------------------------------------------------------------------------------------------------
//��������� �����
//----------------------------------------------------------------------------------------------------
void CThreadServerUnit::Start(SOCKET socket_client)
{
 Stop();
 sClient_My.Socket=socket_client;
 sClient_My.UserGUID="";
 sClient_My.Login="";
 sClient_My.Password="";
 sClient_My.vector_Data.clear();
 sClient_My.vector_Data.reserve(MAX_PACKAGE_LENGTH); 
 sClient_My.StuffingEnabled=false;
 SetClientOnLine(true);
 sProtectedVariables.SendPing=false;
 cEvent_Exit.ResetEvent();
 cWinThread_Thread=AfxBeginThread((AFX_THREADPROC)ThreadServerUnit,this);
 cWinThread_Thread->m_bAutoDelete=FALSE;
}
//----------------------------------------------------------------------------------------------------
//���������� �����
//----------------------------------------------------------------------------------------------------
void CThreadServerUnit::Stop(void)
{
 if (cWinThread_Thread!=NULL)
 {
  cEvent_Exit.SetEvent();
  WaitForSingleObject(cWinThread_Thread->m_hThread,INFINITE);
  delete(cWinThread_Thread);
  cWinThread_Thread=NULL;
 }
 SetClientOnLine(false);
}
//----------------------------------------------------------------------------------------------------
//������ ����� ���������
//----------------------------------------------------------------------------------------------------
void CThreadServerUnit::SetDocument(CDocument_Main *cDocument_Main_Set_Ptr)
{
 cDocument_Main_Ptr=cDocument_Main_Set_Ptr;
}
//----------------------------------------------------------------------------------------------------
//��� �������� ������������
//----------------------------------------------------------------------------------------------------
void CThreadServerUnit::OnUserAdded(const CUser &cUser)
{
 {
  CRAIICCriticalSection cRAIICCriticalSection(&sProtectedVariables.cCriticalSection);
  {
   sProtectedVariables.vector_CUser_Added.push_back(cUser);
  }
 }
}
//----------------------------------------------------------------------------------------------------
//��� ����� ������������
//----------------------------------------------------------------------------------------------------
void CThreadServerUnit::OnUserDeleted(const CUser &cUser)
{
 {
  CRAIICCriticalSection cRAIICCriticalSection(&sProtectedVariables.cCriticalSection);
  {
   sProtectedVariables.vector_CUser_Deleted.push_back(cUser);
  }
 }
}
//----------------------------------------------------------------------------------------------------
//���� �������� ������ ������������
//----------------------------------------------------------------------------------------------------
void CThreadServerUnit::OnUserChanged(const CUser &cUser)
{
 {
  CRAIICCriticalSection cRAIICCriticalSection(&sProtectedVariables.cCriticalSection);
  {
   sProtectedVariables.vector_CUser_Changed.push_back(cUser);
  }
 }
}



//----------------------------------------------------------------------------------------------------
//���� ��������� �������
//----------------------------------------------------------------------------------------------------
void CThreadServerUnit::OnTaskAdded(const CTask &cTask)
{
 {
  CRAIICCriticalSection cRAIICCriticalSection(&sProtectedVariables.cCriticalSection);
  {
   sProtectedVariables.vector_CTask_Added.push_back(cTask);
  }
 }
}
//----------------------------------------------------------------------------------------------------
//���� ������� �������
//----------------------------------------------------------------------------------------------------
void CThreadServerUnit::OnTaskDeleted(const CTask &cTask)
{
 {
  CRAIICCriticalSection cRAIICCriticalSection(&sProtectedVariables.cCriticalSection);
  {
   sProtectedVariables.vector_CTask_Deleted.push_back(cTask);
  }
 }
}
//----------------------------------------------------------------------------------------------------
//���� �������� ������ �������
//----------------------------------------------------------------------------------------------------
void CThreadServerUnit::OnTaskChanged(const CTask &cTask)
{
 {
  CRAIICCriticalSection cRAIICCriticalSection(&sProtectedVariables.cCriticalSection);
  {
   sProtectedVariables.vector_CTask_Changed.push_back(cTask);
  }
 }
}



//----------------------------------------------------------------------------------------------------
//��� �������� ������
//----------------------------------------------------------------------------------------------------
void CThreadServerUnit::OnProjectAdded(const CProject &cProject)
{
 {
  CRAIICCriticalSection cRAIICCriticalSection(&sProtectedVariables.cCriticalSection);
  {
   sProtectedVariables.vector_CProject_Added.push_back(cProject);
  }
 }
}
//----------------------------------------------------------------------------------------------------
//��� ������ ������
//----------------------------------------------------------------------------------------------------
void CThreadServerUnit::OnProjectDeleted(const CProject &cProject)
{
 {
  CRAIICCriticalSection cRAIICCriticalSection(&sProtectedVariables.cCriticalSection);
  {
   sProtectedVariables.vector_CProject_Deleted.push_back(cProject);
  }
 }
}
//----------------------------------------------------------------------------------------------------
//���� �������� ������ �������
//----------------------------------------------------------------------------------------------------
void CThreadServerUnit::OnProjectChanged(const CProject &cProject)
{
 {
  CRAIICCriticalSection cRAIICCriticalSection(&sProtectedVariables.cCriticalSection);
  {
   sProtectedVariables.vector_CProject_Changed.push_back(cProject);
  }
 }
}
//----------------------------------------------------------------------------------------------------
//������� ������� ��������� ��� �������� �����
//----------------------------------------------------------------------------------------------------
void CThreadServerUnit::SendPing(void)
{
 {
  CRAIICCriticalSection cRAIICCriticalSection(&sProtectedVariables.cCriticalSection);
  {
   sProtectedVariables.SendPing=true;
  }
 }
}

//----------------------------------------------------------------------------------------------------
//���������� �������
//----------------------------------------------------------------------------------------------------
void CThreadServerUnit::ClientDisconnect(const SClient& sClient)
{ 
 if (sClient.Socket==INVALID_SOCKET) return;
 //�������� ����� ������� � ����������� �����
 unsigned long nb=0;
 ioctlsocket(sClient.Socket,FIONBIO,(unsigned long *)&nb);
 //��������� �������
 shutdown(sClient.Socket,SD_BOTH);
 closesocket(sClient.Socket);
}
//----------------------------------------------------------------------------------------------------
//������, � ���� �� ������
//----------------------------------------------------------------------------------------------------
void CThreadServerUnit::SetClientOnLine(bool state)
{
 {
  CRAIICCriticalSection cRAIICCriticalSection(&sProtectedVariables.cCriticalSection);
  {
   sProtectedVariables.ClientOnLine=state;
  }
 }
 if (state==false) cDocument_Main_Ptr->SetUserConnected(sClient_My.UserGUID,false);
}
//----------------------------------------------------------------------------------------------------
//��������, � ���� �� ������
//----------------------------------------------------------------------------------------------------
bool CThreadServerUnit::GetClientOnLine(void)
{
 bool ret=false;
 {
  CRAIICCriticalSection cRAIICCriticalSection(&sProtectedVariables.cCriticalSection);
  {
   ret=sProtectedVariables.ClientOnLine;
  }
 }
 return(ret);
}
//----------------------------------------------------------------------------------------------------
//�������� ���� ���������
//----------------------------------------------------------------------------------------------------
void CThreadServerUnit::Processing(void)
{
 const static long timeout_us=10000;//������� �� �������� �������� �������
 //��� ������� � ������� �������
 while(1)
 {
  if (WaitForSingleObject(cEvent_Exit.m_hObject,0)==WAIT_OBJECT_0) break;//������� ������ �� �����

  bool on_exit; 
  LinkProcessing(sClient_My,on_exit);
  if (on_exit==true) break;

  fd_set Readen;
  FD_ZERO(&Readen);//�������� ������
  fd_set Exeption;
  FD_ZERO(&Exeption);//�������� ������
  //��������� ����� �������
  FD_SET(sClient_My.Socket,&Readen);//��������� ����� �������
  FD_SET(sClient_My.Socket,&Exeption);//��������� ����� �������   
  //��� ������� � ��������
  timeval timeout;
  timeout.tv_sec=0;
  timeout.tv_usec=timeout_us;
  long ret=select(FD_SETSIZE,&Readen,0,&Exeption,&timeout);
  if (ret<0) break;
  if (ret>0)//��������� ������� � ��������
  {
   if (FD_ISSET(sClient_My.Socket,&Exeption)) break;//���� � �������
   if (FD_ISSET(sClient_My.Socket,&Readen))//������ ����� �� ������ (�.�. ������� ��� ������)
   {     
    long receive=recv(sClient_My.Socket,Buffer,BUFFER_SIZE,0); 
    if (receive<=0) break;//������ ����������
    //������������ �������� ������
    bool on_exit;
    NewDataFromClient(sClient_My,Buffer,receive,on_exit);
    if (on_exit==true) break;
   }
  }
 }
 ClientDisconnect(sClient_My);
 SetClientOnLine(false);
}

//----------------------------------------------------------------------------------------------------
//��������� �����
//----------------------------------------------------------------------------------------------------
void CThreadServerUnit::LinkProcessing(SClient &sClient,bool &on_exit)
{ 
 size_t size;
 size_t n;

 on_exit=false;
 //�������� ������, ������� ���������� �������� �������
 vector<CUser> vector_CUser_Deleted;//�������� ������������
 vector<CUser> vector_CUser_Changed;//��������� ������������
 vector<CUser> vector_CUser_Added;//����������� ������������

 vector<CTask> vector_CTask_Deleted;//�������� �������
 vector<CTask> vector_CTask_Changed;//��������� �������
 vector<CTask> vector_CTask_Added;//����������� �������

 vector<CProject> vector_CProject_Deleted;//�������� �������
 vector<CProject> vector_CProject_Changed;//��������� �������
 vector<CProject> vector_CProject_Added;//����������� �������

 bool send_ping;//��������� �� ������� ��������� ��� �������� �����

 {
  CRAIICCriticalSection cRAIICCriticalSection(&sProtectedVariables.cCriticalSection);
  {
   vector_CUser_Deleted=sProtectedVariables.vector_CUser_Deleted;
   vector_CUser_Added=sProtectedVariables.vector_CUser_Added;
   vector_CUser_Changed=sProtectedVariables.vector_CUser_Changed;

   sProtectedVariables.vector_CUser_Deleted.clear();
   sProtectedVariables.vector_CUser_Added.clear();
   sProtectedVariables.vector_CUser_Changed.clear();

   vector_CTask_Deleted=sProtectedVariables.vector_CTask_Deleted;
   vector_CTask_Added=sProtectedVariables.vector_CTask_Added;
   vector_CTask_Changed=sProtectedVariables.vector_CTask_Changed;

   sProtectedVariables.vector_CTask_Deleted.clear();
   sProtectedVariables.vector_CTask_Added.clear();
   sProtectedVariables.vector_CTask_Changed.clear();

   vector_CProject_Deleted=sProtectedVariables.vector_CProject_Deleted;
   vector_CProject_Added=sProtectedVariables.vector_CProject_Added;
   vector_CProject_Changed=sProtectedVariables.vector_CProject_Changed;

   sProtectedVariables.vector_CProject_Deleted.clear();
   sProtectedVariables.vector_CProject_Added.clear();
   sProtectedVariables.vector_CProject_Changed.clear();

   send_ping=sProtectedVariables.SendPing;

   sProtectedVariables.SendPing=false;
  }
 }
 if (send_ping==true)
 {
  cTransceiver_Ping.SendPingDataToClientInPackage(sClient,SERVER_ANSWER_PING,SERVER_COMMAND_NOTHING,cEvent_Exit,on_exit);
  if (on_exit==true) return;
 }

 //������� ������ �������� �������������
 size=vector_CUser_Deleted.size();
 for(n=0;n<size;n++)
 {
  CUser &cUser=vector_CUser_Deleted[n];
  cTransceiver_User.SendUserDataToClientInPackage(sClient,cUser,SERVER_ANSWER_DELETED_USER,SERVER_COMMAND_NOTHING,cEvent_Exit,on_exit);
  if (on_exit==true) return;
 }
 //������� ������ ����������� �������������
 size=vector_CUser_Added.size();
 for(n=0;n<size;n++)
 {
  CUser &cUser=vector_CUser_Added[n];
  cTransceiver_User.SendUserDataToClientInPackage(sClient,cUser,SERVER_ANSWER_ADDED_USER,SERVER_COMMAND_NOTHING,cEvent_Exit,on_exit);
  if (on_exit==true) return;
 }
 //������� ������ ��������� �������������
 size=vector_CUser_Changed.size();
 for(n=0;n<size;n++)
 {
  CUser &cUser=vector_CUser_Changed[n];
  cTransceiver_User.SendUserDataToClientInPackage(sClient,cUser,SERVER_ANSWER_CHANGED_USER,SERVER_COMMAND_NOTHING,cEvent_Exit,on_exit);
  if (on_exit==true) return;
 }

 //������� ������ �������� �������
 size=vector_CTask_Deleted.size();
 for(n=0;n<size;n++)
 {
  CTask &cTask=vector_CTask_Deleted[n];
  cTransceiver_Task.SendTaskDataToClientInPackage(sClient,cTask,SERVER_ANSWER_DELETED_TASK,SERVER_COMMAND_NOTHING,cEvent_Exit,on_exit);
  if (on_exit==true) return;
 }
 //������� ������ ����������� �������
 size=vector_CTask_Added.size();
 for(n=0;n<size;n++)
 {
  CTask &cTask=vector_CTask_Added[n];
  cTransceiver_Task.SendTaskDataToClientInPackage(sClient,cTask,SERVER_ANSWER_ADDED_TASK,SERVER_COMMAND_NOTHING,cEvent_Exit,on_exit);
  if (on_exit==true) return;
 }
 //������� ������ ��������� �������
 size=vector_CTask_Changed.size();
 for(n=0;n<size;n++)
 {
  CTask &cTask=vector_CTask_Changed[n];
  cTransceiver_Task.SendTaskDataToClientInPackage(sClient,cTask,SERVER_ANSWER_CHANGED_TASK,SERVER_COMMAND_NOTHING,cEvent_Exit,on_exit);
  if (on_exit==true) return;
 }

 //������� ������ �������� ��������
 size=vector_CProject_Deleted.size();
 for(n=0;n<size;n++)
 {
  CProject &cProject=vector_CProject_Deleted[n];
  cTransceiver_Project.SendProjectDataToClientInPackage(sClient,cProject,SERVER_ANSWER_DELETED_PROJECT,SERVER_COMMAND_NOTHING,cEvent_Exit,on_exit);
  if (on_exit==true) return;
 }
 //������� ������ ����������� ��������
 size=vector_CProject_Added.size();
 for(n=0;n<size;n++)
 {
  CProject &cProject=vector_CProject_Added[n];
  cTransceiver_Project.SendProjectDataToClientInPackage(sClient,cProject,SERVER_ANSWER_ADDED_PROJECT,SERVER_COMMAND_NOTHING,cEvent_Exit,on_exit);
  if (on_exit==true) return;
 }
 //������� ������ ��������� ��������
 size=vector_CProject_Changed.size();
 for(n=0;n<size;n++)
 {
  CProject &cProject=vector_CProject_Changed[n];
  cTransceiver_Project.SendProjectDataToClientInPackage(sClient,cProject,SERVER_ANSWER_CHANGED_PROJECT,SERVER_COMMAND_NOTHING,cEvent_Exit,on_exit);
  if (on_exit==true) return;
 }
}


//----------------------------------------------------------------------------------------------------
//������� ������ �� �������
//----------------------------------------------------------------------------------------------------
void CThreadServerUnit::NewDataFromClient(SClient& sClient,char *data,unsigned long length,bool &on_exit)
{
 long n;
 on_exit=false;
 SServerCommand::SHeader sServerCommand_sHeader; 
 //����������� �������� ������ 
 for(n=0;n<length;n++)
 {
  unsigned char byte=static_cast<unsigned char>(data[n]);
  if (sClient.StuffingEnabled==false)//������������ ��������
  {
   if (byte==PROTOCOL_STUFFING)//�������� ������������
   { 
    sClient.StuffingEnabled=true;
    continue;
   }
   if (byte==PROTOCOL_BEGIN_PACKAGE)//�������� ������ ������� ������
   {
    sClient.vector_Data.clear();
	continue;
   }
   if (byte==PROTOCOL_END_PACKAGE)//������� ������� ���������
   {
	sServerCommand_sHeader=*(reinterpret_cast<SServerCommand::SHeader*>(&sClient.vector_Data[0]));
    //�������������� �������� �������
    if (sServerCommand_sHeader.CommandID==SERVER_COMMAND_GET_CLIENT_PROGRAMM_CRC) ExecuteCommand_GetClientProgrammCRC(sClient,static_cast<SERVER_COMMAND>(sServerCommand_sHeader.CommandID),on_exit); 
    if (sServerCommand_sHeader.CommandID==SERVER_COMMAND_GET_CLIENT_PROGRAMM_AND_LOADER) ExecuteCommand_GetClientProgrammAndLoader(sClient,static_cast<SERVER_COMMAND>(sServerCommand_sHeader.CommandID),on_exit); 
    if (sServerCommand_sHeader.CommandID==SERVER_COMMAND_AUTORIZATION) ExecuteCommand_Autorization(sClient,static_cast<SERVER_COMMAND>(sServerCommand_sHeader.CommandID),on_exit); 
    if (sServerCommand_sHeader.CommandID==SERVER_COMMAND_GET_USER_BOOK) ExecuteCommand_GetUserBook(sClient,static_cast<SERVER_COMMAND>(sServerCommand_sHeader.CommandID),on_exit); 
    if (sServerCommand_sHeader.CommandID==SERVER_COMMAND_GET_TASK_BOOK) ExecuteCommand_GetTaskBook(sClient,static_cast<SERVER_COMMAND>(sServerCommand_sHeader.CommandID),on_exit); 
    if (sServerCommand_sHeader.CommandID==SERVER_COMMAND_GET_PROJECT_BOOK) ExecuteCommand_GetProjectBook(sClient,static_cast<SERVER_COMMAND>(sServerCommand_sHeader.CommandID),on_exit); 
    if (sServerCommand_sHeader.CommandID==SERVER_COMMAND_ADDED_TASK) ExecuteCommand_AddedTask(sClient,static_cast<SERVER_COMMAND>(sServerCommand_sHeader.CommandID),on_exit);
    if (sServerCommand_sHeader.CommandID==SERVER_COMMAND_DELETED_TASK) ExecuteCommand_DeletedTask(sClient,static_cast<SERVER_COMMAND>(sServerCommand_sHeader.CommandID),on_exit);
    if (sServerCommand_sHeader.CommandID==SERVER_COMMAND_CHANGED_TASK) ExecuteCommand_ChangedTask(sClient,static_cast<SERVER_COMMAND>(sServerCommand_sHeader.CommandID),on_exit);
    if (sServerCommand_sHeader.CommandID==SERVER_COMMAND_ADDED_PROJECT) ExecuteCommand_AddedProject(sClient,static_cast<SERVER_COMMAND>(sServerCommand_sHeader.CommandID),on_exit);
    if (sServerCommand_sHeader.CommandID==SERVER_COMMAND_DELETED_PROJECT) ExecuteCommand_DeletedProject(sClient,static_cast<SERVER_COMMAND>(sServerCommand_sHeader.CommandID),on_exit);
    if (sServerCommand_sHeader.CommandID==SERVER_COMMAND_CHANGED_PROJECT) ExecuteCommand_ChangedProject(sClient,static_cast<SERVER_COMMAND>(sServerCommand_sHeader.CommandID),on_exit);
	if (on_exit==true) return;
    sClient.vector_Data.clear();
	continue;
   }
  }
  else//������������ �������
  {
   if (byte==PROTOCOL_STUFFING_00) byte=0x00;
   if (byte==PROTOCOL_STUFFING_FE) byte=0xFE;
   if (byte==PROTOCOL_STUFFING_FF) byte=0xFF;
   sClient.StuffingEnabled=false;
  }
  //��������� ���� � �����
  sClient.vector_Data.push_back(byte);
  size_t cmd_length=sClient.vector_Data.size();
  //if (cmd_length>=MAX_PACKAGE_LENGTH) sClient.vector_Data.clear();//��������� ������ �������  
 }
}
//----------------------------------------------------------------------------------------------------
//��������� ������� ��������� CRC ���������� ���������
//----------------------------------------------------------------------------------------------------
void CThreadServerUnit::ExecuteCommand_GetClientProgrammCRC(SClient& sClient,SERVER_COMMAND command,bool &on_exit)
{
 cTransceiver_File.SendCRCClientProgrammFileToClient(sClient,command,CLIENT_PROGRAMM_FILE_NAME,cEvent_Exit,on_exit);
}
//----------------------------------------------------------------------------------------------------
//��������� ������� ��������� ������ ���������� ��������� � ����������
//----------------------------------------------------------------------------------------------------
void CThreadServerUnit::ExecuteCommand_GetClientProgrammAndLoader(SClient& sClient,SERVER_COMMAND command,bool &on_exit)
{
 cTransceiver_File.SendClientProgrammAndLoaderFileToClient(sClient,command,CLIENT_PROGRAMM_FILE_NAME,LOADER_FILE_NAME,cEvent_Exit,on_exit);
}


//----------------------------------------------------------------------------------------------------
//��������� ������� �����������
//----------------------------------------------------------------------------------------------------
void CThreadServerUnit::ExecuteCommand_Autorization(SClient& sClient,SERVER_COMMAND command,bool &on_exit)
{
 cTransceiver_Autorization.ExecuteAutorization(cDocument_Main_Ptr,sClient,command,cEvent_Exit,on_exit);
 cDocument_Main_Ptr->SetUserConnected(sClient.UserGUID,true);
}
//----------------------------------------------------------------------------------------------------
//��������� ������� ��������� ������ �������������
//----------------------------------------------------------------------------------------------------
void CThreadServerUnit::ExecuteCommand_GetUserBook(SClient& sClient,SERVER_COMMAND command,bool &on_exit)
{
 cTransceiver_User.SendUserBook(cDocument_Main_Ptr,sClient,command,cEvent_Exit,on_exit);
}

//----------------------------------------------------------------------------------------------------
//��������� ������� ��������� ������ �������
//----------------------------------------------------------------------------------------------------
void CThreadServerUnit::ExecuteCommand_GetTaskBook(SClient& sClient,SERVER_COMMAND command,bool &on_exit)
{
 cTransceiver_Task.SendTaskBook(cDocument_Main_Ptr,sClient,command,cEvent_Exit,on_exit);
}

//----------------------------------------------------------------------------------------------------
//��������� ������� ��������� ������ ��������
//----------------------------------------------------------------------------------------------------
void CThreadServerUnit::ExecuteCommand_GetProjectBook(SClient& sClient,SERVER_COMMAND command,bool &on_exit)
{
 cTransceiver_Project.SendProjectBook(cDocument_Main_Ptr,sClient,command,cEvent_Exit,on_exit);
}

//----------------------------------------------------------------------------------------------------
//��������� ������� ���������� ������ �������
//----------------------------------------------------------------------------------------------------
void CThreadServerUnit::ExecuteCommand_AddedTask(SClient& sClient,SERVER_COMMAND command,bool &on_exit)
{
 cTransceiver_Task.AddTask(cDocument_Main_Ptr,sClient,command,cEvent_Exit,on_exit);
}

//----------------------------------------------------------------------------------------------------
//��������� ������� �������� �������
//----------------------------------------------------------------------------------------------------
void CThreadServerUnit::ExecuteCommand_DeletedTask(SClient& sClient,SERVER_COMMAND command,bool &on_exit)
{
 cTransceiver_Task.DeleteTask(cDocument_Main_Ptr,sClient,command,cEvent_Exit,on_exit);
}

//----------------------------------------------------------------------------------------------------
//��������� ������� ��������� �������
//----------------------------------------------------------------------------------------------------
void CThreadServerUnit::ExecuteCommand_ChangedTask(SClient& sClient,SERVER_COMMAND command,bool &on_exit)
{
 cTransceiver_Task.ChangeTask(cDocument_Main_Ptr,sClient,command,cEvent_Exit,on_exit);
}

//----------------------------------------------------------------------------------------------------
//��������� ������� ���������� ������ �������
//----------------------------------------------------------------------------------------------------
void CThreadServerUnit::ExecuteCommand_AddedProject(SClient& sClient,SERVER_COMMAND command,bool &on_exit)
{
 cTransceiver_Project.AddProject(cDocument_Main_Ptr,sClient,command,cEvent_Exit,on_exit);
}

//----------------------------------------------------------------------------------------------------
//��������� ������� �������� �������
//----------------------------------------------------------------------------------------------------
void CThreadServerUnit::ExecuteCommand_DeletedProject(SClient& sClient,SERVER_COMMAND command,bool &on_exit)
{
 cTransceiver_Project.DeleteProject(cDocument_Main_Ptr,sClient,command,cEvent_Exit,on_exit);
}

//----------------------------------------------------------------------------------------------------
//��������� ������� ��������� �������
//----------------------------------------------------------------------------------------------------
void CThreadServerUnit::ExecuteCommand_ChangedProject(SClient& sClient,SERVER_COMMAND command,bool &on_exit)
{
 cTransceiver_Project.ChangeProject(cDocument_Main_Ptr,sClient,command,cEvent_Exit,on_exit);
}