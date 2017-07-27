#include "cthreadserver.h"

//====================================================================================================
//���������� ����������
//====================================================================================================

CThreadServer cThreadServer;//��������� �����

//====================================================================================================
//������� ������
//====================================================================================================

UINT ThreadServer(LPVOID pParam)
{
 CThreadServer *cThreadServer_Ptr=reinterpret_cast<CThreadServer*>(pParam);
 if (cThreadServer_Ptr==NULL) return(0);
 cThreadServer_Ptr->Processing();
 return(0);
}
//====================================================================================================
//����������� ������
//====================================================================================================
CThreadServer::CThreadServer(void)
{
 cWinThread_Thread=NULL;
 cDocument_Main_Ptr=NULL;
}
//====================================================================================================
//���������� ������
//====================================================================================================
CThreadServer::~CThreadServer()
{ 
 Stop();
}
//====================================================================================================
//������� ������
//====================================================================================================

//----------------------------------------------------------------------------------------------------
//��������� �����
//----------------------------------------------------------------------------------------------------
void CThreadServer::Start(void)
{
 Stop();
 cEvent_Exit.ResetEvent();
 cWinThread_Thread=AfxBeginThread((AFX_THREADPROC)ThreadServer,this);
 cWinThread_Thread->m_bAutoDelete=FALSE;
}
//----------------------------------------------------------------------------------------------------
//���������� �����
//----------------------------------------------------------------------------------------------------
void CThreadServer::Stop(void)
{ 
 if (cWinThread_Thread!=NULL)
 {
  cEvent_Exit.SetEvent();
  WaitForSingleObject(cWinThread_Thread->m_hThread,INFINITE);
  delete(cWinThread_Thread);
  cWinThread_Thread=NULL;
 }
}
//----------------------------------------------------------------------------------------------------
//������ ����� ���������
//----------------------------------------------------------------------------------------------------
void CThreadServer::SetDocument(CDocument_Main *cDocument_Main_Set_Ptr)
{
 cDocument_Main_Ptr=cDocument_Main_Set_Ptr;
}
//----------------------------------------------------------------------------------------------------
//��� �������� ������������
//----------------------------------------------------------------------------------------------------
void CThreadServer::OnUserAdded(const SUser &sUser)
{
 //�������� ���� ������� � ���������� ������������
 list<CThreadServerUnit*>::iterator iterator_list_CThreadServerUnitPtr=list_CThreadServerUnitPtr.begin();
 list<CThreadServerUnit*>::iterator iterator_list_CThreadServerUnitPtr_end=list_CThreadServerUnitPtr.end();
 while(iterator_list_CThreadServerUnitPtr!=iterator_list_CThreadServerUnitPtr_end)
 {   
  CThreadServerUnit *cThreadServerUnit_Ptr=(*iterator_list_CThreadServerUnitPtr);
  cThreadServerUnit_Ptr->OnUserAdded(sUser);
  iterator_list_CThreadServerUnitPtr++;
 }
}
//----------------------------------------------------------------------------------------------------
//��� ����� ������������
//----------------------------------------------------------------------------------------------------
void CThreadServer::OnUserDeleted(const SUser &sUser)
{
 //�������� ���� ������� � �������� ������������
 list<CThreadServerUnit*>::iterator iterator_list_CThreadServerUnitPtr=list_CThreadServerUnitPtr.begin();
 list<CThreadServerUnit*>::iterator iterator_list_CThreadServerUnitPtr_end=list_CThreadServerUnitPtr.end();
 while(iterator_list_CThreadServerUnitPtr!=iterator_list_CThreadServerUnitPtr_end)
 {   
  CThreadServerUnit *cThreadServerUnit_Ptr=(*iterator_list_CThreadServerUnitPtr);
  cThreadServerUnit_Ptr->OnUserDeleted(sUser);
  iterator_list_CThreadServerUnitPtr++;
 }
}
//----------------------------------------------------------------------------------------------------
//���� �������� ������ ������������
//----------------------------------------------------------------------------------------------------
void CThreadServer::OnUserChanged(const SUser &sUser)
{
 //�������� ���� ������� � ��������� ������ ������������
 list<CThreadServerUnit*>::iterator iterator_list_CThreadServerUnitPtr=list_CThreadServerUnitPtr.begin();
 list<CThreadServerUnit*>::iterator iterator_list_CThreadServerUnitPtr_end=list_CThreadServerUnitPtr.end();
 while(iterator_list_CThreadServerUnitPtr!=iterator_list_CThreadServerUnitPtr_end)
 {   
  CThreadServerUnit *cThreadServerUnit_Ptr=(*iterator_list_CThreadServerUnitPtr);
  cThreadServerUnit_Ptr->OnUserChanged(sUser);
  iterator_list_CThreadServerUnitPtr++;
 }
}

