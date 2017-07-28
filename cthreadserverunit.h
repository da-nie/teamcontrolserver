#ifndef THREAD_SERVER_UNIT_H
#define THREAD_SERVER_UNIT_H

//====================================================================================================
//описание
//====================================================================================================

//Класс серверного потока для одного клиента

//====================================================================================================
//подключаемые библиотеки
//====================================================================================================
#include "stdafx.h"
#include <list>
#include <vector>
#include <string>
#include "cdocument_main.h"
#include "protocol.h"
#include "craiiccriticalsection.h"
#include "ctransceiver_user.h"
#include "ctransceiver_task.h"
#include "ctransceiver_project.h"
#include "ctransceiver_autorization.h"
#include "ctransceiver_file.h"

using namespace std;

//====================================================================================================
//класс серверного потока для одного клиента
//====================================================================================================

class CThreadServerUnit
{
 //дружественные функции
 friend UINT ThreadServerUnit(LPVOID pParam);//функция потока
 protected:
  CEvent cEvent_Exit;//требование на выход из потока
  CWinThread *cWinThread_Thread;//поток обработки
  char *Buffer;//буфер приёма/передачи
  SClient sClient_My;//клиент
  CDocument_Main *cDocument_Main_Ptr;//указатель на документ
  
  //защищённые переменные класса
  struct SProtectedVariables
  {
   //данные для синхронизации с клиентами
   vector<SUser> vector_SUser_Deleted;//удалённые пользователи
   vector<SUser> vector_SUser_Changed;//изменённые пользователи
   vector<SUser> vector_SUser_Added;//добавленные пользователи

   vector<STask> vector_STask_Deleted;//удалённые задания
   vector<STask> vector_STask_Changed;//изменённые задания 
   vector<STask> vector_STask_Added;//добавленные задания

   vector<SProject> vector_SProject_Deleted;//удалённые проекты
   vector<SProject> vector_SProject_Changed;//изменённые проекты
   vector<SProject> vector_SProject_Added;//добавленные проекты

   bool ClientOnLine;//подключён ли клиент (работает ли поток)

   CCriticalSection cCriticalSection;//критическая секция для доступа
  } sProtectedVariables;

  CTransceiver_User cTransceiver_User;//класс приёмо-передатчика данных пользователя
  CTransceiver_Task cTransceiver_Task;//класс приёмо-передатчика данных задания
  CTransceiver_Project cTransceiver_Project;//класс приёмо-передатчика данных проектов
  CTransceiver_File cTransceiver_File;//класс приёмо-передатчика файлов программы
  CTransceiver_Autorization cTransceiver_Autorization;//класс приёмо-передатчика данных авторизации
 public:  
  //конструктор
  CThreadServerUnit(void);
  //деструктор
  ~CThreadServerUnit();
   void Start(SOCKET socket_client);//запустить поток
   void Stop(void);//остановить поток
   void SetDocument(CDocument_Main *cDocument_Main_Set_Ptr);//задать класс документа
   void InitClient(SOCKET socket_client);//инициализировать клиента
   void OnUserAdded(const SUser &sUser);//был добавлен пользователь
   void OnUserDeleted(const SUser &sUser);//был удалён пользователь
   void OnUserChanged(const SUser &sUser);//были изменены данные пользователя

   void OnTaskAdded(const STask &sTask);//было добавлено задание
   void OnTaskDeleted(const STask &sTask);//было удалено задания
   void OnTaskChanged(const STask &sTask);//были изменены данные задания

   void OnProjectAdded(const SProject &sProject);//был добавлен проект
   void OnProjectDeleted(const SProject &sProject);//был удален проект
   void OnProjectChanged(const SProject &sProject);//были изменены данные задания

   bool GetClientOnLine(void);//получить, в сети ли клиент   
 protected:
  void ClientDisconnect(const SClient& sClient);//отключение от клиента
  void SetClientOnLine(bool state);//задать, в сети ли клиент

  void Processing(void);//основной цикл обработки
  void LinkProcessing(SClient &sClient,bool &on_exit);//обработка связи

  void NewDataFromClient(SClient& sClient,char *data,unsigned long length,bool &on_exit);//приняты данные от клиента
  void ExecuteCommand_GetClientProgrammCRC(SClient& sClient,SERVER_COMMAND command,bool &on_exit);//обработка команды получения CRC клиентской программы
  void ExecuteCommand_GetClientProgrammAndLoader(SClient& sClient,SERVER_COMMAND command,bool &on_exit);//обработка команды получения файлов клиентской программы и загрузчика
  void ExecuteCommand_Autorization(SClient& sClient,SERVER_COMMAND command,bool &on_exit);//обработка команды авторизации
  void ExecuteCommand_GetUserBook(SClient& sClient,SERVER_COMMAND command,bool &on_exit);//обработка команды получения данных пользователей
  void ExecuteCommand_GetTaskBook(SClient& sClient,SERVER_COMMAND command,bool &on_exit);//обработка команды получения заданий пользователя
  void ExecuteCommand_GetProjectBook(SClient& sClient,SERVER_COMMAND command,bool &on_exit);//обработка команды получения проектов
  void ExecuteCommand_AddedTask(SClient& sClient,SERVER_COMMAND command,bool &on_exit);//обработка команды добавления нового задания
  void ExecuteCommand_DeletedTask(SClient& sClient,SERVER_COMMAND command,bool &on_exit);//обработка команды удаления задания
  void ExecuteCommand_ChangedTask(SClient& sClient,SERVER_COMMAND command,bool &on_exit);//обработка команды изменения задания
  void ExecuteCommand_AddedProject(SClient& sClient,SERVER_COMMAND command,bool &on_exit);//обработка команды добавления нового проекта
  void ExecuteCommand_DeletedProject(SClient& sClient,SERVER_COMMAND command,bool &on_exit);//обработка команды удаления проекта
  void ExecuteCommand_ChangedProject(SClient& sClient,SERVER_COMMAND command,bool &on_exit);//обработка команды изменения проекта
};

#endif



