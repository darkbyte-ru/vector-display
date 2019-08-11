# Vortex
Данный проект основан на коде [Neuronix](https://electrotransport.ru/ussr/index.php?topic=59044.0). В процессе сборки бортового компьютера обнаружилось, что дисплеи `ITEAD Nextion` бывают оригинальные и не очень (реплика от TJC). Оба они идентичны внешнему виду и аппаратному обеспечению. Отличие заключается в программном обеспечении, которое делает несовместимыми файлы прошивок дисплея между собой. В оригинальном проекте бортового компьютера использовалась реплика дисплея от фирмы TJC (TJC4832T035). В данном репозитории опубликованы файлы прошивки под оригинальный дисплей Nextion NX4832T035.

# Железо
Для сборки потребуется
- ESP32 (любая отладочная плата)
- NX4832T035 (не TJC4832T035!)
- Bluetooth HC-05
- RTC DS3231 (по желанию)
- индикатор MAX7219 (по желанию)

# Сборка железа
Bluetooth модуль подключается к любым IO пинам (по умолчанию 25 и 26). 
Дисплей подключается к порту UART2 (RX2 TX2). 
RTC подключается к шине I2С (SDA 21, SCL 22). 
Индикатор MAX7219 к любым цифровым пинам (задаётся на `settings.h`).

# Сборка прошивки ESP32
Прошивка написана с использованием фреймворка [platform.io](https://platformio.org/). Как вариант, можно использовать [VSCode](https://code.visualstudio.com/). VSCode -> File -> Preferences -> Extensions - набрать в поисковой строке "*platformio ide*" и установить.

Перед сборкой проекта необходимо создать файл конфигурации. Для примера в директории src имеется `settings.h.example`.

Первоначальную прошивку контроллера потребуется провести через COM порт. Для этого в конфигурационном файле проекта `platformio.ini` необходимо разкомментировать три соответствующие строки, указав COM порт, на котором определился ESP32, и закомментировать три строки относящиеся к обновлению по воздуху (OTA). Последующие обновления можно производить по воздуху, указав корректный IP адрес устройства в параметре `upload_port`.

Для того, чтобы иметь возможность обновлять прошивку дисплея через контроллер необходимо также загрузить SPIFFS. Для этого в консоли VSCode необходимо выполнить команду `pio run -t uploadfs`.

# Прошивка дисплея
Загрузить прошивку в дисплей можно несколькими способами: через uart, с помощью microsd флешки, через веб интерфейс ESP32. В случае с флешкой достаточно скопировать файл `1.tft` из директории display в корень карты (отформатированной в fat32), вставить её в дисплей и подать питание. После успешной прошивки карту необходимо извлечь. В случае, если дисплей уже подключен к ESP32, и в неё загружена прошивка, то можно с помощью браузера зайти по адресу http://ip-адрес-esp32/tft, и загрузить файл `1.tft`.

Если во время загрузки прошивки дисплей написал "*Model does not match*", значит скорее всего у вас клон дисплея от фирмы TJC. Также дисплей может не подружиться с флеш картой, в таком случае стоит попробовать взять другую.

# Корпус
При наличии 3D принтера корпус можно распечатать по модели [VectorMonitorTFT](https://www.tinkercad.com/things/fy87wSfSnDF). Перед печатью стоит обратить внимание на то, что в данной модели не предусмотрены никакие пазы для соединения двух половинок. Также стоит замерить размер конкретно вашей платы с дисплей, так как в моём случае плата оказалась на пару миллиметров длиннее.

# Настройка Bluetooth
Для того, чтобы модуль bluetooth при включении автоматически соединялся с контроллером необходимо его настроить. Для этого потребуется подключить его к любому UART порту (usb-uart адаптеру, arduino/esp32) и при нажатой кнопке **KEY** отправить команды:
```
AT+RMAAD
AT+ROLE=1
AT+CMODE=0
AT+CLASS=0
AT+INQM=1,9,48
AT+INQ
```
При этом контроллер мотор колеса должен быть включен, и находиться в радиусе действия Bluetooth.
В ответ на последнюю команду модуль выдаст список устройств поблизости в формате:
```
+INQ:address,type,signal
```
Например: *+INQ:14:2:110007,0,7FFF*

Если в радиусе действия окажется несколько устройств, то для каждого можно запросить имя командой: `AT+RNAME? <address>`
По умолчанию блутуз модуль контроллера мотор колеса имеет название `HC-05`
Когда нужный адрес будет найден, то нужно сохранить его командой:
```
AT+LINK=<address> (например, AT+LINK=14,2,110007)
AT+INIT
```
При этом в address нужно заменить символ ":" на ",". Если ответ модуля будет `OK`, то кнопку KEY можно отпускать.

# В случае проблем
Рекомендую обратиться к оригинальной инструкции на форуме [electrotransport](https://electrotransport.ru/ussr/index.php?topic=59044.0), либе в Issue.
