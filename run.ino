#include "SPI.h"
#include "Adafruit_GFX.h"
#include "Adafruit_ILI9341.h"
#include "math.h"
#include "UTFT.h"
#include "UTouch.h"

#define ILI9341_BLACK       0x0000  ///<   0,   0,   0
#define ILI9341_DARKGREEN   0x03E0  ///<   0, 125,   0


// For the Adafruit shield, these are the default.
#define TFT_DC 9
#define TFT_CS 10

// Use hardware SPI (on Uno, #13, #12, #11) and the above for CS/DC
// If using the breakout, change pins as desired
Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC);

//24SPI, 28SPI
UTFT myGLCD(ILI9341_S5P,MOSI,SCK,10,8,9);

 //touch panel (TCLK, TCS, TDIN, TDOUT, IRQ), XPT2046
UTouch myTouch(6, 5, 4, 3, 2);

int16_t old_x = 10, 
        old_y = 10, 
        pad_topX = 10, 
        pad_topY = 10, 
        pad_bottomX = 229, 
        pad_bottomY = 229;

uint16_t pad_bg = ILI9341_BLACK, 
         reticle_color = ILI9341_DARKGREEN;

void setup()
{

  Serial.begin(9600);
  tft.begin();
 
  
  myGLCD.InitLCD(LANDSCAPE);
  myTouch.InitTouch(LANDSCAPE);
  myGLCD.clrScr();

  
  Serial.println(F("Starting up.."));
  delay(10);
  Serial.println(FillScreen(ILI9341_BLACK));
  delay(10);
  

  Serial.println("Klaarty");
  delay(30)
  myGLCD.clrScr();
  myTouch.setPrecision(PREC_HIGH);
  

  draw_Main();
}

void loop() {
  int16_t x, y;
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

unsigned long draw_Main() {
  unsigned long start = micros();
  tft.fillRect(pad_topX-2,pad_topY-2,pad_bottomX+2,pad_bottomY+2, pad_bg);
  tft.drawRect(pad_topX-2,pad_topY-2,pad_bottomX+2,pad_bottomY+2, reticle_color);  
  tft.fillRect(239,8,312,58,pad_bg); //button1 background 
  tft.fillRect(239,68,312,116,pad_bg); //button2 background 
  tft.fillRect(239,124,312,174,pad_bg); //button3 background 
  tft.fillRect(239,182,312,232,pad_bg); //button4 background 
  
  //Draw in green
  tft.drawRect(239,8,312,58,reticle_color); //button1 rectangle
  tft.drawRect(239,68,312,116,reticle_color); //button2 rectangle
  tft.drawRect(239,124,312,174,reticle_color); //button3 rectangle
  tft.drawRect(239,182,312,232,reticle_color); //button4rectangle

  tft.println("MIDI",240,22); //button1 text
  tft.println("X",240,78); //button2 text - line 1
  tft.println("Y",240,134); //button3 text - line 1
  tft.println("X+Y",240,192); //button4 text - line 1

  yield();
  return micros() - start;  
}

unsigned long draw_Pad(int16_t x, int16_t y) {
      // we draw 3 three lines for x and three lines for y, for better visibility
    unsigned long start = micros();
    tft.drawLine(old_x-1,pad_topY,old_x-1,pad_bottomY, pad_bg); // clear old line x-1
    tft.drawLine(old_x+1,pad_topY,old_x+1,pad_bottomY, pad_bg); // clear old line x+1
    tft.drawLine(old_x,pad_topY,old_x,pad_bottomY, pad_bg);     // clear old line x
    tft.drawLine(pad_topX,old_y-1,pad_bottomY,old_y-1, pad_bg); // clear old line y-1    
    tft.drawLine(pad_topX,old_y+1,pad_bottomY,old_y+1, pad_bg); // clear old line y+1    
    tft.drawLine(pad_topX,old_y,pad_bottomY,old_y, pad_bg);     // clear old line y

    //Draw in green
    tft.drawLine(x-1,pad_topY,x-1,pad_bottomY, reticle_color);         // draw new line x-1
    tft.drawLine(x+1,pad_topY,x+1,pad_bottomY, reticle_color);         // draw new line x+1
    tft.drawLine(x,pad_topY,x,pad_bottomY, reticle_color);             // draw new line x
    tft.drawLine(pad_topX,y-1,pad_bottomX,y-1, reticle_color);         // draw new line1 y-1
    tft.drawLine(pad_topX,y+1,pad_bottomX,y+1, reticle_color);         // draw new line2 y+1
    tft.drawLine(pad_topX,y,pad_bottomX,y, reticle_color);             // draw new line3 y
      
    yield();
    return micros() - start;  
}