#include <WiFi.h>
#include <ArduinoOTA.h>
#include <ESP32Servo.h>

// =====================
// WIFI CONFIG
// =====================
const char* ssid = "WIFI_NAME";
const char* password = "WIFI_PASSWORD";

// =====================
// Pin Configuration
// =====================
#define TRIG_PIN    25
#define ECHO_PIN    26

#define BUZZER_PIN  27
#define SERVO_PIN   32

#define LED_PIN     33

Servo trashServo;

bool isOpen = false;

// =====================
// Read Distance HC-SR04
// =====================
float readDistance()
{
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);

  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);

  digitalWrite(TRIG_PIN, LOW);

  long duration = pulseIn(ECHO_PIN, HIGH, 30000);

  if (duration == 0)
    return -1;

  return duration * 0.0343 / 2.0;
}

// =====================
// Membership Function
// =====================

// Dekat
float muDekat(float x)
{
  if (x <= 10)
    return 1.0;

  else if (x < 20)
    return (20.0 - x) / 10.0;

  return 0.0;
}

// Sedang
float muSedang(float x)
{
  if (x <= 10 || x >= 40)
    return 0.0;

  else if (x <= 25)
    return (x - 10.0) / 15.0;

  return (40.0 - x) / 15.0;
}

// Jauh
float muJauh(float x)
{
  if (x <= 30)
    return 0.0;

  else if (x < 50)
    return (x - 30.0) / 20.0;

  return 1.0;
}

// =====================
// Setup
// =====================
void setup()
{
  Serial.begin(115200);

  // =====================
  // WIFI
  // =====================
  WiFi.begin(ssid, password);

  Serial.print("Connecting to WiFi");

  int wifiTimeout = 20;

  while (WiFi.status() != WL_CONNECTED && wifiTimeout > 0)
  {
    delay(500);
    Serial.print(".");
    wifiTimeout--;
  }

  if (WiFi.status() == WL_CONNECTED)
  {
    Serial.println();
    Serial.println("WiFi Connected");

    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());

    ArduinoOTA.setHostname("SmartTrashBin");
    ArduinoOTA.begin();

    Serial.println("OTA Ready");
  }
  else
  {
    Serial.println();
    Serial.println("WiFi Failed");
  }

  // =====================
  // Hardware Setup
  // =====================
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(LED_PIN, OUTPUT);

  digitalWrite(BUZZER_PIN, LOW);
  digitalWrite(LED_PIN, LOW);

  trashServo.attach(SERVO_PIN);
  trashServo.write(0);

  Serial.println("SMART TRASH BIN FUZZY LOGIC");
}

// =====================
// Loop
// =====================
void loop()
{
  ArduinoOTA.handle();

  float distance = readDistance();

  if (distance <= 0 || distance > 400)
  {
    delay(200);
    return;
  }

  // =====================
  // FUZZIFICATION
  // =====================
  float dekat  = muDekat(distance);
  float sedang = muSedang(distance);
  float jauh   = muJauh(distance);

  // =====================
  // DEFUZZIFICATION
  // =====================
  float numerator =
      (dekat * 90.0) +
      (sedang * 45.0) +
      (jauh * 0.0);

  float denominator =
      dekat + sedang + jauh;

  float fuzzyOutput = 0;

  if (denominator > 0)
  {
    fuzzyOutput = numerator / denominator;
  }

  int servoAngle = (int)fuzzyOutput;
  servoAngle = constrain(servoAngle, 0, 90);

  // =====================
  // Servo Control
  // =====================
  bool shouldOpen = servoAngle > 20;

  if (shouldOpen && !isOpen)
  {
    digitalWrite(BUZZER_PIN, HIGH);
    delay(150);
    digitalWrite(BUZZER_PIN, LOW);

    isOpen = true;
  }

  if (!shouldOpen)
  {
    isOpen = false;
  }

  trashServo.write(servoAngle);

  // =====================
  // LED Indicator
  // =====================
  digitalWrite(LED_PIN, shouldOpen);

  // =====================
  // Serial Monitor
  // =====================
  Serial.println("============================");

  Serial.print("Jarak      : ");
  Serial.print(distance);
  Serial.println(" cm");

  Serial.print("μ Dekat    : ");
  Serial.println(dekat, 2);

  Serial.print("μ Sedang   : ");
  Serial.println(sedang, 2);

  Serial.print("μ Jauh     : ");
  Serial.println(jauh, 2);

  Serial.print("Defuzzy    : ");
  Serial.println(fuzzyOutput, 2);

  Serial.print("Servo      : ");
  Serial.print(servoAngle);
  Serial.println(" derajat");

  Serial.print("Open       : ");
  Serial.println(shouldOpen);

  delay(500);
}