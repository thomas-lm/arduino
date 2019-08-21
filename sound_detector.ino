#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32
#define SOUND_PIN A0
#define NB_AVG_MESURE 1000

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

//Init des variables
int currentDB = 0.;
int minDB = 999;
int maxDB = 0;

void setup() {
  Serial.begin(9600);
  pinMode(SOUND_PIN, INPUT);
  
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3C for 128x32
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }

  // Init screen
  display.clearDisplay();
  display.fillRect(0, 0, display.width(), display.height(), WHITE);
  display.display();
  delay(500);
}

void loop() {
  long estim = 0;
  for(int i=0; i<NB_AVG_MESURE; i++) {
    int mesure = analogRead(SOUND_PIN);
    if(estim < mesure) estim = mesure;    
  }

  //méthode de mesure du décibel
  currentDB = - 20. * log(1 - estim / 1023.) + 30.;

  if(minDB > currentDB) {
    minDB = currentDB;
  }
  if(maxDB < currentDB) {
    maxDB = currentDB;
  }
  
  displayMesure();
  delay(100);
}

void displayMesure() {
  display.clearDisplay();
  display.setTextSize(4);
  display.setTextColor(WHITE);
  display.setCursor(0, 2);
  display.print(String(currentDB));

  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(80,0);
  display.print("-");
  display.setCursor(80,16);
  display.print("+");
  
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(90, 0);
  display.print(String(minDB));
  display.setCursor(90, 16);
  display.print(String(maxDB));
  display.display();
}
