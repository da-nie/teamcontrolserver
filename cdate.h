#ifndef CDATE_H
#define CDATE_H

//====================================================================================================
//описание
//====================================================================================================

//Класс даты

//====================================================================================================
//подключаемые библиотеки
//====================================================================================================

//====================================================================================================
//класс даты
//====================================================================================================

class CDate
{
 private:
  long Year;//год
  long Month;//месяц
  long Day;//число
 public:
  //конструктор
  CDate(void);
  CDate(const long &year,const long &month,const long &day);
  //деструктор
  ~CDate();
  //операции сравнения
  bool operator<(const CDate &cDate) const;
  bool operator>(const CDate &cDate) const;
  bool operator==(const CDate &cDate) const;
  void SetDate(const long &year,const long &month,const long &day);//установить дату
  void GetDate(long &year,long &month,long &day) const;//получить дату
  long GetYear(void) const;//получить год
  long GetMonth(void) const;//получить месяц
  long GetDay(void) const;//получить число
};

#endif
