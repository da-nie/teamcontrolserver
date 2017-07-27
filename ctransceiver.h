#ifndef CTRANSCEIVER_H
#define CTRANSCEIVER_H

//====================================================================================================
//описание
//====================================================================================================

//Класс приёмо-передатчика

//====================================================================================================
//подключаемые библиотеки
//====================================================================================================
#include "stdafx.h"
#include <list>
#include <vector>
#include <string>
#include "cdocument_main.h"
#include "protocol.h"

using namespace std;

//====================================================================================================
//структуры
//====================================================================================================

//клиент
struct SClient
{
 SOCKET Socket;//сокет клиента
 CString Login;//логин
 CString Password;//пароль
 CString UserGUID;//назначенный по результатам активации по логину и паролю GUID
 bool StuffingEnabled;//включён ли байтстаффинг
 vector<unsigned char> vector_Data;//принятые данные
};

//====================================================================================================
//класс приёмо-передатчика
//====================================================================================================

class CTransceiver
{
 protected:
 public:  
  //конструктор
  CTransceiver(void);
  //деструктор
  ~CTransceiver();
 protected:
  void SetString(CString &text,char *buffer,long length);//задать строку по массиву
  void SendBeginPackage(SOCKET socket_client,CEvent &cEvent_Exit,bool &on_exit);//отправить код начала пакета
  void SendEndPackage(SOCKET socket_client,CEvent &cEvent_Exit,bool &on_exit);//отправить код конца пакета
  void SendAnswer(SOCKET socket_client,SERVER_ANSWER answer,SERVER_COMMAND command,const char *data,unsigned long size,CEvent &cEvent_Exit,bool &on_exit);//отправить ответ клиенту
  void SendPart(SOCKET socket_client,const char *data,unsigned long size,CEvent &cEvent_Exit,bool &on_exit);//отправить данные клиенту с выполнением байтстаффинга
  void SendData(SOCKET socket_client,const char *package,long size,CEvent &cEvent_Exit,bool &on_exit);//отправить данные клиенту
  void Pause(long time_ms);//пауза

};

#endif



