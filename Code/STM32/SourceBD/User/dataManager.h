#ifndef __DATMANAGER_H_
#define __DATMANAGER_H_

#include "sigAnalog_Detect.h"
#include "sigDigital_Detect.h"
#include "RelayCon.h"

#define typeAnalogDet	0xA1
#define typeDigitalDet	0xA2
#define typeRelayCon	0xA3

#define datsRole_Disp	0x01
#define datsRole_DNLoad 0x02
#define datsRole_UPLoad 0x03

typedef union{

	sigAna_Det_datsType	datsSigAna_Det;
	sigDig_Det_datsType	datsSigDig_Det;
	RelayCon_datsType 	datsRelayCon;
}SourceBD_Data;

typedef enum{

	datsAnalogDet	= typeAnalogDet,
	datsDigitalDet	= typeDigitalDet,
	datsRelayCon	= typeRelayCon,
}SourceBD_DataType;

typedef enum{

	roleDisp	= datsRole_Disp,
	roleDNLoad 	= datsRole_DNLoad,
	roleUPLoad 	= datsRole_UPLoad,
}datsRole;

typedef struct{

	SourceBD_DataType	datsType;
	datsRole			role;
	SourceBD_Data		dats;
}SourceBD_MEAS;

void osMsg_memPoolInit(void);

#endif

