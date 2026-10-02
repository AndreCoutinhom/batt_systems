#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>
#include <WebSocketsServer.h>
#include <ArduinoJson.h>
#include <Wire.h>
#include <VL53L0X.h>   // Pololu
#include <ESPmDNS.h>

// ===== AP =====
const char* AP_SSID = "ESP32-AP";
const char* AP_PASS = "12345678";

// ===== Pines / limites =====
#define MOTOR_PIN 10
#define MAX_CM    200
#define SDA_PIN   6
#define SCL_PIN   7

// ===== HTTP + WS =====
WebServer server(80);
WebSocketsServer ws(81);

// ===== Sensor =====
VL53L0X lox;

// ===== Thresholds (cm) =====
volatile uint16_t limiteLeve  = 100;
volatile uint16_t limiteMedio = 50;
volatile uint16_t limiteAlto  = 10;

// ===== HTML/CSS/JS embutidos =====
extern const char INDEX_HTML[] PROGMEM;
extern const char STYLES_CSS[] PROGMEM;
extern const char APP_JS[]     PROGMEM;

// ===== Níveis =====
enum Level { LV_FORA, LV_LEVE, LV_MEDIO, LV_ALTO };

static inline uint16_t clamp_cm(int v){
  if (v < 0) return 0;
  if (v > MAX_CM) return MAX_CM;
  return (uint16_t)v;
}
static inline void normalizeThresholds(){
  if (limiteMedio > limiteLeve)  limiteMedio = limiteLeve;
  if (limiteAlto  > limiteMedio) limiteAlto  = limiteMedio;
}
static inline const char* levelToStr(Level lv){
  switch (lv) {
    case LV_ALTO:  return "ALTO";
    case LV_MEDIO: return "MÉDIO";
    case LV_LEVE:  return "LEVE";
    default:       return "FORA";
  }
}
static inline Level classify(uint16_t d){
  if (d <= limiteAlto)       return LV_ALTO;
  else if (d <= limiteMedio) return LV_MEDIO;
  else if (d <= limiteLeve)  return LV_LEVE;
  else                       return LV_FORA;
}

// ===== HTTP handlers =====
void handleRoot(){  server.send_P(200, "text/html; charset=utf-8", INDEX_HTML); }
void handleCss(){   server.send_P(200, "text/css", STYLES_CSS); }
void handleJs(){    server.send_P(200, "application/javascript", APP_JS); }
void handleOk(){    server.send(200, "text/plain", "OK"); }

// ===== WebSocket =====
void wsEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length){
  if (type == WStype_TEXT){
    StaticJsonDocument<192> doc;
    DeserializationError err = deserializeJson(doc, payload, length);
    if (!err){
      if (doc.containsKey("limiteLeve"))  limiteLeve  = clamp_cm((int)doc["limiteLeve"]);
      if (doc.containsKey("limiteMedio")) limiteMedio = clamp_cm((int)doc["limiteMedio"]);
      if (doc.containsKey("limiteAlto"))  limiteAlto  = clamp_cm((int)doc["limiteAlto"]);
      normalizeThresholds();

      StaticJsonDocument<160> ack;
      ack["ack"] = "thresholds";
      ack["limiteLeve"]  = limiteLeve;
      ack["limiteMedio"] = limiteMedio;
      ack["limiteAlto"]  = limiteAlto;
      ack["units"]       = "cm";
      String out; serializeJson(ack, out);
      ws.sendTXT(num, out);
    }
  }
}

// ===== Broadcast =====
void sendReadingAll(uint16_t dist_cm, Level lv){
  StaticJsonDocument<128> out;
  out["distance"] = dist_cm;
  out["units"]    = "cm";
  out["level"]    = levelToStr(lv);
  String s; serializeJson(out, s);
  ws.broadcastTXT(s);
}

// ===== PWM do motor =====
const int LEDC_FREQ = 2000;
const int LEDC_BITS = 8;

