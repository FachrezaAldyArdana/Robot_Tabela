# Robot Tanam Benih Langsung Otomatis Berbasis ESP32 & IoT Blynk

Proyek robotika agrikultur berupa robot tanam benih langsung (*direct seeding robot*) otomatis berbasis mikrokontroler ESP32. Robot bergerak secara periodik berdasarkan jarak tanam yang telah ditentukan, melepaskan benih menggunakan mekanisme katup servo, memantau ketersediaan benih di dalam wadah menggunakan sensor ultrasonik, serta dapat dikendalikan dari jarak jauh melalui aplikasi Blynk IoT.

---

## 📌 Fitur Utama

- **Navigasi Presisi Bertahap (Finite State Machine)**: Menggunakan logika FSM *non-blocking* berbasis `millis()` untuk mengatur siklus kerja:
  1. Bergerak maju sejauh $\approx 20\text{ cm}$
  2. Pengereman dinamis (*dynamic braking*) motor DC
  3. Berhenti dan membuka katup servo untuk menjatuhkan benih
  4. Menutup katup servo dan bersiap melanjutkan siklus berikutnya
- **Monitoring Kapasitas Benih Real-Time**: Sensor ultrasonik mendeteksi sisa isi benih di dalam wadah dan mengonversinya menjadi persentase ($0\text{–}100\%$) secara berkala tiap 500 ms tanpa mengganggu pergerakan roda.
- **Kendali Jarak Jauh via IoT Blynk**: Tombol Start/Stop terintegrasi pada aplikasi Blynk untuk mengaktifkan atau menghentikan robot secara instan dari jarak jauh.
- **Penyelarasan Kecepatan Motor (PWM)**: Menggunakan driver L298N dengan konfigurasi PWM terpisah antara roda kiri dan kanan untuk menjaga jalur robot tetap lurus di tanah.

---

## 🛠️ Komponen & Perangkat Keras

1. **ESP32 Development Board** (30 Pin / 38 Pin)
2. **Driver Motor L298N** (Dual H-Bridge Motor Driver)
3. **2x Motor DC + Roda** (Aktuator Penggerak Robot)
4. **Micro Servo SG90 / MG90S** (Mekanisme Katup Pelepas Benih)
5. **Sensor Ultrasonik HC-SR04** (Pemantau Level Wadah Benih)
6. **Baterai / Catu Daya DC** (Penyuplai daya motor & mikrokontroler)
7. Sasis robot dan wadah penampung benih

---

## 🔌 Konfigurasi Pinout

| Modul / Komponen | Pin Modul | Pin ESP32 (GPIO) | Keterangan |
| :--- | :--- | :--- | :--- |
| **Driver Motor L298N** | ENA | **GPIO 13** | PWM Kecepatan Motor Kiri |
| | ENB | **GPIO 21** | PWM Kecepatan Motor Kanan |
| | IN1 | **GPIO 17** | Logika Arah Motor Kiri |
| | IN2 | **GPIO 16** | Logika Arah Motor Kiri |
| | IN3 | **GPIO 22** | Logika Arah Motor Kanan |
| | IN4 | **GPIO 23** | Logika Arah Motor Kanan |
| **Sensor Ultrasonik** | Trig | **GPIO 27** | Pemicu Ultrasonik |
| | Echo | **GPIO 26** | Penerima Pantulan Gema |
| **Micro Servo** | Signal (PWM) | **GPIO 12** | Katup Buka-Tutup Benih |

---

## 📱 Konfigurasi Pin Virtual Blynk

| Datastream / Pin | Tipe Data | Keterangan |
| :--- | :--- | :--- |
| **V0** | Integer (0 / 1) | Saklar Start / Stop Kendali Operasional Robot |
| **V1** | Integer (0 – 100) | Tampilan Persentase Kapasitas Wadah Benih |

---

## 📚 Kebutuhan Pustaka (Libraries)

Pastikan pustaka berikut sudah terpasang di Arduino IDE:
- [Blynk](https://github.com/blynkkk/blynk-library) oleh Volodymyr Shymanskyy
- [ESP32Servo](https://github.com/madhephaestus/ESP32Servo) oleh Kevin Harrington
- `WiFi.h` *(Bawaan board package ESP32)*

---

## ⚙️ Petunjuk Pemasangan & Kalibrasi

1. **Klon Repositori**:
   ```bash
   git clone [https://github.com/FachrezaAldyArdana/NAMA_REPO_KAMU.git](https://github.com/FachrezaAldyArdana/NAMA_REPO_KAMU.git)
