# Процесс загрузки прошивки

## Загрузка прошивки в плату Arduino

Для того чтобы загрузить прошивку в плату газоанализатора под управлением Arduino(Nano или другие версии) необходимо скомпилировать проект в среде [ARDUINO IDE](https://www.arduino.cc/en/software) ([подробная инструкция](https://arduinomaster.ru/platy-arduino/proshivka-zagruzchik-arduino-ide/)).

При изменении схемы подключения пинов датчика MICS-6814 необходимо изменить значения в программе:

```cpp
#define PIN_CO  A2
#define PIN_NO2 A3
#define PIN_NH3 A4
```

## Загрузка прошивки в плату STM32F103C8Tx

### Первый способ

Для загрузки файла прошивки формата .hex можно воспользоваться программатором отладчиком [St link V2](https://www.st.com/en/development-tools/st-link-v2.html) или его бюджетной копией.
Загрузка прошивки в плату в таком случае осуществляется при помощи программ STM32Utility или [STMCubeProgrammer](https://wiki.st.com/stm32mpu/wiki/STM32CubeProgrammer).

### Второй способ

Для прошивки вторым способом, необходимо подключить к UART1 микроконтроллера(пины PA9-TX, PA10-RX) любой ttl конвертер (например, [такой](https://www.chipdip.ru/catalog/popular/usb-uart))
и воспользоваться STMCubeProgrammer или [Flasher-STM32](https://www.st.com/en/development-tools/flasher-stm32.html), подключив перед этим пин boot0 микроконтроллера к напряжению 3.3 вольт.

Подробные руководства можно найти [здесь](https://blog.kvv213.com/2020/01/kak-proshit-chip-stm32/).
