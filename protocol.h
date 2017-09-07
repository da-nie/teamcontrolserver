#ifndef PROTOCOL_H
#define PROTOCOL_H

//====================================================================================================
//описание
//====================================================================================================

//структуры протокола обмена

//====================================================================================================
//подключаемые библиотеки
//====================================================================================================
#include "stdafx.h"

//====================================================================================================
//макроопределения
//====================================================================================================

//максимальный размер строки команды
#define MAX_PACKAGE_LENGTH 10000

//параметры байтстаффинга
//конец команды
#define PROTOCOL_END_PACKAGE   0x00
//начало команды
#define PROTOCOL_BEGIN_PACKAGE 0xFF
//стаффинг
#define PROTOCOL_STUFFING      0xFE
//код 0x00
#define PROTOCOL_STUFFING_00   0x01
//код 0xFE
#define PROTOCOL_STUFFING_FE   0x02
//код 0xFF
#define PROTOCOL_STUFFING_FF   0x03


//команды серверу
enum SERVER_COMMAND
{
 //нет команды
 SERVER_COMMAND_NOTHING=0,
 //запрос CRC файла клиентской программы
 SERVER_COMMAND_GET_CLIENT_PROGRAMM_CRC=1,
 //запрос клиентской программы и загрузчика для обновления
 SERVER_COMMAND_GET_CLIENT_PROGRAMM_AND_LOADER=2,
 //авторизация
 SERVER_COMMAND_AUTORIZATION=3,
 //получение базы пользователей
 SERVER_COMMAND_GET_USER_BOOK=4,
 //получение базы заданий
 SERVER_COMMAND_GET_TASK_BOOK=5,
 //получение базы проектов
 SERVER_COMMAND_GET_PROJECT_BOOK=6,
 //новое задание
 SERVER_COMMAND_ADDED_TASK=7,
 //задание удалено
 SERVER_COMMAND_DELETED_TASK=8,
 //задание изменено
 SERVER_COMMAND_CHANGED_TASK=9,
 //новый проект
 SERVER_COMMAND_ADDED_PROJECT=10,
 //проект удален
 SERVER_COMMAND_DELETED_PROJECT=11,
 //проект изменен
 SERVER_COMMAND_CHANGED_PROJECT=12,

 //проверка связи
 SERVER_COMMAND_PING=13
};

//ответы сервера
enum SERVER_ANSWER
{
 //команда выполнена успешно и не требует ответа
 SERVER_ANSWER_OK=0,
 //команда не выполнена и не требует ответа
 SERVER_ANSWER_ERROR=1,
 //команда авторизации выполнена успешно и не требует ответа
 SERVER_ANSWER_AUTORIZATION_OK=2,
 //передача CRC файла клиентской программы
 SERVER_ANSWER_CLIENT_PROGRAMM_CRC=3,
 //передача клиентской программы для обновления
 SERVER_ANSWER_CLIENT_PROGRAMM_AND_LOADER=4,
 //передаётся база пользователей
 SERVER_ANSWER_USER_BOOK=5,
 //передаётся удалённый пользователь
 SERVER_ANSWER_DELETED_USER=6,
 //передаётся добавленный пользователь
 SERVER_ANSWER_ADDED_USER=7,
 //передаётся изменённый пользователь
 SERVER_ANSWER_CHANGED_USER=8,

 //передаётся база заданий пользователя
 SERVER_ANSWER_TASK_BOOK=9,
 //передаётся удалённое задание
 SERVER_ANSWER_DELETED_TASK=10,
 //передаётся добавленное задание
 SERVER_ANSWER_ADDED_TASK=11,
 //передаётся изменённое задание
 SERVER_ANSWER_CHANGED_TASK=12,

 //передаётся база проектов
 SERVER_ANSWER_PROJECT_BOOK=13,
 //передаётся удалённый проект
 SERVER_ANSWER_DELETED_PROJECT=14,
 //передаётся добавленный проект
 SERVER_ANSWER_ADDED_PROJECT=15,
 //передаётся изменённый проект
 SERVER_ANSWER_CHANGED_PROJECT=16,

 //проверка связи
 SERVER_ANSWER_PING=17

};

