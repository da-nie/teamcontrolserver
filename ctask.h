#ifndef CTASK_H
#define CTASK_H

//====================================================================================================
//описание
//====================================================================================================

//Класс задания

//====================================================================================================
//подключаемые библиотеки
//====================================================================================================
#include "stdafx.h"
#include "csafestring.h"
#include "cdate.h"
#include "protocol.h"

//====================================================================================================
//класс задания
//====================================================================================================
class CTask
{
 private:
  long Index;//индекс задания
  CSafeString FromUserGUID;//от пользователя
  CSafeString ForUserGUID;//для пользователя
  CSafeString ProjectGUID;//по проекту
  CSafeString Task;//задание
  long State;//состояние
  CDate cDate;//срок
  CSafeString TaskGUID;//идентификатор задания
  CSafeString Answer;//ответ на задание
  bool AnswerNotRead;//прочитан ли ответ на задание
  bool PlannedPosition;//плановая позиция
  //-Переменные класса-------------------------------------------------------
 public:
  //-Конструктор класса------------------------------------------------------
  CTask(void);
  //-Деструктор класса-------------------------------------------------------
  ~CTask();
  //-Функции класса----------------------------------------------------------
  const long& GetIndex(void) const;//получиь индекс
  const CSafeString& GetFromUserGUID(void) const;//получить уникальный идентификатор от какого пользователя задание
  const CSafeString& GetForUserGUID(void) const;//получить уникальный идентификатор для какого пользователя задание
  const CSafeString& GetProjectGUID(void) const;//получить уникальный идентификатор проекта
  const CSafeString& GetTask(void) const;//получить задание
  const long& GetState(void) const;//получиь состояние задания
  const CDate& GetDate(void) const;//получить срок задания
  const CSafeString& GetTaskGUID(void) const;//получить уникальный идентификатор задания
  const CSafeString& GetAnswer(void) const;//получить ответ на задание
  bool GetAnswerNotRead(void) const;//получить, прочитан ли ответ на задание  
  bool GetPlannedPosition(void) const;//получить, является ли задание плановой позицией

  void SetIndex(const long& index);//задать индекс
  void SetFromUserGUID(const char *from_user_guid);//задать уникальный идентификатор от какого пользователя задание
  void SetForUserGUID(const char *for_user_guid);//задать уникальный идентификатор для какого пользователя задание
  void SetProjectGUID(const char *project_guid);//задать уникальный идентификатор проекта
  void SetTask(const char *task);//задать задание
  void SetState(const long &state);//получиь состояние задания
  void SetDate(const CDate &cDate_Set);//задать срок задания
  void SetTaskGUID(const char *task_guid);//задать уникальный идентификатор задания
  void SetAnswer(const char *answer);//задать ответ на задание
  void SetAnswerNotRead(bool state);//задать прочитан ли ответ на задание
  void SetPlannedPosition(bool state);//задать, является ли задание плановой позицией

  bool IsFromOrForUserGUID(const char *guid) const;//это задание от или для пользователя с уникальным идентификатором
  bool IsForUserGUID(const char *guid) const;//это задание для пользователя с уникальным идентификатором
  bool IsTaskGUID(const char *guid) const;//верный ли TaskGUID
  bool IsAnswerNotRead(void) const;//прочитан ли ответ на задание
  bool IsPlannedPosition(void) const;//является ли задание плановой позицией
};
#endif
