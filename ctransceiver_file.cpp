#include "ctransceiver_file.h"

//====================================================================================================
//макроопределения
//====================================================================================================

//====================================================================================================
//конструктор класса
//====================================================================================================
CTransceiver_File::CTransceiver_File(void)
{
}
//====================================================================================================
//деструктор класса
//====================================================================================================
CTransceiver_File::~CTransceiver_File()
{ 
}
//====================================================================================================
//функции класса
//====================================================================================================

//----------------------------------------------------------------------------------------------------
//передать клиенту контрольную сумму файла клиентской программы
//----------------------------------------------------------------------------------------------------
void CTransceiver_File::SendCRCClientProgrammFileToClient(SClient &sClient,SERVER_COMMAND command,const CString &programm_file_name,CEvent &cEvent_Exit,bool &on_exit)
{
 on_exit=false;
 vector<char> vector_programm_data;
 unsigned short programm_crc16;
 if (LoadFileAndCreateCRC(programm_file_name,vector_programm_data,programm_crc16)==false)
 {
  SendAnswer(sClient.Socket,SERVER_ANSWER_ERROR,command,NULL,0,cEvent_Exit,on_exit);
  return;
 }

 SendBeginPackage(sClient.Socket,cEvent_Exit,on_exit);
 if (on_exit==true) return;

 SServerAnswer::SHeader sServerAnswer_sHeader;
 sServerAnswer_sHeader.AnswerID=SERVER_ANSWER_CLIENT_PROGRAMM_CRC;
 sServerAnswer_sHeader.CommandID=command;
 SendPart(sClient.Socket,reinterpret_cast<char*>(&sServerAnswer_sHeader),sizeof(SServerAnswer::SHeader),cEvent_Exit,on_exit);
 if (on_exit==true) return;

 SServerAnswer::SClientProgrammCRCHeader sServerAnswer_sClientProgrammCRCHeader;
 sServerAnswer_sClientProgrammCRCHeader.CRC=programm_crc16;
 SendPart(sClient.Socket,reinterpret_cast<char*>(&sServerAnswer_sClientProgrammCRCHeader),sizeof(SServerAnswer::SClientProgrammCRCHeader),cEvent_Exit,on_exit);
 if (on_exit==true) return;

 SendEndPackage(sClient.Socket,cEvent_Exit,on_exit);
}
//----------------------------------------------------------------------------------------------------
//передать клиенту программу и загрузчик
//----------------------------------------------------------------------------------------------------
void CTransceiver_File::SendClientProgrammAndLoaderFileToClient(SClient &sClient,SERVER_COMMAND command,const CString &programm_file_name,const CString &loader_file_name,CEvent &cEvent_Exit,bool &on_exit)
{
 on_exit=false;
 vector<char> vector_programm_data;
 unsigned short programm_crc16;
 if (LoadFileAndCreateCRC(programm_file_name,vector_programm_data,programm_crc16)==false)
 {
  SendAnswer(sClient.Socket,SERVER_ANSWER_ERROR,command,NULL,0,cEvent_Exit,on_exit);
  return;
 }

 vector<char> vector_loader_data;
 unsigned short loader_crc16;
 if (LoadFileAndCreateCRC(loader_file_name,vector_loader_data,loader_crc16)==false)
 {
  SendAnswer(sClient.Socket,SERVER_ANSWER_ERROR,command,NULL,0,cEvent_Exit,on_exit);
  return;
 }

 SendBeginPackage(sClient.Socket,cEvent_Exit,on_exit);
 if (on_exit==true) return;

 SServerAnswer::SHeader sServerAnswer_sHeader;
 sServerAnswer_sHeader.AnswerID=SERVER_ANSWER_CLIENT_PROGRAMM_AND_LOADER;
 sServerAnswer_sHeader.CommandID=command;
 SendPart(sClient.Socket,reinterpret_cast<char*>(&sServerAnswer_sHeader),sizeof(SServerAnswer::SHeader),cEvent_Exit,on_exit);
 if (on_exit==true) return;

 SServerAnswer::SClientProgrammAndLoaderDataHeader sServerAnswer_sClientProgrammAndLoaderDataHeader;
 sServerAnswer_sClientProgrammAndLoaderDataHeader.ProgrammCRC=programm_crc16;
 sServerAnswer_sClientProgrammAndLoaderDataHeader.LoaderCRC=loader_crc16;
 sServerAnswer_sClientProgrammAndLoaderDataHeader.ProgrammSize=vector_programm_data.size();
 sServerAnswer_sClientProgrammAndLoaderDataHeader.LoaderSize=vector_loader_data.size();
 SendPart(sClient.Socket,reinterpret_cast<char*>(&sServerAnswer_sClientProgrammAndLoaderDataHeader),sizeof(SServerAnswer::SClientProgrammAndLoaderDataHeader),cEvent_Exit,on_exit);
 if (on_exit==true) return;
 
 //передаём данные загрузчика
 SendPart(sClient.Socket,reinterpret_cast<char*>(&vector_loader_data[0]),vector_loader_data.size(),cEvent_Exit,on_exit);
 if (on_exit==true) return;
 //передаём данные программы
 SendPart(sClient.Socket,reinterpret_cast<char*>(&vector_programm_data[0]),vector_programm_data.size(),cEvent_Exit,on_exit);
 if (on_exit==true) return;

 SendEndPackage(sClient.Socket,cEvent_Exit,on_exit); 
}

//----------------------------------------------------------------------------------------------------
//загрузить файл и вычислить CRC
//----------------------------------------------------------------------------------------------------
bool CTransceiver_File::LoadFileAndCreateCRC(const CString &file_name,vector<char> &vector_data,unsigned short &crc16)
{
 vector_data.clear();
 crc16=0;
 FILE *file=fopen(file_name,"rb"); 
 if (file==NULL) return(false);
 while(1)
 {  
  unsigned char byte;
  if (fread(&byte,sizeof(unsigned char),1,file)<=0) break;
  crc16^=(byte<<8);
  for (unsigned char i=0;i<8;i++) 
  { 
   if (crc16&0x8000) crc16=(crc16<<1)^0x1021;
                else crc16<<=1;
  }
  vector_data.push_back(byte);
 }
 fclose(file);
 return(true);
}
