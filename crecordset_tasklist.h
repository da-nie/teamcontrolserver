#ifndef CRECORDSET_TASKLIST_H
#define CRECORDSET_TASKLIST_H

//====================================================================================================
//описание
//====================================================================================================

//Класс записи информации о задачах

//====================================================================================================
//подключаемые библиотеки
//====================================================================================================
#include "stdafx.h"
#include <string>
#include "protocol.h"
#include "ctask.h"
using namespace std;

//====================================================================================================
//макроопределения
//====================================================================================================

//ответ на задание не прочитан
#define TASK_RECORD_PROPERTYS_MASK_ANSWER_NOT_READ (1<<0)
//задание является плановой позицией
#define TASK_RECORD_PROPERTYS_MASK_PLANNED_POSITION (1<<1)

//====================================================================================================
//класс записи информации о задачах
//====================================================================================================
class CRecordset_TaskList:public CRecordset
{
 protected:
  //-Переменные класса-------------------------------------------------------
  long Index;//индекс задания
  CString FromUserGUID;//от кого
  CString ForUserGUID;//для кого
  CString ProjectGUID;//по проекту
  CString Task;//задание
  long State;//состояние задания
  long Year;//год
  long Month;//месяц
  long Day;//число
  long Propertys;//свойства задания
  CString TaskGUID;//идентификатор задания 
  CString Answer;//ответ
 public:
  //-Конструктор класса------------------------------------------------------
  CRecordset_TaskList(CDatabase* pDatabase=NULL);
  //-Деструктор класса-------------------------------------------------------
  ~CRecordset_TaskList();
  //-Функции класса----------------------------------------------------------
  void SetRecord(const CTask& cTask);//задать запись
  void GetRecord(CTask& cTask);//получить запись
  virtual CString GetDefaultConnect(void);
  virtual CString GetDefaultSQL(void);
  virtual void DoFieldExchange(CFieldExchange* pFX);
};

//====================================================================================================
//класс RAII (Resource Acquisition is Initialization) для работы с CRecordset_TaskList
//====================================================================================================

class CRAIICRecordset_TaskList
{
 private:
  CRAIICRecordset_TaskList (const CRAIICRecordset_TaskList& cRAIICRecordset_TaskList);
  CRAIICRecordset_TaskList& operator=(const CRAIICRecordset_TaskList& cRAIICRecordset_TaskList);
  CRecordset_TaskList cRecordset_TaskList;//экземпляр объекта
  bool Ok;//результат захвата объекта
 public:
  CRAIICRecordset_TaskList(CDatabase *cDatabase_Ptr,const CString &table_name);//конструктор
  ~CRAIICRecordset_TaskList();//деструктор
  bool IsOk(void);//проверка, произошёл ли захват объекта
  CRecordset_TaskList& GetMainObject(void);//получение оригинального объекта
};

#endif