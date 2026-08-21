/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "i2c.h"
#include "tim.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

#include<stdio.h>
#include<string.h>
#include<math.h>
#include "oled.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/*
int note[7][18]=
{
		{},
		{659,698,740,784,831,880,932,988,1047,1109,1175,1245,1319,1397,1480,1568,1661,1760},
		{494,523,554,587,622,659,698,740,784,831,880,932,988,1047,1109,1175,1245,1319},
		{392,415,440,466,494,523,554,587,622,659,698,740,784,831,880,932,988,1047},
		{294,311,330,350,370,392,415,440,466,494,523,554,587,622,659,698,740,784},
		{220,233,247,262,277,294,311,330,350,370,392,415,440,466,494,523,554,587},
		{165,175,185,196,208,220,233,247,262,277,294,311,330,350,370,392,415,440}
};*/
float temptemp1;

typedef enum {
    SYS_INIT,
    MENU_MAIN,
    MODE_PAUSE,
    MODE_SPIN,
    MODE_TARGET,
    MODE_CHEAT,
    MODE_SLEEP,
    SYS_ERROR
} SystemState;

SystemState sysState = SYS_INIT;

char mainmenu1[]="1.Pause";
char mainmenu2[]="2.Spin";
char mainmenu3[]="3.Target";
char mainmenu5[]="5.Cheat";
char mainmenu4[]="4.Sleep";

int tim4counter=-1;
int shuangji=0;
int danji=0;

int test1=0;
int test2=0;

int stateshift;//ec11
int locationstate;//N20
int targetlocationstate;//N20

//测速相关
int vState=0;
int N20counter1;
int timcounter1;
int N20counter2;
int timcounter2;
float realspeed;

int yinfu=1;

int note[49]=
		{0,51428,48648,45918,43269,40909,38626,36437,34351,32490,30612,
		28938,27272,25787,24324,22959,21686,20454,19313,18218,17208,16245,
		15332,14469,13657,12893,12162,11479,10830,10227,9656,9109,8595,8115,
		7659,7228,6823,6442,6081,5739,5418,5113,4825,4554,4300,4059,3831,3615,3412};

int puzi[257]={0,
		// 第17小节：11,11, 13,13, 11,11, 6,6, 11,11,11,11, 11,11,11,11
		11,11, 13,13, 11,11, 6,6, 11,11,11,11, 11,11,11,11,

		// 第18小节：0,0,0,0, 11,11,13,13, 11,11,13,13, 16,16,13,13
		0,0,0,0, 11,11,13,13, 11,11,13,13, 16,16,13,13,

		// 第19小节：11,11,13,13, 11,11,6,6, 9,9,9,9, 9,9,9,9
		11,11,13,13, 11,11,6,6, 9,9,9,9, 9,9,9,9,

		// 第20小节：0,0,0,0, 21,21,21,21, 20,20,16,16, 13,13,13,13
		0,0,0,0, 21,21,21,21, 20,20,16,16, 13,13,13,13,

		// 第21小节：结构同第17小节
		11,11, 13,13, 11,11, 6,6, 11,11,11,11, 11,11,11,11,

		// 第22小节：结构同第18小节
		0,0,0,0, 11,11,13,13, 11,11,13,13, 16,16,13,13,

		// 第23小节：23 21 6 -> 11,11,13,13, 11,11,9,9, 18,18,18,18 （关键区别：结尾是6，不是1）
		11,11,13,13, 11,11,9,9, 6,6,6,6,6,6,6,6,

		// 第24小节：6（低八度）-00 -> 6,6,6,6,6,6,6,6, 0,0,0,0,0,0,0,0 （低音6有延音，输出8次；00休止输出8次）
		6,6,6,6,6,6,6,6, 0,0,0,0,0,0,0,0,

		// 第25小节：2_(高八度)3_（高八度）2_（高八度）6_2（高八度）- -> 23,23, 25,25, 23,23, 18,18, 23,23,23,23,23,23,23,23 （2-有延音，输出8次）
		23,23, 25,25, 23,23, 18,18, 23,23,23,23,23,23,23,23,

		// 第26小节
		0,0,0,0, 23,23, 25,25, 23,23, 25,25, 28,28, 25,25,

		// 第27小节
		23,23, 25,25, 23,23, 18,18, 21,21,21,21,21,21,21,21,

		// 第28小节
		0,0,0,0, 33,33,33,33, 32,32, 28,28, 25,25,25,25,

		// 第29小节
		23,23, 25,25, 23,23, 18,18, 23,23,23,23,23,23,23,23,

		// 第30小节
		0,0,0,0, 23,23, 25,25, 23,23, 25,25, 28,28, 25,25,

		// 第31小节
		23,23, 25,25, 23,23, 21,21, 18,18,18,18,18,18,18,18,

		// 第32小节
		18,18,18,18,18,18,18,18, 0,0,0,0,0,0,0,0
};
//要写谱子了
int yijingbofang=0;

int kaishibofang;

int dii=0;
void play()
{
	if(kaishibofang==1)
	{
		yijingbofang++;
		if(dii!=0)
		{
			if(dii==4)
			{
				dii=0;
			}else
			{
				dii++;
			}
			__HAL_TIM_SET_COMPARE(&htim4,TIM_CHANNEL_4,0.5*__HAL_TIM_GET_AUTORELOAD(&htim4));
		}else if(note[puzi[yijingbofang]]==0)
		{
			__HAL_TIM_SET_COMPARE(&htim4,TIM_CHANNEL_4,0);
		}else
		{
			__HAL_TIM_SET_AUTORELOAD(&htim4,note[puzi[yijingbofang]]);
			__HAL_TIM_SET_COMPARE(&htim4,TIM_CHANNEL_4,note[puzi[yijingbofang]]*0.02);
		}
		if(yijingbofang==256) yijingbofang=0;
	}
}

long tim4auto;/*
void play1(int x,int y,float t)
{
	if(x==7)
	{
		//延长
		HAL_Delay(t*309);
		return;
	}
	long count=9000000/note[x][y];
	long pro=180000/note[x][y];
	__HAL_TIM_SET_AUTORELOAD(&htim4,count);
	tim4auto=count;
	__HAL_TIM_SET_COMPARE(&htim4,TIM_CHANNEL_4,pro);
	if(t>=1)
	{
		HAL_Delay(t*309);//每个八分音符时间
		__HAL_TIM_SET_COMPARE(&htim4,TIM_CHANNEL_4,0);
		HAL_Delay(0);
	}else
	{
		t*=2;
		HAL_Delay(t*155);//每个十六分音符时间
		__HAL_TIM_SET_COMPARE(&htim4,TIM_CHANNEL_4,0);
		HAL_Delay(0);
	}
}*/

/*
void HAL_SYSTICK_Callback(void)
{

	  __HAL_TIM_SET_COMPARE(&htim4,TIM_CHANNEL_4,50);//蜂鸣器响起
	  speeddetect();
	  fm();
	  Keydetect();
}*/

void fm()
{
	  if(dii==0)
	  {
		  play();
	  }else if(dii<=9)
	  {
		  dii++;
		  __HAL_TIM_SET_COMPARE(&htim4,TIM_CHANNEL_4,50);
	  }else if(dii==10)
	  {
		  dii=0;
		  __HAL_TIM_SET_COMPARE(&htim4,TIM_CHANNEL_4,0);
	  }
}

