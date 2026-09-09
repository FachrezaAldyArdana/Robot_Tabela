# Prototipe Robot Penanam Benih Padi Langsung (TABELA) Otomatis Berbasis IoT

Proyek Kerja Praktik berupa rancang bangun prototipe robot penanam benih padi langsung (*direct seeding robot*) berbasis mikrokontroler ESP32 DevKit V1 dan platform IoT Blynk. Proyek ini dikembangkan di bawah studi kasus **PT Innovasindo Smart System** sebagai bentuk penerapan mekanisasi dan otomasi di sektor pertanian (*smart farming*).

---

## 📌 Latar Belakang & Tujuan

Konsumsi beras nasional di Indonesia mencapai $\pm 50\text{ juta ton/tahun}$. Namun, proses budidaya padi konvensional masih banyak bergantung pada metode tanam manual yang membutuhkan banyak tenaga kerja serta memakan waktu. 

Melalui metode **Tanam Benih Langsung (Tabela)**, robot ini dirancang untuk:
- Mengotomasi penanaman benih padi di lahan dengan interval jarak tanam teratur ($20\text{ cm}$)
- Memantau kapasitas wadah penampung benih (*seed box*) secara *real-time* via sensor ultrasonik.
- Mengurangi ketergantungan tenaga kerja manual dan mempercepat proses tanam.

---

## ⚙️ Cara Kerja & Diagram Alir Sistem

Sistem beroperasi menggunakan logika kendali berbasis mesin status (*Finite State Machine*):
1. **Inisialisasi & Cek Kapasitas**: Saat saklar aktif, sensor ultrasonik mengukur persentase benih pada *seed box*. Jika benih terdeteksi $\le 30\%$, sistem memberikan peringatan untuk pengisian ulang.
2. **Aktivasi IoT**: Pengguna menekan tombol saklar (V0) di aplikasi Blynk untuk memulai operasi robot.
3. **Siklus Penanaman ($20\text{ cm}$)**:
   - Robot bergerak maju selama interval waktu yang setara dengan jarak $20\text{ cm}$.
   - Pengereman dinamis (*dynamic brake*) diaktifkan agar robot berhenti presisi.
   - Katup servo membuka ke sudut $63^\circ$ selama 2 detik untuk menjatuhkan benih.
   - Katup servo kembali menutup ke sudut $100^\circ$, lalu robot melanjutkan siklus berikutnya secara otomatis.

---

## 🛠️ Komponen & Konfigurasi Pinout

### Spesifikasi Perangkat Keras:
- **Mikrokontroler**: ESP32 DevKit V1
- **Sensor**: Sensor Ultrasonik HC-SR04
- **Aktuator**:
  - Motor DC + Roda Mecanum / Penggerak Berpola
  - Driver Motor L298N (Dual H-Bridge)
  - Micro Servo SG90 (Mekanisme Katup Pelepas Benih)
- **Daya**: Paket Baterai DC

### Tabel Pinout:

| Modul / Komponen | Pin Modul | Pin ESP32 (GPIO) | Deskripsi Fungsional |
| :--- | :--- | :--- | :--- |
| **Driver Motor L298N** | ENA | **GPIO 13** | PWM Sinyal Kecepatan Roda Kiri |
| | ENB | **GPIO 21** | PWM Sinyal Kecepatan Roda Kanan |
| | IN1 / IN2 | **GPIO 17 / 16** | Logika Arah Roda Kiri |
| | IN3 / IN4 | **GPIO 22 / 23** | Logika Arah Roda Kanan |
| **Sensor Ultrasonik** | Trig | **GPIO 27** | Sinyal Pemicu Pulsa Ultrasonik |
| | Echo | **GPIO 26** | Penerima Pantulan Pulsa |
| **Servo Penabur Benih**| Signal | **GPIO 12** | Kendali Bukaan Sudut Katup |

---
### 1. Pengujian Mekanisme Penebaran Benih (Interval Jarak)
*Ambang buka katup diatur pada setiap kelipatan 20 cm:*

| Jarak Tempuh | Sudut Servo | Kondisi Katup | Status Siklus |
| :---: | :---: | :---: | :--- |
| **0 cm** | 100° | Tertutup | Siap jalan |
| **10 cm** | 100° | Tertutup | Bergerak maju |
| **20 cm** | **63°** | **Terbuka** | Penebaran benih titik ke-1 |
| **30 cm** | 100° | Tertutup | Bergerak maju |
| **40 cm** | **63°** | **Terbuka** | Penebaran benih titik ke-2 |
| **60 cm** | **63°** | **Terbuka** | Penebaran benih titik ke-3 |
| **80 cm** | **63°** | **Terbuka** | Penebaran benih titik ke-4 |
| **100 cm**| **63°** | **Terbuka** | Penebaran benih titik ke-5 |

### 2. Akurasi Pembacaan Level Benih (Seed Box)
*Kalibrasi wadah benih: Kosong (9 cm) – Penuh (3 cm):*

| Level Wadah (%) | Jarak Aktual (cm) | Pembacaan Sensor (cm) | Selisih Error (cm) |
| :---: | :---: | :---: | :---: |
| **0%** | 9,0 | 8,9 | 0,1 |
| **25%** | 7,5 | 7,0 | 0,5 |
| **50%** | 6,0 | 5,3 | 0,7 |
| **75%** | 4,5 | 4,0 | 0,5 |
| **100%** | 3,0 | 3,0 | **0,0** |

## ⚠️ Batasan Masalah & Asumsi Kerja

- Pengujian prototipe diasumsikan pada permukaan tanah yang rata dengan lebar jalur tanam seragam.
- Volume genangan air sawah berada pada kedalaman minimal yang aman untuk pergerakan roda sasis robot.
- Fokus utama prototipe adalah validasi konsep fungsional kendali servo dan pemantauan level benih berbasis IoT.