//----------------------------------------------------------------------------------------------------
//���� ��������� �������
//----------------------------------------------------------------------------------------------------
void CThreadServer::OnTaskAdded(const STask &sTask)
{
 //�������� ���� ������� � ���������� �������
 list<CThreadServerUnit*>::iterator iterator_list_CThreadServerUnitPtr=list_CThreadServerUnitPtr.begin();
 list<CThreadServerUnit*>::iterator iterator_list_CThreadServerUnitPtr_end=list_CThreadServerUnitPtr.end();
 while(iterator_list_CThreadServerUnitPtr!=iterator_list_CThreadServerUnitPtr_end)
 {   
  CThreadServerUnit *cThreadServerUnit_Ptr=(*iterator_list_CThreadServerUnitPtr);
  cThreadServerUnit_Ptr->OnTaskAdded(sTask);
  iterator_list_CThreadServerUnitPtr++;
 }
}
//----------------------------------------------------------------------------------------------------
//���� ������� �������
//----------------------------------------------------------------------------------------------------
void CThreadServer::OnTaskDeleted(const STask &sTask)
{
 //�������� ���� ������� � �������� �������
 list<CThreadServerUnit*>::iterator iterator_list_CThreadServerUnitPtr=list_CThreadServerUnitPtr.begin();
 list<CThreadServerUnit*>::iterator iterator_list_CThreadServerUnitPtr_end=list_CThreadServerUnitPtr.end();
 while(iterator_list_CThreadServerUnitPtr!=iterator_list_CThreadServerUnitPtr_end)
 {   
  CThreadServerUnit *cThreadServerUnit_Ptr=(*iterator_list_CThreadServerUnitPtr);
  cThreadServerUnit_Ptr->OnTaskDeleted(sTask);
  iterator_list_CThreadServerUnitPtr++;
 }
}
//----------------------------------------------------------------------------------------------------
//���� �������� ������ �������
//----------------------------------------------------------------------------------------------------
void CThreadServer::OnTaskChanged(const STask &sTask)
{
 //�������� ���� ������� � ��������� �������
 list<CThreadServerUnit*>::iterator iterator_list_CThreadServerUnitPtr=list_CThreadServerUnitPtr.begin();
 list<CThreadServerUnit*>::iterator iterator_list_CThreadServerUnitPtr_end=list_CThreadServerUnitPtr.end();
 while(iterator_list_CThreadServerUnitPtr!=iterator_list_CThreadServerUnitPtr_end)
 {   
  CThreadServerUnit *cThreadServerUnit_Ptr=(*iterator_list_CThreadServerUnitPtr);
  cThreadServerUnit_Ptr->OnTaskChanged(sTask);
  iterator_list_CThreadServerUnitPtr++;
 }
}


