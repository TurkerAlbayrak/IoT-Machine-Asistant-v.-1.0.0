# 📡 ESP32 & Python MQTT – Senkron Endüstriyel IoT Simülasyonu

![IoT](https://img.shields.io/badge/IoT-Industry%204.0-blue)
![ESP32](https://img.shields.io/badge/Platform-ESP32%20(Wokwi)-green)
![Python](https://img.shields.io/badge/Backend-Python%20MQTT-yellow)
![Protocol](https://img.shields.io/badge/Protocol-MQTT%20Request%2FResponse-orange)

Bu proje, **ESP32 (Wokwi Simülasyonu)** ile **Python MQTT Veri Üreticisi** arasında **Senkron (Request–Response)** haberleşme sağlayan bir Endüstriyel IoT (IIoT) simülasyonudur.

Klasik MQTT "Fire and Forget" (Yolla ve Unut) yapısı yerine, endüstriyel standartlara (SCADA/PLC) daha yakın olan **"El Sıkışmalı" (Handshake)** bir mimari kullanır.

---
<img width="1918" height="983" alt="node-red1" src="https://github.com/user-attachments/assets/3df05020-ea76-4e28-b99a-be8032b4f55b" />
<img width="1918" height="978" alt="wokwi1" src="https://github.com/user-attachments/assets/be7e257a-371d-46bd-87f6-94b812ba08e4" />
<img width="1523" height="751" alt="terminal1" src="https://github.com/user-attachments/assets/8b32e6fa-1247-4e45-8869-6e70076af9e3" />
<img width="1120" height="992" alt="pythoncode1" src="https://github.com/user-attachments/assets/a0102a6b-368c-43bb-a40f-ef409200d101" />
<img width="1917" height="988" alt="image" src="https://github.com/user-attachments/assets/c937aa8b-9000-40ea-a7c8-81dea848ba68" />
<img width="1916" height="986" alt="image" src="https://github.com/user-attachments/assets/86ecad1f-fc62-4dba-a9f4-c78e0cf1d3a9" />
<img width="1918" height="987" alt="image" src="https://github.com/user-attachments/assets/01cfcf66-1c22-4551-b432-f90c8ee6b051" />
<img width="1917" height="982" alt="image" src="https://github.com/user-attachments/assets/33a51c1a-439e-4179-9c46-703aae149676" />
<img width="1703" height="1077" alt="image" src="https://github.com/user-attachments/assets/2d59cad8-66b6-4fee-b5aa-2d36662d4b20" />
<img width="1918" height="992" alt="image" src="https://github.com/user-attachments/assets/3d15633f-23c3-41e4-8114-675eb3285fe2" />

## 🎯 Proje Amacı ve Farkı

Normal MQTT uygulamalarında sensör sürekli veri basar (Publish) ve abone (Subscribe) bu veriyi yakalamaya çalışır. Ancak bu durum veri yığılmasına veya işlemcinin şişmesine yol açabilir.

**Bu projede ise:**
* ✅ **Veri birikmesi olmaz:** Sadece istendiğinde veri üretilir.
* ✅ **Tam Senkronizasyon:** ESP32 ne zaman hazırsa o zaman veri ister.
* ✅ **Gerçek Zamanlı Kontrol:** Her istek = 1 taze veri.
* ✅ **Endüstriyel Uyumluluk:** Master-Slave veya Polling mantığı simüle edilir.

---

## 🏗 Sistem Mimarisi

Sistem, ESP32'nin "Master" gibi davranıp Python "Slave" simülatöründen veri talep etmesi üzerine kuruludur.

| Cihaz | Rol | Topic | Görev |
| :--- | :--- | :--- | :--- |
| **ESP32** | İstemci (Master) | `publish -> factory/request` | Veri talep eder ("send"). |
| **Python** | Sunucu (Slave) | `subscribe -> factory/request` | İsteği dinler. |
| **Python** | Sunucu (Slave) | `publish -> factory/data` | Sensör verisini JSON olarak döner. |
| **ESP32** | İstemci (Master) | `subscribe -> factory/data` | Gelen JSON verisini işler. |

### 🔄 Akış Diyagramı
1. **ESP32:** 1 saniye bekle -> `factory/request` kanalına "send" mesajı at.
2. **Python:** Mesajı al -> O anlık sensör değerlerini (Titreşim, Isı vb.) hesapla.
3. **Python:** `factory/data` kanalına JSON paketini gönder.
4. **ESP32:** Veriyi al -> JSON'u ayrıştır (Parse) -> Seri Port'a yaz.

---

## 🧠 Simüle Edilen Sensör Verileri

Python scripti, gerçek bir fabrikayı simüle etmek için matematiksel fonksiyonlar kullanır:

* **Titreşim (Vibration X, Y, Z):** Sinüs dalgaları + Rastgele gürültü (Motor balansı simülasyonu).
* **Sıcaklık (Temperature):** Zamana bağlı dalgalı artış/azalış.
* **Nem (Humidity):** Periyodik değişimler.
* **Gaz (Gas):** Normal değerler + Rastgele "Tehlike" (Alarm) sıçramaları.

**Örnek JSON Çıktısı:**
```json
{
  "vibration": {
    "x": 1.12,
    "y": -0.84,
    "z": 9.81
  },
  "temperature": 26.4,
  "humidity": 55.2,
  "gas": 312
}
```

Bash
python mqtt_responder.py
Not: Python scripti arka planda sürekli çalışmalıdır, çünkü ESP32'den gelecek istekleri dinler.

2️⃣ ESP32 Tarafı (Wokwi Simülasyonu)
Wokwi üzerinde yeni bir ESP32 projesi oluşturun ve aşağıdaki adımları uygulayın.

diagram.json (Kütüphaneler):
Aşağıdaki kütüphaneleri simülasyona eklediğinizden emin olun:

JSON
{
  "version": 1,
  "author": "Anonymous maker",
  "editor": "wokwi",
  "parts": [ { "type": "board-esp32-devkit-c-v4", "id": "esp", "top": 0, "left": 0, "attrs": {} } ],
  "connections": [],
  "dependencies": {} // Buraya kütüphaneler eklenecek.
}
Gerekli Kütüphaneler: PubSubClient, ArduinoJson

WiFi Ayarları:
Wokwi'nin sanal WiFi ağını kullanın:

SSID: Wokwi-GUEST

Password: (Boş bırakın)

MQTT Broker:
Kodunuzda broker.hivemq.com (veya tercih ettiğiniz başka bir public broker) kullanın.

⚙️ Teknik Detaylar
Zamanlama (Timing)
ESP32 kodu içerisinde const long interval = 1000; değeri polling (sorgulama) sıklığını belirler.

1000 ms: Standart izleme.

100 ms: Yüksek hızlı titreşim analizi (Simülasyon performansına bağlıdır).

Neden Request-Response?
Endüstriyel otomasyonda (PLC/SCADA), bir sensörün kontrolsüzce veri göndermesi (Spamming) istenmez. Merkezi ünite (Master), sırayla sensörleri gezerek (Polling) "Durumun ne?" diye sorar. Bu proje, MQTT protokolü üzerinde bu güvenli mimariyi uygular.
