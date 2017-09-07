#ifndef CDOCUMENT_MAIN_H
#define CDOCUMENT_MAIN_H

//====================================================================================================
//описание
//====================================================================================================

//Класс документов для списка пользователей.

//ВНИМАНИЕ!!!
//База данных должна быть создана именно как Microsoft Paradox Driver (*.db ) и ни как иначе!
//Просто база с типом Paradox работать не будет - будут ошибки доступа!!!
//Обязательно наличие ключа - без него не работаю многие операции с базой.
//Имя файла и таблицы должно быть не боле 8 символов (если в системе не установлена BDE).
//Иначе чудесный драйвер Microsoft не может эту базу открыть. GetShortPathName должна бы помочь для
//длинных имён, но пока не удалось с её помощью подключиться к базе.
//В CRecordset RFX нужно делать строго в порядке следования полей в базе! Иначе в SQL будут ошибки в запросах.
//Никогда нельзя задавать значение автоинкреметной переменной! Просто не менять её в методе SetRecord.
//Автоинкрементная переменная не должна быть ограничена функцией обмена DoFieldExchange - т.е. там её нельзя задавать!

//====================================================================================================
//подключаемые библиотеки
//====================================================================================================

#include <list>
#include "stdafx.h"
#include "craiiccriticalsection.h"
#include "cuserdatabaseengine_software.h"
#include "ctaskdatabaseengine_software.h"
#include "cprojectdatabaseengine_software.h"

#include "cuserdatabaseengine_sql.h"
#include "ctaskdatabaseengine_sql.h"
#include "cprojectdatabaseengine_sql.h"

#include "ctaskexporthtml.h"
#include "ctaskexporttxt.h"

using namespace std;

//====================================================================================================
//структуры
//====================================================================================================

//настройки сервера
struct SServerSettings
{
 unsigned short Port;//порт сервера
};

//подключённые пользователи
struct SConnected
{
 CSafeString GUID;//идентификатор подключённого пользователя
 bool Connected;//состояние
};

//====================================================================================================
//класс документов
//====================================================================================================

class CDocument_Main:public CDocument 
{
 protected:
  //-Переменные класса-------------------------------------------------------
  //защищённые переменные класса
  struct SProtectedVariables
  {
   CIUserDatabaseEngine *cIUserDatabaseEngine_Ptr;//указатель на класс работы с базой данных пользователей
   CIProjectDatabaseEngine *cIProjectDatabaseEngine_Ptr;//указатель на класс работы с базой данных проектов
   CITaskDatabaseEngine *cITaskDatabaseEngine_Ptr;//указатель на класс работы с базой данных заданий

   SServerSettings sServerSettings;//настройки сервера
   CCriticalSection cCriticalSection;//критическая секция для доступа к классу

   list <SConnected> list_SConnected;//подключённые пользователи
   bool ChangeConnectedList;//изменился ли список подключённх пользователей
  } sProtectedVariables;

  CITaskExport *cITaskExport_Ptr;//указатель на класс экспорта заданий
 public:
  //-Конструктор класса------------------------------------------------------
  CDocument_Main(void);
  //-Деструктор класса-------------------------------------------------------
  ~CDocument_Main();
  //-Функции класса----------------------------------------------------------
  list<SConnected> GetConnectedList(void);//получить список подключённых пользователей
  bool GetChangeConnectedListAndResetState(void);//получить, изменился ли список подключённых пользователей и сбросить значение
  void SaveState(void);//сохранение состояния
  bool AddUser(CUser& cUser);//добавить пользователя
  bool ChangeUser(long index,const CUser& cUser);//задать пользователя
  bool GetUser(long index,CUser &cUser);//получить пользователя по индексу
  void DeleteUser(long index);//удалить пользователя по индексу
  list<CUser> GetAllUser(void);//получить список всех пользователей
  bool FindUserByLoginAndPassword(const CSafeString& login,const CSafeString& password,CUser& cUser);//найти пользователя по логину и паролю
  bool FindUserByGUID(const CSafeString& guid,CUser& cUser);//найти пользователя по GUID
  bool ChangeUserByGUID(const CSafeString& guid,const CUser& cUser);//изменить пользователя по GUID
  bool DeleteUserByGUID(const CSafeString& guid);//удалить пользователя по GUID
  bool FindTaskByGUID(const CSafeString &guid,CTask &cTask);//найти задание по GUID
  bool FindProjectByGUID(const CSafeString &guid,CProject &cProject);//найти проект по GUID
  void GetServerSettings(SServerSettings &sServerSettings);//получить настройки сервера
  void SetServerSettings(const SServerSettings &sServerSettings);//установить настройки сервера (сервер будет перезапущен)
  list<CTask> GetAllTaskForUserGUID(const CSafeString &guid);//получить все задания для и от пользователя с заданным GUID
  list<CTask> GetAllTask(void);//получить все задания
  list<CProject> GetAllProject(void);//получить все проекты

  bool AddTask(CTask &cTask);//добавить задание
  bool DeleteTask(const CTask &cTask);//удалить задание
  bool ChangeTask(const CTask &cTask);//изменить задание

  bool AddProject(CProject &cProject);//добавить проект
  bool DeleteProject(const CProject &cProject);//удалить проект
  bool ChangeProject(const CProject &cProject);//изменить проект

  void ExportTaskBase(const CString &file_name);//экспорт базы заданий

  void ResetUserListBase(void);//удаление базы пользователей
  void ResetTaskListBase(void);//удаление базы заданий
  void ResetProjectListBase(void);//удаление базы проектов

  void SetUserConnected(const CSafeString& guid,bool connected);//задать, в сети ли пользователь

  void SendPing(void);//отправить пользователям сигнал проверки связи
 protected:
  //-Функции класса----------------------------------------------------------  
  bool CreateGUID(CSafeString &cSafeString_GUID);//создать GUID
  //-Прочее------------------------------------------------------------------
  DECLARE_DYNCREATE(CDocument_Main) 
};

#endif