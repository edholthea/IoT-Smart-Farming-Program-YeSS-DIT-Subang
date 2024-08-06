#define BLYNK_TEMPLATE_ID "TMPL6obZKUsoo"
#define BLYNK_TEMPLATE_NAME "IoT Smart Farming YeSS DIT Subang"
#define BLYNK_AUTH_TOKEN "kq294Dnq75nmQCOSsCC0mOYODoAR49O5"

#include "DHT.h"  //memasukkan library DHT11
#include <LiquidCrystal_I2C.h>  // memasukkan library lcd i2c

#include <ESP8266WiFi.h>                    // memasukkan library wifi ESP8266
#include <WiFiClient.h>                     // memasukkan library wifi client
#define BLYNK_PRINT Serial
#include <BlynkSimpleEsp8266.h> 


#define DHTPIN 2     // PIN 2 ke sensor DHT11
#define DHTTYPE DHT11   // tipe sensor yang digunakan DHT 11
# define BTN_PIN 13 // tombol
bool stat = false;

//Setting baris dan kolom untuk lcd i2c, 0x27 adalah address atau alamat lcd i2c
int lcdColumns = 16;
int lcdRows = 2;
LiquidCrystal_I2C lcd(0x27, lcdColumns, lcdRows);

DHT dht(DHTPIN, DHTTYPE); // DHT11

const int dry = 686; // nilai untuk soil sensor ketika kering, digunakan untuk mapping konversi ke persentase
const int wet = 321; // nilai untuk soil sensor ketika basah digunakan untuk mapping konversi ke persentase

int relay1  = 14;   // Pin 14 terhubung ke relay1
int relay2  = 12;   // Pin 14 terhubung ke relay
int statuspompa1;
int statuspompa2;
int sensoranalog = A0;


char auth[] = BLYNK_AUTH_TOKEN;
char ssid[] = "Edi Nurhadi";  // nama SSID WiFi
char pass[] = "edinur12";  // Password WiFi


void setup() {            // Fungsi setup dijalankan pertama kali saat perangkat dinyalakan
  Serial.begin(9600);     // Serial monitor pada baudrate 9600
  pinMode (BTN_PIN,INPUT_PULLUP);
  Blynk.begin(auth, ssid, pass, "blynk.cloud",8080);
  dht.begin();            // Memulai sensor DHT11
  lcd.init();             // inisiasi atau memulai lcd i2c
  lcd.backlight();        // menyalakan backlight lcd i2c
  pinMode(relay1,OUTPUT);  // Relay sebagai OUTPUT
  pinMode(relay2,OUTPUT);  // Relay sebagai OUTPUT

}

