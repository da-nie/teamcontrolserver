#include "csafestring.h"

//====================================================================================================
//����������� ������
//====================================================================================================
CSafeString::CSafeString(char* str)
{ 
 Size=strlen(str);
 String_Ptr=new char[Size+1];
 strcpy(String_Ptr,str);
}
//====================================================================================================
//����������� ����������� ������
//====================================================================================================
CSafeString::CSafeString(const CSafeString& cSafeString)
{
 Size=cSafeString.Size;
 String_Ptr=new char[Size+1];
 strcpy(String_Ptr,cSafeString.String_Ptr);
}
//====================================================================================================
//���������� ������
//====================================================================================================
CSafeString::~CSafeString() 
{
 if (String_Ptr!=NULL) delete[](String_Ptr);
 String_Ptr=NULL;
 Size=0;
}

//====================================================================================================
//������� ������
//====================================================================================================

//----------------------------------------------------------------------------------------------------
//�������� ������ ������
//----------------------------------------------------------------------------------------------------
unsigned long CSafeString::GetLength(void) const
{
 return(Size);
}
//----------------------------------------------------------------------------------------------------
//�������� ������
//----------------------------------------------------------------------------------------------------
long CSafeString::Compare(const CSafeString& cSafeString) const
{
 return(strcmp(*this,cSafeString.String_Ptr));
}
//----------------------------------------------------------------------------------------------------
//�������� "="
//----------------------------------------------------------------------------------------------------
CSafeString& CSafeString::operator=(const CSafeString& cSafeString)
{
 if (this!=&cSafeString)
 {
  delete[](this->String_Ptr);
  Size=cSafeString.Size;
  this->String_Ptr=new char[Size+1];
  strcpy(this->String_Ptr,cSafeString.String_Ptr);
 }
 return(*this);
}
//----------------------------------------------------------------------------------------------------
//�������� "="
//----------------------------------------------------------------------------------------------------
CSafeString& CSafeString::operator=(const char *str)
{
 delete[](this->String_Ptr);
 Size=strlen(str);
 this->String_Ptr=new char[Size+1];
 strcpy(this->String_Ptr,str);
 return(*this);
}
//----------------------------------------------------------------------------------------------------
//�������� "+"
//----------------------------------------------------------------------------------------------------
CSafeString operator+(const CSafeString& cSafeString_Left, const CSafeString& cSafeString_Right)
{
 CSafeString cSafeString=cSafeString_Left;
 return(cSafeString+=cSafeString_Right);
}
//----------------------------------------------------------------------------------------------------
//�������� "+="
//----------------------------------------------------------------------------------------------------
CSafeString& CSafeString::operator+=(const CSafeString& cSafeString)
{
 unsigned long n;
 unsigned long m;
 unsigned long new_string_size=Size+cSafeString.Size;
 char* new_string_ptr=new char[new_string_size+1];
 for(n=0;n<Size;n++) new_string_ptr[n]=String_Ptr[n];
 for(n=Size,m=0;n<=new_string_size;n++,m++) new_string_ptr[n]=cSafeString.String_Ptr[m];
 delete[](String_Ptr);
 String_Ptr=new_string_ptr;
 Size=new_string_size;
 return(*this);
}
//----------------------------------------------------------------------------------------------------
//�������� "+="
//----------------------------------------------------------------------------------------------------
CSafeString& CSafeString::operator+=(const char *str)
{
 unsigned long n;
 unsigned long m;
 unsigned long new_string_size=Size+strlen(str);
 char* new_string_ptr=new char[new_string_size+1];
 for(n=0;n<Size;n++) new_string_ptr[n]=String_Ptr[n];
 for(n=Size,m=0;n<=new_string_size;n++,m++) new_string_ptr[n]=str[m];
 delete[](String_Ptr);
 String_Ptr=new_string_ptr;
 Size=new_string_size;
 return(*this);
}
//----------------------------------------------------------------------------------------------------
//�������� "+="
//----------------------------------------------------------------------------------------------------
CSafeString& CSafeString::operator+=(const char symbol)
{
 unsigned long n;
 unsigned long new_string_size=Size+1;
 char* new_string_ptr=new char[new_string_size+1];
 for(n=0;n<Size;n++) new_string_ptr[n]=String_Ptr[n];
 new_string_ptr[n]=symbol;
 n++;
 new_string_ptr[n]=0;
 delete[](String_Ptr);
 String_Ptr=new_string_ptr;
 Size=new_string_size;
 return(*this);
}
//----------------------------------------------------------------------------------------------------
//�������� "(const char *)"
//----------------------------------------------------------------------------------------------------
CSafeString::operator const char*(void) const
{
 return(this->String_Ptr);
}
