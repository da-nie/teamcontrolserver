#ifndef CDATE_H
#define CDATE_H

//====================================================================================================
//��������
//====================================================================================================

//����� ����

//====================================================================================================
//������������ ����������
//====================================================================================================

//====================================================================================================
//����� ����
//====================================================================================================

class CDate
{
 private:
  long Year;//���
  long Month;//�����
  long Day;//�����
 public:
  //�����������
  CDate(void);
  CDate(const long &year,const long &month,const long &day);
  //����������
  ~CDate();
  //�������� ���������
  bool operator<(const CDate &cDate) const;
  bool operator>(const CDate &cDate) const;
  bool operator==(const CDate &cDate) const;
  void SetDate(const long &year,const long &month,const long &day);//���������� ����
  void GetDate(long &year,long &month,long &day) const;//�������� ����
  long GetYear(void) const;//�������� ���
  long GetMonth(void) const;//�������� �����
  long GetDay(void) const;//�������� �����
};

#endif
