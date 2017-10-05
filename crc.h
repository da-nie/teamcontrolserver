#ifndef CRC_H
#define CRC_H

#include <vector>
#include "stdio.h"
#include "csafestring.h"

using namespace std;

bool LoadFileAndCreateCRC(const CSafeString &file_name,vector<char> &vector_data,unsigned short &crc16);//загрузить файл и вычислить CRC

#endif