void speedde()
{
	  if(vState==0)
	  {
		  N20counter1=__HAL_TIM_GET_COUNTER(&htim3);
		  timcounter1=__HAL_TIM_GET_COUNTER(&htim4);
		  vState=1;
	  }else if(vState<=3)
	  {vState++;}else
	  {
		  N20counter2=__HAL_TIM_GET_COUNTER(&htim3);
		  timcounter2=__HAL_TIM_GET_COUNTER(&htim4);
		  if(N20counter1>=11000&&N20counter2<=3400)
		  {
			  realspeed=-(N20counter2+13860-N20counter1);
		  }else if(N20counter2>=11000&&N20counter1<=3400)
		  {
			  realspeed=(N20counter1+13860-N20counter2);
		  }else
		  {
			  realspeed=-(N20counter2-N20counter1);
		  }
		  float timtemp;
		  if(timcounter1>=40000&&timcounter2<=10000)
		  {
			  timtemp=timcounter2-timcounter1+50000;
		  }else
		  {
			  timtemp=timcounter2-timcounter1;
		  }

		  realspeed=realspeed/693/timtemp*300000;
		  vState=0;

		  //显示speed
		  showspeed();
		  //OLED_ShowFrame();
	  }
}
extern int sec;
extern int msec;

int seccounter1;
int mseccounter1;
int mmsec1;
int seccounter2;
int mseccounter2;
int mmsec2;
void speeddetect()
{
	  if(vState==0)
	  {
		  N20counter1=__HAL_TIM_GET_COUNTER(&htim3);
		  seccounter1=sec;
		  mseccounter1=msec;
		  mmsec1=HAL_GetTick();
		  vState=1;
	  }else if(vState<=1)
	  {vState++;}else
	  {
		  N20counter2=__HAL_TIM_GET_COUNTER(&htim3);
		  seccounter2=sec;
		  mseccounter2=msec;
		  mmsec2=HAL_GetTick();
		  if(N20counter1>=11000&&N20counter2<=3400)
		  {
			  realspeed=-(N20counter2+13860-N20counter1);
		  }else if(N20counter2>=11000&&N20counter1<=3400)
		  {
			  realspeed=(N20counter1+13860-N20counter2);
		  }else
		  {
			  realspeed=-(N20counter2-N20counter1);
		  }

		  float timtemp;
		  timtemp=(seccounter2-seccounter1)*1000+(mseccounter2-mseccounter1);

		  realspeed=realspeed/693/timtemp*30000;
		  vState=0;

		  //显示speed
		  showspeed();
		  //OLED_ShowFrame();
	  }
}

int a;
void speedbihuan(int stemp)
{
	if(stemp<=0)
	{
		stemp*=-1;
	}
	if(a<=3)
	{
		a++;
	}else
	{
		int realspeedi=realspeed;
		if(realspeed<=0)
		{
			realspeedi*=-1;
		}
		int scha=realspeedi-stemp;
		if(scha>=-8&&scha<=8)
		{
			if(scha<=-1||scha>=1)
			{
				__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_3,(__HAL_TIM_GET_COMPARE(&htim2,TIM_CHANNEL_3)-scha*8/3));
			}
		}
	}
}

void speedbihuan2(int stemp)
{
	if(stemp<=0)
	{
		stemp*=-1;
	}
	if(a<=30)
	{
		a++;
	}else
	{
		int realspeedi=realspeed;
		if(realspeed<=0)
		{
			realspeedi*=-1;
		}
		int scha=realspeedi-stemp;
		if(scha>=-20&&scha<=20)
		{
			if(scha<=-1||scha>=1)
			{
				__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_3,(__HAL_TIM_GET_COMPARE(&htim2,TIM_CHANNEL_3)-scha*2/3));
			}
		}
	}
}

void speedfail()
{
	  if(vState==0)
	  {
		  N20counter1=__HAL_TIM_GET_COUNTER(&htim3);
		  vState=1;
	  }else if(vState<=0)
	  {vState++;}else
	  {
		  N20counter2=__HAL_TIM_GET_COUNTER(&htim3);
		  if(N20counter1>=11000&&N20counter2<=3400)
		  {
			  realspeed=-(N20counter2+13860-N20counter1);
		  }else if(N20counter2>=11000&&N20counter1<=3400)
		  {
			  realspeed=(N20counter1+13860-N20counter2);
		  }else
		  {
			  realspeed=-(N20counter2-N20counter1);
		  }
		  float timtemp=1.0;
		  realspeed=realspeed/1386/timtemp*600;
		  vState=0;

		  //显示speed
		  showspeed();
		  //OLED_ShowFrame();
	  }
}

//圈数相关
int setround;
int realround=-1;

int mubiao=10;
int cha;

//位置相关
double realn20=0;
int realarea=1;
char realareaout[3];

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
void stoph()
{
	  HAL_GPIO_WritePin(fu_GPIO_Port, fu_Pin, RESET);
	  HAL_GPIO_WritePin(zheng_GPIO_Port, zheng_Pin, RESET);
}

void stops()
{
	  HAL_GPIO_WritePin(fu_GPIO_Port, fu_Pin, SET);
	  HAL_GPIO_WritePin(zheng_GPIO_Port, zheng_Pin, SET);
}

void showspeed()
{
	char realspeedout[20];
	sprintf(realspeedout,"V = %.1f",realspeed);
	OLED_PrintString(3, 36, realspeedout, &font16x16, OLED_COLOR_NORMAL);
}

void clearlocation()
{
	OLED_SetPixel(105+18*cos(realn20), 39-18*sin(realn20), OLED_COLOR_REVERSED);
	OLED_SetPixel(105+17.5*cos(realn20), 39-17.5*sin(realn20), OLED_COLOR_REVERSED);
	OLED_SetPixel(105+17*cos(realn20), 39-17*sin(realn20), OLED_COLOR_REVERSED);
	OLED_SetPixel(105+16.5*cos(realn20), 39-16.5*sin(realn20), OLED_COLOR_REVERSED);
	OLED_SetPixel(105+16*cos(realn20), 39-16*sin(realn20), OLED_COLOR_REVERSED);
	char spitempshowlocation[]="  ";
	OLED_PrintString(100, 30, spitempshowlocation, &font16x16, OLED_COLOR_NORMAL);
}

