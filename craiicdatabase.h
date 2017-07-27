#ifndef CRAII_CDATABASE_H
#define CRAII_CDATABASE_H

//====================================================================================================
//описание
//====================================================================================================

//Класс RAII для работы с базой данных

//====================================================================================================
//подключаемые библиотеки
//====================================================================================================

#include "stdafx.h"

using namespace std;

//====================================================================================================
//класс RAII базы данных
//====================================================================================================
class CRAIICDatabase
{
 private:
  CDatabase *cDatabase_Ptr;//указатель на класс базы данных
 public:
  CRAIICDatabase(CDatabase *cDatabase_Set_Ptr,const char *init)//конструктор
  {
   cDatabase_Ptr=cDatabase_Set_Ptr;
   cDatabase_Set_Ptr->Open(NULL,FALSE,FALSE,init,TRUE);
  }
  ~CRAIICDatabase()//деструктор
  {
   if (cDatabase_Ptr->IsOpen()==TRUE) cDatabase_Ptr->Close();
  }
  bool IsOpen(void)//отрыта ли база
  {
   if (cDatabase_Ptr->IsOpen()==TRUE) return(true);
   return(false);
  }
};

#endif