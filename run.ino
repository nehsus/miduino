#include "SPI.h"
#include "Adafruit_GFX.h"
#include "Adafruit_ILI9341.h"
#include "math.h"
//Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_MOSI, TFT_CLK, TFT_RST, TFT_MISO);
#include <UTFT.h>
#include <UTouch.h>

#define pad_bg 0,0,0 
#define reticle_color 123,125,0 //R,G,B

#define pad_topX 10
#define pad_topY 10
#define pad_bottomX 229
#define pad_bottomY 229

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
UTouch myTouch(6, 5, 4, 3, 2);

extern uint8_t BigFont[];

void setup()
{
  Serial.begin(9600);
 
  tft.begin();
 
  myGLCD.InitLCD(LANDSCAPE);
  myGLCD.clrScr();
  myTouch.InitTouch(LANDSCAPE);

  Serial.println(F("Starting up.."));
  delay(10);
  Serial.println(FillScreen(ILI9341_BLACK));
  delay(10);


  Serial.println("Klaarty");
  delay(30)
  myGLCD.clrScr();

  myTouch.setPrecision(PREC_EXTREME);

  draw_Main()
}

void loop()
{
  long x, y, oldX, oldY;
  //byte valueX;
  //byte valueY;  
  while (myTouch.dataAvailable() == true) 
  {
    myTouch.read();
    x = myTouch.getX();
    y = myTouch.getY();
    if ((x!=-1) and (y!=-1))
    {
      
      if((x>=pad_topX) and (x<=pad_bottomX) and (y>=pad_topX) and (y <=pad_bottomY)) // XY pad touch
      {
        draw_Pad(x,y);
        old_x=x;
        old_y=y;
        Serial.print("x= ");
        Serial.print(x);    
        Serial.print("   y= ");            
        Serial.print(y);  
        Serial.println("   ");    
      }
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
  tft.drawLine(0, h/2, w-1, h/2, ILI9341_DARKGREEN);
  tft.drawLine(w/2, 0, w/2, h-1, ILI9341_DARKGREEN);

  yield();
  return micros() - start;
}

unsigned long FillPointer(long x, long y, uint16_t color) {
  unsigned long start = micros();
  tft.fillCircle(x, y, 3, color);
  yield();
  return micros() - start;  
}

void draw_Main() {
  unsigned long start = micros();
  myGLCD.setColor(pad_bg);
  myGLCD.fillRect(pad_topX-2,pad_topY-2,pad_bottomX+2,pad_bottomY+2);
  myGLCD.setColor(reticle_color);
  myGLCD.drawRect(pad_topX-2,pad_topY-2,pad_bottomX+2,pad_bottomY+2);  
  myGLCD.setColor(pad_bg);
  myGLCD.setBackColor(pad_bg);

  myGLCD.fillRect(239,8,312,58); //button1 background 
  myGLCD.fillRect(239,68,312,116); //button2 background 
  myGLCD.fillRect(239,124,312,174); //button3 background 
  myGLCD.fillRect(239,182,312,232); //button4 background 
  myGLCD.setColor(reticle_color);
  myGLCD.drawRect(239,8,312,58); //button1 rectangle
  myGLCD.drawRect(239,68,312,116); //button2 rectangle
  myGLCD.drawRect(239,124,312,174); //button3 rectangle
  myGLCD.drawRect(239,182,312,232); //button4rectangle

  myGLCD.print("MIDI",240,22); //button1 text
  myGLCD.print("X",240,78); //button2 text - line 1
  myGLCD.print("Y",240,134); //button3 text - line 1
  myGLCD.print("X+Y",240,192); //button4 text - line 1

  yield();
  return micros() - start;  
}

void draw_Pad(long x, long y)
{
      // we draw 3 three lines for x and three lines for y
      // for better visibility
      unsigned long start = micros();
      myGLCD.setColor(pad_bg);
      myGLCD.drawLine(old_x-1,pad_topY,old_x-1,pad_bottomY); // clear old line x-1
      myGLCD.drawLine(old_x+1,pad_topY,old_x+1,pad_bottomY); // clear old line x+1
      myGLCD.drawLine(old_x,pad_topY,old_x,pad_bottomY);     // clear old line x
      myGLCD.drawLine(pad_topX,old_y-1,pad_bottomY,old_y-1); // clear old line y-1    
      myGLCD.drawLine(pad_topX,old_y+1,pad_bottomY,old_y+1); // clear old line y+1    
      myGLCD.drawLine(pad_topX,old_y,pad_bottomY,old_y);     // clear old line y
      myGLCD.setColor(reticle_color);
      myGLCD.drawLine(x-1,pad_topY,x-1,pad_bottomY);         // draw new line x-1
      myGLCD.drawLine(x+1,pad_topY,x+1,pad_bottomY);         // draw new line x+1
      myGLCD.drawLine(x,pad_topY,x,pad_bottomY);             // draw new line x
      myGLCD.drawLine(pad_topX,y-1,pad_bottomX,y-1);         // draw new line1 y-1
      myGLCD.drawLine(pad_topX,y+1,pad_bottomX,y+1);         // draw new line2 y+1
      myGLCD.drawLine(pad_topX,y,pad_bottomX,y);             // draw new line3 y
      yield();
      return micros() - start;  
}