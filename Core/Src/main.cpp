/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
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
#include "uart.hpp"
#include "pin.hpp"
#include "oled.h"
#include "fonts.h"
#include "menu.hpp"
#include "sa818.h"
#include "i2s.hpp"
#include "wav_player.hpp"
#include "adc.hpp"
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

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
I2C_HandleTypeDef hi2c1;

SD_HandleTypeDef hsd;

TIM_HandleTypeDef htim10;
DMA_HandleTypeDef hdma_sdio_tx;
DMA_HandleTypeDef hdma_sdio_rx;
UART_HandleTypeDef huart1;
DMA_HandleTypeDef hdma_usart1_rx;
/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_I2C1_Init(void);

static void MX_TIM10_Init(void);
static void MX_SDIO_SD_Init(void);
static void MX_DMA_Init(void);

/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

uart uart1(uart::uart1,115200);
uart uart2(uart::uart2,9600);

pin led1(GPIOD, pin::PIN0, pin::out,pin::PullUp,pin::SPEED_LOW);
pin sa818_ptt(GPIOE, pin::PIN1, pin::out,pin::PullUp,pin::SPEED_LOW);
pin sa818_pd(GPIOE, pin::PIN3, pin::out,pin::PullUp,pin::SPEED_LOW);

pin pd11(GPIOD,pin::PIN11,pin::out,pin::PullDown,pin::SPEED_HIGH
);
pin pd12(GPIOD,pin::PIN12,pin::out,pin::PullDown,pin::SPEED_HIGH);
pin pd13(GPIOD,pin::PIN13,pin::out,pin::PullDown,pin::SPEED_HIGH);
pin pd14(GPIOD,pin::PIN14,pin::out,pin::PullDown,pin::SPEED_HIGH);

pin pd15(GPIOD,pin::PIN15,pin::in,pin::PullDown,pin::SPEED_HIGH);
pin pd8(GPIOD,pin::PIN8,pin::in,pin::PullDown,pin::SPEED_HIGH);
pin pd9(GPIOD,pin::PIN9,pin::in,pin::PullDown,pin::SPEED_HIGH);
pin pd10(GPIOD,pin::PIN10,pin::in,pin::PullDown,pin::SPEED_HIGH);
adc adc_bat(adc::ADC_1);
oled oled1(&hi2c1,0x78,&htim10);
wav_player wav_player_(i2s::I2S2);
sa818 sa818_(&uart2,&sa818_pd,&sa818_ptt);
menu menu1(&oled1, &uart1,&wav_player_,&sa818_);

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
  MX_DMA_Init();

  MX_GPIO_Init();
  MX_TIM10_Init();
  MX_I2C1_Init();

  led1.init();
  uart1.init();
  oled1.init();
  sa818_.sa818_configure(1,CHANNEL[4],CHANNEL[4],"0000",0,"0000");
  pd11.init();
  pd12.init();
  pd13.init();
  pd14.init();
  pd15.init();
  pd8.init();
  pd9.init();
  pd10.init();
  adc_bat.init();
  adc_bat.adc_setEquation(3.3/(2*4096),0);
  sa818_.sa818_power(sa818::on);
  uart1.send_recive("\r\nSTART\r\n","START");
  wav_player_.init();
  MX_SDIO_SD_Init();
  MX_FATFS_Init();
  char x[225];

  sprintf(x,"\n\rFATFSMount %i\n\r",(uint)retSD);
  uart1.send_recive(x,x);


	//File object
  FRESULT res=f_mount(&SDFatFS, SDPath, 1);
	FIL fil;
  //Mount drive
  sprintf(x,"\n\rmounting %i\n\r",(uint)res);
  uart1.send_recive(x,x);
  sa818_.sa818_txrx_mode(sa818::tx);
  int file_er;
	if ( res == FR_OK) {
		//Mounted OK, turn on RED LED
    uart1.send_recive("Play Laugh.wav","Laugh.wav");
		
		file_er=wav_player_.file_select("Laugh.wav");
    sprintf(x,"File loaded = %i\n\r",file_er);
    uart1.send_recive(x,"Laugh.wav");

    wav_player_.play();

    while(!wav_player_.isEndOfFile())
    {
    wav_player_.process();

    }
    uart1.send_recive("Done Playing","human1.wav");

		//Unmount drive, don't forget this!
		f_mount(0, "", 1);
	}
  sa818_.sa818_txrx_mode(sa818::rx);
  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  //MX_USART1_UART_Init();
  /* USER CODE BEGIN 2 */

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {


    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
    // oled1.oled_print(h,Font_11x18,0,0);
    // oled1.oled_refresh();
    // led1.write(pin::on);
    // uart1.send_recive(h,h);
    // HAL_Delay(500);
    // led1.write(pin::off);


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
  RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 192;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV4;
  RCC_OscInitStruct.PLL.PLLQ = 8;
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

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_3) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_I2S;
  PeriphClkInitStruct.PLLI2S.PLLI2SN = 200;
  PeriphClkInitStruct.PLLI2S.PLLI2SM = 8;
  PeriphClkInitStruct.PLLI2S.PLLI2SR = 4;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  
}


/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
 
  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();


}

