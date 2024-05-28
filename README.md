**General Information and References**
-

1) This project is created and tested with STM32F070RB. When implementing this code to other projects with different MCUs, the configurations of pins, ADC, DMA, timer and UART will need to be changed inside of each one's source file.

2) An in-built ADC calibration function is being called before starting the measuring process. This function makes a significant accuracy difference and can also be calibrated further.

3) The ADC Sampling Time setting affects the result accuracy significantly and the most accurate results are obtained by using the highest sampling value, which is 239.5 cycles for this MCU. Other MCU's may have different settings and user should try them.

4) As default, a 12-bit ADC in ADC_Channel_0 (A0) with a continous conversion mode (enabled) and DMA continous reply mode (enabled) is used. This can be configured in adc_dma_driver.h header file if needed.

5) While LM35's datasheet specifies it needs an input voltage of 4-30 Volts, 3.3 Volts is used as Vref to LM35 in this project. Also, STM32's ADC works with 3.3V, too. It passed the test to give accurate values between 15-50 *C range, the further temperature ranges haven't been tested. 
CAUTION: If a 5V Vref is supplied to the LM35, because of the STM32's pins work with 3.3 Volts, an ADC voltage, as an input to MCU, that passes 3.3 Volts may damage the pins. A basic voltage-divider implementation might be needed to ensure the ADC output from sensor won't exceed 3.3 Volts.

6) To get more accurate results, the voltage supply to LM35 is measured with a voltmeter at the closest point and got a result of 3.316 Volts. Hence calculator function uses the 3.316 Volts, as defined in adc_dma_driver.h file and can be changed.

7) Reading with DMA won't work if user disables DMA Continous Reply mode in STM32CubeMX settings or DMA HAL files. It's enabled by default in the source files. 
 

**Things to do before trying the project codes**
-

1) ADC, DMA, timer and UART drivers' header and source files are located in /Drivers/Hardware_Drivers /Inc and /Src folders. Since STM32CubeIDE doesn't have default paths to these folders, these folders should be added to project's path, in order to resolve the "header files and source codes can't be found" error. To do so, user should right-click to the project, then Properties -> C/C++ Build -> Settings -> MCU GCC Compiler -> Include Paths, then add the /Hardware_Drivers/Inc folder to the path.

2) To use UART's TX and console write functions without errors and to display the float variables succesfully on serial console, user should enable the "Use float with printf from newlib-nano (-u _print_float)" setting. To do so, user should right-click to the project, then Properties -> C/C++ Build -> Settings -> MCU Settings and then enable the tickbox for "Use float with printf from newlib-nano (-u _print_float)".

3) In order to get more accurate readings, user should try the available sampling time settings for his/her MCU and test to see if results are improved, as mentioned in General Information -> 3rd Note. User can change these settings defined in adc_dma_driver.h header file.

**Changelog & Updates**
-

**Known Bugs and Limitations**
1) When the cable connections between MCU and sensor is disconnected or somehow damaged, a noise occures on LM35's ADC signal cable (close to LM35's connector side) that makes the signal fluctuate even with a pull-down resistor with different values are tested, which makes harder to develop error handling algorithms and determine if the signal is healthy or not.
   
2) Because the timer used in this project is a 16-bit timer, the period between measurements parameters' max value is 65535 (2^16) milliseconds, which is 65.5 seconds. If a measurement period longer than 65.5 seconds is needed in some applications, a new timer function with a different timer prescaler approach is needed.

**To Do List**
-

-Prior to bug 1, A robust error handling and user information mechanism is needed. 
-A filtering or averaging mechanism may needed to be implemented to get more accurate results.
-An auto-config function may needed to be implemented that automatically calculates and changes some of the config variables when user changes other related settings, especially for ADC resolution, sampling time and continous conversion modes.