void loop() {           // Fungsi looping atau berulang, fungsi ini dijalankan berulangkali
Blynk.run();
//tombol();
    // put your main code here, to run repeatedly:
  if (WiFi.status() != WL_CONNECTED) { // Cek koneksi Wi-Fi
    Serial.println("Reconnecting to Wi-Fi...");
    lcd.setCursor(0,0);
    lcd.print("Koneksi Ke WiFi...");
    lcd.setCursor(0,1);
    lcd.print("kebundata.com");
    WiFi.disconnect(); // Putuskan koneksi Wi-Fi sebelumnya
    delay(1000);
    WiFi.begin(ssid, pass); // Mulai koneksi Wi-Fi
    while (WiFi.status() != WL_CONNECTED) { // Tunggu koneksi Wi-Fi tersambung
      delay(500);
      Serial.print(".");
    }
    Serial.println("Wi-Fi Connected!");
    lcd.setCursor(0,0);
    lcd.print("Tersambung Ke WiFi...");
    delay (2000);
    lcd.clear();
    Blynk.config(auth); // Konfigurasi ulang koneksi Blynk
  }

  float h = dht.readHumidity()- 19;     // membaca nilai kelembaban udara 
  float t = dht.readTemperature()- 1.2;  // membaca nilai suhu, dalam celcius

  // jika ada kegagalan pembacaan sensor DHT maka akan keluar dan membaca sensor lagi
  if (isnan(h) || isnan(t)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }

  int nilai = analogRead(sensoranalog);                // Pembacaan nilai soil sensor pada A0 (Analog 0)
  int kair = map(nilai, wet, dry, 100, 0);  // perintah merubah nilai soil sensor ke dalam persentase

  if(statuspompa1 == 1)
  {
   digitalWrite(relay1,LOW); 
  }
    if(statuspompa1 == 0)
  {
   digitalWrite(relay1,HIGH); 
  }

    if(statuspompa2 == 1)
  {
   digitalWrite(relay2,LOW); 
  }
    if(statuspompa2 == 0)
  {
   digitalWrite(relay2,HIGH); 
  }
 
  /*if (digitalRead (BTN_PIN) == 0) {
    stat =!stat;
    delay (200);
    digitalWrite (relay1, stat);
    Serial.println(digitalRead(BTN_PIN));
    Serial.println("Relay = " && digitalRead(Relay));
  lcd.clear();
  lcd.setCursor(0,0);                         // setting posisi huruf di lcd ke kolom 0, baris 0
  lcd.print("POMPA      : ");                  // mencetak di lcd suhu
  lcd.setCursor(12,0);                        // setting posisi huruf di lcd ke kolom 12, baris 0 
  lcd.print(statuspompa1);            
 } */

  
  Serial.print(F("Humidity: "));              // mencetak atau menampilkan teks "Humidity" ke serial monitor
  Serial.print(h);                            // mencetak atau menampilkan nilai humidity dari sensor DHT11 ke serial monitor
  Serial.print(F("%  Temperature: "));        // mencetak atau menampilkan teks "% Temperature" ke serial monitor
  Serial.print(t);                          // mencetak atau menampilkan nilai Temperature dari sensor DHT11 ke serial monitor, +1 adalah kalibrasi ketika dibandingkan dengan termometer, perbedaannya 1° C
  Serial.println(F("°C "));                   // mencetak atau menampilkan teks "C" ke serial monitor
  Serial.print(F("Hujan: "));      // mencetak atau menampilkan teks "Kelembaban Tanah" ke serial monitor
  Serial.print(nilai);                         // mencetak atau menampilkan nilai hasil konversi mapping (persentase) ke serial monitor
  Serial.println ("%");                       // mencetak atau menampilkan teks "%" ke serial monitor
  delay (1000);                               // Memberi jeda 1 detik
  lcd.clear();
  lcd.setCursor(0,0);                         // setting posisi huruf di lcd ke kolom 0, baris 0
  lcd.print("Kel. Tanah :");                   // mencetak di lcd Kadar air
  lcd.setCursor(12,0);                        // setting posisi huruf di lcd ke kolom 12, baris 0
  lcd.print(kair);                            // menampilkan hasil pembacaan dari hasil konversi mapping ke lcd
  lcd.setCursor(15,0);                        // setting posisi huruf di lcd ke kolom 15, baris 0
  lcd.print("%");                             // mencetak di lcd %
  lcd.setCursor(0,1);                         // setting posisi huruf di lcd ke kolom 0, baris 0
  lcd.print("kebundata.com");                   // mencetak di lcd Kadar air
  delay(2000);
  lcd.clear();
  
  lcd.setCursor(0,0);                         // setting posisi huruf di lcd ke kolom 0, baris 0
  lcd.print("Suhu      : ");                  // mencetak di lcd suhu
  lcd.setCursor(12,0);                        // setting posisi huruf di lcd ke kolom 12, baris 0 
  lcd.print(t);                               // menampilkan hasil pembacaan temperature dht11
  lcd.setCursor(18,0);                        // setting posisi huruf di lcd ke kolom 18, baris 0 
  lcd.print("°C");                            // mencetak di lcd °C
  lcd.setCursor(0,1);                         // setting posisi huruf di lcd ke kolom 0, baris 1
  lcd.print("Kelembaban: ");                  // mencetak di lcd kelembaban
  lcd.setCursor(12,1);                        // setting posisi huruf di lcd ke kolom 12, baris 1
  lcd.print(h);                               // menampilkan hasil pembacaan humidity dht11
  lcd.setCursor(19,1);                        // setting posisi huruf di lcd ke kolom 19, baris 1
  lcd.println("%");                           // mencetak di lcd %
  delay(2000);                                // memberi jeda 2 detik,

  //kirim data ke blynk
  Blynk.virtualWrite(V0, h); // Mengirimkan data suhu ke pin virtual V0
  Blynk.virtualWrite(V1, t); // Mengirimkan data suhu ke pin virtual V1
  Blynk.virtualWrite(V2, kair); // Mengirimkan data suhu ke pin virtual V2
  Serial.println("Status Pompa :" + String (statuspompa1));

  
}

 BLYNK_WRITE (V3)
  {
     statuspompa1 = param.asInt();
  }

   BLYNK_WRITE (V4)
  {
     statuspompa2 = param.asInt();
  }

  /*void tombol() {
  if (digitalRead (BTN_PIN) == 0) {
    stat =!stat;
    delay (200);
    digitalWrite (statuspompa1, stat);
    Serial.println(digitalRead(BTN_PIN));
    //Serial.println("Relay = " && digitalRead(Relay));
  lcd.clear();
  lcd.setCursor(0,0);                         // setting posisi huruf di lcd ke kolom 0, baris 0
  lcd.print("POMPA      : ");                  // mencetak di lcd suhu
  lcd.setCursor(12,0);                        // setting posisi huruf di lcd ke kolom 12, baris 0 
  lcd.print(statuspompa1);            
  } 

  
}*/
