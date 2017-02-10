#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x3F, 20, 4);

const int OUT_PIN = A1;
const int IN_PIN = A0;
const int MAX_ADC_VALUE = 1023;
const float IN_STRAY_CAP_TO_GND = 24.48;
const float IN_CAP_TO_GND = IN_STRAY_CAP_TO_GND;
const float R_PULLUP = 34.8;

void setup() {
  lcd.init();
  lcd.backlight();
  pinMode(OUT_PIN, OUTPUT);
  pinMode(IN_PIN, OUTPUT);
  lcd.setCursor(0, 0);
  lcd.print("   CapacimetrINO    ");
  lcd.setCursor(0, 1);
  lcd.print("  Pagina Facebook:  ");
  lcd.setCursor(0, 2);
  lcd.print("Progetti & Tutorial");
  lcd.setCursor(0, 3);
  lcd.print("      Ardunio       ");
  delay(4000);
  lcd.clear();
}

void loop() {
  pinMode(IN_PIN, INPUT);
  digitalWrite(OUT_PIN, HIGH);
  int val = analogRead(IN_PIN);
  digitalWrite(OUT_PIN, LOW);

  if (val < 1000) {
    pinMode(IN_PIN, OUTPUT);
    float capacitance = (float)val * IN_CAP_TO_GND / (float)(MAX_ADC_VALUE -
                        val);
    lcd.setCursor(0, 0);
    lcd.print(F("Capacita: "));
    lcd.setCursor(10, 0);
    lcd.print(capacitance, 3);
    lcd.setCursor(17, 0);
    lcd.print(F("pF "));
    lcd.setCursor(0, 2);
    lcd.print("Tempo: ");
    lcd.setCursor(7, 2);
    lcd.print(val);
    lcd.setCursor(11, 2);
    lcd.print("mS");
  }

  else {
    pinMode(IN_PIN, OUTPUT);
    delay(1);
    pinMode(OUT_PIN, INPUT_PULLUP);
    unsigned long u1 = micros();
    unsigned long t;
    int digVal;

    do {
      digVal = digitalRead(OUT_PIN);
      unsigned long u2 = micros();
      t = u2 > u1 ? u2 - u1 : u1 - u2;
    }

    while ((digVal < 1) && (t < 400000L));

    pinMode(OUT_PIN, INPUT);
    val = analogRead(OUT_PIN);
    digitalWrite(IN_PIN, HIGH);
    int dischargeTime = (int)(t / 1000L) * 5;
    delay(dischargeTime);
    pinMode(OUT_PIN, OUTPUT);
    digitalWrite(OUT_PIN, LOW);
    digitalWrite(IN_PIN, LOW);

    float capacitance = -(float)t / R_PULLUP
                        / log(1.0 - (float)val / (float)MAX_ADC_VALUE);

    lcd.setCursor(0, 0);
    lcd.print(F("Capacita: "));

    if (capacitance > 1000.0) {
      lcd.setCursor(10, 0);
      lcd.print(capacitance / 1000.0, 2);
      lcd.setCursor(17, 0);
      lcd.print(F("uF "));
      lcd.setCursor(0, 2);
      lcd.print("Tempo: ");
      lcd.setCursor(7, 2);
      lcd.print(val);
      lcd.setCursor(11, 2);
      lcd.print("mS");
    }

    else {
      lcd.setCursor(10, 0);
      lcd.print(capacitance, 2);
      lcd.setCursor(17, 0);
      lcd.print(F("nF "));
      lcd.setCursor(0, 2);
      lcd.print("Tempo: ");
      lcd.setCursor(7, 2);
      lcd.print(val);
      lcd.setCursor(11, 2);
      lcd.print("mS");
    }
  }

  while (millis() % 1000 != 0);
}

