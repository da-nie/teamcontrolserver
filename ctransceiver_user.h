#ifndef CTRANSCEIVER_USER_H
#define CTRANSCEIVER_USER_H

//====================================================================================================
//описание
//====================================================================================================

//Класс приёмо-передатчика данных пользователей

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
//класс приёмо-передатчика данных пользователей
//====================================================================================================

class CTransceiver_User:public CTransceiver
{
 protected:
  unsigned long Version;//номер версии структуры
 public:  
  //конструктор
  CTransceiver_User(void);
  //деструктор
  ~CTransceiver_User();
  //функции класса
  void SendUserDataToClient(const SClient &sClient,const CUser &cUser,CEvent &cEvent_Exit,bool &on_exit);//передать клиенту данные пользователя
  void SendUserDataToClientInPackage(const SClient &sClient,const CUser &cUser,SERVER_ANSWER answer,SERVER_COMMAND command,CEvent &cEvent_Exit,bool &on_exit);//передать клиенту данные пользователя в виде полного пакета
  void SendUserBook(CDocument_Main *cDocument_Main_Ptr,SClient& sClient,SERVER_COMMAND command,CEvent &cEvent_Exit,bool &on_exit);//передача базы данных пользователей

};

#endif
