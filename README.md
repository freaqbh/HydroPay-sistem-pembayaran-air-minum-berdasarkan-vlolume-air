# 💧 HydroPay - IoT Smart Water Dispenser with Closed-Loop Control

HydroPay adalah sistem dispenser air berbasis IoT yang menggunakan **closed-loop control system** untuk mengontrol volume air berdasarkan setpoint (target volume). Sistem ini mensimulasikan pembayaran berbasis volume (seperti QRIS) dan mengontrol aliran air secara otomatis.

---

## 🚀 Features

* 🔁 Closed-loop control (feedback-based system)
* 💧 Simulasi pengisian air berbasis volume (ml)
* 🎯 Setpoint volume (input dari Serial / simulasi QRIS)
* ⚡ Kontrol valve menggunakan relay
* 📟 OLED display (SSD1306) untuk monitoring real-time
* 💰 Perhitungan biaya otomatis berdasarkan volume
* 🧪 Simulasi berjalan di Wokwi (ESP32)

---

## 🧠 System Concept

HydroPay menggunakan konsep **feedback control system**:

```
Setpoint (Target Volume)
        ↓
Controller (ESP32)
        ↓
Valve (Relay)
        ↓
Water Flow (Simulated)
        ↓
Sensor Feedback (mlCount)
        ↓
Comparison → Control Action
```

### 📌 Penjelasan:

* **Setpoint**: Volume air yang diinginkan (misalnya 500 ml)
* **Feedback**: Volume aktual (`mlCount`)
* **Controller**: Logika program ESP32
* **Actuator**: Relay (valve)
* **Output**: Air yang mengalir

---

## ⚙️ Hardware Components

| Komponen               | Deskripsi                      |
| ---------------------- | ------------------------------ |
| ESP32                  | Microcontroller utama          |
| OLED SSD1306           | Display informasi              |
| Push Button            | Input manual                   |
| Relay Module           | Kontrol valve air              |
| LED (opsional)         | Indikator valve                |
| Flow Sensor (simulasi) | Digantikan dengan increment ml |

---

## 🔌 Pin Configuration

| Komponen       | Pin ESP32 |
| -------------- | --------- |
| Button         | GPIO 18   |
| Relay (Valve)  | GPIO 19   |
| OLED SDA       | GPIO 21   |
| OLED SCL       | GPIO 22   |
| LED (opsional) | GPIO 5    |

---

## 💻 How It Works

### 1. Idle Mode

* Sistem menunggu input dari:

  * Tombol
  * Serial (simulasi QRIS)

### 2. Input Setpoint

* User memasukkan target volume (contoh: `300 ml`)
* Bisa melalui Serial Monitor:

  ```
  300
  ```

### 3. Filling Process

* Valve terbuka
* Volume bertambah secara simulasi
* OLED menampilkan:

  * Volume saat ini
  * Target volume
  * Biaya

### 4. Closed-Loop Control

* Sistem membandingkan:

  ```
  mlCount >= targetVolume
  ```
* Jika tercapai:

  * Valve ditutup otomatis
  * Masuk ke halaman pembayaran

### 5. Payment Simulation

* Sistem menampilkan total biaya
* Simulasi QRIS ditampilkan di OLED

---

## 💰 Pricing Model

Harga ditentukan sebagai:

```
Rp 100 / 100 ml
```

Contoh:

* 500 ml → Rp 500
* 1000 ml → Rp 1000

---

## 🧪 Simulation (Wokwi)

Project ini dirancang untuk dijalankan di:
👉 https://wokwi.com/

### Langkah:

1. Pilih ESP32
2. Tambahkan:

   * OLED SSD1306
   * Button
   * Relay / LED
3. Upload kode
4. Gunakan Serial Monitor untuk input volume

---

## 📊 Example Usage

| Input | Output                              |
| ----- | ----------------------------------- |
| 300   | Sistem mengisi 300 ml lalu berhenti |
| 500   | Sistem mengisi 500 ml lalu berhenti |

---

## 🔥 Future Improvements

* 💳 Integrasi QRIS real (Midtrans / Xendit)
* 🌐 Backend server (Golang + gRPC)
* 📱 Dashboard web (Vue.js)
* 🔍 Sensor flow real (bukan simulasi)
* ⚠️ Leak detection system
* 🤖 AI-based consumption prediction

---

## 📚 Academic Value

Project ini mencakup konsep:

* IoT System Design
* Embedded System (ESP32)
* Feedback Control System
* Smart Metering
* Fintech Integration (simulated)

---

## 🧠 Key Insight

> “HydroPay menggunakan pendekatan closed-loop control dengan setpoint berupa volume air yang ditentukan dari input pengguna, serta feedback berupa volume aktual yang diukur secara real-time.”

---

## 📜 License

Free to use for educational purposes.

---

## 🙌 Author

Developed by: **[Your Name]**
Project: HydroPay IoT System
