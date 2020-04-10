void start_message_on_led () { // стартовое сообщение
  display.clearDisplay(); // очистить экран

  display.setTextSize(3); // масштаб пикселей 1:3
  display.setTextColor(SSD1306_WHITE); // белый текст
  display.setCursor(10, 20);  // начать выводить текст по середине экрана

  display.println(F("GasDnW")); // сообщение
  display.display(); // вывести на экран
  delay(1500); // задержка 1.5 секунды

  display.clearDisplay(); // очистить экран
  display.setTextSize(2); // масштаб пикселей 1:1
  display.setCursor(10, 0);  // начать выводить текст по середине экрана
  display.println(F("GAS LEAK")); // сообщение
  display.println(F("MONITORING SYSTEM")); // сообщение
  display.display(); // вывести на экран

  display.setTextSize(1); // для последующих сообщений размер текста 2
  display.setCursor(0, 0);  // начать выводить текст по с левого верхнего угла экрана
  display.clearDisplay(); // очистить экран
}

void display_message(String msg, bool newline, bool clr) { // вывод текста на дисплей
  if (clr) { // очищаем экран если true
    display.setCursor(0, 0);
    display.clearDisplay();
  }
  if (newline) { // выводим текст с новой строки если true
    display.println(msg);
  }
  else {
    display.print(msg);
  }
  display.display(); // вывести на экран
}

void printWifiStatus() // вывод статуса WiFi соединения в монитор порта и на дисплей
{
  delay(1000);
  // SSID сети, к которой вы подключены
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());
  display_message("SSID: ", false, true);
  display_message(WiFi.SSID(), true, false);

  // IP-адрес WiFi модуля
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);
  String ipString = String(ip[0]);
  for (byte octet = 1; octet < 4; ++octet) {
    ipString += '.' + String(ip[octet]);
  }
  display_message("IP Address: ", false, false);
  display_message(ipString, true, false);

  // полученный уровень сигнала
  long rssi = WiFi.RSSI();
  Serial.print("Signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
  display_message("Signal strength (RSSI):", false, false);
  display_message(String(rssi), false, false);
  display_message(" dBm", false, false);
}

String serialize(bool dht, bool mq2) { // сериализуем данные с датчиков в JSON
  StaticJsonDocument<capacity> doc;   // Объект JSON
  // записываем значения в объект JSON
  // фильтрация данных
  if (dht) {
    doc["Temperature"] = temperature;
    doc["Hudimity"] = hudimity;
  }

  if (mq2) {
    doc["LPG"] = lpg;
    doc["CO"] = co;
    doc["Smoke"] = smoke;
  }
  //   сериализуем объект JSON
  String json = doc.as<String>();
  return json;
}

String get_data_from_MQ2() {
  // получаем данные с датчика MQ2
  lpg = mq2.readLPG();  // Сжиженные углеводородные газы
  co = mq2.readCO(); // Угарный газ
  smoke = mq2.readSmoke();  // Дым
  String data = serialize(false, true); // сериализуем в строку
  return data; // возвращаем
}


String get_data_from_DHT11() {
  // получаем данные с датчика DHT11
  hudimity = dht.readHumidity(); // Влажность
  temperature = dht.readTemperature(); // Температура
  String data = serialize(true, false); // сериализуем в строку
  return data; // возвращаем
}

bool gas_detected() {
  // проверка на присутствие газа  
  if (lpg > 0 || co > 0 || smoke > 0) return true;
  return false;
}

void display_data_from_sensors() {
  // выводим данные с датчиком на дисплей
  display_message("Data from sensors: ", true, true);

  display_message("Temperature: ", false, false);
  display_message(String(temperature), false, false);
  display_message(" C", true, false);

  display_message("Hudimity: ", false, false);
  display_message(String(hudimity), false, false);
  display_message(" %", true, false);

  display_message("LPG: ", false, false);
  display_message(String(lpg), false, false);
  display_message(" ppm", true, false);

  display_message("CO: ", false, false);
  display_message(String(co), false, false);
  display_message(" ppm", true, false);

  display_message("Smoke: ", false, false);
  display_message(String(smoke), false, false);
  display_message(" ppm", true, false);
}

void httppost (String data, String endpoint) { // HTTP POST запрос на сервер
  client.stop();
  Serial.println("Start connection...");
  display_message("Start...", false, false);
  if (client.connect(server, port)) {
    Serial.println("Connected to server");
    display_message("Connected...", false, false);
    // Делаем POST запрос
    display_message("Sending.", false, false);
    client.println("POST /" + endpoint + " HTTP/1.1");
    client.println("Host: " + String(server) + ":" + String(port));
    client.println("Accept: */*");
    client.println("Content-Type: application/json");
    client.print("Content-Length: ");
    client.println(data.length());
    client.println();
    client.print(data);
    display_message("Sent!", false, false);
  }
  else {
    Serial.println("Connection failed!");
    display_message("Connection failed!", true, false);
  }
}
