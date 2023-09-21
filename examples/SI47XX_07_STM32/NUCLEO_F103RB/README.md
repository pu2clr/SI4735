# NUCLEO-F103RB

The "NUCLEO-F103RB" is a development board manufactured by STMicroelectronics. It is part of their STM32 Nucleo series, which is designed to provide an affordable and flexible way for developers to prototype and evaluate microcontroller-based projects. The NUCLEO-F103RB board is built around the STM32F103RB microcontroller, which belongs to the STM32 family based on the ARM Cortex-M architecture. It offers various I/O pins, connectivity options, and features to support a wide range of applications. Developers can program and debug the microcontroller using various integrated development environments (IDEs) and toolchains.



## Hardware Setup

1. __Connect the Board:__ Plug in the NUCLEO-F103RB board to your computer via USB. It will act as a virtual serial port for programming and debugging.

2. __Power Supply Selection:__ The board can be powered through USB or an external power supply. Make sure to select the appropriate power supply option based on your setup.

__Software Setup:__

1. __Install Arduino IDE:__
   If you haven't already, download and install the Arduino IDE from the official Arduino website: [Arduino Software](https://www.arduino.cc/en/software)

2. __Install STM32 Boards Package:__
   To add support for STM32 boards to the Arduino IDE, you need to install the STM32 boards package. Here's how:
   
   - Open the Arduino IDE.
   - Go to `File` > `Preferences`.
   - In the "Additional Boards Manager URLs" field, add the following URL:
     ```
     https://github.com/stm32duino/BoardManagerFiles/raw/master/STM32/package_stm_index.json
     ```
   - Click `OK` to close the preferences window.

3. __Install STM32 Boards Core:__
   Now, you need to install the STM32 boards core using the Boards Manager:
   
   - Go to `Tools` > `Board` > `Boards Manager...`
   - Search for "STM32" and find "STM32 Cores by STMicroelectronics." Click `Install`.

4. __Select NUCLEO-F103RB Board:__
   After the installation is complete, you can select the NUCLEO-F103RB board in the Arduino IDE:
   
   - Go to `Tools` > `Board` and scroll down to the `STMicroelectronics` section.
   - Choose `NUCLEO-F103RB`.

5. __Select Upload Method:__
   Choose the appropriate upload method. For the NUCLEO-F103RB, you can use the "STM32CubeProgrammer (DFU)" upload method. This will allow you to upload your sketches using the built-in ST-Link debugger.

## Upload a Sketch:

1. Write your Arduino code using the Arduino framework and libraries, as you would with any Arduino project.

2. Make sure to set the correct board and upload method in the Arduino IDE.

3. Click the `Upload` button to compile and upload your code to the NUCLEO-F103RB board.

That's it! Your code should now be running on the NUCLEO-F103RB board. Remember that while the basic usage is similar to other Arduino boards, there might be STM32-specific features and configurations you can explore as well.


## Arduino ATmega328 based board and NUCLEO-F103RB pinout  


| NUCLEO-F103RB Pin | NUCLEO-F103RB Function | Arduino Pin |
|-------------------|-----------------------|-------------|
| PA0 - PA15        | GPIO                  | D0 - D13    |
| PB0 - PB15        | GPIO                  | D0 - D13    |
| PC0 - PC15        | GPIO                  | D0 - D13    |
| PD0 - PD15        | GPIO                  | D0 - D13    |
| PE0 - PE15        | GPIO                  | D0 - D13    |
| PF0 - PF15        | GPIO                  | D0 - D13    |
| PG0 - PG15        | GPIO                  | D0 - D13    |
| PH0 - PH15        | GPIO                  | D0 - D13    |
| PI0 - PI15        | GPIO                  | D0 - D13    |
| PA13, PA14        | SWD Debug             | -           |
| PA9, PA10         | UART1                 | RX, TX      |
| PB6, PB7          | UART1                 | RX, TX      |
| PA2, PA3          | UART2                 | RX, TX      |
| PA11, PA12        | USB                   | -           |
| PB10, PB11        | UART3                 | RX, TX      |
| PA5, PA6, PA7     | SPI1                  | MOSI, MISO, SCK |
| PB5, PB4, PB3     | SPI1                  | MOSI, MISO, SCK |
| PA4, PA6, PA5     | SPI1                  | SS, MOSI, MISO, SCK |
| PA4, PA5, PA6, PA7 | I2C1                  | SDA, SCL    |
| PB7, PB6          | I2C1                  | SDA, SCL    |
| PA9, PA10         | I2C1                  | SDA, SCL    |