inline void motorDuty(uint8_t duty){
  ledcWrite(MOTOR_PIN, duty);
}

  inline void motorByLevel(Level lv){
    switch(lv){
      case LV_ALTO:  motorDuty(220); break;
      case LV_MEDIO: motorDuty(140); break;
      case LV_LEVE:  motorDuty(80);  break;
      default:       motorDuty(0);   break;
    }
  }

// ===== Long Range =====
void tuneVL53_LongRange() {
  lox.setMeasurementTimingBudget(200000); // 200 ms
  lox.setSignalRateLimit(0.1);
}

// ===== Leitura robusta (mediana) =====
uint16_t readDistanceCmStable() {
  const int N = 5;
  uint16_t v[N]; int k = 0;

  for (int i = 0; i < N; i++) {
    uint16_t d = lox.readRangeSingleMillimeters();
    if (!lox.timeoutOccurred()) {
      v[k++] = clamp_cm(d / 10);
    }
    delay(10);
  }

  if (k == 0) return MAX_CM + 1;

  for (int i = 1; i < k; i++) {
    uint16_t t = v[i];
    int j = i - 1;
    while (j >= 0 && v[j] > t) { v[j+1] = v[j]; j--; }
    v[j+1] = t;
  }
  return v[k/2];
}

// ===== Loop de medição =====
uint32_t lastMeasureMs = 0;
const uint32_t measurePeriodMs = 300;

void setup(){
  Serial.begin(115200);
  delay(300);
  Serial.println("\n[MAIN] Boot AP-only / WebServer + WebSocketsServer");

  // Rede AP
  WiFi.mode(WIFI_AP);
  bool ok = WiFi.softAP(AP_SSID, AP_PASS);
  Serial.printf("[MAIN] softAP: %s\n", ok ? "OK" : "ERRO");
  Serial.print("[MAIN] AP IP: "); Serial.println(WiFi.softAPIP());

  // mDNS
  if (MDNS.begin("esp32")) {
    Serial.println("[MAIN] mDNS: esp32.local");
  } else {
    Serial.println("[MAIN] mDNS: falhou (use 192.168.4.1)");
  }

  // HTTP
  server.on("/", handleRoot);
  server.on("/styles.css", handleCss);
  server.on("/app.js", handleJs);
  server.on("/ok", handleOk);
  server.begin();

  // WebSocket
  ws.begin();
  ws.onEvent(wsEvent);

  // PWM do motor
  bool pwmOk = ledcAttach(MOTOR_PIN, LEDC_FREQ, LEDC_BITS);
  Serial.printf("[MAIN] ledcAttach(%d): %s\n", MOTOR_PIN, pwmOk ? "OK" : "FALHOU");
  motorDuty(0);


  // I2C + VL53L0X
  Wire.begin(SDA_PIN, SCL_PIN);
  lox.setTimeout(500);
  if (!lox.init()){
    Serial.println("[MAIN] VL53L0X FALHOU");
  } else {
    tuneVL53_LongRange();
  }

  Serial.printf("[MEM] free=%u  max=%u\n", ESP.getFreeHeap(), ESP.getMaxAllocHeap());
}

void loop(){
  server.handleClient();
  ws.loop();

  uint32_t now = millis();
  if (now - lastMeasureMs > measurePeriodMs){
    lastMeasureMs = now;

    uint16_t dist_cm = readDistanceCmStable();
    Level lv = classify(dist_cm);

    ===== DEBUG no Monitor Serial =====
    if (dist_cm > MAX_CM) {
      Serial.println("[VL53L0X] Distância: FORA DE ALCANCE / TIMEOUT");
    } else {
      Serial.printf("[VL53L0X] Distância: %u cm  |  Nível: %s\n",
                    dist_cm, levelToStr(lv));
    }
    ==================================

    motorByLevel(lv);
    sendReadingAll(dist_cm, lv);
  }
}
