// Библиотеки
#include "WiFiEsp.h"
#include "DHT.h"
#include "MQ2.h"
#include <ArduinoJson.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// Константы
#define DHTPIN 6 // инициализиция объекта WiFi клиента
#define DHTTYPE DHT11 // Тип датчика DHT, в нашем случае DHT11
#define MQ2PIN A0 // инициализиция объекта WiFi клиента

// Настройки для дисплея OLED 0.96
#define SCREEN_WIDTH 128 // ширина дисплея OLED в пикселях
#define SCREEN_HEIGHT 64 // высота дисплея OLED в пикселях
#define OLED_RESET     4 // Пин перезагрузки/рестарта
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// RX TX пины для модуля ESP-01
#define RX 19
#define TX 20

#ifndef HAVE_HWSERIAL1
#include "SoftwareSerial.h"
SoftwareSerial Serial1(RX, TX);
#endif

WiFiEspClient client; // инициализиция объекта WiFi клиента

DHT dht(DHTPIN, DHTTYPE); //Инициация датчика влажности и температуры
MQ2 mq2(MQ2PIN); //Инициация датчика газа
int lpg, co, smoke, hudimity, temperature; // переменные для хранения значений с датчиков

char ssid[] = "khvnxali";            // SSID сети
char pass[] = "1234567q";        // пароль от сети
int status = WL_IDLE_STATUS;     // статус WiFi

char server[] = "192.168.43.223"; // Тип датчика DHT, в нашем случае DHT11
const int port = 8000; // Порт

const int capacity = JSON_OBJECT_SIZE(5); // Размер объекта JSON
