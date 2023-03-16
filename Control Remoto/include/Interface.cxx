#include "Interface.h"

class Display {


public:

    void Display(Adafruit_SSD1306 oled_ptr){
        
        this->oled = oled_ptr;
        oled.begin(SSD1306_SWITCHCAPVCC,0x3C);
        oled.clearDisplay();

    }

    void hub(){
        
        oled.clearDisplay();
        oled.setTextColor(ILI9341_WHITE);
        //oled.setCursor(10,30);
        //oled.setTextSize(2);
        //oled.print("");
        oled.display();

    }

};