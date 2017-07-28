#ifndef CRECORDSET_TASKLIST_H
#define CRECORDSET_TASKLIST_H

//====================================================================================================
//описание
//====================================================================================================

// ласс записи информации о задачах

//====================================================================================================
//подключаемые библиотеки
//====================================================================================================
#include "stdafx.h"
#include <string>
#include "protocol.h"
using namespace std;

//====================================================================================================
//структуры
//====================================================================================================

struct STask
{
 long Index;//индекс задани€
 CString FromUserGUID;//от пользовател€
 CString ForUserGUID;//дл€ пользовател€
 CString ProjectGUID;//по проекту
 CString Task;//задание
 long State;//состо€ние
 long Year;//год
 long Month;//мес€ц
 long Day;//число
 CString TaskGUID;//идентификатор задани€
};

//====================================================================================================
//класс записи информации о задачах
//====================================================================================================
class CRecordset_TaskList:public CRecordset
{
 protected:
  //-ѕеременные класса-------------------------------------------------------
  long Index;//индекс задани€
  CString FromUserGUID;//от кого
  CString ForUserGUID;//дл€ кого
  CString ProjectGUID;//по проекту
  CString Task;//задание
  long State;//состо€ние задани€
  long Year;//год
  long Month;//мес€ц
  long Day;//число
  CString TaskGUID;//идентификатор задани€  
 public:
  //- онструктор класса------------------------------------------------------
  CRecordset_TaskList(CDatabase* pDatabase=NULL);
  //-ƒеструктор класса-------------------------------------------------------
  ~CRecordset_TaskList();
  //-‘ункции класса----------------------------------------------------------
  void SetRecord(const STask& sTask);//задать запись
  void GetRecord(STask& sTask);//получить запись
  virtual CString GetDefaultConnect(void);
  virtual CString GetDefaultSQL(void);
  virtual void DoFieldExchange(CFieldExchange* pFX);
};

//====================================================================================================
//класс RAII (Resource Acquisition is Initialization) дл€ работы с CRecordset_TaskList
//====================================================================================================

class CRAIICRecordset_TaskList
{
 private:
  CRAIICRecordset_TaskList (const CRAIICRecordset_TaskList& cRAIICRecordset_TaskList);
  CRAIICRecordset_TaskList& operator=(const CRAIICRecordset_TaskList& cRAIICRecordset_TaskList);
  CRecordset_TaskList cRecordset_TaskList;//экземпл€р объекта
  bool Ok;//результат захвата объекта
 public:
  CRAIICRecordset_TaskList(CDatabase *cDatabase_Ptr,const CString &table_name);//конструктор
  ~CRAIICRecordset_TaskList();//деструктор
  bool IsOk(void);//проверка, произошЄл ли захват объекта
  CRecordset_TaskList& GetMainObject(void);//получение оригинального объекта
};

#endif