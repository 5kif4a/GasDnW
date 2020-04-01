#include "0_constants_and_libs.h"

void loop() {
  // если сервер отключен, останавливаем клиент
  if (!client.connected()) {
    Serial.println("Disconnecting from server...");
    client.stop();
    //    while (true); // ничего не делаем
  }

  // если есть входящие байты с сервера, читаем их и выводим в мониторе порта
  while (client.available()) {
    char c = client.read();
    Serial.write(c);
  }

  // получаем данные с сервера
  String dht11_data = get_data_from_DHT11();
  String mq2_data = get_data_from_MQ2();

  // выводим данные на дисплей  
  display_data_from_sensors();
  
  // отправляем на сервер данные с датчика DHT11
  httppost(dht11_data, "/dht");
  
  if (gas_detected()) { // если датчик MQ2 обнаружил газы
    display_message("Warning!!! Gas detected", true, false);
    httppost(mq2_data, "/mq2"); // отправляем на сервер данные с датчика
  }

  // делаем запрос каждые 2.5 секунды
  delay(2500);
}
