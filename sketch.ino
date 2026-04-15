#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// Pin Configuration
const int buttonPin = 18; 
const int relayPin = 19;  

// Variables
bool isFilling = false;
float mlCount = 0.0;
float targetVolume = 100.0; // SETPOINT
int pricePer100ml = 100;
long lastUpdate = 0;

void setup() {
  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(relayPin, OUTPUT);
  digitalWrite(relayPin, LOW);

  Serial.begin(115200);

  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    for(;;);
  }
  
  showIdleScreen();
}

void loop() {

  // INPUT SETPOINT dari Serial (simulasi QRIS)
  if (Serial.available()) {
    targetVolume = Serial.parseFloat();
    mlCount = 0;
    isFilling = true;
    digitalWrite(relayPin, HIGH);
  }

  // Tombol untuk start manual
  if (digitalRead(buttonPin) == LOW) {
    delay(300);
    isFilling = true;
    mlCount = 0;
    digitalWrite(relayPin, HIGH);
  }

  // CLOSED LOOP CONTROL
  if (isFilling) {
    if (millis() - lastUpdate >= 100) {
      mlCount += 5.0;

      updateLiveDisplay();

      // FEEDBACK CONTROL
      if (mlCount >= targetVolume) {
        digitalWrite(relayPin, LOW); // tutup valve
        isFilling = false;
        showPaymentScreen();
        mlCount = 0;
        showIdleScreen();
      }

      lastUpdate = millis();
    }
  }
}

void showIdleScreen() {
  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setTextSize(1);
  display.setCursor(10, 5);
  display.println("HYDROPAY SYSTEM");
  display.drawLine(0, 20, 128, 20, WHITE);
  
  display.setCursor(10, 30);
  display.print("Target: ");
  display.print(targetVolume);
  display.println(" ml");

  display.setCursor(10, 50);
  display.println("> Tekan / Input QRIS");
  display.display();
}

void updateLiveDisplay() {
  int currentPrice = (mlCount / 100.0) * pricePer100ml;
  
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.println("--- CLOSED LOOP ---");

  display.setCursor(0, 10);
  display.print("Target: ");
  display.print(targetVolume);
  display.println(" ml");
  
  display.setTextSize(2);
  display.setCursor(0, 20);
  display.print(int(mlCount)); display.println(" ml");
  
  display.setTextSize(1);
  display.setCursor(0, 45);
  display.print("Biaya: Rp "); 
  display.println(currentPrice);

  display.display();
}

void drawMockQR() {
  for(int i=0; i<30; i+=4) {
    for(int j=0; j<30; j+=4) {
      if(random(0,2) == 1) {
        display.fillRect(85 + i, 15 + j, 3, 3, WHITE);
      }
    }
  }
}

void showPaymentScreen() {
  int totalPrice = (targetVolume / 100.0) * pricePer100ml;

  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.println("PEMBAYARAN");

  display.setCursor(0, 20);
  display.print("Volume: "); display.print(targetVolume); display.println("ml");

  display.setTextSize(2);
  display.setCursor(0, 35);
  display.print("Rp"); display.println(totalPrice);

  drawMockQR();

  display.display();
  delay(5000);

  display.clearDisplay();
  display.setCursor(20, 25);
  display.println("SELESAI!");
  display.display();
  delay(2000);
}