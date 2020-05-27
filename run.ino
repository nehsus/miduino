#include "SPI.h"
#include "Adafruit_GFX.h"
#include "Adafruit_ILI9341.h"
#include "math.h"
//Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_MOSI, TFT_CLK, TFT_RST, TFT_MISO);
#include <UTFT.h>
#include <UTouch.h>


#define ILI9341_BLACK       0x0000  ///<   0,   0,   0
#define ILI9341_NAVY        0x000F  ///<   0,   0, 123
#define ILI9341_DARKGREEN   0x03E0  ///<   0, 125,   0
#define ILI9341_DARKCYAN    0x03EF  ///<   0, 125, 123
#define ILI9341_MAROON      0x7800  ///< 123,   0,   0
#define ILI9341_PURPLE      0x780F  ///< 123,   0, 123
#define ILI9341_OLIVE       0x7BE0  ///< 123, 125,   0
#define ILI9341_LIGHTGREY   0xC618  ///< 198, 195, 198
#define ILI9341_DARKGREY    0x7BEF  ///< 123, 125, 123
#define ILI9341_BLUE        0x001F  ///<   0,   0, 255
#define ILI9341_GREEN       0x07E0  ///<   0, 255,   0
#define ILI9341_CYAN        0x07FF  ///<   0, 255, 255
#define ILI9341_RED         0xF800  ///< 255,   0,   0
#define ILI9341_MAGENTA     0xF81F  ///< 255,   0, 255
#define ILI9341_YELLOW      0xFFE0  ///< 255, 255,   0
#define ILI9341_WHITE       0xFFFF  ///< 255, 255, 255
#define ILI9341_ORANGE      0xFD20  ///< 255, 165,   0
#define ILI9341_GREENYELLOW 0xAFE5  ///< 173, 255,  41
#define ILI9341_PINK        0xFC18  ///< 255, 130, 198

// For the Adafruit shield, these are the default.
#define TFT_DC 9
#define TFT_CS 10

// Use hardware SPI (on Uno, #13, #12, #11) and the above for CS/DC
Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC);
// If using the breakout, change pins as desired

//24SPI, 28SPI
UTFT myGLCD(ILI9341_S5P,MOSI,SCK,10,8,9);

 //touch panel (TCLK, TCS, TDIN, TDOUT, IRQ), XPT2046
UTouch  myTouch( 6, 5, 4, 3, 2);

extern uint8_t BigFont[];
int x, y, textSize;

void setup()
{
  Serial.begin(9600);
 
  tft.begin();
  
  Serial.println(F("Starting up.."));
  delay(10);
  Serial.println(FillScreen(ILI9341_WHITE));
  delay(10);
  Serial.println(FillAxes());
  delay(30);
  Serial.println("Klaarty");

  myTouch.InitTouch();
  myTouch.setPrecision(PREC_HI);  
}

void loop()
{
  while (true)
  {
    if (myTouch.dataAvailable())
    {
      myTouch.read();
      x=myTouch.getX();
      y=myTouch.getY();

      Serial.println(FillPointer(x, y))    
      Serial.print("x= ");
      Serial.print(x);    
      Serial.print("   y= ");            
      Serial.print(y);  
      Serial.println("   ");    

    }
  }
}

unsigned long FillScreen(uint16_t color ) {
  unsigned long start = micros();
  tft.fillScreen(color);
  yield();
  return micros() - start;
}

unsigned long FillAxes() {
  unsigned long start = micros();
  int          w = tft.width(),
               h = tft.height();

  //tft.fillScreen(ILI9341_BLACK);
  //yield();
  tft.drawLine(0, h/2, w-1, h/2, ILI9341_BLACK);
  tft.drawLine(w/2, 0, w/2, h-1, ILI9341_BLACK);

  yield();
  return micros() - start;
}

unsigned long FillPointer(int x, int y) {
  unsigned long start = micros();
  tft.fillCircle(x, y, 1, ILI9341_PURPLE);
  yield();
  return micros() - start;  
}