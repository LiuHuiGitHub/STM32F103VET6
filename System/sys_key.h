#ifndef __SYS_KEY_H__
#define __SYS_KEY_H__

/*独立按键*/
#define KEY_WKUP			PAin(0)						//高电平有效
#define KEY_S2				PCin(2)
#define KEY_S3				PCin(1)
void sys_keyInit(void);

#endif
