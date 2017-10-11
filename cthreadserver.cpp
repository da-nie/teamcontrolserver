#include "cthreadserver.h"

//====================================================================================================
//глобальные переменные
//====================================================================================================

CThreadServer cThreadServer;//серверный поток

//====================================================================================================
//функция потока
//====================================================================================================

UINT ThreadServer(LPVOID pParam)
{
 CThreadServer *cThreadServer_Ptr=reinterpret_cast<CThreadServer*>(pParam);
 if (cThreadServer_Ptr==NULL) return(0);
 cThreadServer_Ptr->Processing();
 return(0);
}
//====================================================================================================
//конструктор класса
//====================================================================================================
CThreadServer::CThreadServer(void)
{
 cWinThread_Thread=NULL;
 cDocument_Main_Ptr=NULL;
}
//====================================================================================================
//деструктор класса
//====================================================================================================
CThreadServer::~CThreadServer()
{ 
 Stop();
}
//====================================================================================================
//функции класса
//====================================================================================================

//----------------------------------------------------------------------------------------------------
//запустить поток
//----------------------------------------------------------------------------------------------------
void CThreadServer::Start(void)
{
 Stop();
 cEvent_Exit.ResetEvent();
 cWinThread_Thread=AfxBeginThread((AFX_THREADPROC)ThreadServer,this);
 cWinThread_Thread->m_bAutoDelete=FALSE;
}
//----------------------------------------------------------------------------------------------------
//остановить поток
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
//задать класс документа
//----------------------------------------------------------------------------------------------------
void CThreadServer::SetDocument(CDocument_Main *cDocument_Main_Set_Ptr)
{
 cDocument_Main_Ptr=cDocument_Main_Set_Ptr;
}
//----------------------------------------------------------------------------------------------------
//был добавлен пользователь
//----------------------------------------------------------------------------------------------------
void CThreadServer::OnUserAdded(const CUser &cUser)
{
 //сообщаем всем потокам о добавлении пользователя
 list<CThreadServerUnit*>::iterator iterator_list_CThreadServerUnitPtr=list_CThreadServerUnitPtr.begin();
 list<CThreadServerUnit*>::iterator iterator_list_CThreadServerUnitPtr_end=list_CThreadServerUnitPtr.end();
 while(iterator_list_CThreadServerUnitPtr!=iterator_list_CThreadServerUnitPtr_end)
 {   
  CThreadServerUnit *cThreadServerUnit_Ptr=(*iterator_list_CThreadServerUnitPtr);
  cThreadServerUnit_Ptr->OnUserAdded(cUser);
  iterator_list_CThreadServerUnitPtr++;
 }
}
//----------------------------------------------------------------------------------------------------
//был удалён пользователь
//----------------------------------------------------------------------------------------------------
void CThreadServer::OnUserDeleted(const CUser &cUser)
{
 //сообщаем всем потокам о удалении пользователя
 list<CThreadServerUnit*>::iterator iterator_list_CThreadServerUnitPtr=list_CThreadServerUnitPtr.begin();
 list<CThreadServerUnit*>::iterator iterator_list_CThreadServerUnitPtr_end=list_CThreadServerUnitPtr.end();
 while(iterator_list_CThreadServerUnitPtr!=iterator_list_CThreadServerUnitPtr_end)
 {   
  CThreadServerUnit *cThreadServerUnit_Ptr=(*iterator_list_CThreadServerUnitPtr);
  cThreadServerUnit_Ptr->OnUserDeleted(cUser);
  iterator_list_CThreadServerUnitPtr++;
 }
}
//----------------------------------------------------------------------------------------------------
//были изменены данные пользователя
//----------------------------------------------------------------------------------------------------
void CThreadServer::OnUserChanged(const CUser &cUser)
{
 //сообщаем всем потокам о изменении данных пользователя
 list<CThreadServerUnit*>::iterator iterator_list_CThreadServerUnitPtr=list_CThreadServerUnitPtr.begin();
 list<CThreadServerUnit*>::iterator iterator_list_CThreadServerUnitPtr_end=list_CThreadServerUnitPtr.end();
 while(iterator_list_CThreadServerUnitPtr!=iterator_list_CThreadServerUnitPtr_end)
 {   
  CThreadServerUnit *cThreadServerUnit_Ptr=(*iterator_list_CThreadServerUnitPtr);
  cThreadServerUnit_Ptr->OnUserChanged(cUser);
  iterator_list_CThreadServerUnitPtr++;
 }
}