static void MX_I2C1_Init(void)
{

  /* USER CODE BEGIN I2C1_Init 0 */

  /* USER CODE END I2C1_Init 0 */

  /* USER CODE BEGIN I2C1_Init 1 */

  /* USER CODE END I2C1_Init 1 */
  hi2c1.Instance = I2C1;
  hi2c1.Init.ClockSpeed = 400000;
  hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C1_Init 2 */

  /* USER CODE END I2C1_Init 2 */

}



/**
  * @brief SDIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_SDIO_SD_Init(void)
{

  /* USER CODE BEGIN SDIO_Init 0 */

  /* USER CODE END SDIO_Init 0 */

  /* USER CODE BEGIN SDIO_Init 1 */

  /* USER CODE END SDIO_Init 1 */

  hsd.Instance = SDIO;
  hsd.Init.ClockEdge = SDIO_CLOCK_EDGE_RISING;
  hsd.Init.ClockBypass = SDIO_CLOCK_BYPASS_DISABLE;
  hsd.Init.ClockPowerSave = SDIO_CLOCK_POWER_SAVE_DISABLE;
  hsd.Init.BusWide = SDIO_BUS_WIDE_1B;
  hsd.Init.HardwareFlowControl = SDIO_HARDWARE_FLOW_CONTROL_DISABLE;
  hsd.Init.ClockDiv = 4;
  // char x[10];
  // sprintf(x,"sdcard initialize error=%i\r\n",HAL_SD_Init(&hsd));
  // uart1.send_recive(x,x);
  // if (HAL_SD_Init(&hsd) != HAL_OK)
  // {
  //   Error_Handler();
  // }
  // if (HAL_SD_ConfigWideBusOperation(&hsd, SDIO_BUS_WIDE_4B) != HAL_OK)
  // {
  //   Error_Handler();
  // }
  /* USER CODE BEGIN SDIO_Init 2 */

  /* USER CODE END SDIO_Init 2 */

}
/**
  * @brief TIM10 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM10_Init(void)
{

  /* USER CODE BEGIN TIM10_Init 0 */

  /* USER CODE END TIM10_Init 0 */

  /* USER CODE BEGIN TIM10_Init 1 */

  /* USER CODE END TIM10_Init 1 */
  htim10.Instance = TIM10;
  htim10.Init.Prescaler = 200;
  htim10.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim10.Init.Period = 65535;
  htim10.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim10.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim10) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM10_Init 2 */

  /* USER CODE END TIM10_Init 2 */

}
/**
  * Enable DMA controller clock
  */
static void MX_DMA_Init(void)
{

  /* DMA controller clock enable */
  __HAL_RCC_DMA2_CLK_ENABLE();

  /* DMA interrupt init */
  /* DMA2_Stream0_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA2_Stream0_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA2_Stream0_IRQn);
  /* DMA2_Stream2_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA2_Stream2_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA2_Stream2_IRQn);
  /* DMA2_Stream3_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA2_Stream3_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA2_Stream3_IRQn);
  /* DMA2_Stream6_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA2_Stream6_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA2_Stream6_IRQn);

}
/* USER CODE BEGIN 4 */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
  //call the function in the class

    uart1.rx_cplt_callback();
}

uint8_t ok_debounce=0;
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{

  //TODO Check on the htime10 setup 
  if(htim->Instance==htim10.Instance) //htim10 is now setup to refresh 15 times a second 
  {
    //Debounce OK
    // if(HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_15))
    // {
    //   if(ok_debounce==0){
    //     ok_debounce++;
    //   }
    //   else
    //   {
    //     if(ok_debounce==1){
    //       menu1.menu_ok();
    //       ok_debounce=0;
    //     }
    //   }
    // }
    // else
    // {
    //   ok_debounce=0;
    // }
    if(GPIOB->IDR& (GPIO_PIN_15))
    {
      menu1.menu_ok();
    }
    
    //char h1[]="tim";
    //uart1.send_recive(h1,h1);
    if(oled1.oled_isOledOn())
    {
      menu1.keyboard_poll();
      oled1.oled_update_battery(adc_bat.adc_getValue()); //Get the battery voltage and print it
      menu1.menu_print();//update the menu portion of the display
      oled1.oled_refresh();//Send the data to the display
      //oled1.Counter_increment();
      //char h[]="oled";
      //uart1.send_recive(h,h);

    }


  }
}

/**
 * @brief This function turns off the trigger
 * 
 * @param GPIO_Pin 
 */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  // //Fatfs object
	// FATFS FatFs;
	// //File object
	// FIL fil;
  // //Mount drive
	// if (f_mount(&FatFs, "", 1) == FR_OK) {
	// 	//Mounted OK, turn on RED LED
		
	// 	wav_player_.file_select("human.wav");
  //   wav_player_.play();
  //   while(!wav_player_.isEndOfFile())
  //   {
  //   wav_player_.process();
  //   }
	// 	//Unmount drive, don't forget this!
	// 	f_mount(0, "", 1);
	// }
	

}
//I2S callback

void HAL_I2S_TxCpltCallback(I2S_HandleTypeDef *hi2s)
{
  if(hi2s->Instance == SPI2)
  {
    wav_player_.cplt_transfer_callback();
  }
}

void HAL_I2S_TxHalfCpltCallback(I2S_HandleTypeDef *hi2s)
{
  if(hi2s->Instance == SPI2)
  {
    wav_player_.half_transfer_callback();
  }
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
  __disable_irq();
  while (1)
  {
  }
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
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