void showlocation()
{
	OLED_SetPixel(105+18*cos(realn20), 39-18*sin(realn20), OLED_COLOR_REVERSED);
	OLED_SetPixel(105+17.5*cos(realn20), 39-17.5*sin(realn20), OLED_COLOR_REVERSED);
	OLED_SetPixel(105+17*cos(realn20), 39-17*sin(realn20), OLED_COLOR_REVERSED);
	OLED_SetPixel(105+16.5*cos(realn20), 39-16.5*sin(realn20), OLED_COLOR_REVERSED);
	OLED_SetPixel(105+16*cos(realn20), 39-16*sin(realn20), OLED_COLOR_REVERSED);

	realn20=__HAL_TIM_GET_COUNTER(&htim3)%1386;
	realarea=1+realn20/39;
	sprintf(realareaout,"%d",realarea);
	char tempshowlocation[]="  ";
	OLED_PrintString(100, 30, tempshowlocation, &font16x16, OLED_COLOR_NORMAL);
	OLED_PrintString(100, 30, realareaout, &font16x16, OLED_COLOR_NORMAL);

	realn20=realn20*3.1415926535/693;

	OLED_SetPixel(105+18*cos(realn20), 39-18*sin(realn20), OLED_COLOR_NORMAL);
	OLED_SetPixel(105+17.5*cos(realn20), 39-17.5*sin(realn20), OLED_COLOR_NORMAL);
	OLED_SetPixel(105+17*cos(realn20), 39-17*sin(realn20), OLED_COLOR_NORMAL);
	OLED_SetPixel(105+16.5*cos(realn20), 39-16.5*sin(realn20), OLED_COLOR_NORMAL);
	OLED_SetPixel(105+16*cos(realn20), 39-16*sin(realn20), OLED_COLOR_NORMAL);
	OLED_DrawCircle(105, 39, 20, OLED_COLOR_NORMAL);

	//showspeed();

	//OLED_ShowFrame();
}

void Keyde()
{
	  if(tim4counter!=-1)
	  {
	  		if(tim4counter+4000<=__HAL_TIM_GET_COUNTER(&htim4))
	  		{
	  			danji=1;
	  			tim4counter=-1;
	  		}else if(tim4counter>=45999&&tim4counter+4000>=__HAL_TIM_GET_COUNTER(&htim4)+49999)
	  		{
	  			danji=1;
	  			tim4counter=-1;
	  		}
	  }

	  if(HAL_GPIO_ReadPin(Key_GPIO_Port, Key_Pin)==GPIO_PIN_RESET)
	  {
	  		HAL_Delay(10);
	  		while(HAL_GPIO_ReadPin(Key_GPIO_Port, Key_Pin)==GPIO_PIN_RESET);
	  		HAL_Delay(10);
	  		if(tim4counter==-1)
	  		{
	  			tim4counter=__HAL_TIM_GET_COUNTER(&htim4);
	  		}
	  		else
	  		{
	  			shuangji=1;
	  			tim4counter=-1;
	  		}
	  }
}

int keyi=0;
int secqian;
int msecqian;

void Keydetect()
{
	  if(keyi!=0)
	  {
		  float dtime=(sec-secqian)*1000+(msec-msecqian);
	  		if(dtime>=400)
	  		{
	  			danji=1;
	  			keyi=0;
	  		}
	  }

	  if(HAL_GPIO_ReadPin(Key_GPIO_Port, Key_Pin)==GPIO_PIN_RESET)
	  {
	  		HAL_Delay(10);
	  		while(HAL_GPIO_ReadPin(Key_GPIO_Port, Key_Pin)==GPIO_PIN_RESET);
	  		HAL_Delay(10);
	  		if(keyi==0)
	  		{
	  			secqian=sec;
	  			msecqian=msec;
	  			keyi=1;
	  		}
	  		else
	  		{
	  			shuangji=1;
	  			keyi=0;
	  		}
	  }
}

void Keyfail()
{

	  if(keyi!=0)
	  {
		  keyi++;
	  		if(keyi==4)
	  		{
	  			danji=1;
	  			keyi=0;
	  		}
	  }

	  if(HAL_GPIO_ReadPin(Key_GPIO_Port, Key_Pin)==GPIO_PIN_RESET)
	  {
	  		HAL_Delay(10);
	  		while(HAL_GPIO_ReadPin(Key_GPIO_Port, Key_Pin)==GPIO_PIN_RESET);
	  		HAL_Delay(10);
	  		if(keyi==0)
	  		{
	  			keyi=1;
	  		}
	  		else
	  		{
	  			shuangji=1;
	  			keyi=0;
	  		}
	  }
}

void winsys_Init()
{
	char qidong[]="starting";
	OLED_PrintString(13, 0, qidong, &font16x16, OLED_COLOR_NORMAL);
	OLED_ShowFrame();

	HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_RESET);
	HAL_Delay(500);

	HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_SET);
	HAL_Delay(500);

	HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_RESET);
	HAL_Delay(500);

	HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_SET);
	HAL_Delay(500);

	HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_RESET);
	HAL_Delay(500);

	HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_SET);

	__HAL_TIM_SET_COMPARE(&htim4,TIM_CHANNEL_4,0.2*__HAL_TIM_GET_AUTORELOAD(&htim4));
	HAL_Delay(400);
	__HAL_TIM_SET_COMPARE(&htim4,TIM_CHANNEL_4,0);

}
void timtest(int a)
{
	char message[20];
	char temp[]="      ";
	int counter;
	if(a==1) counter=__HAL_TIM_GET_COUNTER(&htim1);
	if(a==2) counter=__HAL_TIM_GET_COUNTER(&htim2);
	if(a==3) counter=__HAL_TIM_GET_COUNTER(&htim3);
	if(a==4) counter=__HAL_TIM_GET_COUNTER(&htim4);

	OLED_PrintString(13, 51, temp, &font16x16, OLED_COLOR_NORMAL);

	sprintf(message,"C=%d",counter);
	OLED_PrintString(13, 51, message, &font16x16, OLED_COLOR_NORMAL);
}


