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


## REFERENCES

* https://www.waveshare.com/wiki/XNUCLEO-F103RB
* https://os.mbed.com/platforms/ST-Nucleo-F103RB/
* https://jpralves.net/post/2016/11/15/stm-nucleo.html
