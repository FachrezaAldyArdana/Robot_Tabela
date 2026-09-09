#define BLYNK_TEMPLATE_ID "TMPL6z_K-imgr"
#define BLYNK_TEMPLATE_NAME "monitoring"
#define BLYNK_PRINT Serial

#include <WiFi.h>
#include <BlynkSimpleEsp32.h>
#include <ESP32Servo.h>

// ================= WIFI =================
char ssid[] = "FnR";
char pass[] = "rasahguyaguyu";
char auth[] = "k9FxgJhGqZ41_gpO_3k2wyKEHvvCMIDH";

// ================= PIN L298N =================
#define ENA 13
#define ENB 21
#define IN1 17
#define IN2 16
#define IN3 22
#define IN4 23

// ================= PWM =================
#define PWM_FREQ 1000
#define PWM_RES  8

// ================= ULTRASONIC =================
#define trig 27
#define echo 26
const float JARAK_PENUH  = 2.0; // cm saat wadah penuh
const float JARAK_KOSONG = 9.0; // cm saat wadah kosong 

// ================= SERVO =================
#define servoPin 12
const int SERVO_TUTUP = 100;
const int SERVO_BUKA  = 63;

// ================= MOTOR =================
const int SPEED_KIRI  = 165;
const int SPEED_KANAN = 140;

// ================= WAKTU =================
const unsigned long TIME_MAJU_20CM = 400; // Sesuaikan waktu untuk jarak 20cm
const unsigned long TIME_BRAKE     = 200;
const unsigned long SERVO_DELAY    = 2000; // Beri waktu lebih lama agar isi turun

// ================= GLOBAL =================
Servo servo;
bool robotAktif = false;
unsigned long lastActionTime = 0;
unsigned long lastUltraTime = 0;
const unsigned long ULTRA_INTERVAL = 500; // Baca sensor tiap 0.5 detik
int isiWadah = 0;

enum RobotState {
  IDLE,
  MAJU_20CM,
  REM_MOTOR,
  STOP_AND_DOSE, // State baru untuk proses servo
  TUTUP_SERVO
};

RobotState currentState = IDLE;

// ================= BLYNK =================
BLYNK_WRITE(V0) {
  robotAktif = param.asInt();
  if (!robotAktif) {
    currentState = IDLE;
    berhenti();
  }
}

// ================= SETUP =================
void setup() {
  Serial.begin(115200);

  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  ledcAttach(ENA, PWM_FREQ, PWM_RES);
  ledcAttach(ENB, PWM_FREQ, PWM_RES);

  pinMode(trig, OUTPUT);
  pinMode(echo, INPUT);

  servo.setPeriodHertz(50); 
  servo.attach(servoPin, 500, 2400);
  servo.write(SERVO_TUTUP);

  Blynk.begin(auth, ssid, pass);
}

// ================= LOOP =================
void loop() {
  Blynk.run();
  unsigned long now = millis();

  // --- PEMBACAAN SENSOR (Setiap 500ms tanpa mengganggu gerakan) ---
  if (now - lastUltraTime >= ULTRA_INTERVAL) {
    lastUltraTime = now;
    isiWadah = bacaIsiWadah();
    Blynk.virtualWrite(V1, isiWadah);
    Serial.print("Isi Wadah: "); Serial.print(isiWadah); Serial.println("%");
  }

  if (!robotAktif) return;

  // --- LOGIKA PERGERAKAN ---
  switch (currentState) {
    case IDLE:
      maju();
      lastActionTime = now;
      currentState = MAJU_20CM;
      break;

    case MAJU_20CM:
      if (now - lastActionTime >= TIME_MAJU_20CM) {
        rem();
        lastActionTime = now;
        currentState = REM_MOTOR;
      }
      break;

    case REM_MOTOR:
      if (now - lastActionTime >= TIME_BRAKE) {
        berhenti();
        servo.write(SERVO_BUKA);
        lastActionTime = now;
        currentState = STOP_AND_DOSE;
      }
      break;

    case STOP_AND_DOSE:
      if (now - lastActionTime >= SERVO_DELAY) {
        servo.write(SERVO_TUTUP);
        lastActionTime = now;
        currentState = TUTUP_SERVO;
      }
      break;

    case TUTUP_SERVO:
      // Jeda sebentar setelah tutup sebelum maju lagi
      if (now - lastActionTime >= 500) {
        maju();
        lastActionTime = now;
        currentState = MAJU_20CM;
      }
      break;
  }
}

// ================= SENSOR FUNCTION =================
int bacaIsiWadah() {
  digitalWrite(trig, LOW);
  delayMicroseconds(2);
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);

  // Timeout 20ms cukup untuk jarak ~3 meter
  long durasi = pulseIn(echo, HIGH, 20000); 
  
  if (durasi == 0) return 0;

  float jarak = durasi * 0.034 / 2;
  
  // Membatasi jarak agar tidak keluar range
  if (jarak > JARAK_KOSONG) jarak = JARAK_KOSONG;
  if (jarak < JARAK_PENUH) jarak = JARAK_PENUH;

  // Map jarak ke persen (0-100%)
  // JARAK_KOSONG (misal 15cm) = 0%
  // JARAK_PENUH (misal 3cm) = 100%
  int persen = map(jarak * 10, JARAK_KOSONG * 10, JARAK_PENUH * 10, 0, 100);
  return constrain(persen, 0, 100);
}

// ================= MOTOR FUNCTIONS =================
void maju() {
  ledcWrite(ENA, SPEED_KIRI);
  ledcWrite(ENB, SPEED_KANAN);
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void berhenti() {
  ledcWrite(ENA, 0);
  ledcWrite(ENB, 0);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}

void rem() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, HIGH);
}