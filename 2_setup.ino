void setup() {
  // инициализируем Serial для отладки
  Serial.begin(115200);

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Адрес 0x3С для 128x64
    Serial.println(F("SSD1306 allocation failed"));
    while (true);
  }
  start_message_on_led(); // стартовое сообщение

  // инициализируем Serial для модуля ESP-01
  Serial1.begin(9600);
  // инициализируем модуль ESP-01
  WiFi.init(&Serial1);

  display_message("Initializing WiFi module", true, false);

  // проверка на наличие WiFi модуля
  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("WiFi shield not present");
    display_message("WiFi shield not present. Please check WiFi module", true, false);
    while (true); // не продолжать
  }

  // попытка подключиться к сети WiFi
  while ( status != WL_CONNECTED) {
    Serial.print("Attempting to connect to WPA SSID: ");
    Serial.println(ssid);
    display_message("Attempting to connect to WPA SSID: ", false, false);
    display_message(ssid, true, false);
    // Подключение к сети WPA / WPA2
    status = WiFi.begin(ssid, pass);
  }

  Serial.println("You're connected to the network");
  display_message("You're connected to the network", true, false);

  // вывести статус WiFi
  printWifiStatus();

  // запустить модули DHT11 и MQ2
  dht.begin();
  mq2.begin();
}