//----------------------------------------------------------------------------------------------------
//��� �������� ������
//----------------------------------------------------------------------------------------------------
void CThreadServer::OnProjectAdded(const SProject &sProject)
{
 //�������� ���� ������� � ���������� �������
 list<CThreadServerUnit*>::iterator iterator_list_CThreadServerUnitPtr=list_CThreadServerUnitPtr.begin();
 list<CThreadServerUnit*>::iterator iterator_list_CThreadServerUnitPtr_end=list_CThreadServerUnitPtr.end();
 while(iterator_list_CThreadServerUnitPtr!=iterator_list_CThreadServerUnitPtr_end)
 {   
  CThreadServerUnit *cThreadServerUnit_Ptr=(*iterator_list_CThreadServerUnitPtr);
  cThreadServerUnit_Ptr->OnProjectAdded(sProject);
  iterator_list_CThreadServerUnitPtr++;
 }
}
//----------------------------------------------------------------------------------------------------
//��� ������ ������
//----------------------------------------------------------------------------------------------------
void CThreadServer::OnProjectDeleted(const SProject &sProject)
{
 //�������� ���� ������� � �������� �������
 list<CThreadServerUnit*>::iterator iterator_list_CThreadServerUnitPtr=list_CThreadServerUnitPtr.begin();
 list<CThreadServerUnit*>::iterator iterator_list_CThreadServerUnitPtr_end=list_CThreadServerUnitPtr.end();
 while(iterator_list_CThreadServerUnitPtr!=iterator_list_CThreadServerUnitPtr_end)
 {   
  CThreadServerUnit *cThreadServerUnit_Ptr=(*iterator_list_CThreadServerUnitPtr);
  cThreadServerUnit_Ptr->OnProjectDeleted(sProject);
  iterator_list_CThreadServerUnitPtr++;
 }
}
//----------------------------------------------------------------------------------------------------
//���� �������� ������ �������
//----------------------------------------------------------------------------------------------------
void CThreadServer::OnProjectChanged(const SProject &sProject)
{
 //�������� ���� ������� � ��������� �������
 list<CThreadServerUnit*>::iterator iterator_list_CThreadServerUnitPtr=list_CThreadServerUnitPtr.begin();
 list<CThreadServerUnit*>::iterator iterator_list_CThreadServerUnitPtr_end=list_CThreadServerUnitPtr.end();
 while(iterator_list_CThreadServerUnitPtr!=iterator_list_CThreadServerUnitPtr_end)
 {   
  CThreadServerUnit *cThreadServerUnit_Ptr=(*iterator_list_CThreadServerUnitPtr);
  cThreadServerUnit_Ptr->OnProjectChanged(sProject);
  iterator_list_CThreadServerUnitPtr++;
 }
}

//----------------------------------------------------------------------------------------------------
//�������� �������
//----------------------------------------------------------------------------------------------------
SOCKET CThreadServer::CreateServer(void)
{
 //��������� ��������� �������
 if (cDocument_Main_Ptr==NULL) return(INVALID_SOCKET);
 SServerSettings sServerSettings;
 cDocument_Main_Ptr->GetServerSettings(sServerSettings);

 //��� �����������
 SOCKET socket_server=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);//������ �����
 struct sockaddr_in server_addr;
 server_addr.sin_family=AF_INET;
 server_addr.sin_port=htons(sServerSettings.Port);//����
 server_addr.sin_addr.s_addr=htonl(INADDR_ANY);//����� ����������� � ����� ������� (�.�. � ����� ������� ����)
 //��������� ����� � �������
 if (bind(socket_server,(SOCKADDR*)&server_addr,sizeof(server_addr))==SOCKET_ERROR)
 {
  MessageBox(NULL,"�� ���� ��������� ���� ������","������",MB_OK);
  return(INVALID_SOCKET);
 }
 //��������� ���� �� �������������
 if (listen(socket_server,1000)==SOCKET_ERROR)
 {
  MessageBox(NULL,"�� ���� ��������� ����� �� �������������","������",MB_OK);
  return(INVALID_SOCKET);
 }
 //�������� ����� ������� � ������������� �����
 unsigned long nb=1;
 if (ioctlsocket(socket_server,FIONBIO,(unsigned long *)&nb)==SOCKET_ERROR)
 {
  closesocket(socket_server);
  MessageBox(NULL,"�� ���� ��������� ����� ������� � ������������� �����","������",MB_OK);
  return(INVALID_SOCKET);
 }
 return(socket_server);
}
//----------------------------------------------------------------------------------------------------
//���������� ���� �������� � ������� ������ ���� ��������
//----------------------------------------------------------------------------------------------------
void CThreadServer::DisconnectAllClientAndResetClientList(void)
{
 list<CThreadServerUnit*>::iterator iterator_list_CThreadServerUnitPtr=list_CThreadServerUnitPtr.begin();
 list<CThreadServerUnit*>::iterator iterator_list_CThreadServerUnitPtr_end=list_CThreadServerUnitPtr.end();
 while(iterator_list_CThreadServerUnitPtr!=iterator_list_CThreadServerUnitPtr_end)
 {   
  CThreadServerUnit *cThreadServerUnit_Ptr=(*iterator_list_CThreadServerUnitPtr);
  cThreadServerUnit_Ptr->Stop();
  delete(cThreadServerUnit_Ptr);
  iterator_list_CThreadServerUnitPtr++;
 }
 list_CThreadServerUnitPtr.clear();
}
//----------------------------------------------------------------------------------------------------
//����������� ������ �������
//----------------------------------------------------------------------------------------------------
void CThreadServer::NewClient(SOCKET socket_server)
{
 struct sockaddr client_addr;
 int addr_size=sizeof(sockaddr);
 SOCKET socket_client=accept(socket_server,(SOCKADDR*)&client_addr,&addr_size);
 //�������� ����� ������� � ������������� �����
 unsigned long nb=1;
 if (ioctlsocket(socket_client,FIONBIO,(unsigned long *)&nb)==INVALID_SOCKET)
 {
  shutdown(socket_client,SD_BOTH);
  closesocket(socket_client);
  return;
 }
 CThreadServerUnit *cThreadServerUnit_Ptr=new CThreadServerUnit;
 cThreadServerUnit_Ptr->SetDocument(cDocument_Main_Ptr);
 cThreadServerUnit_Ptr->Start(socket_client);
 list_CThreadServerUnitPtr.push_back(cThreadServerUnit_Ptr);//��������� ����� ����� ��������� �������
}

