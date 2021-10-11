#include "crc.h"

//----------------------------------------------------------------------------------------------------
//загрузить файл и вычислить CRC
//----------------------------------------------------------------------------------------------------
bool LoadFileAndCreateCRC(const CSafeString &file_name,std::vector<char> &vector_data,unsigned short &crc16)
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