//----------------------------------------------------------------------------------------------------
//было добавлено задание
//----------------------------------------------------------------------------------------------------
void CThreadServer::OnTaskAdded(const CTask &cTask)
{
 //сообщаем всем потокам о добавлении задания
 list<CThreadServerUnit*>::iterator iterator_list_CThreadServerUnitPtr=list_CThreadServerUnitPtr.begin();
 list<CThreadServerUnit*>::iterator iterator_list_CThreadServerUnitPtr_end=list_CThreadServerUnitPtr.end();
 while(iterator_list_CThreadServerUnitPtr!=iterator_list_CThreadServerUnitPtr_end)
 {
  CThreadServerUnit *cThreadServerUnit_Ptr=(*iterator_list_CThreadServerUnitPtr);
  cThreadServerUnit_Ptr->OnTaskAdded(cTask);
  iterator_list_CThreadServerUnitPtr++;
 }
}
//----------------------------------------------------------------------------------------------------
//было удалено задание
//----------------------------------------------------------------------------------------------------
void CThreadServer::OnTaskDeleted(const CTask &cTask)
{
 //сообщаем всем потокам о удалении задания
 list<CThreadServerUnit*>::iterator iterator_list_CThreadServerUnitPtr=list_CThreadServerUnitPtr.begin();
 list<CThreadServerUnit*>::iterator iterator_list_CThreadServerUnitPtr_end=list_CThreadServerUnitPtr.end();
 while(iterator_list_CThreadServerUnitPtr!=iterator_list_CThreadServerUnitPtr_end)
 {   
  CThreadServerUnit *cThreadServerUnit_Ptr=(*iterator_list_CThreadServerUnitPtr);
  cThreadServerUnit_Ptr->OnTaskDeleted(cTask);
  iterator_list_CThreadServerUnitPtr++;
 }
}
//----------------------------------------------------------------------------------------------------
//были изменены данные задания
//----------------------------------------------------------------------------------------------------
void CThreadServer::OnTaskChanged(const CTask &cTask)
{
 //сообщаем всем потокам о изменении задания
 list<CThreadServerUnit*>::iterator iterator_list_CThreadServerUnitPtr=list_CThreadServerUnitPtr.begin();
 list<CThreadServerUnit*>::iterator iterator_list_CThreadServerUnitPtr_end=list_CThreadServerUnitPtr.end();
 while(iterator_list_CThreadServerUnitPtr!=iterator_list_CThreadServerUnitPtr_end)
 {   
  CThreadServerUnit *cThreadServerUnit_Ptr=(*iterator_list_CThreadServerUnitPtr);
  cThreadServerUnit_Ptr->OnTaskChanged(cTask);
  iterator_list_CThreadServerUnitPtr++;
 }
}


