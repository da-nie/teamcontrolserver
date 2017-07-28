#include "cthreadserverunit.h"

//====================================================================================================
//макроопределения
//====================================================================================================

//размер буфера приёма
#define BUFFER_SIZE 16384

//====================================================================================================
//функция потока
//====================================================================================================

UINT ThreadServerUnit(LPVOID pParam)
{
 CThreadServerUnit *cThreadServer_Ptr=reinterpret_cast<CThreadServerUnit*>(pParam);
 if (cThreadServer_Ptr==NULL) return(0);
 cThreadServer_Ptr->Processing();
 return(0);
}
//====================================================================================================
//конструктор класса
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
//деструктор класса
//====================================================================================================
CThreadServerUnit::~CThreadServerUnit()
{ 
 Stop();
 delete[](Buffer);
}
//====================================================================================================
//функции класса
//====================================================================================================

//----------------------------------------------------------------------------------------------------
//запустить поток
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
 cEvent_Exit.ResetEvent();
 cWinThread_Thread=AfxBeginThread((AFX_THREADPROC)ThreadServerUnit,this);
 cWinThread_Thread->m_bAutoDelete=FALSE;
}
//----------------------------------------------------------------------------------------------------
//остановить поток
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
//задать класс документа
//----------------------------------------------------------------------------------------------------
void CThreadServerUnit::SetDocument(CDocument_Main *cDocument_Main_Set_Ptr)
{
 cDocument_Main_Ptr=cDocument_Main_Set_Ptr;
}
//----------------------------------------------------------------------------------------------------
//был добавлен пользователь
//----------------------------------------------------------------------------------------------------
void CThreadServerUnit::OnUserAdded(const SUser &sUser)
{
 {
  CRAIICCriticalSection cRAIICCriticalSection(&sProtectedVariables.cCriticalSection);
  {
   sProtectedVariables.vector_SUser_Added.push_back(sUser);
  }
 }
}
//----------------------------------------------------------------------------------------------------
//был удалён пользователь
//----------------------------------------------------------------------------------------------------
void CThreadServerUnit::OnUserDeleted(const SUser &sUser)
{
 {
  CRAIICCriticalSection cRAIICCriticalSection(&sProtectedVariables.cCriticalSection);
  {
   sProtectedVariables.vector_SUser_Deleted.push_back(sUser);
  }
 }
}
//----------------------------------------------------------------------------------------------------
//были изменены данные пользователя
//----------------------------------------------------------------------------------------------------
void CThreadServerUnit::OnUserChanged(const SUser &sUser)
{
 {
  CRAIICCriticalSection cRAIICCriticalSection(&sProtectedVariables.cCriticalSection);
  {
   sProtectedVariables.vector_SUser_Changed.push_back(sUser);
  }
 }
}



//----------------------------------------------------------------------------------------------------
//было добавлено задание
//----------------------------------------------------------------------------------------------------
void CThreadServerUnit::OnTaskAdded(const STask &sTask)
{
 {
  CRAIICCriticalSection cRAIICCriticalSection(&sProtectedVariables.cCriticalSection);
  {
   sProtectedVariables.vector_STask_Added.push_back(sTask);
  }
 }
}
//----------------------------------------------------------------------------------------------------
//было удалено задание
//----------------------------------------------------------------------------------------------------
void CThreadServerUnit::OnTaskDeleted(const STask &sTask)
{
 {
  CRAIICCriticalSection cRAIICCriticalSection(&sProtectedVariables.cCriticalSection);
  {
   sProtectedVariables.vector_STask_Deleted.push_back(sTask);
  }
 }
}
//----------------------------------------------------------------------------------------------------
//были изменены данные задания
//----------------------------------------------------------------------------------------------------
void CThreadServerUnit::OnTaskChanged(const STask &sTask)
{
 {
  CRAIICCriticalSection cRAIICCriticalSection(&sProtectedVariables.cCriticalSection);
  {
   sProtectedVariables.vector_STask_Changed.push_back(sTask);
  }
 }
}



