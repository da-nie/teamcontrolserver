#ifndef CITASKDATABASE_ENGINE_H
#define CITASKDATABASE_ENGINE_H

//====================================================================================================
//описание
//====================================================================================================

//Абстрактный класс работы с базой данных заданий

//====================================================================================================
//подключаемые библиотеки
//====================================================================================================

#include <list>
#include "stdafx.h"
#include "craiicdatabase.h"
#include "craiicrecordset.h"
#include "crecordset_tasklist.h"

using namespace std;

//====================================================================================================
//структуры
//====================================================================================================

//====================================================================================================
//абстрактный класс работы с базой данных заданий
//====================================================================================================

class CITaskDatabaseEngine
{
 public:
  //-Деструктор класса-------------------------------------------------------
  virtual ~CITaskDatabaseEngine() {};
  //-Функции класса----------------------------------------------------------
  virtual bool FindTaskByGUID(const CSafeString &guid,CTask &cTask)=0;//найти задание по GUID
  virtual list<CTask> GetAllTaskForUserGUID(const CSafeString &guid)=0;//получить все задания для и от пользователя с заданным GUID
  virtual list<CTask> GetAllTask(void)=0;//получить все задания
  virtual list<CTask> GetCommonTask(void)=0;//получить все общие задания
  virtual bool AddTask(CTask &cTask)=0;//добавить задание
  virtual bool DeleteTask(const CTask &cTask)=0;//удалить задание
  virtual bool ChangeTask(const CTask &cTask,bool &for_user_change,bool &common_change,CTask &cTask_Deleted,CTask &cTask_Added)=0;//изменить задание
  virtual void ResetBase(void)=0;//очистить базу
};

#endif