//----------------------------------------------------------------------------------------------------
//был добавлен проект
//----------------------------------------------------------------------------------------------------
void CThreadServer::OnProjectAdded(const CProject &cProject)
{
 //сообщаем всем потокам о добавлении задания
 list<CThreadServerUnit*>::iterator iterator_list_CThreadServerUnitPtr=list_CThreadServerUnitPtr.begin();
 list<CThreadServerUnit*>::iterator iterator_list_CThreadServerUnitPtr_end=list_CThreadServerUnitPtr.end();
 while(iterator_list_CThreadServerUnitPtr!=iterator_list_CThreadServerUnitPtr_end)
 {   
  CThreadServerUnit *cThreadServerUnit_Ptr=(*iterator_list_CThreadServerUnitPtr);
  cThreadServerUnit_Ptr->OnProjectAdded(cProject);
  iterator_list_CThreadServerUnitPtr++;
 }
}
//----------------------------------------------------------------------------------------------------
//был удален проект
//----------------------------------------------------------------------------------------------------
void CThreadServer::OnProjectDeleted(const CProject &cProject)
{
 //сообщаем всем потокам о удалении задания
 list<CThreadServerUnit*>::iterator iterator_list_CThreadServerUnitPtr=list_CThreadServerUnitPtr.begin();
 list<CThreadServerUnit*>::iterator iterator_list_CThreadServerUnitPtr_end=list_CThreadServerUnitPtr.end();
 while(iterator_list_CThreadServerUnitPtr!=iterator_list_CThreadServerUnitPtr_end)
 {   
  CThreadServerUnit *cThreadServerUnit_Ptr=(*iterator_list_CThreadServerUnitPtr);
  cThreadServerUnit_Ptr->OnProjectDeleted(cProject);
  iterator_list_CThreadServerUnitPtr++;
 }
}
//----------------------------------------------------------------------------------------------------
//были изменены данные проекта
//----------------------------------------------------------------------------------------------------
void CThreadServer::OnProjectChanged(const CProject &cProject)
{
 //сообщаем всем потокам о изменении задания
 list<CThreadServerUnit*>::iterator iterator_list_CThreadServerUnitPtr=list_CThreadServerUnitPtr.begin();
 list<CThreadServerUnit*>::iterator iterator_list_CThreadServerUnitPtr_end=list_CThreadServerUnitPtr.end();
 while(iterator_list_CThreadServerUnitPtr!=iterator_list_CThreadServerUnitPtr_end)
 {   
  CThreadServerUnit *cThreadServerUnit_Ptr=(*iterator_list_CThreadServerUnitPtr);
  cThreadServerUnit_Ptr->OnProjectChanged(cProject);
  iterator_list_CThreadServerUnitPtr++;
 }
}
//----------------------------------------------------------------------------------------------------
//отправка пользователям сообщения для проверки связи
//----------------------------------------------------------------------------------------------------
void CThreadServer::SendPing(void)
{
 //сообщаем всем потокам о посылке сообщения для проверки связи
 list<CThreadServerUnit*>::iterator iterator_list_CThreadServerUnitPtr=list_CThreadServerUnitPtr.begin();
 list<CThreadServerUnit*>::iterator iterator_list_CThreadServerUnitPtr_end=list_CThreadServerUnitPtr.end();
 while(iterator_list_CThreadServerUnitPtr!=iterator_list_CThreadServerUnitPtr_end)
 {   
  CThreadServerUnit *cThreadServerUnit_Ptr=(*iterator_list_CThreadServerUnitPtr);
  cThreadServerUnit_Ptr->SendPing();
  iterator_list_CThreadServerUnitPtr++;
 }
}

