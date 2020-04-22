#include "0_constants_and_libs.h"

void loop() {
  // если есть входящие байты с сервера, читаем их и выводим в мониторе порта
  while (client.available()) {
    char c = client.read();
    Serial.write(c);
  }

  // получаем данные с датчиков
  String dht11_data = get_data_from_DHT11();
  String mq2_data = get_data_from_MQ2();

  // выводим данные на дисплей
  display_data_from_sensors();

  // если с момента последнего соединения прошло 5 секунд,
  if (millis() - lastConnectionTime > postingInterval) {
    // отправляем на сервер данные с датчика DHT11
    httppost(dht11_data, "dht");
    
    if (gas_detected()) { // если датчик MQ2 обнаружил газы
      display_message(" Warning!!! Gas detected", false, false);
      httppost(mq2_data, "mq2"); // отправляем на сервер данные с датчика
    }
  }
}
