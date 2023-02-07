#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x3C,128,64);  // set the LCD address to 0x3F for a 16 chars and 2 line display

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  lcd.init();
  lcd.clear();         
  lcd.backlight();      // Make sure backlight is on
  lcd.blink();
  
  // Print a message on both lines of the LCD.
  //lcd.setCursor(2,0);   //Set cursor to character 2 on line 0
  //lcd.print("Hello world!");
  
  //lcd.setCursor(2,1);   //Move cursor to character 2 on line 1
 // lcd.print("LCD Tutorial");
}

void loop() {
  lcd.blink();
  delay(500);
  digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(1000);                       // wait for a second
  digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
  delay(1000);                       // wait for a second
}
