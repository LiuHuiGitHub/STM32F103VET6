#include "stdio.h"
#include "stdlib.h"
#include "string.h"

FILE *fp_CanMsg_txt;
FILE *fp_CanMsg_c;
FILE *fp_CanMsg_h;

#define MAX_ROW_SIZE		100
unsigned char NowRow[MAX_ROW_SIZE];

#define ROW_NULL	(0u)
#define ROW_TEXT	(1u)
#define ROW_EOF		(2u)

void RowDelSpace(unsigned char *pdata)
{
	unsigned char *p = pdata;
	while(*p!='\n' && *p!='\0')
	{
		if(*p!=' ' && *p!='\t')
		{
			*pdata = *p;
			pdata++;
		}
		p++;
	}
	*pdata = '\n';
}

unsigned char RowNoteDel(FILE *p)
{
	unsigned char buff[MAX_ROW_SIZE];
	int ch;
	int i;
	unsigned char note = 0;
	ch=fgetc(p);
	if(ch != EOF)
	{
		ungetc(ch,p);
		fgets(buff, 100, p);
		RowDelSpace(buff);
		if(*buff == '\n')
			return ROW_NULL;
		for(i=0; buff[i]!='\n'; i++)
		{
			if(buff[i]=='/'){
				if(i != 0){
					strncpy(NowRow, buff, i);
					NowRow[i] = '\n';
					return ROW_TEXT;
				}
				return ROW_NULL;
			}
			else if(buff[i] == '\0')
				return ROW_EOF;
		}
		strncpy(NowRow, buff, i);
		NowRow[i] = '\n';
		return ROW_TEXT;
	}
	return ROW_EOF;
}

#define CAN_MSG_NAME_LEN		(100)
typedef struct
{
	unsigned char Name[CAN_MSG_NAME_LEN];
	unsigned int Id;
	unsigned int Time;
	unsigned char Data[3][8];
}LightCanMsg_STRUCT;

LightCanMsg_STRUCT s_LightCanMsg[100];
unsigned char s_LightCanMsgNum = 0;
enum
{
	LIGHT_MSG_NAME = 0,
	LIGHT_MSG_ID,
	LIGHT_MSG_BYTE,
	LIGHT_MSG_BIT_START,
	LIGHT_MSG_BIT_OVER,
	LIGHT_MSG_ON,
	LIGHT_MSG_OFF,
	LIGHT_MSG_TIME,
	LIGHT_NUM_OF_MSG,
}LightStrToMsg_ENUM;

void CanMsgReadLight(LightCanMsg_STRUCT *msg, unsigned char *pdata)
{
	unsigned char i;
	unsigned char *p = pdata;
	unsigned char StrToNumBuff[CAN_MSG_NAME_LEN];
	unsigned char MsgName[CAN_MSG_NAME_LEN];
	unsigned int StrToNum[LIGHT_NUM_OF_MSG];
	unsigned char LightMaskByte = 0;
	
	for(i=0; i<LIGHT_NUM_OF_MSG; i++)
	{
		while(*pdata != ',' && *pdata != '\n')
		{
			pdata++;
		}
		if(i == LIGHT_MSG_NAME)
		{
			memset(MsgName, '\0', CAN_MSG_NAME_LEN);
			strncpy(MsgName, p, pdata-p);
		}
		else
		{
			memset(StrToNumBuff, '\0', CAN_MSG_NAME_LEN);
			strncpy(StrToNumBuff, p, pdata-p);
			if(i == LIGHT_MSG_ID)
			{
				sscanf(StrToNumBuff, "%x", &StrToNum[i]);
			}
			else
			{
				sscanf(StrToNumBuff, "%d",&StrToNum[i]);
			}
		}
		pdata++;
		p = pdata;
	}
	strcpy(msg->Name, MsgName);
	msg->Id = StrToNum[LIGHT_MSG_ID];
	msg->Time = StrToNum[LIGHT_MSG_TIME];
	memset(msg->Data[0], 0xff, 8);
	memset(msg->Data[1], 0x00, 8);
	memset(msg->Data[2], 0x00, 8);
	for(i=StrToNum[LIGHT_MSG_BIT_START]; i<=StrToNum[LIGHT_MSG_BIT_OVER]; i++)
	{
		LightMaskByte |= 1<<i;
	}
	msg->Data[0][StrToNum[LIGHT_MSG_BYTE]] &= ~LightMaskByte;
	msg->Data[1][StrToNum[LIGHT_MSG_BYTE]] = StrToNum[LIGHT_MSG_ON]<<StrToNum[LIGHT_MSG_BIT_START];
	msg->Data[2][StrToNum[LIGHT_MSG_BYTE]] = StrToNum[LIGHT_MSG_OFF]<<StrToNum[LIGHT_MSG_BIT_START];
	s_LightCanMsgNum++;
}

