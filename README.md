# Smart Trash Bin Berbasis ESP32 dan Logika Fuzzy

## Deskripsi Proyek

Smart Trash Bin merupakan tempat sampah pintar yang dapat membuka dan menutup tutup secara otomatis menggunakan sensor ultrasonik HC-SR04 dan servo motor.

Sistem ini menggunakan metode Logika Fuzzy untuk menentukan sudut bukaan tutup berdasarkan jarak objek yang terdeteksi.

## Fitur

* Membuka tutup tempat sampah secara otomatis
* Deteksi jarak menggunakan sensor HC-SR04
* Implementasi Logika Fuzzy
* Kontrol servo motor
* Indikator LED
* Notifikasi buzzer
* OTA (Over-The-Air Update)

## Komponen yang Digunakan

* ESP32 DevKit V1
* Sensor Ultrasonik HC-SR04
* Servo Motor SG90
* LED
* Buzzer
* Tong Sampah

## Konfigurasi Pin

| Komponen     | GPIO ESP32 |
| ------------ | ---------- |
| Trig HC-SR04 | GPIO 25    |
| Echo HC-SR04 | GPIO 26    |
| Buzzer       | GPIO 27    |
| Servo Motor  | GPIO 32    |
| LED          | GPIO 33    |

## Cara Kerja Sistem

1. Sensor HC-SR04 mendeteksi jarak objek.
2. Nilai jarak diproses menggunakan logika fuzzy.
3. Sistem melakukan proses fuzzifikasi dan defuzzifikasi.
4. Hasil defuzzifikasi digunakan untuk menentukan sudut servo.
5. Servo membuka atau menutup tutup tempat sampah.
6. LED dan buzzer berfungsi sebagai indikator.

## Perangkat Lunak

* Visual Studio Code
* PlatformIO
* Bahasa C++
* Library ESP32Servo
* ArduinoOTA


## Pengembang

Kelompok 1

Mahasiswa Teknik Informatika
