/**
  @page Project_L0 SLIB code read-out protection on the AT32F403A series readme file
  
  @verbatim 
  
  ******************************************************************************
  * @file    Project_L1/readme.txt 
  * @author  ArteryTek SW Team
  * @version V1.0.0
  * @date    2019-12-23
  * @brief   readme
  ******************************************************************************
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, ARTERYTEK SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2019 ArteryTek</center></h2>
  ******************************************************************************
  @endverbatim  
 
@par Example Description 

   In this Project_L1 project the MCU is already preloaded with FIR-Filter SLIB protected IP-Code (done in Project_L0 project).
   To call the IP-Code FIR_lowpass_filter() function:
   - The fir_filter.h header file is included in main.c file
   - Symbol definition file (fir_filter_symbol.txt for MDK-ARM) containing the protected function symbols 
     and addresses is included in this project. Noting that symbol definition file have to be generated in Project_L0 
	 in order to replace the existing one in this project.
  

 @note The application needs to ensure that the SysTick time base is always set to 1 millisecond to have correct operation.
 
 
@par Directory contents 

  - Project_L1/src/srcmain.c                             Main program
  - Project_L1/src/system_at32f4xx.c                     AT32F4xx system clock configuration file
  - Project_L1/src/at32f4xx_it.c                         Interrupt handlers 
  - Project_L1/src/math_helper.c                         Calculation of SNR 
  - Project_L1/src/arm_fir_data.c                        Contains FIR_Filter's Input signal and Reference output signal computed with MATLAB
  - Project_L1/inc/at32f4xx_it.h                         Interrupt handlers header file
  - Project_L1/inc/math_helper.h                         math_helper header file

Related IP-Code files provided from Project_L0:
  - Project_L1/MDK_V5/fir_filter_symbol.txt              Symbol definition file for MDK-ARM project containing SLIB protected code symbols and addresses
  - Project_L1/inc/fir_filter.h                          FIR-Filter SLIB protected IP-Code header file

     
@par Hardware and Software environment  

  - This example runs on AT32F403AVGT7 devices.
  - This example has been tested with AT-START-F403A V0.1 board.
  - This example has been tested with the following toolchain:
      - Keil MDK-ARM uVision V5.23.0.0 


@par How to use it ? 

Before running this project the following conditions must be met:
- The same toolchain and compiler version must be used for both projects in Project_L0 and Project_L1: for instance if MDK-ARM V5.23 is used 
  in Project_L0 then it must be used for Project_L1 too.
- Check if any Read out protection, write protection or SLIB protection is enabled, if yes disable this protection 
  using Artery ICP Programmer. For more details on using Artery ICP Programmer, please refer to Artery ICP Programmer user manual.  

In order to make the program work, you must do the following :

Configurate the compiler to generate an execute only IP-Code avoiding any data read from it (literal pools and branch tables) . 
	1. Open project located in Project_L0 directory and choose your preferred toolchain 
	2. Select the SLIB-IP-Code-XO project configuration
	3. Rebuild all files. 
	4. Run the example following the sequence below:
		(a) Power on the board then load the code, WHITE LED should toggle continuously, if not press RESET button;
        (b) Press the user button key to execute the IP-code called in main.c file,
		(c) If the IP-Code is successfully executed, the GREEN LED should toggle continuously. Else the RED LED toggles continuously.  
		        

 */ 