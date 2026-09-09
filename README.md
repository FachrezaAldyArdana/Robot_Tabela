# Prototipe Robot Penanam Benih Padi Langsung (TABELA) Otomatis Berbasis IoT

Proyek Kerja Praktik berupa rancang bangun prototipe robot penanam benih padi langsung (*direct seeding robot*) berbasis mikrokontroler ESP32 DevKit V1 dan platform IoT Blynk. Proyek ini dikembangkan di bawah studi kasus **PT Innovasindo Smart System** sebagai bentuk penerapan mekanisasi dan otomasi di sektor pertanian (*smart farming*)[cite: 5].

---

## 📌 Latar Belakang & Tujuan

Konsumsi beras nasional di Indonesia mencapai $\pm 50\text{ juta ton/tahun}$[cite: 5]. Namun, proses budidaya padi konvensional masih banyak bergantung pada metode tanam manual yang membutuhkan banyak tenaga kerja serta memakan waktu[cite: 5]. 

Melalui metode **Tanam Benih Langsung (Tabela)**, robot ini dirancang untuk[cite: 5]:
- Mengotomasi penanaman benih padi di lahan dengan interval jarak tanam teratur ($20\text{ cm}$)[cite: 4, 5].
- Memantau kapasitas wadah penampung benih (*seed box*) secara *real-time* via sensor ultrasonik[cite: 4, 5].
- Mengurangi ketergantungan tenaga kerja manual dan mempercepat proses tanam[cite: 5].

---

## ⚙️ Cara Kerja & Diagram Alir Sistem

Sistem beroperasi menggunakan logika kendali berbasis mesin status (*Finite State Machine*)[cite: 4]:
1. **Inisialisasi & Cek Kapasitas**: Saat saklar aktif, sensor ultrasonik mengukur persentase benih pada *seed box*[cite: 4, 5]. Jika benih terdeteksi $\le 30\%$, sistem memberikan peringatan untuk pengisian ulang[cite: 5].
2. **Aktivasi IoT**: Pengguna menekan tombol saklar (V0) di aplikasi Blynk untuk memulai operasi robot[cite: 4, 5].
3. **Siklus Penanaman ($20\text{ cm}$)**:
   - Robot bergerak maju selama interval waktu yang setara dengan jarak $20\text{ cm}$[cite: 4, 5].
   - Pengereman dinamis (*dynamic brake*) diaktifkan agar robot berhenti presisi[cite: 4].
   - Katup servo membuka ke sudut $63^\circ$ selama 2 detik untuk menjatuhkan benih[cite: 4, 5].
   - Katup servo kembali menutup ke sudut $100^\circ$, lalu robot melanjutkan siklus berikutnya secara otomatis[cite: 4, 5].

---

## 🛠️ Komponen & Konfigurasi Pinout

### Spesifikasi Perangkat Keras:
- **Mikrokontroler**: ESP32 DevKit V1[cite: 5]
- **Sensor**: Sensor Ultrasonik HC-SR04[cite: 4, 5]
- **Aktuator**:
  - Motor DC + Roda Mecanum / Penggerak Berpola[cite: 5]
  - Driver Motor L298N (Dual H-Bridge)[cite: 4, 5]
  - Micro Servo SG90 (Mekanisme Katup Pelepas Benih)[cite: 4, 5]
- **Daya**: Paket Baterai DC[cite: 4, 5]

### Tabel Pinout:

| Modul / Komponen | Pin Modul | Pin ESP32 (GPIO) | Deskripsi Fungsional |
| :--- | :--- | :--- | :--- |
| **Driver Motor L298N** | ENA | **GPIO 13** | PWM Sinyal Kecepatan Roda Kiri[cite: 4] |
| | ENB | **GPIO 21** | PWM Sinyal Kecepatan Roda Kanan[cite: 4] |
| | IN1 / IN2 | **GPIO 17 / 16** | Logika Arah Roda Kiri[cite: 4] |
| | IN3 / IN4 | **GPIO 22 / 23** | Logika Arah Roda Kanan[cite: 4] |
| **Sensor Ultrasonik** | Trig | **GPIO 27** | Sinyal Pemicu Pulsa Ultrasonik[cite: 4] |
| | Echo | **GPIO 26** | Penerima Pantulan Pulsa[cite: 4] |
| **Servo Penabur Benih**| Signal | **GPIO 12** | Kendali Bukaan Sudut Katup[cite: 4] |

---

## 📊 Hasil Pengujian Sistem

### 1. Pengujian Mekanisme Penebaran Benih (Interval Jarak)
*Ambang buka katup diatur pada setiap kelipatan $20\text{ cm}$:*[cite: 4, 5]

| Jarak Tempuh | Sudut Servo | Kondisi Katup | Status Siklus |
| :---: | :---: | :---: | :--- |
| **$0\text{ cm}$** | $100^\circ$ | Tertutup | Siap jalan[cite: 5] |
| **$10\text{ cm}$** | $100^\circ$ | Tertutup | Bergerak maju[cite: 5] |
| **$20\text{ cm}$** | **$63^\circ$** | **Terbuka** | Penebaran benih titik ke-1[cite: 5] |
| **$30\text{ cm}$** | $100^\circ$ | Tertutup | Bergerak maju[cite: 5] |
| **$40\text{ cm}$** | **$63^\circ$** | **Terbuka** | Penebaran benih titik ke-2[cite: 5] |
| **$60\text{ cm}$** | **$63^\circ$** | **Terbuka** | Penebaran benih titik ke-3[cite: 5] |
| **$80\text{ cm}$** | **$63^\circ$** | **Terbuka** | Penebaran benih titik ke-4[cite: 5] |
| **$100\text{ cm}$**| **$63^\circ$** | **Terbuka** | Penebaran benih titik ke-5[cite: 5] |

### 2. Akurasi Pembacaan Level Benih (*Seed Box*)
*Kalibrasi wadah benih: Kosong ($9\text{ cm}$) – Penuh ($3\text{ cm}$):*[cite: 4, 5]

| Level Wadah (%) | Jarak Aktual (cm) | Pembacaan Sensor (cm) | Selisih Error (cm) |
| :---: | :---: | :---: | :---: |
| **$0\%$** | $9{,}0$ | $8{,}9$ | $0{,}1$[cite: 5] |
| **$25\%$** | $7{,}5$ | $7{,}0$ | $0{,}5$[cite: 5] |
| **$50\%$** | $6{,}0$ | $5{,}3$ | $0{,}7$[cite: 5] |
| **$75\%$** | $4{,}5$ | $4{,}0$ | $0{,}5$[cite: 5] |
| **$100\%$** | $3{,}0$ | $3{,}0$ | **$0{,}0$**[cite: 5] |

---

## ⚠️ Batasan Masalah & Asumsi Kerja

- Pengujian prototipe diasumsikan pada permukaan tanah yang rata dengan lebar jalur tanam seragam[cite: 5].
- Volume genangan air sawah berada pada kedalaman minimal yang aman untuk pergerakan roda sasis robot[cite: 5].
- Fokus utama prototipe adalah validasi konsep fungsional kendali servo dan pemantauan level benih berbasis IoT[cite: 4, 5].


-