### Example code

```cpp
const int ledPin = PA4;  // D4 (Arduino Uno)

void setup() {
  pinMode(ledPin, OUTPUT); 
}

void loop() {
  digitalWrite(ledPin, HIGH); // Turn the LED ON
  delay(1000); 
  digitalWrite(ledPin, LOW); // Turn the LED off
  delay(1000); 
}
```


```cpp
#include <Arduino.h>
#include <U8g2lib.h>

// Initialize U8g2 library
U8G2_SSD1306_128X64_NONAME_F_SW_I2C u8g2(U8G2_R0, /* clock=*/ SCL, /* data=*/ SDA, /* reset=*/ U8X8_PIN_NONE);

void setup() {
  // Initialize the OLED display
  u8g2.begin();
}

void loop() {
  // Draw the "Hello, World!" message
  u8g2.firstPage();
  do {
    u8g2.setFont(u8g2_font_ncenB14_tr);
    u8g2.drawStr(0,24,"Hello,");
    u8g2.drawStr(0,48,"World!");
  } while ( u8g2.nextPage() );
  
  // Add a delay
  delay(1000);
}
```

## Advantages of NUCLEO-F103RB vs. Regular STM32F103 Boards

The NUCLEO-F103RB and regular STM32F103 boards are both development boards based on the STM32F103 microcontroller series by STMicroelectronics. They are designed for embedded applications and are commonly used for prototyping, testing, and development of microcontroller-based systems. While they are similar in their core capabilities, there are advantages to each depending on your specific needs. Here's a breakdown to help you understand the advantages of each platform:

### NUCLEO-F103RB

### Key Advantages:

1. __Mbed-Enabled__: One of the key advantages is its compatibility with the Mbed OS, allowing for rapid prototyping and development.
   
2. __Arduino Compatibility__: It comes with an Arduino Uno V3 connector which makes it compatible with Arduino shields, broadening the hardware options you can use during prototyping.

3. __ST-Link Debugger__: It comes with an integrated ST-Link debugger/programmer, meaning you won't have to purchase an external debugging tool.
   
4. __Plug-and-Play__: The NUCLEO board is designed for ease of use, often being as simple as a drag-and-drop to program.
  
5. __Community Support__: Being a part of ST's Nucleo family, it benefits from strong community support, including libraries and example code.

6. __Morpho Headers__: These additional pin headers provide extra connectivity options, extending the functionality and making it easier to connect to custom hardware.

7. __3.3V and 5V Operation__: Many Nucleo boards, including the F103RB, have support for both 3.3V and 5V operation, which can be extremely convenient.

8. __Comprehensive Documentation__: The NUCLEO series is generally well-documented, making it easier for beginners to get started.

### STM32F103 "Blue Pill" or Regular Boards

### Key Advantages:

1. __Cost-Effectiveness__: One of the most significant advantages is the cost. These boards are often much cheaper, which makes them ideal for budget projects.

2. __Smaller Form Factor__: They are generally smaller and more compact, making them more suitable for projects where space is a constraint.

3. __Simplicity__: Fewer features mean fewer things to go wrong or configure, making these boards somewhat easier to troubleshoot.
   
4. __Flexibility for Customization__: The bare-bones nature of the regular STM32F103 boards allows for more flexibility in terms of custom configurations.
  
5. __Direct Access to Pins__: These boards often provide more direct access to the microcontroller pins, offering greater control at the hardware level.

6. __Lower Power Consumption__: Lacking additional components like a built-in debugger or unnecessary peripherals often results in lower power consumption.
   
7. __Better for Embedded Systems__: If you're designing an embedded system where you don't need all the extra features, a regular STM32F103 board might be more suitable.

8. __Broad Availability__: Given their popularity, these boards are readily available from a variety of vendors, increasing the options for procurement.

### Summary

- Choose __NUCLEO-F103RB__ if you're looking for ease of use, more built-in features, and better community support.
  
- Go for the __regular STM32F103 boards__ if you're constrained by budget, space, or you need more direct control over the hardware.

Both boards offer the reliability and performance of the STM32F103 series, so your specific project requirements will largely dictate which is the better option for you.



## REFERENCES

* https://www.waveshare.com/wiki/XNUCLEO-F103RB
* https://os.mbed.com/platforms/ST-Nucleo-F103RB/
* https://jpralves.net/post/2016/11/15/stm-nucleo.html
