#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// OLED
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// Pin
const int relayPin = 19;
const int buzzerPin = 5;

// State
enum State {
  IDLE,
  WAIT_PAYMENT,
  FILLING,
  FINISHED
};

State currentState = IDLE;

// Variabel
float mlCount = 0;
float targetVolume = 0;
int pricePer100ml = 100;
long lastUpdate = 0;

bool nearFullBeeped = false;

// ================== SETUP ==================
void setup() {
  pinMode(relayPin, OUTPUT);
  pinMode(buzzerPin, OUTPUT);

  digitalWrite(relayPin, LOW);

  Serial.begin(115200);

  Wire.begin(21, 22);

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("OLED GAGAL!");
    for (;;);
  }

  Serial.println("OLED OK");

  showMenu();
}

// ================== LOOP ==================
void loop() {

  // PILIH MENU
  if (Serial.available() && currentState == IDLE) {
    int choice = Serial.parseInt();

    if (choice == 1) targetVolume = 300;
    else if (choice == 2) targetVolume = 600;
    else if (choice == 3) targetVolume = 1000;

    if (targetVolume > 0) {
      currentState = WAIT_PAYMENT;
      showPaymentScreen();
    }
  }

  // BAYAR
  if (Serial.available() && currentState == WAIT_PAYMENT) {
    int pay = Serial.parseInt();
    if (pay == 1) {
      currentState = FILLING;
      mlCount = 0;
      nearFullBeeped = false;
      digitalWrite(relayPin, HIGH);
      startBeep(); // 🔊 mulai isi
    }
  }

  // ISI AIR
  if (currentState == FILLING) {
    if (millis() - lastUpdate >= 100) {
      mlCount += 5;

      updateDisplay();

      // ⚠️ hampir penuh (80%)
      if (!nearFullBeeped && mlCount >= 0.8 * targetVolume) {
        warningBeep();
        nearFullBeeped = true;
      }

      // selesai
      if (mlCount >= targetVolume) {
        digitalWrite(relayPin, LOW);
        currentState = FINISHED;
        finishBeep();
        showFinish();
      }

      lastUpdate = millis();
    }
  }

  // RESET
  if (currentState == FINISHED) {
    delay(3000);
    targetVolume = 0;
    currentState = IDLE;
    showMenu();
  }
}

// ================== DISPLAY ==================
void showMenu() {
  display.clearDisplay();

  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(10, 0);
  display.println("PILIH VOLUME");

  display.drawLine(0, 10, 128, 10, WHITE);

  display.setCursor(10, 20);
  display.println("1. 300 ml");

  display.setCursor(10, 30);
  display.println("2. 600 ml");

  display.setCursor(10, 40);
  display.println("3. 1000 ml");

  display.setCursor(10, 55);
  display.println("Input: 1/2/3");

  display.display();
}

void showPaymentScreen() {
  int totalPrice = (targetVolume / 100.0) * pricePer100ml;

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);

  display.setCursor(0, 0);
  display.println("SCAN & PAY");

  display.setCursor(0, 15);
  display.print("Volume: ");
  display.print(targetVolume);
  display.println(" ml");

  display.setCursor(0, 25);
  display.print("Total: Rp");
  display.println(totalPrice);

  drawMockQR();

  display.setCursor(0, 55);
  display.println("Ketik 1 = bayar");

  display.display();
}

void updateDisplay() {
  int price = (mlCount / 100.0) * pricePer100ml;

  display.clearDisplay();
  display.setTextColor(WHITE);

  display.setTextSize(1);
  display.setCursor(0, 0);
  display.println("MENGISI...");

  display.setCursor(0, 10);
  display.print("Target: ");
  display.print(targetVolume);

  display.setTextSize(2);
  display.setCursor(0, 20);
  display.print((int)mlCount);
  display.println(" ml");

  display.setTextSize(1);
  display.setCursor(0, 50);
  display.print("Rp ");
  display.println(price);

  display.display();
}

void showFinish() {
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(10, 20);
  display.println("SELESAI!");
  display.display();
}

// ================== SOUND ==================
void startBeep() {
  tone(buzzerPin, 1500);
  delay(300);
  noTone(buzzerPin);
}

void warningBeep() {
  for (int i = 0; i < 2; i++) {
    tone(buzzerPin, 2000);
    delay(500);
    noTone(buzzerPin);
    delay(500);
  }
}

void finishBeep() {
  int melody[] = {1000, 1500, 2000};

  for (int i = 0; i < 3; i++) {
    tone(buzzerPin, melody[i]);
    delay(500);
  }

  noTone(buzzerPin);
}

// ================== QR MOCK ==================
void drawMockQR() {
  for (int i = 0; i < 30; i += 4) {
    for (int j = 0; j < 30; j += 4) {
      if (random(0, 2)) {
        display.fillRect(85 + i, 15 + j, 3, 3, WHITE);
      }
    }
  }
}
