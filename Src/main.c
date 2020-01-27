/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "fatfs.h"
#include "spi.h"
#include "usart.h"
#include "gpio.h"
#include "stdlib.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "fatfs_sd.h"
#include "string.h"
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

FATFS fs; //file system
FIL fil; //file
FRESULT fresult; // to store result
#define bfs	128
char buff_sign[64];
char buffer1[bfs];
char buffer2[bfs];
char initbuf[128];

uint8_t buffer1_ready=0;
uint8_t buffer2_ready=0;
UINT br, bw;	//file read/write count
FATFS *pfs;
DWORD fre_clust;
uint32_t total, free_space;

uint8_t count1 = 0;
uint8_t count2 = 0;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
/* to find the size of data in the buffer */
int bufsize (char *buf)
{
	int i=0;
	while (*buf++ != '\0') i++;
	return i;
}

void bufclear (char* buffer)  // clear buffer
{
	for (int i=0; i<bfs; i++)
	{
		buffer[i] = '\0';
	}
}

int parsestr(char* buffer,char* pstring, char pchar){

	char *prs1; int i=0;
	prs1 = strstr(buffer,pstring);
	prs1 = strchr(prs1,pchar);
	prs1++;
    i = atoi(prs1);
    return i;
}

void UART_SD_Init(void){
	  LL_GPIO_ResetOutputPin(GPIOB, LL_GPIO_PIN_3);
	  LL_mDelay(5000);

	  fresult = f_mount(&fs,"",0);
	  fresult = f_open(&fil, "uartinit.txt",FA_OPEN_ALWAYS | FA_READ | FA_WRITE );
	  if(fresult != FR_OK) LL_GPIO_SetOutputPin(GPIOB, LL_GPIO_PIN_3);
	  	  else LL_GPIO_ResetOutputPin(GPIOB, LL_GPIO_PIN_3);
	  f_read (&fil, initbuf, fil.fsize, &br);
	  fresult = f_close(&fil);

	  params.baudrate = parsestr(initbuf,"baudrate",'=');
	  params.datawidth = parsestr(initbuf,"datawidth",'=');
	  params.stopbit = parsestr(initbuf,"stopbit",'=');

	  if (params.baudrate<1 || params.baudrate>115200 )  params.baudrate=115200;
	  if (params.datawidth==8 )  params.datawidth=LL_USART_DATAWIDTH_8B;  //etc etc etc .....
	  if (params.stopbit==1 )  params.stopbit=LL_USART_STOPBITS_1;

	  MX_USART2_UART_Init(params);
	  MX_USART1_UART_Init(params);
}

void process_serial_data1(uint8_t ch);
void process_serial_data2(uint8_t ch);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

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
  MX_SPI1_Init();
  MX_FATFS_Init();
  UART_SD_Init();



  /* USER CODE BEGIN 2 */

  USART1_RegisterCallback(process_serial_data1);
  USART2_RegisterCallback(process_serial_data2);



	 /* create file */
	  fresult = f_open(&fil, "sniffer.txt",FA_OPEN_ALWAYS | FA_READ | FA_WRITE );

	  strcpy(buffer1,"UART Sniffer by Filipcek Zubek\n");
	  fresult = f_write(&fil,buffer1, bufsize(buffer1),&bw);

  	  f_close(&fil);

  	bufclear(buffer1);


  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {

    /* USER CODE END WHILE */

	  if (buffer1_ready){
		fresult = f_open(&fil, "sniffer.txt",FA_OPEN_ALWAYS | FA_READ | FA_WRITE );
		strcpy(buff_sign,"RX1;");
		fresult = f_lseek(&fil, fil.fsize);
		fresult = f_write(&fil,buff_sign, bufsize(buff_sign),&bw);
		fresult = f_write(&fil,buffer1, bufsize(buffer1),&bw);
		strcpy(buff_sign,";\n");
		fresult = f_write(&fil,buff_sign, bufsize(buff_sign),&bw);
	    f_close(&fil);
	    bufclear(buffer1);
	    buffer1_ready=0;
	  }

	  if (buffer2_ready){
		fresult = f_open(&fil, "sniffer.txt",FA_OPEN_ALWAYS | FA_READ | FA_WRITE );
		strcpy(buff_sign,"RX2;");
		fresult = f_lseek(&fil, fil.fsize);
		fresult = f_write(&fil,buff_sign, bufsize(buff_sign),&bw);
		fresult = f_write(&fil,buffer2, bufsize(buffer2),&bw);
		strcpy(buff_sign,";\n");
		fresult = f_write(&fil,buff_sign, bufsize(buff_sign),&bw);
	    f_close(&fil);
	    bufclear(buffer2);
	    buffer2_ready=0;
	  }


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
  LL_FLASH_SetLatency(LL_FLASH_LATENCY_2);

   if(LL_FLASH_GetLatency() != LL_FLASH_LATENCY_2)
  {
  Error_Handler();  
  }
  LL_RCC_HSI_Enable();

  //  Wait till HSI is ready
  while(LL_RCC_HSI_IsReady() != 1)
  {
    
  }
  LL_RCC_HSI_SetCalibTrimming(16);
  LL_RCC_PLL_ConfigDomain_SYS(LL_RCC_PLLSOURCE_HSI_DIV_2, LL_RCC_PLL_MUL_16);
  LL_RCC_PLL_Enable();

  //  Wait till PLL is ready
  while(LL_RCC_PLL_IsReady() != 1)
  {
    
  }
  LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1);
  LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_2);
  LL_RCC_SetAPB2Prescaler(LL_RCC_APB1_DIV_1);
  LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_PLL);

   // Wait till System clock is ready
  while(LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_PLL)
  {
  
  }
  LL_SetSystemCoreClock(64000000);
  LL_RCC_SetUSARTClockSource(LL_RCC_USART1_CLKSOURCE_PCLK1);
}

/* USER CODE BEGIN 4 */
void process_serial_data1(uint8_t ch)
{
	buffer1[count1]=ch;

	if (buffer1[count1]=='\n' || buffer1[count1]=='\r' || (buffer1[count1-1]=='\r' && buffer1[count1]=='\n') ){
		buffer1[count1]=0;
		buffer1_ready=1;
		count1=0;
		return;
	}
	count1++;
}

void process_serial_data2(uint8_t ch)
{
	buffer2[count2]=ch;

	if (buffer2[count2]=='\n' || buffer2[count2]=='\r' || (buffer2[count2-1]=='\r' && buffer2[count2]=='\n')){
		buffer2[count2]=0;
		buffer2_ready=1;
		count2=0;
		return;
	}

	count2++;
}
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */

  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(char *file, uint32_t line)
{ 
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
