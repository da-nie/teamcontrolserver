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
  bool AnswerReferenceExist;//есть ссылка в ответе исполнителя
  bool TaskReferenceExist;//есть ссылка в задании
  bool Common;//задание является общим
  CSafeString AnswerReference;//ссылка на файл в ответе исполнителя
  CSafeString TaskReference;//ссылка на файл в задании
  //долнительная информация
  bool ChangeCommonState;//был изменён статус общности (нужно потоку, чтобы знать, когда заново передавать базу общих заданий)

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
  const CSafeString& GetAnswerReference(void) const;//получить ссылку на файл в ответе исполнителя
  const CSafeString& GetTaskReference(void) const;//получить ссылку на файл в задании
  bool GetAnswerNotRead(void) const;//получить, прочитан ли ответ на задание  
  bool GetPlannedPosition(void) const;//получить, является ли задание плановой позицией
  bool GetAnswerReferenceExist(void) const;//получить, есть ли ссылка в ответе исполнителя
  bool GetTaskReferenceExist(void) const;//получить, есть ли ссылка в задании
  bool GetCommon(void) const;//получить, является ли задание общим

  void SetIndex(const long& index);//задать индекс
  void SetFromUserGUID(const char *from_user_guid);//задать уникальный идентификатор от какого пользователя задание
  void SetForUserGUID(const char *for_user_guid);//задать уникальный идентификатор для какого пользователя задание
  void SetProjectGUID(const char *project_guid);//задать уникальный идентификатор проекта
  void SetTask(const char *task);//задать задание
  void SetState(const long &state);//получиь состояние задания
  void SetDate(const CDate &cDate_Set);//задать срок задания
  void SetTaskGUID(const char *task_guid);//задать уникальный идентификатор задания
  void SetAnswer(const char *answer);//задать ответ на задание
  void SetAnswerReference(const char *reference);//задать ссылку на файл в ответе исполнителя
  void SetTaskReference(const char *reference);//задать ссылку на файл в задании
  void SetAnswerNotRead(bool state);//задать прочитан ли ответ на задание
  void SetPlannedPosition(bool state);//задать, является ли задание плановой позицией
  void SetAnswerReferenceExist(bool state);//задать, есть ли ссылка в ответе исполнителя
  void SetTaskReferenceExist(bool state);//задать, есть ли ссылка в задании
  void SetCommon(bool state);//установить, является ли задание общим
  void SetChangeCommonState(bool state);//установить, изменение статуса общности задания

  bool IsFromOrForUserGUID(const char *guid) const;//это задание от или для пользователя с уникальным идентификатором
  bool IsForUserGUID(const char *guid) const;//это задание для пользователя с уникальным идентификатором
  bool IsTaskGUID(const char *guid) const;//верный ли TaskGUID
  bool IsAnswerNotRead(void) const;//прочитан ли ответ на задание
  bool IsPlannedPosition(void) const;//является ли задание плановой позицией
  bool IsAnswerReferenceExist(void) const;//есть ли ссылка в ответе исполнителя
  bool IsTaskReferenceExist(void) const;//есть ли ссылка в задании
  bool IsCommon(void) const;//является ли задание общим
  bool IsChangeCommonState(void) const;//был ли изменён статус общности задания
};
#endif