//----------------------------------------------------------------------------------------------------
//�������� ���� ���������
//----------------------------------------------------------------------------------------------------
void CThreadServer::Processing(void)
{
 const static long timeout_us=10000;//������� �� �������� �������� �������

 //�������� ���� ��������� ��������
 DisconnectAllClientAndResetClientList();
 //�������� ����� �������
 SOCKET socket_server=CreateServer();
 if (socket_server==INVALID_SOCKET) return;
 //��� ������� � ������� ������� 
 while(1)
 {
  if (WaitForSingleObject(cEvent_Exit.m_hObject,0)==WAIT_OBJECT_0) break;//������� ������ �� �����
  //������� �� ������ ������������� ��������
  list<CThreadServerUnit*>::iterator iterator_list_CThreadServerUnitPtr=list_CThreadServerUnitPtr.begin();
  list<CThreadServerUnit*>::iterator iterator_list_CThreadServerUnitPtr_end=list_CThreadServerUnitPtr.end();
  while(iterator_list_CThreadServerUnitPtr!=iterator_list_CThreadServerUnitPtr_end)
  {   
   CThreadServerUnit *cThreadServerUnit_Ptr=(*iterator_list_CThreadServerUnitPtr);
   bool online=cThreadServerUnit_Ptr->GetClientOnLine();
   if (online==false)
   {    
    cThreadServerUnit_Ptr->Stop();
    delete(cThreadServerUnit_Ptr);
    //������� ������� �� ������
    list_CThreadServerUnitPtr.erase(iterator_list_CThreadServerUnitPtr);
    iterator_list_CThreadServerUnitPtr=list_CThreadServerUnitPtr.begin();
	iterator_list_CThreadServerUnitPtr_end=list_CThreadServerUnitPtr.end();
    continue;
   } 
   iterator_list_CThreadServerUnitPtr++;
  }
  //������������ ������� � ������� �������
  fd_set Readen;
  FD_ZERO(&Readen);//�������� ������
  FD_SET(socket_server,&Readen);//��������� ����� �������
  fd_set Exeption;
  FD_ZERO(&Exeption);//�������� ������
  FD_SET(socket_server,&Exeption);//��������� ����� �������

  //��� ������� � ������� �������
  timeval timeout;
  timeout.tv_sec=0;
  timeout.tv_usec=timeout_us;
  if (select(FD_SETSIZE,&Readen,0,&Exeption,&timeout)>0)//��������� ������� � ��������
  {
   if (FD_ISSET(socket_server,&Readen)) NewClient(socket_server);//����� ������ ����� ������������ � �������
   //�� ��������� �� ���������� �� ������ �������?
   if (FD_ISSET(socket_server,&Exeption)) break;//��� �����-�� ����. ������� �� �����
  }
 }
 //��������� ����������� �� ����� ���������
 DisconnectAllClientAndResetClientList();
 //�������� ����� ������� � ����������� �����
 unsigned long nb=0;
 ioctlsocket(socket_server,FIONBIO,(unsigned long *)&nb);
 //��������� ������
 shutdown(socket_server,SD_BOTH);
 closesocket(socket_server);
}

