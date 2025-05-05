#include <Wire.h> // Diperlukan untuk komunikasi I2C
#include <LiquidCrystal_I2C.h> // Library untuk LCD I2C

// --- Pengaturan Pin ---
const int SENSOR_PIN = A0;      // Pin analog potensiometer
const int LED_GREEN_PIN = 2;    // Pin LED Hijau
const int LED_YELLOW_PIN = 3;   // Pin LED Kuning
const int LED_RED_PIN = 4;      // Pin LED Merah

// --- Pengaturan Kalibrasi & Threshold ---
const float MAX_ANALOG_READ = 1023.0;
const float MAX_WIND_SPEED_KMH = 100.0;
const float SPEED_THRESHOLD_LOW = 30.0;
const float SPEED_THRESHOLD_MEDIUM = 70.0;

// --- Inisialisasi LCD ---
// Alamat I2C umum adalah 0x27 atau 0x3F. Wokwi biasanya menggunakan 0x27.
// Ukuran LCD: 16 kolom, 2 baris.
LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {
  // Mulai komunikasi serial (tetap berguna untuk debug)
  Serial.begin(9600);
  Serial.println("Simulasi Pengukur Kecepatan Angin - LCD + LED");
  Serial.println("---------------------------------------------");

  // Inisialisasi LCD
  lcd.init(); // Mulai LCD
  lcd.backlight(); // Nyalakan lampu latar
  lcd.clear(); // Bersihkan layar
  lcd.setCursor(0, 0); // Pindah kursor ke baris 0, kolom 0
  lcd.print("Wind Speed Sim");
  lcd.setCursor(0, 1); // Pindah kursor ke baris 1, kolom 0
  lcd.print("Initializing...");
  delay(1500); // Tampilkan pesan awal sebentar
  lcd.clear(); // Bersihkan layar untuk data utama

  // Atur pin LED sebagai OUTPUT
  pinMode(LED_GREEN_PIN, OUTPUT);
  pinMode(LED_YELLOW_PIN, OUTPUT);
  pinMode(LED_RED_PIN, OUTPUT);

  // Pastikan semua LED mati saat memulai
  digitalWrite(LED_GREEN_PIN, LOW);
  digitalWrite(LED_YELLOW_PIN, LOW);
  digitalWrite(LED_RED_PIN, LOW);
}

void loop() {
  // 1. Baca nilai analog dari potensiometer
  int sensorValue = analogRead(SENSOR_PIN);

  // 2. Petakan nilai analog ke rentang kecepatan angin (km/jam)
  float windSpeed_kmh = (sensorValue / MAX_ANALOG_READ) * MAX_WIND_SPEED_KMH;

  // 3. Konversi ke m/s
  float windSpeed_mps = windSpeed_kmh / 3.6;

  // --- Tampilkan ke Serial Monitor (Tetap ada) ---
  Serial.print("Sensor: ");
  Serial.print(sensorValue);
  Serial.print(" | Speed: ");
  Serial.print(windSpeed_kmh, 1);
  Serial.print(" km/h (");
  Serial.print(windSpeed_mps, 1);
  Serial.print(" m/s)");

  // --- Kontrol LED ---
  String currentLevel = ""; // Untuk ditampilkan juga di LCD
  if (windSpeed_kmh < SPEED_THRESHOLD_LOW) {
    digitalWrite(LED_GREEN_PIN, HIGH);
    digitalWrite(LED_YELLOW_PIN, LOW);
    digitalWrite(LED_RED_PIN, LOW);
    Serial.println(" -> Low");
    currentLevel = "LOW "; // Spasi agar menimpa teks lama di LCD
  } else if (windSpeed_kmh < SPEED_THRESHOLD_MEDIUM) {
    digitalWrite(LED_GREEN_PIN, LOW);
    digitalWrite(LED_YELLOW_PIN, HIGH);
    digitalWrite(LED_RED_PIN, LOW);
    Serial.println(" -> Medium");
    currentLevel = "MED "; // Spasi agar menimpa teks lama di LCD
  } else {
    digitalWrite(LED_GREEN_PIN, LOW);
    digitalWrite(LED_YELLOW_PIN, LOW);
    digitalWrite(LED_RED_PIN, HIGH);
    Serial.println(" -> High");
    currentLevel = "HIGH";
  }

  // --- Tampilkan ke LCD ---
  // Baris 1: Tampilkan kecepatan dalam km/jam
  lcd.setCursor(0, 0); // Awal baris 1
  lcd.print("Speed: ");
  lcd.print(windSpeed_kmh, 1); // Angka kecepatan (1 desimal)
  lcd.print(" km/h "); // Unit (tambahkan spasi untuk overwrite)

  // Baris 2: Tampilkan kecepatan dalam m/s dan Level
  lcd.setCursor(0, 1); // Awal baris 2
  lcd.print(windSpeed_mps, 1); // Angka kecepatan (1 desimal)
  lcd.print(" m/s "); // Unit (tambahkan spasi)

  // Tampilkan Level di sisa space baris 2
  lcd.setCursor(11, 1); // Kolom ke-11, baris 2
  lcd.print("Lvl:" + currentLevel); // Tampilkan Level (LOW/MED/HIGH)


  // Tunggu sebentar sebelum membaca lagi
  delay(500);
}