void ReadLightMsg(void)
{
	unsigned char RowError;
	unsigned char i = 0;
	unsigned char LightTypFlag = 0;
	rewind(fp_CanMsg_txt);
	do
	{
		RowError = RowNoteDel(fp_CanMsg_txt);
		if(RowError == ROW_TEXT)
		{
			if(strncmp(NowRow, "StartLightTypedef", strlen("StartLightTypedef")) == 0)
			{
				LightTypFlag = 1;
			}
			else if(strncmp(NowRow, "EndLightTypedef", strlen("EndLightTypedef")) == 0)
			{
				LightTypFlag = 0;
			}
			else if(LightTypFlag == 1)
			{
				CanMsgReadLight(&s_LightCanMsg[i++], NowRow);
			}
		}
	}while(RowError != ROW_EOF);
}

#define Circle_NUM			(100u)
typedef struct
{
	unsigned char Name[CAN_MSG_NAME_LEN];
	unsigned int Id;
	unsigned int Time;
	unsigned char Data[Circle_NUM+1][8];
}CircleCanMsg_STRUCT;
CircleCanMsg_STRUCT s_CircleCanMsg[5];
unsigned char s_CircleCanMsgNum = 0;
enum
{
	Circle_MSG_NAME = 0,
	Circle_MSG_ID,
	Circle_MSG_BYTE,
	Circle_MSG_BIT_START,
	Circle_MSG_BIT_LEN,
	Circle_MSG_VALUE,
	Circle_MSG_TIME,
	Circle_NUM_OF_MSG,
}LightStrToMsg_ENUM;
void CanMsgReadCircle(CircleCanMsg_STRUCT *msg, unsigned char *pdata)
{
	unsigned char i, j;
	unsigned char *p = pdata;
	unsigned char StrToNumBuff[CAN_MSG_NAME_LEN];
	unsigned char MsgName[CAN_MSG_NAME_LEN];
	unsigned int StrToNum[Circle_NUM_OF_MSG];
	unsigned int u32_CircleMask = 0;
	unsigned char u8_CircleByteLen;
	unsigned int u32_Circletmp;

	for(i=0; i<Circle_NUM_OF_MSG; i++)
	{
		while(*pdata != ',' && *pdata != '\n')
		{
			pdata++;
		}
		if(i == Circle_MSG_NAME)
		{
			memset(MsgName, '\0', CAN_MSG_NAME_LEN);
			strncpy(MsgName, p, pdata-p);
		}
		else
		{
			memset(StrToNumBuff, '\0', CAN_MSG_NAME_LEN);
			strncpy(StrToNumBuff, p, pdata-p);
			if(i == Circle_MSG_ID)
			{
				sscanf(StrToNumBuff, "%x", &StrToNum[i]);
			}
			else
			{
				sscanf(StrToNumBuff, "%d",&StrToNum[i]);
			}
		}
		pdata++;
		p = pdata;
	}
	strcpy(msg->Name, MsgName);
	msg->Id = StrToNum[Circle_MSG_ID];
	msg->Time = StrToNum[Circle_MSG_TIME];
	memset(msg->Data[0], 0xff, 8);
	memset(msg->Data[1], 0x00, 8);
	for(i=StrToNum[Circle_MSG_BIT_START]; i<StrToNum[Circle_MSG_BIT_START]+StrToNum[Circle_MSG_BIT_LEN]; i++)
	{
		u32_CircleMask |= 1<<i;
	}
	u8_CircleByteLen = StrToNum[Circle_MSG_BIT_START]+StrToNum[Circle_MSG_BIT_LEN];
	u8_CircleByteLen = u8_CircleByteLen/8 + ((u8_CircleByteLen%8)&&1);
	for (i=0; i<u8_CircleByteLen; i++)
	{
		msg->Data[0][StrToNum[Circle_MSG_BYTE]+i] &= ~u32_CircleMask;
		u32_CircleMask >>= 8;
	}
	StrToNum[Circle_MSG_VALUE] <<= StrToNum[Circle_MSG_BIT_START];
	for (j=0; j<Circle_NUM; j++)
	{
		u32_Circletmp = StrToNum[Circle_MSG_VALUE]*j/Circle_NUM;
		for (i=StrToNum[Circle_MSG_BYTE]+u8_CircleByteLen-1; i>=StrToNum[Circle_MSG_BYTE]; i--)
		{
			msg->Data[j+1][i] |= u32_Circletmp;
			u32_Circletmp >>= 8;
		}
	}
	s_CircleCanMsgNum++;
}
void ReadCircleMsg(void)
{
	unsigned char RowError;
	unsigned char i = 0;
	unsigned char CircleTypFlag = 0;
	rewind(fp_CanMsg_txt);
	do
	{
		RowError = RowNoteDel(fp_CanMsg_txt);
		if(RowError == ROW_TEXT)
		{
			if(strncmp(NowRow, "StartCircleTypedef", strlen("CircleTypedefStart")) == 0)
			{
				CircleTypFlag = 1;
			}
			else if(strncmp(NowRow, "EndCircleTypedef", strlen("CircleTypedefEnd")) == 0)
			{
				CircleTypFlag = 0;
			}
			else if(CircleTypFlag == 1)
			{
				CanMsgReadCircle(&s_CircleCanMsg[i++], NowRow);
			}
		}
	}while(RowError != ROW_EOF);
}

