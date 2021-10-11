#ifndef CRC_H
#define CRC_H

#include <vector>
#include "stdio.h"
#include "csafestring.h"



bool LoadFileAndCreateCRC(const CSafeString &file_name,std::vector<char> &vector_data,unsigned short &crc16);//загрузить файл и вычислить CRC

#endif