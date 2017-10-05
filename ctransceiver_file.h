#ifndef CTRANSCEIVER_FILE_H
#define CTRANSCEIVER_FILE_H

//====================================================================================================
//описание
//====================================================================================================

//Класс приёмо-передатчика файлов

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
#include "crc.h"

using namespace std;

//====================================================================================================
//класс приёмо-передатчика файлов
//====================================================================================================

class CTransceiver_File:public CTransceiver
{
 protected:
 public:  
  //конструктор
  CTransceiver_File(void);
  //деструктор
  ~CTransceiver_File();
  //функции класса
  void SendCRCClientProgrammFileToClient(SClient &sClient,const SCRC &sCRC,SERVER_COMMAND command,const CSafeString &programm_file_name,const CSafeString &loader_file_name,CEvent &cEvent_Exit,bool &on_exit);//передать клиенту контрольную сумму файла клиентской программы
  void SendClientProgrammAndLoaderFileToClient(SClient &sClient,const SCRC &sCRC,SERVER_COMMAND command,const CSafeString &programm_file_name,const CSafeString &loader_file_name,CEvent &cEvent_Exit,bool &on_exit);//передать клиенту файл
 protected:
};

#endif
