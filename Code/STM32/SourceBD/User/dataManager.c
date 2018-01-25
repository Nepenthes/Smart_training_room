#include "dataManager.h"

osPoolId  memid_SourceBD_pool;								 
osPoolDef(SourceBD_pool, 10, SourceBD_MEAS);                  // 内存池定义
osMessageQId  msgid_MsgBox_SourceBD;
osMessageQDef(MsgBox_SourceBD, 8, &SourceBD_MEAS);            // 消息队列定义

void osMsg_memPoolInit(void){

	static bool memInit_flg = false;

	if(!memInit_flg){

		memid_SourceBD_pool		= osPoolCreate(osPool(SourceBD_pool));	//创建内存池
		msgid_MsgBox_SourceBD 	= osMessageCreate(osMessageQ(MsgBox_SourceBD), NULL);   //创建消息队列

		memInit_flg = true;
	}
}
