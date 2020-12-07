/**
  @page Project_L0 SLIB code read-out protection on the AT32F403A series readme file
  
  @verbatim 
  
  ******************************************************************************
  * @file    Project_L0/readme.txt 
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

  This project (STEP1 of the SLIB example) describes how to generate an execute-only IP-Code (using compiler options), place and protect it 
  (protection using SLIB) in some preferred sectors in internal flash memory.
  The FIR Filter algorithm from CMSIS library is chosen as an example of IP-Code to be protected.
  Enabling SLIB is managed by software in SLIB_Enable() function.
  The IP-Code function FIR_lowpass_filter() is called in main.c file.
  Note that symbol definition file have to be generated in SLIP-IP-Code-XO project configuration described below, it has to be used in
  customer project Project_L1.
  
  With the SLIP-IP-Code-XO project configurations, the compiler is configured to generate an execute-only IP-Code avoiding any data read from 
  it (avoiding literal pools and branch tables).
  
  @note The IP-Code and IP-Data are placed in sector 2 to sector 5 which will be protected.
  
        Activating the SLIB on these sectors is completed by SLIB_Enable() function descripes in main file.
        Artery ICP Programmer can be used to activate SLIB as well, in that case, comment the define USE_SLIB_FUNCTION.

 @note The application needs to ensure that the SysTick time base is always set to 1 millisecond to have correct operation.
 
 
@par Directory contents 

  - Project_L0/src/srcmain.c                             Main program
  - Project_L0/src/system_at32f4xx.c                     AT32F4xx system clock configuration file
  - Project_L0/src/at32f4xx_it.c                         Interrupt handlers 
  - Project_L0/src/math_helper.c                         Calculation of SNR 
  - Project_L0/src/arm_fir_data.c                        Contains FIR_Filter's Input signal and Reference output signal computed with MATLAB
  - Project_L0/inc/at32f4xx_it.h                         Interrupt handlers header file
  - Project_L0/inc/fir_filter.h                          FIR-Filter SLIB protected IP-Code header file
  - Project_L0/inc/math_helper.h                         math_helper header file

FIR Filter files representing the IP-Code to be protected
  - Project_L0/src/fir_filter.c                          File containing global FIR Low Pass Filter function   
  - Project_L0/src/arm_fir_f32.c                         File containing FIR Filter basic function from CMSIS lib
  - Project_L0/src/arm_fir_init_f32.c                    File containing FIR Filter Initialization function from CMSIS lib
  - Project_L0/src/fir_coefficient.c                     File containing FIR Coefficients generated using fir1() MATLAB function.
     
@par Hardware and Software environment  

  - This example runs on AT32F403AVGT7 devices.
  - This example has been tested with AT-START-403A V0.1 board.
  - This example has been tested with the following toolchain:
      - Keil MDK-ARM uVision V5.23.0.0 

@par Generating symbol definition file 
	Generating symbol definition file is mandatory for Project_L1 and the provided file must be replaced by the newly generated. 
	To generate symbol definition file required for Project_L1, click on Rebuild all button then symbol definition file is created. 
        - For MDK-ARM toolchain: fir_filter_symbol.txt symbol definition file is created inProject_L0\MDK_V5\SLIB-IP-CODE-XO.
          It must be modified by deleting all lines and keeping only FIR-Filter IP-Code functions (please follow the provided fir_filter_symbol.txt file
          in Project_L1\MDK_V5). Once modified, copy it to Project_L1\MDK_V5 and replace the existing file.

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
		(c) If "USE_SLIB_FUNCTION" is defined and SLIB is never enabled, go to step (e). 
        (d) If the IP-Code is successfully executed, the GREEN LED should toggle continuously. Else the RED LED toggles continuously.  
		(e) If "USE_SLIB_FUNCTION" is defined and SLIB is never enabled, the the SLIB_Enable() function will set SLIB configuration once the
            IP-Code is correctly executed. The GREEN LED will toggle some time and then a system rest is executed to activate SLIB. 
            The code will run to step (b) again.            

 */ 