#define ID_NUM			(30u)
unsigned int u16_idBuff[ID_NUM][2];
unsigned char u8_idNum = 0;

void CanMsgOutputH(void)
{
	unsigned char i, j;
	fputs("#ifndef __CAN_MSG_H__\n#define __CAN_MSG_H__\n\n", fp_CanMsg_h);
	fputs("#include \"typedef.h\"\n\n", fp_CanMsg_h);

	fprintf(fp_CanMsg_h, "#define MAX_CIRCLE_ANGLE\t\t\t\t(%du)\n\n", Circle_NUM);

	fputs("typedef enum\n{\n", fp_CanMsg_h);
	for (i=0; i<s_LightCanMsgNum; i++)
	{
		if (i == 0)
		{
			fprintf(fp_CanMsg_h, "\t%s = 0,\n", s_LightCanMsg[i].Name);
		} 
		else
		{
			fprintf(fp_CanMsg_h, "\t%s,\n", s_LightCanMsg[i].Name);
		}
	}
	fputs("\tNUM_OF_LIGHT_NUM,\n", fp_CanMsg_h);
	fputs("}Light_ENUM;\n\n", fp_CanMsg_h);

	fputs("typedef struct\n{\n", fp_CanMsg_h);
	fputs("\tUINT16 Id;\n", fp_CanMsg_h);
	fputs("\tUINT16 Time;\n", fp_CanMsg_h);
	fputs("\tUINT8 Data[3][8];\n", fp_CanMsg_h);
	fputs("}LightCanMsg_STRUCT;\n\n", fp_CanMsg_h);

	fprintf(fp_CanMsg_h, "extern const LightCanMsg_STRUCT s_Light[%s];\n\n", "NUM_OF_LIGHT_NUM");


	fputs("typedef enum\n{\n", fp_CanMsg_h);
	for (i=0; i<s_CircleCanMsgNum; i++)
	{

		if (i == 0)
		{
			fprintf(fp_CanMsg_h, "\t%s = 0,\n", s_CircleCanMsg[i].Name);
		} 
		else
		{
			fprintf(fp_CanMsg_h, "\t%s,\n", s_CircleCanMsg[i].Name);
		}
	}
	fputs("\tNUM_OF_Circle_NUM,\n", fp_CanMsg_h);
	fputs("}Circle_ENUM;\n\n", fp_CanMsg_h);

	fputs("typedef struct\n{\n", fp_CanMsg_h);
	fputs("\tUINT16 Id;\n", fp_CanMsg_h);
	fputs("\tUINT16 Time;\n", fp_CanMsg_h);
	fprintf(fp_CanMsg_h, "\tUINT8 Data[%s][8];\n", "MAX_CIRCLE_ANGLE+1");
	fputs("}CircleCanMsg_STRUCT;\n\n", fp_CanMsg_h);
	
	fprintf(fp_CanMsg_h, "extern const CircleCanMsg_STRUCT s_Circle[%s];\n\n", "NUM_OF_Circle_NUM");


	memset(u16_idBuff, 0, ID_NUM);
	fputs("typedef enum\n{\n", fp_CanMsg_h);
	for (i=0; i<s_LightCanMsgNum; i++)
	{
		for(j=0; j<=u8_idNum; j++)
		{
			if(u16_idBuff[j][0] == 0)
			{
				u16_idBuff[u8_idNum][0] = s_LightCanMsg[i].Id;
				u16_idBuff[u8_idNum++][1] = s_LightCanMsg[i].Time;
				if (i == 0)
				{
					fprintf(fp_CanMsg_h, "\tID_0x%x = 0,\n", s_LightCanMsg[i].Id);
				}
				else
				{
					fprintf(fp_CanMsg_h, "\tID_0x%x,\n", s_LightCanMsg[i].Id);
				}
				break;
			}
			else if(u16_idBuff[j][0] == s_LightCanMsg[i].Id)
			{
				break;
			}
		}
	}
	for (i=0; i<s_CircleCanMsgNum; i++)
	{
		for(j=0; j<=u8_idNum; j++)
		{
			if(u16_idBuff[j][0] == 0)
			{
				u16_idBuff[u8_idNum][0] = s_CircleCanMsg[i].Id;
				u16_idBuff[u8_idNum++][1] = s_CircleCanMsg[i].Time;
				fprintf(fp_CanMsg_h, "\tID_0x%x,\n", s_CircleCanMsg[i].Id);
				break;
			}
			else if(u16_idBuff[j][0]  == s_CircleCanMsg[i].Id)
			{
				break;
			}
		}
	}
	fputs("\tNUM_OF_ID_NUM,\n", fp_CanMsg_h);
	fputs("}ID_ENUM;\n\n", fp_CanMsg_h);


	fputs("typedef struct\n{\n", fp_CanMsg_h);
	fputs("\tUINT16 Id;\n", fp_CanMsg_h);
	fputs("\tUINT8 Data[8];\n", fp_CanMsg_h);
	fputs("\tUINT16 Time;\n", fp_CanMsg_h);
	fputs("}CanMsg_STRUCT;\n\n", fp_CanMsg_h);

	fprintf(fp_CanMsg_h, "extern CanMsg_STRUCT s_CanMsgBuff[%s];\n\n", "NUM_OF_ID_NUM");

	fprintf(fp_CanMsg_h, "extern const ID_ENUM e_Light_To_Id[%s];\n", "NUM_OF_LIGHT_NUM");
	fprintf(fp_CanMsg_h, "extern const ID_ENUM e_Circle_To_Id[%s];\n\n", "NUM_OF_Circle_NUM");

	fputs("#endif\n", fp_CanMsg_h);
}
void CanMsgOutputC(void)
{
	unsigned char i, j;
	fputs("#include \"CanMsg.h\"\n", fp_CanMsg_c);
	fputs("#include \"typedef.h\"\n\n", fp_CanMsg_c);

	fprintf(fp_CanMsg_c, "const LightCanMsg_STRUCT s_Light[%s]=\n{\n", "NUM_OF_LIGHT_NUM");
	for (i=0; i<s_LightCanMsgNum; i++)
	{
		fprintf(fp_CanMsg_c, "\t0x%x,%d,/*%s*/\n", s_LightCanMsg[i].Id, s_LightCanMsg[i].Time, s_LightCanMsg[i].Name);
		fprintf(fp_CanMsg_c, "\t0x%x,0x%x,0x%x,0x%x,0x%x,0x%x,0x%x,0x%x,\n", s_LightCanMsg[i].Data[0][0], s_LightCanMsg[i].Data[0][1], \
			s_LightCanMsg[i].Data[0][2], s_LightCanMsg[i].Data[0][3], s_LightCanMsg[i].Data[0][4], s_LightCanMsg[i].Data[0][5],\
			s_LightCanMsg[i].Data[0][6], s_LightCanMsg[i].Data[0][7]);
		fprintf(fp_CanMsg_c, "\t0x%x,0x%x,0x%x,0x%x,0x%x,0x%x,0x%x,0x%x,\n", s_LightCanMsg[i].Data[1][0], s_LightCanMsg[i].Data[1][1], \
			s_LightCanMsg[i].Data[1][2], s_LightCanMsg[i].Data[1][3], s_LightCanMsg[i].Data[1][4], s_LightCanMsg[i].Data[1][5],\
			s_LightCanMsg[i].Data[1][6], s_LightCanMsg[i].Data[1][7]);
		fprintf(fp_CanMsg_c, "\t0x%x,0x%x,0x%x,0x%x,0x%x,0x%x,0x%x,0x%x,\n", s_LightCanMsg[i].Data[2][0], s_LightCanMsg[i].Data[2][1], \
			s_LightCanMsg[i].Data[2][2], s_LightCanMsg[i].Data[2][3], s_LightCanMsg[i].Data[2][4], s_LightCanMsg[i].Data[2][5],\
			s_LightCanMsg[i].Data[2][6], s_LightCanMsg[i].Data[2][7]);
		fputs("\n", fp_CanMsg_c);
	}
	fputs("};\n\n", fp_CanMsg_c);


	fprintf(fp_CanMsg_c, "const CircleCanMsg_STRUCT s_Circle[%s]=\n{\n", "NUM_OF_Circle_NUM");
	for (i=0; i<s_CircleCanMsgNum; i++)
	{
		fprintf(fp_CanMsg_c, "\t0x%x,%d,/*%s*/\n", s_CircleCanMsg[i].Id, s_CircleCanMsg[i].Time, s_CircleCanMsg[i].Name);
		fprintf(fp_CanMsg_c, "\t0x%x,0x%x,0x%x,0x%x,0x%x,0x%x,0x%x,0x%x,\n", s_CircleCanMsg[i].Data[0][0], s_CircleCanMsg[i].Data[0][1], \
			s_CircleCanMsg[i].Data[0][2], s_CircleCanMsg[i].Data[0][3], s_CircleCanMsg[i].Data[0][4], s_CircleCanMsg[i].Data[0][5],\
			s_CircleCanMsg[i].Data[0][6], s_CircleCanMsg[i].Data[0][7]);
		for(j=1; j<=Circle_NUM; j++)
		{
			fprintf(fp_CanMsg_c, "\t0x%x,0x%x,0x%x,0x%x,0x%x,0x%x,0x%x,0x%x,\n", s_CircleCanMsg[i].Data[j][0], s_CircleCanMsg[i].Data[j][1], \
				s_CircleCanMsg[i].Data[j][2], s_CircleCanMsg[i].Data[j][3], s_CircleCanMsg[i].Data[j][4], s_CircleCanMsg[i].Data[j][5],\
				s_CircleCanMsg[i].Data[j][6], s_CircleCanMsg[i].Data[j][7]);
		}
		fputs("\n", fp_CanMsg_c);
	}
	fputs("};\n\n", fp_CanMsg_c);


	fprintf(fp_CanMsg_c, "CanMsg_STRUCT s_CanMsgBuff[%s]=\n{\n", "NUM_OF_ID_NUM");
	for (i=0; i<u8_idNum; i++)
	{
		fprintf(fp_CanMsg_c, "\t0x%x,\n", u16_idBuff[i][0]);
		fprintf(fp_CanMsg_c, "\t0x%x,0x%x,0x%x,0x%x,0x%x,0x%x,0x%x,0x%x,\n", 0, 0, 0, 0, 0, 0, 0, 0);
		fprintf(fp_CanMsg_c, "\t%d,\n", u16_idBuff[i][1]);
		fputs("\n", fp_CanMsg_c);
	}
	fputs("};\n\n", fp_CanMsg_c);


	fprintf(fp_CanMsg_c, "const ID_ENUM e_Light_To_Id[%s]=\n{\n", "NUM_OF_LIGHT_NUM");
	for (i=0; i<s_LightCanMsgNum; i++)
	{
		fprintf(fp_CanMsg_c, "\tID_0x%x,\n",s_LightCanMsg[i].Id);
	}
	fputs("};\n\n", fp_CanMsg_c);
	fprintf(fp_CanMsg_c, "const ID_ENUM e_Circle_To_Id[%s]=\n{\n", "NUM_OF_Circle_NUM");
	for (i=0; i<s_CircleCanMsgNum; i++)
	{
		fprintf(fp_CanMsg_c, "\tID_0x%x,\n",s_CircleCanMsg[i].Id);
	}
	fputs("};\n\n", fp_CanMsg_c);
}

int main(int argc, char *argv[])
{
	char FileList[100];
	char *pFileNameHead;
	strcpy(FileList, argv[0]);
	pFileNameHead = strstr(FileList, "CanMsgCreate.exe");

	*pFileNameHead = '\0';
	strcat(FileList, "CanMsg.txt");
	fp_CanMsg_txt = fopen(FileList,"r");

	*pFileNameHead = '\0';
	strcat(FileList, "CanMsg.h");
	fp_CanMsg_h = fopen(FileList,"w");

	*pFileNameHead = '\0';
	strcat(FileList, "CanMsg.c");
	fp_CanMsg_c = fopen(FileList,"w");

	if(fp_CanMsg_txt == NULL || fp_CanMsg_c == NULL || fp_CanMsg_h == NULL)
	{
		exit(0);
	}
	else
	{
		ReadLightMsg();
		ReadCircleMsg();
		CanMsgOutputH();
		CanMsgOutputC();
		fclose(fp_CanMsg_txt);
		fclose(fp_CanMsg_c);
		fclose(fp_CanMsg_h);
	}
	return 0;
}