version final code firebase.txt
#include <WiFi.h>
#include <Firebase_ESP_Client.h>
#include <DHT.h>
#include <addons/TokenHelper.h>  // For token generation
#include <addons/RTDBHelper.h>

// === Wi-Fi credentials ===
#define WIFI_SSID "TOPNET_2788"
#define WIFI_PASSWORD "9x9uq778dy"

// === Firebase credentials ===
#define API_KEY "AIzaSyCQNL5TOOt7AO92wMiBfuK5UK5n2uV7h1U"
#define DATABASE_URL "https://pfatest-5bf23-default-rtdb.europe-west1.firebasedatabase.app/"

// === Firebase user login ===
#define USER_EMAIL "eya.karoui@insat.ucar.tn"
#define USER_PASSWORD "I love newyork"

// === Firebase objects ===
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

// === Pin Definitions ===
#define DHTPIN 4              // DHT11 connected to GPIO4
#define DHTTYPE DHT11         // Sensor type
#define SOIL_SENSOR_PIN 34    // Soil moisture analog output
#define MQ135_PIN 35          // MQ135 air quality analog output

// === Initialize Sensors ===
DHT dht(DHTPIN, DHTTYPE);


// === Setup ===
void setup() {
  Serial.begin(115200);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\n Connected to Wi-Fi!");

  // Setup Firebase config
  config.api_key = API_KEY;
  config.database_url = DATABASE_URL;

  // Set user credentials
  auth.user.email = USER_EMAIL;
  auth.user.password = USER_PASSWORD;

  // Begin Firebase
  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);

  Serial.println("📡 Firebase initialized.");
}

// === Loop ===
void loop() {
  if (Firebase.ready()) {
    float t = dht.readTemperature();
    float h = dht.readHumidity();
    int rawValue = analogRead(SOIL_SENSOR_PIN);
    int moisture = map(rawValue, 4095, 0, 0, 100);
    int airRaw = analogRead(MQ135_PIN);
    int airQuality = map(airRaw, 0, 4095, 0, 100);

    if (Firebase.RTDB.setFloat(&fbdo, "/sensor/dhtT", t)) {
      Serial.println(" Sent: dhtT to /sensor/dhtT");
    } else {
      Serial.print(" dht Failed to read T: ");
      Serial.println(fbdo.errorReason());
    }
    if (Firebase.RTDB.setFloat(&fbdo, "/sensor/dhtH", h)) {
      Serial.println(" Sent: dhtH to /sensor/dhtH");
    } else {
      Serial.print(" dht Failed to read H: ");
      Serial.println(fbdo.errorReason());
    }
    if (Firebase.RTDB.setInt(&fbdo, "/sensor/moisture", moisture)) {
      Serial.println(" Sent: moisture to /sensor/moisture");
    } else {
      Serial.print(" Failed to read moisture: ");
      Serial.println(fbdo.errorReason());
    }
    if (Firebase.RTDB.setInt(&fbdo, "/sensor/airQ", airQuality)) {
      Serial.println(" Sent: airQuality to /sensor/airQ");
    } else {
      Serial.print(" Failed to send airQ: ");
      Serial.println(fbdo.errorReason());
    }
  } else {
    Serial.println("Firebase not ready.");
  }
  delay(1000);
}
