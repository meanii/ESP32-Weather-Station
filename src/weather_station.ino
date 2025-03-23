#include <Wire.h>
#include <U8g2lib.h>
#include <DHT.h>

// **OLED Display Setup**
U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, U8X8_PIN_NONE, 23, 22);

// **DHT22 Sensor Setup**
#define DHTPIN 21
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

// **Boot Screen**
void bootScreen() {
    u8g2.clearBuffer();  // Proper clearing (No white line)

    u8g2.setFont(u8g2_font_6x10_tf);
    u8g2.drawStr(28, 15, "Weather Station");

    // **Loading Animation**
    for (int i = 0; i < 3; i++) {
        u8g2.drawStr(38, 50, "Initializing");
        for (int j = 0; j <= i; j++) {
            u8g2.drawStr(75 + (j * 3), 50, ".");
        }
        u8g2.sendBuffer();
        delay(500);
        u8g2.clearBuffer();  // Clears only once per cycle
    }
}

void setup() {
    Serial.begin(115200);
    u8g2.begin();
    dht.begin();
    bootScreen();
    delay(1000);
}

void loop() {
    float temperature = dht.readTemperature();
    float humidity = dht.readHumidity();

    if (isnan(temperature) || isnan(humidity)) {
        Serial.println("Failed to read from DHT sensor!");

        u8g2.clearBuffer();
        u8g2.setFont(u8g2_font_6x10_tf);
        u8g2.drawStr(30, 28, "DHT22 ERROR!");
        u8g2.drawStr(30, 48, "Check wiring!");
        u8g2.sendBuffer();
        delay(2000);
        return;
    }

    Serial.print("Temp: ");
    Serial.print(temperature);
    Serial.print("°C  Humidity: ");
    Serial.print(humidity);
    Serial.println("%");

    // **Only Clear Once Per Refresh**
    u8g2.clearBuffer();

    u8g2.setFont(u8g2_font_6x10_tf);
    u8g2.drawStr(28, 10, "Weather Monitor");
    u8g2.drawLine(10, 14, 118, 14);

    // **Temperature Display**
    u8g2.drawStr(10, 30, "Temp:");
    u8g2.setCursor(60, 30);
    u8g2.print(temperature, 1);
    u8g2.print("C");

    // **Humidity Display**
    u8g2.drawStr(10, 50, "Humidity:");
    u8g2.setCursor(80, 50);
    u8g2.print(humidity, 1);
    u8g2.print("%");

    u8g2.sendBuffer();  // Send final frame update
    delay(2000);
}