/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_I2C2_Init();
  MX_TIM1_Init();
  MX_TIM2_Init();
  MX_TIM4_Init();
  MX_TIM3_Init();
  /* USER CODE BEGIN 2 */

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	  switch(sysState)
	  {
	  case SYS_INIT:

		  kaishibofang=0;

		  HAL_Delay(50);

		  OLED_Init();
		  OLED_NewFrame();
		  OLED_ShowFrame();

		  HAL_TIM_Encoder_Start(&htim1, TIM_CHANNEL_ALL);
		  HAL_TIM_Encoder_Start(&htim3, TIM_CHANNEL_ALL);
		  HAL_TIM_Base_Start(&htim4);
		  HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_3);
		  HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_4);

		  winsys_Init();

		  char INIT1[]="Please move the";
		  char INIT2[]="pointer to point";
		  char INIT3[]="betw area 1 & 36";
		  char INIT4[]="press the key";
		  OLED_NewFrame();
		  OLED_PrintString(0, 0, INIT1, &font16x16, OLED_COLOR_NORMAL);
		  OLED_PrintString(0, 15, INIT2, &font16x16, OLED_COLOR_NORMAL);
		  OLED_PrintString(0, 31, INIT3, &font16x16, OLED_COLOR_NORMAL);
		  OLED_PrintString(0, 46, INIT4, &font16x16, OLED_COLOR_NORMAL);
		  OLED_ShowFrame();
		  while(1)
		  {
			  Keydetect();
			  if(danji)
			  {

				  kaishibofang=1;
				  danji=0;
				  break;
			  }
		  }
		  __HAL_TIM_SET_COUNTER(&htim3,0);
		  sysState = MENU_MAIN;

		  break;

	  case MENU_MAIN:
		  int meuState = 0;
		  stateshift=__HAL_TIM_GET_COUNTER(&htim1);
		  int meucounter;
		  while(1)
		  {
			  OLED_NewFrame();
			  OLED_PrintString(10, 0, mainmenu1, &font16x16, (meuState==0?OLED_COLOR_REVERSED:OLED_COLOR_NORMAL));
			  OLED_PrintString(10, 16, mainmenu2, &font16x16, (meuState==1?OLED_COLOR_REVERSED:OLED_COLOR_NORMAL));
			  OLED_PrintString(10, 32, mainmenu3, &font16x16, (meuState==2?OLED_COLOR_REVERSED:OLED_COLOR_NORMAL));
			  OLED_PrintString(10, 48, mainmenu4, &font16x16, (meuState==3?OLED_COLOR_REVERSED:OLED_COLOR_NORMAL));

			  char mentem1[]="C";
			  char mentem2[]="H";
			  char mentem3[]="E";
			  char mentem4[]="A";
			  char mentem5[]="T";
			  OLED_PrintString(90, 0, mentem1, &font16x16, (meuState==4?OLED_COLOR_REVERSED:OLED_COLOR_NORMAL));
			  OLED_PrintString(90, 13, mentem2, &font16x16, (meuState==4?OLED_COLOR_REVERSED:OLED_COLOR_NORMAL));
			  OLED_PrintString(90, 26, mentem3, &font16x16, (meuState==4?OLED_COLOR_REVERSED:OLED_COLOR_NORMAL));
			  OLED_PrintString(90, 40, mentem4, &font16x16, (meuState==4?OLED_COLOR_REVERSED:OLED_COLOR_NORMAL));
			  OLED_PrintString(90, 52, mentem5, &font16x16, (meuState==4?OLED_COLOR_REVERSED:OLED_COLOR_NORMAL));

			  meucounter=__HAL_TIM_GET_COUNTER(&htim1);
			  if(stateshift>=0&&stateshift<=1000&&meucounter>=4000)
			  {
				  meuState+=(5-1);
				  meuState%=5;
				  HAL_Delay(100);
				  stateshift=__HAL_TIM_GET_COUNTER(&htim1);
			  }else if(stateshift<meucounter)
			  {
				  meuState+=1;
				  meuState%=5;
				  HAL_Delay(100);
				  stateshift=__HAL_TIM_GET_COUNTER(&htim1);
			  }else if(stateshift>=4000&&meucounter<=1000&&meucounter>=0)
			  {
				  meuState+=1;
				  meuState%=5;
				  HAL_Delay(100);
				  stateshift=__HAL_TIM_GET_COUNTER(&htim1);
			  }else if(stateshift>meucounter)
			  {
				  meuState+=(5-1);
				  meuState%=5;
				  HAL_Delay(100);
				  stateshift=__HAL_TIM_GET_COUNTER(&htim1);
			  }
			  OLED_ShowFrame();

			  Keydetect();

			  if(danji)
			  {
				  switch(meuState)
				  {
				  case 0:
					  sysState=MODE_PAUSE;
					  break;
				  case 1:
					  sysState=MODE_SPIN;
					  break;
				  case 2:
					  sysState=MODE_TARGET;
					  break;
				  case 3:
					  sysState=MODE_SLEEP;
					  break;
				  case 4:
					  sysState=MODE_CHEAT;
					  break;
				  }
				  __HAL_TIM_SET_COUNTER(&htim1,0);
				  danji=0;
			  }

			  if(shuangji)
			  {
				  sysState = SYS_INIT;
				  shuangji=0;
			  }

			  if(sysState!=MENU_MAIN)
			  {
				  break;
			  }
		  }
		  break;

	  case MODE_PAUSE:

		  char pause1[]="SPEED =      r/m";
		  OLED_NewFrame();
		  OLED_PrintString(0, 0, pause1, &font16x16, (OLED_COLOR_NORMAL));
		  OLED_ShowFrame();

		  int pauState = 0;
		  stateshift=__HAL_TIM_GET_COUNTER(&htim1);//速度调节调用
		  locationstate=__HAL_TIM_GET_COUNTER(&htim3);//位置调用
		  int paucounter;
		  float pauspeed=0;

		  while(sysState==MODE_PAUSE)
		  {
			  //timtest(3);
			  showlocation();
			  if(pauState!=0)
			  {
				  speeddetect();
				  speedbihuan(pauspeed);
			  }else
			  {
				  //遮住速度
			  }
			  OLED_ShowFrame();

			  if(pauState==0)
			  {
				  Keydetect();
				  paucounter=__HAL_TIM_GET_COUNTER(&htim1);
				  if(stateshift>=0&&stateshift<=1000&&paucounter>=4000)
				  {
					  pauspeed-=(stateshift+12000-paucounter)*2.5;
					  stateshift=paucounter;//逆时针
				  }else if(stateshift<paucounter)
				  {
					  pauspeed+=(paucounter-stateshift)*5;
					  stateshift=paucounter;
				  }else if(stateshift>=4000&&paucounter<=1000&&paucounter>=0)
				  {
					  pauspeed+=(paucounter+12000-stateshift)*2.5;
					  stateshift=paucounter;
				  }else if(stateshift>paucounter)
				  {
					  pauspeed-=(stateshift-paucounter)*2.5;
					  stateshift=paucounter;//逆时针
				  }

				  if(pauspeed<-300)
				  {
					  pauspeed=-300;
				  }else
				  {
					  if(pauspeed>300)
					  {
						  pauspeed=300;
					  }
				  }
				  int pauspeedint=pauspeed;
				  char pauspeedout[5];
				  sprintf(pauspeedout,"%d",pauspeedint);

				  OLED_PrintString(0, 0, pause1, &font16x16, (OLED_COLOR_NORMAL));
				  OLED_PrintString(63, 0, pauspeedout, &font16x16, OLED_COLOR_REVERSED);
				  OLED_ShowFrame();

				  if(shuangji)
				  {
					  shuangji=0;
					  sysState=MENU_MAIN;

				  }else if(danji)
				  {
					  danji=0;
					  OLED_PrintString(63, 0, pauspeedout, &font16x16, OLED_COLOR_NORMAL);
					  OLED_ShowFrame();
					  if(pauspeed>0)
					  {
						  temptemp1=0.96551*pauspeed+5.4;
						  __HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_3,(temptemp1*10/3)-1);
						  HAL_GPIO_WritePin(zheng_GPIO_Port, zheng_Pin, SET);
						  HAL_GPIO_WritePin(fu_GPIO_Port, fu_Pin, RESET);
						  pauState=1;
						  a=0;
					  }else if(pauspeed<0)
					  {
						  temptemp1=0.96551*pauspeed-5.4;
						  __HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_3,(-temptemp1*10/3)-1);
						  HAL_GPIO_WritePin(zheng_GPIO_Port, zheng_Pin, RESET);
						  HAL_GPIO_WritePin(fu_GPIO_Port, fu_Pin, SET);
						  pauState=1;
						  a=0;
					  }
				  }

			  }else
			  {
				  Keydetect();
				  if(danji||shuangji)
				  {
					  HAL_GPIO_WritePin(fu_GPIO_Port, fu_Pin, SET);
					  HAL_GPIO_WritePin(zheng_GPIO_Port, zheng_Pin, SET);

					  danji=0;
					  shuangji=0;
					  pauState=0;
				  }
			  }
			  if(sysState!=MODE_PAUSE)
			  {
				  break;
			  }
		  }
		  break;

	  case MODE_SPIN:
		  int spiState = 0;//0=设定v 1=设定圈 2=电机启动到自然停下 3=finish
		  stateshift=__HAL_TIM_GET_COUNTER(&htim1);//速度调节调用
		  locationstate=__HAL_TIM_GET_COUNTER(&htim3);//位置调用
		  int spicounter;//计数器比较
		  float spispeed=0;
		  float spiround=0;
		  char spispeedout[5];
		  char spiroundout[5];

		  OLED_NewFrame();
		  char spin1[]="SPEED =     r/m";
		  OLED_PrintString(3, 0, spin1, &font16x16, OLED_COLOR_NORMAL);
		  char spin2[]="SPIN     R";
		  OLED_PrintString(3, 17, spin2, &font16x16, OLED_COLOR_NORMAL);
		  sprintf(spiroundout,"%d",spiround);
		  OLED_PrintString(44, 17, spiroundout, &font16x16, OLED_COLOR_NORMAL);
		  OLED_ShowFrame();

		  while(1)
		  {
			  //指针区域显示

			  showlocation();
			  switch(spiState)
			  {
			  case 0:

				  //ec11速度旋转调节

				  spicounter=__HAL_TIM_GET_COUNTER(&htim1);
				  if(stateshift>=0&&stateshift<=1000&&spicounter>=4000)
				  {
					  spispeed-=(stateshift+12000-spicounter)*2.5;
					  stateshift=spicounter;//逆时针
				  }else if(stateshift<spicounter)
				  {
					  spispeed+=(spicounter-stateshift)*5;
					  stateshift=spicounter;
				  }else if(stateshift>=4000&&spicounter<=1000&&spicounter>=0)
				  {
					  spispeed+=(spicounter+12000-stateshift)*2.5;
					  stateshift=spicounter;
				  }else if(stateshift>spicounter)
				  {
					  spispeed-=(stateshift-spicounter)*2.5;
					  stateshift=spicounter;//逆时针
				  }

				  if(spispeed<-300)
				  {
					  spispeed=-300;
				  }else
				  {
					  if(spispeed>300)
					  {
						  spispeed=300;
					  }
				  }
				  int spispeedint=spispeed;
				  sprintf(spispeedout,"%d",spispeedint);

				  OLED_PrintString(3, 0, spin1, &font16x16, OLED_COLOR_NORMAL);

				  OLED_PrintString(64, 0, spispeedout, &font16x16, OLED_COLOR_REVERSED);
				  OLED_ShowFrame();

				  if(danji)
				  {
					  spiState=1;
					  danji=0;
				  }else if(shuangji)
				  {
					  sysState=MENU_MAIN;
					  shuangji=0;
				  }

				  break;
			  case 1:

				  //ec11圈数旋转调节

				  spicounter=__HAL_TIM_GET_COUNTER(&htim1);
				  if(stateshift>=0&&stateshift<=1000&&spicounter>=4000)
				  {
					  spiround-=(stateshift+12000-spicounter)*0.5;
					  stateshift=spicounter;//逆时针
				  }else if(stateshift<spicounter)
				  {
					  spiround+=(spicounter-stateshift)*0.5;
					  stateshift=spicounter;
				  }else if(stateshift>=4000&&spicounter<=1000&&spicounter>=0)
				  {
					  spiround+=(spicounter+12000-stateshift)*0.5;
					  stateshift=spicounter;
				  }else if(stateshift>spicounter)
				  {
					  spiround-=(stateshift-spicounter)*0.5;
					  stateshift=spicounter;//逆时针
				  }

				  if(spiround<0)
				  {
					  spiround=0;
				  }else
				  {
					  if(spiround>999)
					  {
						  spiround=999;
					  }
				  }
				  int spiroundint=spiround;
				  sprintf(spiroundout,"%d",spiroundint);

				  OLED_PrintString(3, 0, spin1, &font16x16, OLED_COLOR_NORMAL);
				  OLED_PrintString(3, 17, spin2, &font16x16, OLED_COLOR_NORMAL);

				  OLED_PrintString(64, 0, spispeedout, &font16x16, OLED_COLOR_NORMAL);
				  OLED_PrintString(44, 17, spiroundout, &font16x16, OLED_COLOR_REVERSED);
				  OLED_ShowFrame();

				  if(danji)
				  {
					  spiState=2;
					  danji=0;
				  }else if(shuangji)
				  {
					  spiState=0;
					  shuangji=0;
				  }

				  break;
			  case 2:

				  OLED_PrintString(44, 17, spiroundout, &font16x16, OLED_COLOR_NORMAL);

				  char spitem1[]="spinning";
				  //OLED_PrintString(3, 34, spitem1, &font16x16, OLED_COLOR_NORMAL);
				  OLED_ShowFrame();

				  setround=spiroundint;

				  //启动电机
				  if(spispeed>0&&setround>=1)
				  {
					  temptemp1=0.96551*spispeed+5.4;
					  __HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_3,(temptemp1*10/3)-1);
					  HAL_GPIO_WritePin(zheng_GPIO_Port, zheng_Pin, SET);
					  HAL_GPIO_WritePin(fu_GPIO_Port, fu_Pin, RESET);
				  }else if(spispeed<0&&setround>=1)
				  {
					  temptemp1=0.96551*spispeed-5.4;
					  __HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_3,(-temptemp1*10/3)-1);
					  HAL_GPIO_WritePin(zheng_GPIO_Port, zheng_Pin, RESET);
					  HAL_GPIO_WritePin(fu_GPIO_Port, fu_Pin, SET);
				  }
				  HAL_Delay(50);

				  a=0;

				  speeddetect();
				  realround=0;
				  locationstate=__HAL_TIM_GET_COUNTER(&htim3);

				  clearlocation();
				  speeddetect();

				  HAL_Delay(50);
				  speeddetect();
				  while(1)
				  {

					  if(vState==0)
					  {
						  N20counter1=__HAL_TIM_GET_COUNTER(&htim3);
						  seccounter1=sec;
						  mseccounter1=msec;
						  mmsec1=HAL_GetTick();
						  vState=1;
					  }else if(vState<=1)
					  {vState++;}else
					  {
						  N20counter2=__HAL_TIM_GET_COUNTER(&htim3);
						  seccounter2=sec;
						  mseccounter2=msec;
						  mmsec2=HAL_GetTick();
						  if(N20counter1>=11000&&N20counter2<=3400)
						  {
							  realspeed=-(N20counter2+13860-N20counter1);
						  }else if(N20counter2>=11000&&N20counter1<=3400)
						  {
							  realspeed=(N20counter1+13860-N20counter2);
						  }else
						  {
							  realspeed=-(N20counter2-N20counter1);
						  }

						  float timtemp;
						  timtemp=(seccounter2-seccounter1)*1000+(mseccounter2-mseccounter1);

						  realspeed=realspeed/693/timtemp*30000;
						  vState=0;

						  //显示speed
						  //OLED_ShowFrame();
					  }
					  speedbihuan2(spispeed);

					  cha=__HAL_TIM_GET_COUNTER(&htim3)%1386-locationstate%1386;
					  if(cha<=mubiao&&cha>=-mubiao)
					  {
						  OLED_ShowFrame();
						  realround++;
						  if(setround==realround)
						  {
							  break;
						  }
						  speeddetect();
						  HAL_Delay(50);
						  //showlocation();
					  }
				  }
				  //已经到达buzzer


					__HAL_TIM_SET_COMPARE(&htim4,TIM_CHANNEL_4,0.5*__HAL_TIM_GET_AUTORELOAD(&htim4));
				  stoph();
				  dii=1;

				  char spitem2[]="          ";
				  OLED_PrintString(3, 34, spitem2, &font16x16, OLED_COLOR_NORMAL);
				  OLED_ShowFrame();

				  spiState=3;

				  break;

			  case 3:

				  char spitem3[]="Finish";
				  OLED_PrintString(3, 34, spitem3, &font16x16, OLED_COLOR_NORMAL);
				  OLED_ShowFrame();

				  if(danji||shuangji)
				  {
					  spiState=0;
					  danji=0;
					  shuangji=0;

					  char spitem4[]="      ";
					  OLED_PrintString(3, 34, spitem4, &font16x16, OLED_COLOR_NORMAL);
				  }

				  break;
			  }

			  Keydetect();

			  if(sysState!=MODE_SPIN)
			  {
				  break;
			  }
		  }
		  break;
	  case MODE_TARGET:
		  int tarState = 1;
		  stateshift=__HAL_TIM_GET_COUNTER(&htim1);//速度调节调用
		  locationstate=__HAL_TIM_GET_COUNTER(&htim3);//位置调用
		  int tarcounter;//计数器比较
		  int tarspeed=0;
		  int tarround=0;
		  int tarlocation=0;
		  char tarspeedout[5];
		  char tarroundout[5];
		  char tarlocationout[]="1";
		  char tartemp[]="   ";
		  int e,f;

		  OLED_NewFrame();
		  //char tar1[]="SPEED =     r/m";
		  //OLED_PrintString(3, 0, tar1, &font16x16, OLED_COLOR_NORMAL);
		  char tar2[]="SPIN     R";
		  OLED_PrintString(0, 0, tar2, &font16x16, OLED_COLOR_NORMAL);
		  char tar3[]="   AREA";
		  OLED_PrintString(0, 17, tar3, &font16x16, OLED_COLOR_NORMAL);
		  sprintf(tarspeedout,"%d",tarspeed);
		  sprintf(tarroundout,"%d",tarround);

		  //OLED_PrintString(66, 0, tarspeedout, &font16x16, OLED_COLOR_NORMAL);
		  OLED_PrintString(41, 0, tarroundout, &font16x16, OLED_COLOR_NORMAL);
		  OLED_PrintString(0, 17, tarlocationout, &font16x16, OLED_COLOR_NORMAL);
		  OLED_ShowFrame();

		  while(1)
		  {

			  //指针区域显示

			  showlocation();
			  switch(tarState)
			  {
			  case 0:

				  //ec11速度旋转调节

				  tarcounter=__HAL_TIM_GET_COUNTER(&htim1);
				  if(stateshift>=0&&stateshift<=1000&&tarcounter>=4000)
				  {
					  tarspeed-=(stateshift+12000-tarcounter)*5;
					  stateshift=tarcounter;//逆时针
				  }else if(stateshift<tarcounter)
				  {
					  tarspeed+=(tarcounter-stateshift)*5;
					  stateshift=tarcounter;
				  }else if(stateshift>=4000&&tarcounter<=1000&&tarcounter>=0)
				  {
					  tarspeed+=(tarcounter+12000-stateshift)*5;
					  stateshift=tarcounter;
				  }else if(stateshift>tarcounter)
				  {
					  tarspeed-=(stateshift-tarcounter)*5;
					  stateshift=tarcounter;//逆时针
				  }

				  if(tarspeed<-300)
				  {
					  tarspeed=-300;
				  }else
				  {
					  if(tarspeed>300)
					  {
						  tarspeed=300;
					  }
				  }
				  sprintf(tarspeedout,"%d",tarspeed);

				  OLED_PrintString(64, 0, tartemp, &font16x16, OLED_COLOR_NORMAL);
				  OLED_PrintString(64, 0, tarspeedout, &font16x16, OLED_COLOR_REVERSED);
				  OLED_PrintString(44, 17, tarroundout, &font16x16, OLED_COLOR_NORMAL);
				  OLED_PrintString(44, 34, tarlocationout, &font16x16, OLED_COLOR_NORMAL);
				  OLED_ShowFrame();

				  if(danji)
				  {
					  tarState=1;
					  danji=0;
				  }else if(shuangji)
				  {
					  sysState=MENU_MAIN;
					  shuangji=0;
				  }

				  break;
			  case 1:

				  //ec11圈数旋转调节
				  tarcounter=__HAL_TIM_GET_COUNTER(&htim1);
				  if(stateshift>=0&&stateshift<=1000&&tarcounter>=4000)
				  {
					  tarround-=(stateshift+12000-tarcounter)*0.5;
					  stateshift=tarcounter;//逆时针
				  }else if(stateshift<tarcounter)
				  {
					  tarround+=(tarcounter-stateshift)*0.5;
					  stateshift=tarcounter;
				  }else if(stateshift>=4000&&tarcounter<=1000&&tarcounter>=0)
				  {
					  tarround+=(tarcounter+12000-stateshift)*0.5;
					  stateshift=tarcounter;
				  }else if(stateshift>tarcounter)
				  {
					  tarround-=(stateshift-tarcounter)*0.5;
					  stateshift=tarcounter;//逆时针
				  }

				  if(tarround<0)
				  {
					  tarround=0;
				  }else
				  {
					  if(tarround>999)
					  {
						  tarround=999;
					  }
				  }
				  int tarroundint=tarround;
				  sprintf(tarroundout,"%d",tarroundint);

				  OLED_PrintString(41, 0, tartemp, &font16x16, OLED_COLOR_NORMAL);
				  //OLED_PrintString(64, 0, tarspeedout, &font16x16, OLED_COLOR_NORMAL);
				  OLED_PrintString(41, 0, tarroundout, &font16x16, OLED_COLOR_REVERSED);
				  OLED_PrintString(0, 17, tarlocationout, &font16x16, OLED_COLOR_NORMAL);
				  OLED_ShowFrame();

				  if(danji)
				  {
					  tarState=2;
					  danji=0;
					  setround=tarround;
				  }else if(shuangji)
				  {
					  sysState=MENU_MAIN;
					  shuangji=0;
				  }

				  break;
			  case 2:

				  //ec11区域旋转调节
				  tarcounter=__HAL_TIM_GET_COUNTER(&htim1);
				  if(stateshift>=0&&stateshift<=1000&&tarcounter>=4000)
				  {
					  tarlocation-=(stateshift+12000-tarcounter)*0.5;
					  stateshift=tarcounter;//逆时针
				  }else if(stateshift<tarcounter)
				  {
					  tarlocation+=(tarcounter-stateshift)*0.5;
					  stateshift=tarcounter;
				  }else if(stateshift>=4000&&tarcounter<=1000&&tarcounter>=0)
				  {
					  tarlocation+=(tarcounter+12000-stateshift)*0.5;
					  stateshift=tarcounter;
				  }else if(stateshift>tarcounter)
				  {
					  tarlocation-=(stateshift-tarcounter)*0.5;
					  stateshift=tarcounter;//逆时针
				  }

				  if(tarlocation<1)
				  {
					  tarlocation=1;
				  }else
				  {
					  if(tarlocation>36)
					  {
						  tarlocation=36;
					  }
				  }
				  int tarlocationint=tarlocation;
				  sprintf(tarlocationout,"%d",tarlocationint);

				  //OLED_PrintString(64, 0, tarspeedout, &font16x16, OLED_COLOR_NORMAL);
				  OLED_PrintString(0, 17, tartemp, &font16x16, OLED_COLOR_NORMAL);
				  OLED_PrintString(41, 0, tarroundout, &font16x16, OLED_COLOR_NORMAL);
				  OLED_PrintString(0, 17, tarlocationout, &font16x16, OLED_COLOR_REVERSED);
				  OLED_ShowFrame();

				  if(danji)
				  {
					  tarState=3;
					  danji=0;
				  }else if(shuangji)
				  {
					  tarState=1;
					  shuangji=0;
				  }

				  break;
			  case 3:

				  OLED_PrintString(0, 17, tarlocationout, &font16x16, OLED_COLOR_NORMAL);

				  char tartem1[]="spinning";
				  OLED_PrintString(0, 36, tartem1, &font16x16, OLED_COLOR_NORMAL);
				  OLED_ShowFrame();

				  //启动电机

				  __HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_3,299);

				  HAL_GPIO_WritePin(zheng_GPIO_Port, zheng_Pin, RESET);
				  HAL_GPIO_WritePin(fu_GPIO_Port, fu_Pin, SET);
				  //显示正在转动

				  clearlocation();
				  realround=-1;
				  locationstate=__HAL_TIM_GET_COUNTER(&htim3);
				  while(1)
				  {
					  cha=__HAL_TIM_GET_COUNTER(&htim3)%1386-locationstate%1386;
					  if(cha<=mubiao&&cha>=-mubiao)
					  {
						  realround++;
						  HAL_Delay(75);
						  if(setround==realround+1)
						  {
							  break;
						  }
						  //speeddetect();
						  //showlocation();
						  OLED_ShowFrame();
					  }
				  }
				  __HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_3,149);

				  HAL_GPIO_WritePin(zheng_GPIO_Port, zheng_Pin, RESET);
				  HAL_GPIO_WritePin(fu_GPIO_Port, fu_Pin, SET);
				  while(1)
				  {
					  cha=__HAL_TIM_GET_COUNTER(&htim3)%1386-locationstate%1386;
					  if(cha<=mubiao&&cha>=-mubiao)
					  {
						  realround++;
						  if(setround==realround)
						  {
							  break;
						  }
					  }
				  }

				  __HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_3,99);

				  HAL_GPIO_WritePin(zheng_GPIO_Port, zheng_Pin, RESET);
				  HAL_GPIO_WritePin(fu_GPIO_Port, fu_Pin, SET);
				  int mudi=locationstate%1386+tarlocation*39;
				  mudi%=1386;
				  while(1)
				  {
					  cha=__HAL_TIM_GET_COUNTER(&htim3)%1386-mudi;
					  if(cha<=mubiao&&cha>=-mubiao)
					  {
						  stops();
						  break;
					  }
				  }

				  //已经到达buzzer

					__HAL_TIM_SET_COMPARE(&htim4,TIM_CHANNEL_4,0.5*__HAL_TIM_GET_AUTORELOAD(&htim4));
					dii=1;

				  char tartem2[]="        ";
				  OLED_PrintString(0, 36, tartem2, &font16x16, OLED_COLOR_NORMAL);
				  OLED_ShowFrame();

				  tarState=4;

				  locationstate=__HAL_TIM_GET_COUNTER(&htim3);

				  char tartem3[]="Finish";
				  OLED_PrintString(0, 36, tartem3, &font16x16, OLED_COLOR_NORMAL);
				  OLED_ShowFrame();

				  break;

			  case 4:
				  showlocation();
				  OLED_ShowFrame();

				  if(danji||shuangji)
				  {
					  tarState=1;
					  danji=0;
					  shuangji=0;

					  char tartem4[]="      ";
					  OLED_PrintString(0, 36, tartem4, &font16x16, OLED_COLOR_NORMAL);
				  }


				  tarcounter=__HAL_TIM_GET_COUNTER(&htim3);
				  if(tarcounter%1386-locationstate%1386<=-40||tarcounter%1386-locationstate%1386>=20)
				  {

					  if(tarcounter<=1000&&locationstate>=12859)
					  {
						  e=1;
						  f=-1;
					  }else if(tarcounter>locationstate)
					  {
						  e=1;
						  f=-1;
					  }else if(locationstate<=1000&&tarcounter>=12859)
					  {
						  e=-1;
						  f=-1;
						  //顺时针
					  }else if(locationstate>tarcounter)
					  {
						  e=-1;
						  f=-1;
						  //sun
					  }
				  }

				  if(e==1)
				  {
					  //顺时针转动
					  __HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_3,199);

					  HAL_GPIO_WritePin(zheng_GPIO_Port, zheng_Pin, SET);
					  HAL_GPIO_WritePin(fu_GPIO_Port, fu_Pin, RESET);
					  e=0 ;
					  HAL_Delay(40);
				  }else if(e==-1)
				  {
					  __HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_3,199);

					  HAL_GPIO_WritePin(zheng_GPIO_Port, zheng_Pin, RESET);
					  HAL_GPIO_WritePin(fu_GPIO_Port, fu_Pin, SET);
					  e=0;
					  HAL_Delay(40);
				  }

				  if(f==-1)
				  {
					  while(1)
					  {
						  tarcounter=__HAL_TIM_GET_COUNTER(&htim3);
						  if((tarcounter%1386-locationstate%1386>=-20&&tarcounter%1386-locationstate%1386<=-10))
						  {
							  stops();
							  f=0;
							  HAL_Delay(200);
							  break;
						  }
					  }
				  }

				  break;
			  }

			  Keydetect();

			  if(sysState!=MODE_TARGET)
			  {
				  break;
			  }
		  }
		  break;

	  case MODE_SLEEP:
		  OLED_NewFrame();
		  OLED_ShowFrame();

		  int sleState = 0;
		  stateshift=__HAL_TIM_GET_COUNTER(&htim1);//速度调节调用
		  locationstate=__HAL_TIM_GET_COUNTER(&htim3);//位置调用
		  int slecounter;
		  int locacounter;
		  int slespeed=0;

		  locationstate%=1386;
		  float sletemp=locationstate*40/1386;
		  int sletempint=40-sletemp;
		  __HAL_TIM_SET_COUNTER(&htim1,sletempint);
		  stateshift=__HAL_TIM_GET_COUNTER(&htim1);

		  int c,a,b,slecha,d;
		  d=0;
		  int panduan=0;
		  while(1)
		  {

			  Keydetect();
			  if(shuangji||danji)
			  {
				  shuangji=0;
				  danji=0;
				  sysState=MENU_MAIN;
			  }
			  showlocation();
			  OLED_ShowFrame();

			  slecounter=__HAL_TIM_GET_COUNTER(&htim1);
			  if(stateshift>=0&&stateshift<=1000&&slecounter>=4000)
			  {
				  c=-1;
				  d=1;
				  panduan=1;
				  stateshift=slecounter;//逆时针
			  }else if(stateshift<slecounter)
			  {
				  c=1;
				  d=1;
				  panduan=1;
				  stateshift=slecounter;
			  }else if(stateshift>=4000&&slecounter<=1000&&slecounter>=0)
			  {
				  c=1;
				  d=1;
				  panduan=1;
				  stateshift=slecounter;
			  }else if(stateshift>slecounter)
			  {
				  c=-1;
				  d=1;
				  panduan=1;
				  stateshift=slecounter;//逆时针
			  }else if(slecounter==stateshift&&panduan==1)
			  {

				  locacounter=__HAL_TIM_GET_COUNTER(&htim3);
				  if(locacounter%1386-locationstate%1386<=-50||locacounter%1386-locationstate%1386>=50)
				  {

					  if(locacounter<=1000&&locationstate>=12859)
					  {
						  c=1;
						  d=-1;
					  }else if(locacounter>locationstate)
					  {
						  c=1;
						  d=-1;
					  }else if(locationstate<=1000&&locacounter>=12859)
					  {
						  c=-1;
						  d=-1;
						  //顺时针
					  }else if(locationstate>locacounter)
					  {
						  c=-1;
						  d=-1;
						  //sun
					  }
				  }
			  }

			  if(c==1)
			  {
				  //顺时针转动
				  __HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_3,199);

				  HAL_GPIO_WritePin(zheng_GPIO_Port, zheng_Pin, SET);
				  HAL_GPIO_WritePin(fu_GPIO_Port, fu_Pin, RESET);
				  c=0;
				  HAL_Delay(40);
			  }else if(c==-1)
			  {
				  __HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_3,199);

				  HAL_GPIO_WritePin(zheng_GPIO_Port, zheng_Pin, RESET);
				  HAL_GPIO_WritePin(fu_GPIO_Port, fu_Pin, SET);
				  c=0;
				  HAL_Delay(40);
			  }

			  if(d==1)
			  {
				  while(1)
				  {
					  a=__HAL_TIM_GET_COUNTER(&htim3);
					  b=__HAL_TIM_GET_COUNTER(&htim1);
					  slecha=(a%1386)*40-(40-b%40)*1386;
					  if(slecha>=-3000&&slecha<=3000)
					  {
						  stops();
						  d=0;
						  HAL_Delay(100);
						  locationstate=__HAL_TIM_GET_COUNTER(&htim3);
						  break;
					  }
				  }
			  }

			  if(d==-1)
			  {
				  while(1)
				  {
					  locacounter=__HAL_TIM_GET_COUNTER(&htim3);
					  if((locacounter%1386-locationstate%1386>=-5&&locacounter%1386-locationstate%1386<=5))
					  {
						  stops();
						  d=0;
						  HAL_Delay(200);
						  break;
					  }
				  }
			  }

			  if(sysState!=MODE_SLEEP)
			  {
				  break;
			  }
		  }
		  break;
	  case MODE_CHEAT:
		  OLED_NewFrame();
		  OLED_ShowFrame();

		  int cheState = 0;
		  stateshift=__HAL_TIM_GET_COUNTER(&htim1);//速度调节调用
		  locationstate=__HAL_TIM_GET_COUNTER(&htim3);//位置调用
		  int checounter;
		  int chelocation=0;
		  int chemubiao;

		  char che[]="   AREA";
		  char chelocationout[5];
		  char chetemp[]="   ";
		  OLED_PrintString(0, 0, che, &font16x16, OLED_COLOR_NORMAL);
		  OLED_ShowFrame();

		  __HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_3,999);

		  HAL_GPIO_WritePin(zheng_GPIO_Port, zheng_Pin, SET);
		  HAL_GPIO_WritePin(fu_GPIO_Port, fu_Pin, RESET);

		  while(1)
		  {
		  switch(cheState)
		  {
		  case 0:
			  Keydetect();
			  checounter=__HAL_TIM_GET_COUNTER(&htim1);
			  if(stateshift>=0&&stateshift<=1000&&checounter>=4000)
			  {
				    chelocation-=(stateshift+14000-checounter)*0.5;
			  		stateshift=checounter;//逆时针
			  }else if(stateshift<checounter)
			  {
				    chelocation+=(checounter-stateshift)*0.5;
			  		stateshift=checounter;
			  }else if(stateshift>=4000&&checounter<=1000&&checounter>=0)
			  {
				    chelocation+=(checounter+14000-stateshift)*0.5;
			  		stateshift=checounter;
			  }else if(stateshift>checounter)
			  {
				    chelocation-=(stateshift-checounter)*0.5;
			  		stateshift=checounter;//逆时针
			  }

			  if(chelocation<1)
			  {
				  chelocation=1;
			  }else
			  {
			  		if(chelocation>36)
			  		{
			  			chelocation=36;
			  		}
			  }
			  int chelocationint=chelocation;
			  sprintf(chelocationout,"%d",chelocationint);

			  OLED_PrintString(0, 0, chetemp, &font16x16, OLED_COLOR_NORMAL);
			  OLED_PrintString(0, 0, chelocationout, &font16x16, OLED_COLOR_REVERSED);
			  OLED_ShowFrame();

			  if(danji)
			  {
				  chelocationint=(chelocationint+22)%36;
				  chemubiao=chelocationint*1386/36;
				    cheState=1;
			  		danji=0;
			  }else if(shuangji)
			  {
				    sysState=MENU_MAIN;
				    stoph();
			  		shuangji=0;
			  }
			  break;
		  case 1:
			  OLED_PrintString(0, 0, chelocationout, &font16x16, OLED_COLOR_NORMAL);
			  OLED_ShowFrame();
			  int jiansu=995;
			  while(1)
			  {
				  showlocation();
				  OLED_ShowFrame();

				  __HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_3,jiansu);
				  jiansu=jiansu-10;
				  if(jiansu==155)
				  {
					  int a;
					  while(1)
					  {
						  a=__HAL_TIM_GET_COUNTER(&htim3);
						  if(a%1386-chemubiao>-15&&a%1386-chemubiao<24)
						  {
							  break;
						  }

						  showlocation();

						  a=__HAL_TIM_GET_COUNTER(&htim3);
						  if(a%1386-chemubiao>-15&&a%1386-chemubiao<24)
						  {
							  break;
						  }

						  OLED_ShowFrame();

						  a=__HAL_TIM_GET_COUNTER(&htim3);
						  if(a%1386-chemubiao>-18&&a%1386-chemubiao<24)
						  {
							  break;
						  }
					  }
				  }
				  if(jiansu==35)
				  {
					  stops();
					  break;
				  }
			  }
			  cheState=2;
			  break;
		  case 2:
			  showlocation();
			  OLED_ShowFrame();
			  Keydetect();
			  if(danji)
			  {

				  __HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_3,999);

				  HAL_GPIO_WritePin(zheng_GPIO_Port, zheng_Pin, SET);
				  HAL_GPIO_WritePin(fu_GPIO_Port, fu_Pin, RESET);

				  cheState=0;
				  danji=0;
			  }
			  if(shuangji)
			  {
				  sysState=MENU_MAIN;
				  shuangji=0;
			  }

			  break;
		  }
		  if(sysState!=MODE_CHEAT)
		  {
			  break;
		  }
		  }
		  break;
	  case SYS_ERROR:
		  break;
	  }

    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}
#ifdef USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
