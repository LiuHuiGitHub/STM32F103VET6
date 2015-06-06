#ifndef __CAN_MSG_H__
#define __CAN_MSG_H__

#include "typedef.h"

#define MAX_CIRCLE_ANGLE				(100u)

typedef enum
{
	EMS_MIL = 0,
	EMS_SSMWWarningLampCtorl,
	ESP_ABSFail_Status,
	ESP_EBDFail_Status,
	ESP_ABSFunctionStatus,
	ESP_EBDActiveStatus,
	ABS_BrakePedalStatus,
	SRS_DriverBuckleSwitchStatus,
	SRS_PassengerBuckleSwitchStatus,
	EPS_TorqSensorStatus,
	BCM_LowBeamStatus,
	BCM_HighBeamStatus,
	BCM_FrontFoglampStatus,
	BCM_RearFoglampStatus,
	BCM_TurnIndicatorLeft,
	BCM_TurnIndicatorRight,
	BCM_PositionLampStatus,
	BCM_DriverDoorStatus,
	BCM_PassengerDoorStatus,
	BCM_LeftRearDoorStatus,
	BCM_RightRearDoorStatus,
	BCM_TrunkStatus,
	BCM_HoodStatus,
	BCM_SunroofStatus,
	ESP_VehicleSpeed_Enable,
	NUM_OF_LIGHT_NUM,
}Light_ENUM;

typedef struct
{
	UINT16 Id;
	UINT16 Time;
	UINT8 Data[3][8];
}LightCanMsg_STRUCT;

extern const LightCanMsg_STRUCT s_Light[NUM_OF_LIGHT_NUM];

typedef enum
{
	EMS_EngineSpeed = 0,
	ESP_VehicleSpeed,
	EMS_EngineCoolanTemperature,
	NUM_OF_Circle_NUM,
}Circle_ENUM;

typedef struct
{
	UINT16 Id;
	UINT16 Time;
	UINT8 Data[MAX_CIRCLE_ANGLE+1][8];
}CircleCanMsg_STRUCT;

extern const CircleCanMsg_STRUCT s_Circle[NUM_OF_Circle_NUM];

typedef enum
{
	ID_0x355 = 0,
	ID_0x218,
	ID_0x50,
	ID_0x2a0,
	ID_0x288,
	ID_0x255,
	ID_0x275,
	NUM_OF_ID_NUM,
}ID_ENUM;

typedef struct
{
	UINT16 Id;
	UINT8 Data[8];
	UINT16 Time;
}CanMsg_STRUCT;

extern CanMsg_STRUCT s_CanMsgBuff[NUM_OF_ID_NUM];

extern const ID_ENUM e_Light_To_Id[NUM_OF_LIGHT_NUM];
extern const ID_ENUM e_Circle_To_Id[NUM_OF_Circle_NUM];

#endif