//----------------------------------------------------------------------------------------------------
//был добавлен проект
//----------------------------------------------------------------------------------------------------
void CThreadServerUnit::OnProjectAdded(const SProject &sProject)
{
 {
  CRAIICCriticalSection cRAIICCriticalSection(&sProtectedVariables.cCriticalSection);
  {
   sProtectedVariables.vector_SProject_Added.push_back(sProject);
  }
 }
}
//----------------------------------------------------------------------------------------------------
//был удален проект
//----------------------------------------------------------------------------------------------------
void CThreadServerUnit::OnProjectDeleted(const SProject &sProject)
{
 {
  CRAIICCriticalSection cRAIICCriticalSection(&sProtectedVariables.cCriticalSection);
  {
   sProtectedVariables.vector_SProject_Deleted.push_back(sProject);
  }
 }
}
//----------------------------------------------------------------------------------------------------
//были изменены данные проекта
//----------------------------------------------------------------------------------------------------
void CThreadServerUnit::OnProjectChanged(const SProject &sProject)
{
 {
  CRAIICCriticalSection cRAIICCriticalSection(&sProtectedVariables.cCriticalSection);
  {
   sProtectedVariables.vector_SProject_Changed.push_back(sProject);
  }
 }
}

//----------------------------------------------------------------------------------------------------
//отключение клиента
//----------------------------------------------------------------------------------------------------
void CThreadServerUnit::ClientDisconnect(const SClient& sClient)
{ 
 if (sClient.Socket==INVALID_SOCKET) return;
 //переведём сокет клиента в блокирующий режим
 unsigned long nb=0;
 ioctlsocket(sClient.Socket,FIONBIO,(unsigned long *)&nb);
 //отключаем клиента
 shutdown(sClient.Socket,SD_BOTH);
 closesocket(sClient.Socket);
}
//----------------------------------------------------------------------------------------------------
//задать, в сети ли клиент
//----------------------------------------------------------------------------------------------------
void CThreadServerUnit::SetClientOnLine(bool state)
{
 {
  CRAIICCriticalSection cRAIICCriticalSection(&sProtectedVariables.cCriticalSection);
  {
   sProtectedVariables.ClientOnLine=state;
  }
 }
}
//----------------------------------------------------------------------------------------------------
//получить, в сети ли клиент
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
//основной цикл обработки
//----------------------------------------------------------------------------------------------------
void CThreadServerUnit::Processing(void)
{
 const static long timeout_us=10000;//таймаут на операцию ожидания событий
 //ждём событий с сокетом клиента
 while(1)
 {
  if (WaitForSingleObject(cEvent_Exit.m_hObject,0)==WAIT_OBJECT_0) break;//получен сигнал на выход

  bool on_exit; 
  LinkProcessing(sClient_My,on_exit);
  if (on_exit==true) break;

  fd_set Readen;
  FD_ZERO(&Readen);//обнуляем список
  fd_set Exeption;
  FD_ZERO(&Exeption);//обнуляем список
  //добавляем сокет клиента
  FD_SET(sClient_My.Socket,&Readen);//добавляем сокет клиента
  FD_SET(sClient_My.Socket,&Exeption);//добавляем сокет клиента   
  //ждём событий с сокетами
  timeval timeout;
  timeout.tv_sec=0;
  timeout.tv_usec=timeout_us;
  if (select(FD_SETSIZE,&Readen,0,&Exeption,&timeout)>0)//произошли события с сокетами
  {
   if (FD_ISSET(sClient_My.Socket,&Exeption)) break;//сбой у клиента
   if (FD_ISSET(sClient_My.Socket,&Readen))//клиент готов на чтение (т.е. прислал нам данные)
   {     
    long receive=recv(sClient_My.Socket,Buffer,BUFFER_SIZE,0); 
    if (receive<=0) break;//клиент отключился
    //обрабатываем принятые данные
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
//обработка связи
//----------------------------------------------------------------------------------------------------
void CThreadServerUnit::LinkProcessing(SClient &sClient,bool &on_exit)
{ 
 size_t size;
 size_t n;

 on_exit=false;
 //копируем данные, которые необходимо передать клиенту
 vector<SUser> vector_SUser_Deleted;//удалённые пользователи
 vector<SUser> vector_SUser_Changed;//изменённые пользователи
 vector<SUser> vector_SUser_Added;//добавленные пользователи

 vector<STask> vector_STask_Deleted;//удалённые задания
 vector<STask> vector_STask_Changed;//изменённые задания
 vector<STask> vector_STask_Added;//добавленные задания

 vector<SProject> vector_SProject_Deleted;//удалённые проекты
 vector<SProject> vector_SProject_Changed;//изменённые проекты
 vector<SProject> vector_SProject_Added;//добавленные проекты

 {
  CRAIICCriticalSection cRAIICCriticalSection(&sProtectedVariables.cCriticalSection);
  {
   vector_SUser_Deleted=sProtectedVariables.vector_SUser_Deleted;
   vector_SUser_Added=sProtectedVariables.vector_SUser_Added;
   vector_SUser_Changed=sProtectedVariables.vector_SUser_Changed;

   sProtectedVariables.vector_SUser_Deleted.clear();
   sProtectedVariables.vector_SUser_Added.clear();
   sProtectedVariables.vector_SUser_Changed.clear();

   vector_STask_Deleted=sProtectedVariables.vector_STask_Deleted;
   vector_STask_Added=sProtectedVariables.vector_STask_Added;
   vector_STask_Changed=sProtectedVariables.vector_STask_Changed;

   sProtectedVariables.vector_STask_Deleted.clear();
   sProtectedVariables.vector_STask_Added.clear();
   sProtectedVariables.vector_STask_Changed.clear();

   vector_SProject_Deleted=sProtectedVariables.vector_SProject_Deleted;
   vector_SProject_Added=sProtectedVariables.vector_SProject_Added;
   vector_SProject_Changed=sProtectedVariables.vector_SProject_Changed;

   sProtectedVariables.vector_SProject_Deleted.clear();
   sProtectedVariables.vector_SProject_Added.clear();
   sProtectedVariables.vector_SProject_Changed.clear();
  }
 }
 //передаём список удалённых пользователей
 size=vector_SUser_Deleted.size();
 for(n=0;n<size;n++)
 {
  SUser &sUser=vector_SUser_Deleted[n];
  cTransceiver_User.SendUserDataToClientInPackage(sClient,sUser,SERVER_ANSWER_DELETED_USER,SERVER_COMMAND_NOTHING,cEvent_Exit,on_exit);
  if (on_exit==true) return;
 }
 //передаём список добавленных пользователей
 size=vector_SUser_Added.size();
 for(n=0;n<size;n++)
 {
  SUser &sUser=vector_SUser_Added[n];
  cTransceiver_User.SendUserDataToClientInPackage(sClient,sUser,SERVER_ANSWER_ADDED_USER,SERVER_COMMAND_NOTHING,cEvent_Exit,on_exit);
  if (on_exit==true) return;
 }
 //передаём список изменённых пользователей
 size=vector_SUser_Changed.size();
 for(n=0;n<size;n++)
 {
  SUser &sUser=vector_SUser_Changed[n];
  cTransceiver_User.SendUserDataToClientInPackage(sClient,sUser,SERVER_ANSWER_CHANGED_USER,SERVER_COMMAND_NOTHING,cEvent_Exit,on_exit);
  if (on_exit==true) return;
 }

 //передаём список удалённых заданий
 size=vector_STask_Deleted.size();
 for(n=0;n<size;n++)
 {
  STask &sTask=vector_STask_Deleted[n];
  cTransceiver_Task.SendTaskDataToClientInPackage(sClient,sTask,SERVER_ANSWER_DELETED_TASK,SERVER_COMMAND_NOTHING,cEvent_Exit,on_exit);
  if (on_exit==true) return;
 }
 //передаём список добавленных заданий
 size=vector_STask_Added.size();
 for(n=0;n<size;n++)
 {
  STask &sTask=vector_STask_Added[n];
  cTransceiver_Task.SendTaskDataToClientInPackage(sClient,sTask,SERVER_ANSWER_ADDED_TASK,SERVER_COMMAND_NOTHING,cEvent_Exit,on_exit);
  if (on_exit==true) return;
 }
 //передаём список изменённых заданий
 size=vector_STask_Changed.size();
 for(n=0;n<size;n++)
 {
  STask &sTask=vector_STask_Changed[n];
  cTransceiver_Task.SendTaskDataToClientInPackage(sClient,sTask,SERVER_ANSWER_CHANGED_TASK,SERVER_COMMAND_NOTHING,cEvent_Exit,on_exit);
  if (on_exit==true) return;
 }

 //передаём список удалённых проектов
 size=vector_SProject_Deleted.size();
 for(n=0;n<size;n++)
 {
  SProject &sProject=vector_SProject_Deleted[n];
  cTransceiver_Project.SendProjectDataToClientInPackage(sClient,sProject,SERVER_ANSWER_DELETED_PROJECT,SERVER_COMMAND_NOTHING,cEvent_Exit,on_exit);
  if (on_exit==true) return;
 }
 //передаём список добавленных проектов
 size=vector_SProject_Added.size();
 for(n=0;n<size;n++)
 {
  SProject &sProject=vector_SProject_Added[n];
  cTransceiver_Project.SendProjectDataToClientInPackage(sClient,sProject,SERVER_ANSWER_ADDED_PROJECT,SERVER_COMMAND_NOTHING,cEvent_Exit,on_exit);
  if (on_exit==true) return;
 }
 //передаём список изменённых проектов
 size=vector_SProject_Changed.size();
 for(n=0;n<size;n++)
 {
  SProject &sProject=vector_SProject_Changed[n];
  cTransceiver_Project.SendProjectDataToClientInPackage(sClient,sProject,SERVER_ANSWER_CHANGED_PROJECT,SERVER_COMMAND_NOTHING,cEvent_Exit,on_exit);
  if (on_exit==true) return;
 }

}


//----------------------------------------------------------------------------------------------------
//приняты данные от клиента
//----------------------------------------------------------------------------------------------------
void CThreadServerUnit::NewDataFromClient(SClient& sClient,char *data,unsigned long length,bool &on_exit)
{
 long n;
 on_exit=false;
 SServerCommand::SHeader sServerCommand_sHeader; 
 //анализируем принятые данные 
 for(n=0;n<length;n++)
 {
  unsigned char byte=static_cast<unsigned char>(data[n]);
  if (sClient.StuffingEnabled==false)//байтстаффинг отключён
  {
   if (byte==PROTOCOL_STUFFING)//включаем байтстаффинг
   { 
    sClient.StuffingEnabled=true;
    continue;
   }
   if (byte==PROTOCOL_BEGIN_PACKAGE)//начинаем сборку команды заново
   {
    sClient.vector_Data.clear();
	continue;
   }
   if (byte==PROTOCOL_END_PACKAGE)//команда собрана полностью
   {
	sServerCommand_sHeader=*(reinterpret_cast<SServerCommand::SHeader*>(&sClient.vector_Data[0]));
    //расшифровываем принятую команду
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
  else//байтстаффинг включён
  {
   if (byte==PROTOCOL_STUFFING_00) byte=0x00;
   if (byte==PROTOCOL_STUFFING_FE) byte=0xFE;
   if (byte==PROTOCOL_STUFFING_FF) byte=0xFF;
   sClient.StuffingEnabled=false;
  }
  //добавляем байт в буфер
  sClient.vector_Data.push_back(byte);
  size_t cmd_length=sClient.vector_Data.size();
  //if (cmd_length>=MAX_PACKAGE_LENGTH) sClient.vector_Data.clear();//превысили размер команды  
 }
}
//----------------------------------------------------------------------------------------------------
//обработка команды получения CRC клиентской программы
//----------------------------------------------------------------------------------------------------
void CThreadServerUnit::ExecuteCommand_GetClientProgrammCRC(SClient& sClient,SERVER_COMMAND command,bool &on_exit)
{
 cTransceiver_File.SendCRCClientProgrammFileToClient(sClient,command,CLIENT_PROGRAMM_FILE_NAME,cEvent_Exit,on_exit);
}
//----------------------------------------------------------------------------------------------------
//обработка команды получения файлов клиентской программы и загрузчика
//----------------------------------------------------------------------------------------------------
void CThreadServerUnit::ExecuteCommand_GetClientProgrammAndLoader(SClient& sClient,SERVER_COMMAND command,bool &on_exit)
{
 cTransceiver_File.SendClientProgrammAndLoaderFileToClient(sClient,command,CLIENT_PROGRAMM_FILE_NAME,LOADER_FILE_NAME,cEvent_Exit,on_exit);
}


//----------------------------------------------------------------------------------------------------
//обработка команды авторизации
//----------------------------------------------------------------------------------------------------
void CThreadServerUnit::ExecuteCommand_Autorization(SClient& sClient,SERVER_COMMAND command,bool &on_exit)
{
 cTransceiver_Autorization.ExecuteAutorization(cDocument_Main_Ptr,sClient,command,cEvent_Exit,on_exit);
}
//----------------------------------------------------------------------------------------------------
//обработка команды получения данных пользователей
//----------------------------------------------------------------------------------------------------
void CThreadServerUnit::ExecuteCommand_GetUserBook(SClient& sClient,SERVER_COMMAND command,bool &on_exit)
{
 cTransceiver_User.SendUserBook(cDocument_Main_Ptr,sClient,command,cEvent_Exit,on_exit);
}

//----------------------------------------------------------------------------------------------------
//обработка команды получения данных заданий
//----------------------------------------------------------------------------------------------------
void CThreadServerUnit::ExecuteCommand_GetTaskBook(SClient& sClient,SERVER_COMMAND command,bool &on_exit)
{
 cTransceiver_Task.SendTaskBook(cDocument_Main_Ptr,sClient,command,cEvent_Exit,on_exit);
}

//----------------------------------------------------------------------------------------------------
//обработка команды получения данных проектов
//----------------------------------------------------------------------------------------------------
void CThreadServerUnit::ExecuteCommand_GetProjectBook(SClient& sClient,SERVER_COMMAND command,bool &on_exit)
{
 cTransceiver_Project.SendProjectBook(cDocument_Main_Ptr,sClient,command,cEvent_Exit,on_exit);
}

//----------------------------------------------------------------------------------------------------
//обработка команды добавления нового задания
//----------------------------------------------------------------------------------------------------
void CThreadServerUnit::ExecuteCommand_AddedTask(SClient& sClient,SERVER_COMMAND command,bool &on_exit)
{
 cTransceiver_Task.AddTask(cDocument_Main_Ptr,sClient,command,cEvent_Exit,on_exit);
}

//----------------------------------------------------------------------------------------------------
//обработка команды удаления задания
//----------------------------------------------------------------------------------------------------
void CThreadServerUnit::ExecuteCommand_DeletedTask(SClient& sClient,SERVER_COMMAND command,bool &on_exit)
{
 cTransceiver_Task.DeleteTask(cDocument_Main_Ptr,sClient,command,cEvent_Exit,on_exit);
}

//----------------------------------------------------------------------------------------------------
//обработка команды изменения задания
//----------------------------------------------------------------------------------------------------
void CThreadServerUnit::ExecuteCommand_ChangedTask(SClient& sClient,SERVER_COMMAND command,bool &on_exit)
{
 cTransceiver_Task.ChangeTask(cDocument_Main_Ptr,sClient,command,cEvent_Exit,on_exit);
}

//----------------------------------------------------------------------------------------------------
//обработка команды добавления нового проекта
//----------------------------------------------------------------------------------------------------
void CThreadServerUnit::ExecuteCommand_AddedProject(SClient& sClient,SERVER_COMMAND command,bool &on_exit)
{
 cTransceiver_Project.AddProject(cDocument_Main_Ptr,sClient,command,cEvent_Exit,on_exit);
}

//----------------------------------------------------------------------------------------------------
//обработка команды удаления проекта
//----------------------------------------------------------------------------------------------------
void CThreadServerUnit::ExecuteCommand_DeletedProject(SClient& sClient,SERVER_COMMAND command,bool &on_exit)
{
 cTransceiver_Project.DeleteProject(cDocument_Main_Ptr,sClient,command,cEvent_Exit,on_exit);
}

//----------------------------------------------------------------------------------------------------
//обработка команды изменения проекта
//----------------------------------------------------------------------------------------------------
void CThreadServerUnit::ExecuteCommand_ChangedProject(SClient& sClient,SERVER_COMMAND command,bool &on_exit)
{
 cTransceiver_Project.ChangeProject(cDocument_Main_Ptr,sClient,command,cEvent_Exit,on_exit);
}