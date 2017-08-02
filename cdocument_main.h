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

//====================================================================================================
//подключаемые библиотеки
//====================================================================================================

#include <list>
#include <CString>
#include "stdafx.h"
#include "craiiccriticalsection.h"
#include "cuserdatabaseengine_software.h"
#include "ctaskdatabaseengine_software.h"
#include "cprojectdatabaseengine_software.h"

using namespace std;

//====================================================================================================
//структуры
//====================================================================================================

//настройки сервера
struct SServerSettings
{
 unsigned short Port;//порт сервера
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
  } sProtectedVariables;
 public:
  //-Конструктор класса------------------------------------------------------
  CDocument_Main(void);
  //-Деструктор класса-------------------------------------------------------
  ~CDocument_Main();
  //-Функции класса----------------------------------------------------------
  void SaveState(void);//сохранение состояния
  bool AddUser(SUser& sUser);//добавить пользователя
  bool ChangeUser(long index,const SUser& sUser);//задать пользователя
  bool GetUser(long index,SUser &sUser);//получить пользователя по индексу
  void DeleteUser(long index);//удалить пользователя по индексу
  list<SUser> GetAllUser(void);//получить список всех пользователей
  bool FindUserByLoginAndPassword(const CString& login,const CString& password,SUser& sUser);//найти пользователя по логину и паролю
  bool FindUserByGUID(const CString& guid,SUser& sUser);//найти пользователя по GUID
  bool FindTaskByGUID(const CString &guid,STask &sTask);//найти задание по GUID
  bool FindProjectByGUID(const CString &guid,SProject &sProject);//найти проект по GUID
  void GetServerSettings(SServerSettings &sServerSettings);//получить настройки сервера
  void SetServerSettings(const SServerSettings &sServerSettings);//установить настройки сервера (сервер будет перезапущен)
  list<STask> GetAllTaskForUserGUID(const CString &guid);//получить все задания для и от пользователя с заданным GUID
  list<STask> GetAllTask(void);//получить все задания
  list<SProject> GetAllProject(void);//получить все проекты

  bool AddTask(STask &sTask);//добавить задание
  bool DeleteTask(const STask &sTask);//удалить задание
  bool ChangeTask(const STask &sTask);//изменить задание

  bool AddProject(SProject &sProject);//добавить проект
  bool DeleteProject(const SProject &sProject);//удалить проект
  bool ChangeProject(const SProject &sProject);//изменить проект
 protected:
  //-Функции класса----------------------------------------------------------  
  bool CreateGUID(CString &cString_GUID);//создать GUID
  //-Прочее------------------------------------------------------------------
  DECLARE_DYNCREATE(CDocument_Main) 
};

#endif