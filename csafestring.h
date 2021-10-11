#ifndef CSAFE_STRING_H
#define CSAFE_STRING_H

//====================================================================================================
//описание
//====================================================================================================

//Класс строки, не использующий Copy On Write и безопасный для использования в многопоточных приложениях

//====================================================================================================
//подключаемые библиотеки
//====================================================================================================

#include <string.h>

//====================================================================================================
//класс строки, не использующий Copy On Write и безопасный для использования в многопоточных приложениях
//====================================================================================================

class CSafeString
{
 protected:
  char *String_Ptr;//указатель на строку
  unsigned long Size;//размер строки
 public:
  //конструктор
  CSafeString(char* str="");
  CSafeString(const CSafeString& cSafeString);//конструктор копирования
  //деструктор
  ~CSafeString();
 public:
  //-Функции класса----------------------------------------------------------
  unsigned long GetLength(void) const;//получить размер строки
  long Compare(const CSafeString& cSafeString) const;//сравнить строки
  CSafeString& operator=(const CSafeString& cSafeString);//оператор "="
  CSafeString& operator=(const char *str);//оператор "="
  friend CSafeString operator+(const CSafeString& cSafeString_Left,const CSafeString& cSafeString_Right);//оператор "+"
  CSafeString& operator+=(const CSafeString& cSafeString);//оператор "+="
  CSafeString& operator+=(const char *str);//оператор "+="
  CSafeString& operator+=(const char symbol);//оператор "+="
  operator const char*(void) const;//оператор "(const char *)"
};

#endif
