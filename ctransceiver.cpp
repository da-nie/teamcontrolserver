#include "ctransceiver.h"

//====================================================================================================
//макроопределения
//====================================================================================================

//====================================================================================================
//конструктор класса
//====================================================================================================
CTransceiver::CTransceiver(void)
{
}
//====================================================================================================
//деструктор класса
//====================================================================================================
CTransceiver::~CTransceiver()
{ 
}
//====================================================================================================
//функции класса
//====================================================================================================

//----------------------------------------------------------------------------------------------------
//задать строку по массиву
//----------------------------------------------------------------------------------------------------
void CTransceiver::SetString(CString &text,char *buffer,long length)
{
 text="";
 for(long n=0;n<length;n++) text+=buffer[n];
}

//----------------------------------------------------------------------------------------------------
//отправить код начала пакета
//----------------------------------------------------------------------------------------------------
void CTransceiver::SendBeginPackage(SOCKET socket_client,CEvent &cEvent_Exit,bool &on_exit)
{
 on_exit=false;
 char code=static_cast<unsigned char>(PROTOCOL_BEGIN_PACKAGE);
 SendData(socket_client,&code,1,cEvent_Exit,on_exit);
}
//----------------------------------------------------------------------------------------------------
//отправить код конца пакета
//----------------------------------------------------------------------------------------------------
void CTransceiver::SendEndPackage(SOCKET socket_client,CEvent &cEvent_Exit,bool &on_exit)
{
 on_exit=false;
 char code=PROTOCOL_END_PACKAGE;
 SendData(socket_client,&code,1,cEvent_Exit,on_exit);
}
//----------------------------------------------------------------------------------------------------
//отправить ответ клиенту
//----------------------------------------------------------------------------------------------------
void CTransceiver::SendAnswer(SOCKET socket_client,SERVER_ANSWER answer,SERVER_COMMAND command,const char *data,unsigned long size,CEvent &cEvent_Exit,bool &on_exit)
{
 on_exit=false;
 //отправляем начало пакета
 SendBeginPackage(socket_client,cEvent_Exit,on_exit);
 if (on_exit==true) return;
 //отправляем заголовок
 SServerAnswer::SHeader sServerAnswer_sHeader;
 sServerAnswer_sHeader.AnswerID=answer;
 sServerAnswer_sHeader.CommandID=command;
 SendPart(socket_client,reinterpret_cast<char*>(&sServerAnswer_sHeader),sizeof(SServerAnswer::SHeader),cEvent_Exit,on_exit);
 if (on_exit==true) return;
 //отправляем данные
 if (data!=NULL)
 {
  SendPart(socket_client,data,size,cEvent_Exit,on_exit);
  if (on_exit==true) return;
 }
 //завершаем передачу
 SendEndPackage(socket_client,cEvent_Exit,on_exit);
}
//----------------------------------------------------------------------------------------------------
//отправить данные клиенту с выполнением байтстаффинга
//----------------------------------------------------------------------------------------------------
void CTransceiver::SendPart(SOCKET socket_client,const char *data,unsigned long size,CEvent &cEvent_Exit,bool &on_exit)
{
 on_exit=false;
 vector<unsigned char> vector_answer;
 vector_answer.reserve(size*2);
 //выполняем замену данных с учётом байтстаффинга
 for(long n=0;n<size;n++)
 {
  unsigned char byte=data[n];
  if (byte==0x00)
  {
   vector_answer.push_back(static_cast<unsigned char>(PROTOCOL_STUFFING));
   vector_answer.push_back(static_cast<unsigned char>(PROTOCOL_STUFFING_00));
   continue;
  }
  if (byte==0xFE)
  {
   vector_answer.push_back(static_cast<unsigned char>(PROTOCOL_STUFFING));
   vector_answer.push_back(static_cast<unsigned char>(PROTOCOL_STUFFING_FE));
   continue;
  }
  if (byte==0xFF)
  {
   vector_answer.push_back(static_cast<unsigned char>(PROTOCOL_STUFFING));
   vector_answer.push_back(static_cast<unsigned char>(PROTOCOL_STUFFING_FF));
   continue;
  }
  vector_answer.push_back(byte);
 }
 const char *ptr=reinterpret_cast<char*>(&vector_answer[0]);
 SendData(socket_client,ptr,vector_answer.size(),cEvent_Exit,on_exit);
}
//----------------------------------------------------------------------------------------------------
//отправить данные клиенту
//----------------------------------------------------------------------------------------------------
void CTransceiver::SendData(SOCKET socket_client,const char *package,long size,CEvent &cEvent_Exit,bool &on_exit)
{
 on_exit=false;
 long offset=0;
 while(size>0)
 {
  if (WaitForSingleObject(cEvent_Exit.m_hObject,0)==WAIT_OBJECT_0)
  {
   on_exit=true;
   return;
  }
  //узнаем как дела у наших сокетов
  fd_set Writen;
  FD_ZERO(&Writen);//обнуляем список
  FD_SET(socket_client,&Writen);//добавляем сокет
  fd_set Exeption;
  FD_ZERO(&Exeption);//обнуляем список
  FD_SET(socket_client,&Exeption);//добавляем сокет
  timeval timeout;
  timeout.tv_sec=0;
  timeout.tv_usec=5000;
  //спрашиваем, не готов ли сокет передавать данные
  if (select(0,0,&Writen,&Exeption,&timeout)>0)
  {
   if (FD_ISSET(socket_client,&Exeption)) return;
   if (FD_ISSET(socket_client,&Writen))
   {
    long ret=send(socket_client,package+offset,size,0);
	if (ret==SOCKET_ERROR)
	{
     int error_code=WSAGetLastError();
     if (error_code==WSAEWOULDBLOCK)//надо подождать
     {
      Pause(1);
      continue;
     }
     return;
	}
    size-=ret;
    offset+=ret;	
   }
  }
  else
  {
   return;
  }
 }
}
//----------------------------------------------------------------------------------------------------
//пауза в миллисекундах
//----------------------------------------------------------------------------------------------------
void CTransceiver::Pause(long time_ms)
{
 CEvent cEvent;
 cEvent.ResetEvent();
 WaitForSingleObject(cEvent.m_hObject,time_ms); 
}