//----------------------------------------------------------------------------------------------------
//создание сервера
//----------------------------------------------------------------------------------------------------
SOCKET CThreadServer::CreateServer(void)
{
 //считываем настройки сервера
 if (cDocument_Main_Ptr==NULL) return(INVALID_SOCKET);
 SServerSettings sServerSettings;
 cDocument_Main_Ptr->GetServerSettings(sServerSettings);

 //ждём подключения
 SOCKET socket_server=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);//создаём сокет
 struct sockaddr_in server_addr;
 server_addr.sin_family=AF_INET;
 server_addr.sin_port=htons(sServerSettings.Port);//порт
 server_addr.sin_addr.s_addr=htonl(INADDR_ANY);//ждать подключения с любых адресов (т.е. с любых сетевых карт)
 //связываем адрес с сокетом
 if (bind(socket_server,(SOCKADDR*)&server_addr,sizeof(server_addr))==SOCKET_ERROR)
 {
  MessageBox(NULL,"Не могу назначить порт сокету","Ошибка",MB_OK);
  return(INVALID_SOCKET);
 }
 //назначаем порт на прослушивание
 if (listen(socket_server,1000)==SOCKET_ERROR)
 {
  MessageBox(NULL,"Не могу поставить сокет на прослушивание","Ошибка",MB_OK);
  return(INVALID_SOCKET);
 }
 //переведём сокет сервера в неблокирующий режим
 unsigned long nb=1;
 if (ioctlsocket(socket_server,FIONBIO,(unsigned long *)&nb)==SOCKET_ERROR)
 {
  closesocket(socket_server);
  MessageBox(NULL,"Не могу перевести сокет сервера в неблокирующий режим","Ошибка",MB_OK);
  return(INVALID_SOCKET);
 }
 return(socket_server);
}
//----------------------------------------------------------------------------------------------------
//отключение всех клиентов и очистка списка всех клиентов
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
//подключение нового клиента
//----------------------------------------------------------------------------------------------------
void CThreadServer::NewClient(SOCKET socket_server)
{
 struct sockaddr client_addr;
 int addr_size=sizeof(sockaddr);
 SOCKET socket_client=accept(socket_server,(SOCKADDR*)&client_addr,&addr_size);
 //переведём сокет клиента в неблокирующий режим
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
 list_CThreadServerUnitPtr.push_back(cThreadServerUnit_Ptr);//добавляем новый поток обработки клиента
}

//----------------------------------------------------------------------------------------------------
//основной цикл обработки
//----------------------------------------------------------------------------------------------------
void CThreadServer::Processing(void)
{
 const static long timeout_us=10000;//таймаут на операцию ожидания событий

 //отключим всех возможных клиентов
 DisconnectAllClientAndResetClientList();
 //создадим сокет сервера
 SOCKET socket_server=CreateServer();
 if (socket_server==INVALID_SOCKET) return;
 //ждём событий с сокетом сервера 
 while(1)
 {
  if (WaitForSingleObject(cEvent_Exit.m_hObject,0)==WAIT_OBJECT_0) break;//получен сигнал на выход
  //удаляем из списка отключившихся клиентов
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
    //удаляем клиента из списка
    list_CThreadServerUnitPtr.erase(iterator_list_CThreadServerUnitPtr);
    iterator_list_CThreadServerUnitPtr=list_CThreadServerUnitPtr.begin();
	iterator_list_CThreadServerUnitPtr_end=list_CThreadServerUnitPtr.end();
    continue;
   } 
   iterator_list_CThreadServerUnitPtr++;
  }
  //обрабатываем события с сокетом сервера
  fd_set Readen;
  FD_ZERO(&Readen);//обнуляем список
  FD_SET(socket_server,&Readen);//добавляем сокет сервера
  fd_set Exeption;
  FD_ZERO(&Exeption);//обнуляем список
  FD_SET(socket_server,&Exeption);//добавляем сокет сервера

  //ждём событий с сокетом сервера
  timeval timeout;
  timeout.tv_sec=0;
  timeout.tv_usec=timeout_us;
  if (select(FD_SETSIZE,&Readen,0,&Exeption,&timeout)>0)//произошли события с сокетами
  {
   if (FD_ISSET(socket_server,&Readen)) NewClient(socket_server);//новый клиент хочет подключиться к серверу
   //не произошло ли исключение на сокете сервера?
   if (FD_ISSET(socket_server,&Exeption)) break;//это какой-то сбой. Выходим из цикла
  }
 }
 //закрываем соеднинения со всеми клиентами
 DisconnectAllClientAndResetClientList();
 //переведём сокет сервера в блокирующий режим
 unsigned long nb=0;
 ioctlsocket(socket_server,FIONBIO,(unsigned long *)&nb);
 //отключаем сервер
 shutdown(socket_server,SD_BOTH);
 closesocket(socket_server);
}

