#ifndef CTRANSCEIVER_TASK_H
#define CTRANSCEIVER_TASK_H

//====================================================================================================
//описание
//====================================================================================================

//Класс приёмо-передатчика данных заданий

//====================================================================================================
//подключаемые библиотеки
//====================================================================================================
#include "stdafx.h"
#include <list>
#include <vector>
#include <string>
#include "cdocument_main.h"
#include "protocol.h"
#include "ctransceiver.h"

using namespace std;

//====================================================================================================
//класс приёмо-передатчика данных заданий
//====================================================================================================

class CTransceiver_Task:public CTransceiver
{
 protected:
  unsigned long Version;//номер версии структуры
 public:  
  //конструктор
  CTransceiver_Task(void);
  //деструктор
  ~CTransceiver_Task();
  //функции класса
  void SendTaskDataToClient(SClient &sClient,const CTask &cTask,CEvent &cEvent_Exit,bool &on_exit);//передать клиенту данные задания
  void SendTaskDataToClientInPackage(SClient &sClient,const CTask &cTask,SERVER_ANSWER answer,SERVER_COMMAND command,CEvent &cEvent_Exit,bool &on_exit);//передать клиенту данные задания в виде полного пакета
  bool ReadCTaskInArray(char *ptr,size_t &offset,size_t max_length,CTask &cTask);//получение задания из массива принятых данных
  void SendTaskBook(CDocument_Main *cDocument_Main_Ptr,SClient& sClient,SERVER_COMMAND command,CEvent &cEvent_Exit,bool &on_exit);//передача базы данных заданий
  void SendCommonTaskBook(CDocument_Main *cDocument_Main_Ptr,SClient& sClient,SERVER_COMMAND command,CEvent &cEvent_Exit,bool &on_exit);//передача базы данных общих заданий
  void AddTask(CDocument_Main *cDocument_Main_Ptr,SClient& sClient,SERVER_COMMAND command,CEvent &cEvent_Exit,bool &on_exit);//добавление нового задания
  void DeleteTask(CDocument_Main *cDocument_Main_Ptr,SClient& sClient,SERVER_COMMAND command,CEvent &cEvent_Exit,bool &on_exit);//удаление задания
  void ChangeTask(CDocument_Main *cDocument_Main_Ptr,SClient& sClient,SERVER_COMMAND command,CEvent &cEvent_Exit,bool &on_exit);//изменение задания
 protected:
  void SendTaskList(list<CTask> &list_CTask,SClient& sClient,SERVER_COMMAND command,SERVER_ANSWER answer,CEvent &cEvent_Exit,bool &on_exit);//передача списка заданий
};

#endif
