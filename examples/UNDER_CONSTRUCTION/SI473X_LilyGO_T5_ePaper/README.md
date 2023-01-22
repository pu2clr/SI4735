# LilyGO T5 ePaper

__UNDER CONSTRUCTION...__

[Click here to know how to setup the LilyGO ePaper](https://github.com/Xinyuan-LilyGO/LilyGo-T5-Epaper-Series)

1. Download the repository above to your computer
2. Unzip the downloaded file
3. Go to the unziped folder, select the lib folder and copy all the content to your Arduino folder library (in general Documents/Arduino/libraries)
4. Install / configure the ESP32 on your Arduino IDE
    * On Arduino IDE, go to Preferences, edit the field "Additional Boards Manager URLs:" and add the URL 
https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json
    * Go to Tools menu, select Boards, Board Manager and add the ESP32 board.
5. On Arduino IDE, select __ESP32 Dev Module__
6. Connect the LilyGO T5 ePaper board to the computer
7. On Arduino IDE, menu Tools, select the correspondent port (COM(9) or /dev/xxx.yyy )
8. Compile and upload the sketch


## include

```c++
#include <GxEPD.h>
```

## Declaration

```c++
GxIO_Class io(SPI,  EPD_CS, EPD_DC,  EPD_RSET);
GxEPD_Class display(io, EPD_RSET, EPD_BUSY);
```

## Init

```c++

SPI.begin(SPI_CLK, SPI_MISO, SPI_MOSI);
display.init(); // enable diagnostic output on Serial
```

## Some useful functions

```c++
display.setRotation(1);
display.fillScreen(GxEPD_WHITE);
display.fillScreen(GxEPD_WHITE);
display.setTextColor(GxEPD_RED);
display.setTextColor(GxEPD_BLACK);
display.getTextBounds(str, 0, 0, &x1, &y1, &w, &h);
display.setFont(&FreeMonoBold9pt7b);
display.setFont();
display.setFullWindow();
display.update();
display.setCursor(display.width()  - display.width() / 2, display.height() - 35);
```

### Print 

```c++

display.setFont(&FreeMonoBold9pt7b);
display.setCursor(0, 45);
display.println("Hello SI473X!");
display.update();

display.print(value * i, 2);

```

### Page

```c++

display.firstPage();
display.nextPage()

do {
     display.fillScreen(GxEPD_WHITE);
     display.setCursor(x, y);
     display.print("HelloWorld");
} while (display.nextPage());
```

### Rect 

```c++
// partial update to full screen to preset for partial update of box window
// (this avoids strange background effects)
display.updateWindow(0, 0, GxEPD_WIDTH, GxEPD_HEIGHT, false);

```

```c++
display.fillRect(box_x, box_y, box_w, box_h, GxEPD_BLACK);

display.fillTriangle(0, 
                    display.height() - 1,
                    display.width(), 
                    0,
                    display.width() - 1, 
                    display.height() - 1,
                    GxEPD_BLACK);

display.fillRoundRect(menu[item].x, menu[item].y, box_w, box_h, 3, GxEPD_BLACK);
display.drawRoundRect(menu[prevItem].x, menu[prevItem].y, box_w, box_h, 3, GxEPD_BLACK);


display.updateWindow(box_x, box_y, box_w, box_h, true);
```

### Other

```c++
display.powerDown();

display.drawExampleBitmap(BitmapExample1, 0, 0, GxEPD_WIDTH, GxEPD_HEIGHT, GxEPD_RED);

display.setRotation(r);
display.fillScreen(GxEPD_WHITE);
display.fillRect(0, 0, 8, 8, GxEPD_BLACK);
display.fillRect(display.width() - 18, 0, 16, 16, GxEPD_BLACK);
display.fillRect(display.width() - 25, display.height() - 25, 24, 24, GxEPD_BLACK);
display.fillRect(0, display.height() - 33, 32, 32, GxEPD_BLACK);

box_w = display.width();
box_y = display.height() - display.height() / 3;

```

### Sleep and wakeup 

```c++
    display.powerDown();

    esp_sleep_enable_ext1_wakeup(((uint64_t)(((uint64_t)1) << BUTTON_1)), ESP_EXT1_WAKEUP_ALL_LOW);

    esp_deep_sleep_start();

```
