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
  std::list<CThreadServerUnit*> list_CThreadServerUnitPtr;//список указателей на классы обработки клиентов
  CDocument_Main *cDocument_Main_Ptr;//указатель на документ
 public:  
  //конструктор
  CThreadServer(void);
  //деструктор
  ~CThreadServer();
   void Start(void);//запустить поток
   void Stop(void);//остановить поток
   void SetDocument(CDocument_Main *cDocument_Main_Set_Ptr);//задать класс документа
   void OnUserAdded(const CUser &cUser);//был добавлен пользователь
   void OnUserDeleted(const CUser &cUser);//был удалён пользователь
   void OnUserChanged(const CUser &cUser);//были изменены данные пользователя

   void OnTaskAdded(const CTask &cTask);//было добавлено задание
   void OnTaskDeleted(const CTask &cTask);//было удалено задания
   void OnTaskChanged(const CTask &cTask);//были изменены данные задания

   void OnProjectAdded(const CProject &cProject);//был добавлен проект
   void OnProjectDeleted(const CProject &cProject);//был удален проект
   void OnProjectChanged(const CProject &cProject);//были изменены данные проекта

   void SendPing(void);//отправка пользователям сообщения для проверки связи

 protected:
  SOCKET CreateServer(void);//создание сервера
  void DisconnectAllClientAndResetClientList(void);//отключение всех клиентов и очистка списка всех клиентов
  void NewClient(SOCKET socket_server);//подключение нового клиента
  void Processing(void);//основной цикл обработки
};

#endif



