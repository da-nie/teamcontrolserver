#ifndef THREAD_SERVER_H
#define THREAD_SERVER_H

//====================================================================================================
//описание
//====================================================================================================

//Класс серверного потока

//====================================================================================================
//подключаемые библиотеки
//====================================================================================================
#include "stdafx.h"
#include <list>
#include <vector>
#include <string>
#include "cdocument_main.h"
#include "protocol.h"
#include "cthreadserverunit.h"
#include "craiiccriticalsection.h"

using namespace std;

//====================================================================================================
//класс серверного потока
//====================================================================================================

class CThreadServer
{
 //дружественные функции
 friend UINT ThreadServer(LPVOID pParam);//функция потока
 protected:
  CEvent cEvent_Exit;//требование на выход из потока
  CWinThread *cWinThread_Thread;//поток обработки
  list<CThreadServerUnit*> list_CThreadServerUnitPtr;//список указателей на классы обработки клиентов
  CDocument_Main *cDocument_Main_Ptr;//указатель на документ
 public:  
  //конструктор
  CThreadServer(void);
  //деструктор
  ~CThreadServer();
   void Start(void);//запустить поток
   void Stop(void);//остановить поток
   void SetDocument(CDocument_Main *cDocument_Main_Set_Ptr);//задать класс документа
   void OnUserAdded(const SUser &sUser);//был добавлен пользователь
   void OnUserDeleted(const SUser &sUser);//был удалён пользователь
   void OnUserChanged(const SUser &sUser);//были изменены данные пользователя

   void OnTaskAdded(const STask &sTask);//было добавлено задание
   void OnTaskDeleted(const STask &sTask);//было удалено задания
   void OnTaskChanged(const STask &sTask);//были изменены данные задания

   void OnProjectAdded(const SProject &sProject);//был добавлен проект
   void OnProjectDeleted(const SProject &sProject);//был удален проект
   void OnProjectChanged(const SProject &sProject);//были изменены данные проекта

 protected:
  SOCKET CreateServer(void);//создание сервера
  void DisconnectAllClientAndResetClientList(void);//отключение всех клиентов и очистка списка всех клиентов
  void NewClient(SOCKET socket_server);//подключение нового клиента
  void Processing(void);//основной цикл обработки
};

#endif



