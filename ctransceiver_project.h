#ifndef CTRANSCEIVER_PROJECT_H
#define CTRANSCEIVER_PROJECT_H

//====================================================================================================
//описание
//====================================================================================================

//Класс приёмо-передатчика данных проектов

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
//класс приёмо-передатчика данных проектов
//====================================================================================================

class CTransceiver_Project:public CTransceiver
{
 protected:
 public:  
  //конструктор
  CTransceiver_Project(void);
  //деструктор
  ~CTransceiver_Project();
  //функции класса
  void SendProjectDataToClient(SClient &sClient,const CProject &cProject,CEvent &cEvent_Exit,bool &on_exit);//передать клиенту данные проекта
  void SendProjectDataToClientInPackage(SClient &sClient,const CProject &cProject,SERVER_ANSWER answer,SERVER_COMMAND command,CEvent &cEvent_Exit,bool &on_exit);//передать клиенту данные проекта в виде полного пакета
  bool ReadCProjectInArray(char *ptr,size_t &offset,size_t max_length,CProject &cProject);//получение проекта из массива принятых данных
  void SendProjectBook(CDocument_Main *cDocument_Main_Ptr,SClient& sClient,SERVER_COMMAND command,CEvent &cEvent_Exit,bool &on_exit);//передача базы данных проектов
  void AddProject(CDocument_Main *cDocument_Main_Ptr,SClient& sClient,SERVER_COMMAND command,CEvent &cEvent_Exit,bool &on_exit);//добавление нового проекта
  void DeleteProject(CDocument_Main *cDocument_Main_Ptr,SClient& sClient,SERVER_COMMAND command,CEvent &cEvent_Exit,bool &on_exit);//удаление проекта
  void ChangeProject(CDocument_Main *cDocument_Main_Ptr,SClient& sClient,SERVER_COMMAND command,CEvent &cEvent_Exit,bool &on_exit);//изменение проекта
};

#endif
