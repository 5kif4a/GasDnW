#include "0_constants_and_libs.h"

void loop() {
  // если сервер отключен, останавливаем клиент
  if (!client.connected()) {
    Serial.println("Disconnecting from server...");
    client.stop();
//    while (true); // ничего не делаем
  }

  // если есть входящие байты с сервера, читаем их и печатаем
  while (client.available()) {
    char c = client.read();
    Serial.write(c);
  }

  String json = get_data_from_sensors(); // получаем данные с датчиком
  // делаем POST запрос
  httppost(json);

  // делаем запрос каждые 2.5 секунды
  delay(2500);
}
