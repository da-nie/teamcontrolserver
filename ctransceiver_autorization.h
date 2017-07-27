#ifndef CTRANSCEIVER_AUTORIZATION_H
#define CTRANSCEIVER_AUTORIZATION_H

//====================================================================================================
//описание
//====================================================================================================

//Класс приёмо-передатчика данных авторизации

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
//класс приёмо-передатчика данных авторизации
//====================================================================================================

class CTransceiver_Autorization:public CTransceiver
{
 protected:
 public:  
  //конструктор
  CTransceiver_Autorization(void);
  //деструктор
  ~CTransceiver_Autorization();
  //функции класса
  void ExecuteAutorization(CDocument_Main *cDocument_Main_Ptr,SClient& sClient,SERVER_COMMAND command,CEvent &cEvent_Exit,bool &on_exit);//обработка команды авторизации

};

#endif