//задание не прочитано
#define TASK_STATE_NO_READ 0
//задание прочитано и ожидает выполнения
#define TASK_STATE_READED 1
//задание выполняется
#define TASK_STATE_IS_RUNNING 2
//задание выполнено
#define TASK_STATE_DONE 3
//задание отклонено
#define TASK_STATE_CANCELED 4
//задание завершено
#define TASK_STATE_FINISHED 5

//====================================================================================================
//структуры
//====================================================================================================

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//команды серверу
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#pragma pack(1)
struct SServerCommand
{
 //заголовок
 struct SHeader
 {
  unsigned long CommandID;//идентификатор команды
 };
 //команды серверу
 //кодирование заголовка данных для проверки связи
 struct SPingDataHeader
 {
  unsigned char PingData[10];//тестовое сообщение
 };
 //авторизация
 struct SAutorizationDataHeader
 { 
  unsigned long LoginSize;//размер логина
  unsigned long PasswordSize;//размер пароля
 };
 //кодирование заголовка данных при передаче задания
 struct CTaskDataHeader
 {
  long Index;//индекс задания
  unsigned long FromUserGUIDSize;//от пользователя
  unsigned long ForUserGUIDSize;//для пользователя
  unsigned long ProjectGUIDSize;//по проекту
  unsigned long TaskSize;//задание
  unsigned long TaskGUIDSize;//идентификатор задания
  unsigned long AnswerSize;//ответ
  long State;//состояние
  long Year;//год
  long Month;//месяц
  long Day;//число   
 };
 //кодирование заголовка данных при передаче проекта
 struct CProjectDataHeader
 {
  unsigned long ProjectNameSize;//проект
  unsigned long ProjectGUIDSize;//идентификатор проекта
 };
};
#pragma pack()


//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//ответ сервера
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#pragma pack(1)
struct SServerAnswer
{
 //заголовок
 struct SHeader
 {
  unsigned long AnswerID;//идентификатор ответа
  unsigned long CommandID;//идентификатор команды на которую выдаётся ответ
 };
 //ответ
 //кодирование заголовка данных для проверки связи
 struct SPingDataHeader
 {
  unsigned char PingData[10];//тестовое сообщение
 };
 //кодирование заголовка данных при авторизации
 struct SAutorizationDataHeader
 {
  unsigned long UserGUIDSize;//размер GUID
 };
 //кодирование заголовка данных при передаче CRC файла клиентской программы
 struct SClientProgrammCRCHeader
 {
  unsigned long CRC;//контрольная сумма
 };
 //кодирование заголовка данных при передаче файла клиентской программы и загрузчика
 struct SClientProgrammAndLoaderDataHeader
 {
  unsigned long LoaderCRC;//контрольная сумма загрузчика
  unsigned long ProgrammCRC;//контрольная сумма программы
  unsigned long LoaderSize;//размер загрузчика
  unsigned long ProgrammSize;//размер программы
 };

 //кодирование заголовка данные при передаче параметров пользователя
 struct CUserDataHeader
 {
  unsigned long NameSize;//размер имени
  unsigned long JobTitleSize;//размер должности
  unsigned long TelephoneSize;//размер телефонных данных
  unsigned long DescriptionSize;//размер описания
  unsigned long GUIDSize;//размер GUID
  bool Leader;//является ли начальником
 };
 //кодирование заголовка данных при передаче параметров задания
 struct CTaskDataHeader
 {
  long Index;//индекс задания
  unsigned long FromUserGUIDSize;//от пользователя
  unsigned long ForUserGUIDSize;//для пользователя
  unsigned long ProjectGUIDSize;//по проекту
  unsigned long TaskSize;//задание
  unsigned long TaskGUIDSize;//идентификатор задания
  unsigned long AnswerSize;//ответ
  long State;//состояние
  long Year;//год
  long Month;//месяц
  long Day;//число 
 };
 //кодирование заголовка данных при передаче параметров проекта
 struct CProjectDataHeader
 {
  unsigned long ProjectNameSize;//проект
  unsigned long ProjectGUIDSize;//идентификатор проекта
 };
};
#pragma pack()

#endif