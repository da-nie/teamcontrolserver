#ifndef CTASKDATABASE_ENGINE_SOFTWARE_H
#define CTASKDATABASE_ENGINE_SOFTWARE_H

//====================================================================================================
//описание
//====================================================================================================

//Класс работы с баой данных заданий с использованием программного механизма поиска записей

//====================================================================================================
//подключаемые библиотеки
//====================================================================================================

#include "citaskdatabaseengine.h"
#include <memory>



//====================================================================================================
//структуры
//====================================================================================================

//====================================================================================================
//класс работы с базой данных заданий с использованием программного механизма поиска записей
//====================================================================================================

class CTaskDatabaseEngine_Software:public CITaskDatabaseEngine
{
 protected:
  //-Переменные класса-------------------------------------------------------
  CString TaskListTableName;//имя таблицы базы данных списка задач  
  CString TaskListBaseInitString;//строка инициализации базы данных заданий
  CDatabase cDatabase_TaskList;//база данных списка задач

  std::shared_ptr<CRAIICDatabase> cRAIICDatabase_Ptr;//указатель на класс захвата базы данных
 public:
  //-Конструктор класса------------------------------------------------------
  CTaskDatabaseEngine_Software(const CString &task_list_table_name);
  //-Деструктор класса-------------------------------------------------------
  ~CTaskDatabaseEngine_Software();
  //-Функции класса----------------------------------------------------------
  bool FindTaskByGUID(const CSafeString &guid,CTask &cTask) override;//найти задание по GUID
  std::list<CTask> GetAllTaskForUserGUID(const CSafeString &guid) override;//получить все задания для и от пользователя с заданным GUID
  std::list<CTask> GetAllTask(void) override;//получить все задания
  std::list<CTask> GetCommonTask(void) override;//получить общие задания
  bool AddTask(CTask &cTask) override;//добавить задание
  bool DeleteTask(const CTask &cTask) override;//удалить задание
  bool ChangeTask(const CTask &cTask,bool &for_user_change,bool &common_change,CTask &cTask_Deleted,CTask &cTask_Added) override;//изменить задание
  void ResetBase(void) override;//очистить базу
};

#endif