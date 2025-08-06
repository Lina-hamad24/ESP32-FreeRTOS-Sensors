#include <WiFi.h>
#include <DHT.h>

// === Wi-Fi Configuration ===
const char* ssid = "Ooredoo68CEC4";
const char* password = "FH2KEY3U79&CT";

// === Pin Definitions ===
#define DHTPIN 4              // DHT11 connected to GPIO4
#define DHTTYPE DHT11         // Sensor type
#define SOIL_SENSOR_PIN 34    // Soil moisture analog output
#define MQ135_PIN 35          // MQ135 air quality analog output

// === Initialize Sensors ===
DHT dht(DHTPIN, DHTTYPE);

// === FreeRTOS Task Handles ===
TaskHandle_t TaskDHT;
TaskHandle_t TaskSoil;
TaskHandle_t TaskAir;

// === Wi-Fi Connection Function ===
void connectToWiFi() {
  Serial.print("Connecting to Wi-Fi...");
  WiFi.begin(ssid, password);

  int retries = 0;
  while (WiFi.status() != WL_CONNECTED && retries < 20) {
    delay(500);
    Serial.print(".");
    retries++;
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\nWi-Fi connected!");
    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());
  } else {
    Serial.println("\nWi-Fi connection failed.");
  }
}

// === Task: DHT11 (Temperature & Humidity) ===
void taskDHT(void * parameter) {
  for (;;) {
    float t = dht.readTemperature();
    float h = dht.readHumidity();

    if (isnan(t) || isnan(h)) {
      Serial.println("[DHT11] Failed to read");
    } else {
      Serial.print("[DHT11] Temperature: ");
      Serial.print(t);
      Serial.print(" °C | Humidity: ");
      Serial.print(h);
      Serial.println(" %");
    }

    vTaskDelay(pdMS_TO_TICKS(5000)); // Every 5 seconds
  }
}

// === Task: Soil Moisture Sensor ===
void taskSoil(void * parameter) {
  for (;;) {
    int rawValue = analogRead(SOIL_SENSOR_PIN);
    int moisture = map(rawValue, 4095, 0, 0, 100); // Adjust as needed

    Serial.print("[Soil] Moisture: ");
    Serial.print(moisture);
    Serial.println(" %");

    vTaskDelay(pdMS_TO_TICKS(3000)); // Every 3 seconds
  }
}

// === Task: MQ135 Air Quality Sensor ===
void taskAir(void * parameter) {
  for (;;) {
    int airRaw = analogRead(MQ135_PIN);
    int airQuality = map(airRaw, 0, 4095, 0, 100); // Adjust as needed

    Serial.print("[Air] Quality: ");
    Serial.print(airQuality);
    Serial.println(" %");

    vTaskDelay(pdMS_TO_TICKS(4000)); // Every 4 seconds
  }
}

// === Setup ===
void setup() {
  Serial.begin(115200);
  dht.begin();
  connectToWiFi();

  // Create FreeRTOS tasks (all on core 1)
  xTaskCreatePinnedToCore(taskDHT, "TaskDHT", 2000, NULL, 1, &TaskDHT, 1);
  xTaskCreatePinnedToCore(taskSoil, "TaskSoil", 2000, NULL, 1, &TaskSoil, 1);
  xTaskCreatePinnedToCore(taskAir, "TaskAir", 2000, NULL, 1, &TaskAir, 1);
}

// === Loop (Empty – FreeRTOS handles everything) ===
void loop() {}

