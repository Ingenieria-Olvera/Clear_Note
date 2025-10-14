                         USB-C Receptacle
                    (5 V VBUS, D+, D-, CC pins)
                              |
                              |  (VBUS 5 V)
                              V
                +-------------------------------+
                |  USB ESD / TVS protection(s)  |
                +-------------------------------+
                              |
                              |  (VBUS 5 V)
                              V
                        MCP73871 Charger
            (Power-path; CHG resistor sets charge current)
           BAT_OUT ----+-----------------------+--- SYSTEM_BATT_NODE
                       |                       |
               DW01 + FS8205A             Soft-Latch P-FET
        (Battery protection pair)       (P-channel MOSFET high-side)
            BAT Pack                        |  Gate driven by MCU_HOLD
   LP735977JH + PCM + 2 Wires               |
   (Battery pack, 4.8 Ah, nom. 3.7 V)       +-----> MCU (hold GPIO)
                       |                    |  (MCU asserts to stay on)
                       +--------------------+-----> SYSTEM_BATT_NODE
                              |
                              V
                    (SYSTEM_BATT_NODE ≈ 3.0–4.2 V)

                              |
                              |  (INPUT to power conversion)
                              V
                   TPS62840 Step-Down Regulator
                       (Buck → 3.3 V main rail)
                              |
                              +-------------------------------+
                              |                               |
                          3.3 V Rail                        3.3 V Rail
                          (MAIN LOGIC)                      (ANALOG/AUDIO)
                              |                               |
  +---------------------------+-------------------------------+------------------+
  |                           |               |               |                  |
  |                           |               |               |                  |
ESP32-S3 Module           MicroSD Socket   SGTL5000        E-Ink Module FPC     Keyboard Matrix
(ESP32-S3-WROOM-1)        (MSD-11-A)       (audio codec)    (Adafruit UC8253)     (Omron B3F switches)
  |  Pins:                 |  Mode: SDMMC    | I2S:           | SPI: MOSI          | Rows / Columns
  |  - Native USB D+/D- <--+--or---> SPI     | BCLK, LRCK,    | SCLK (clock)       | Diodes (1N4148) per key
  |  - Boot/EN test pads   |  (choose one)   | DIN (data)     | CS (chip select)   | -> MCU GPIOs
  |  - UART TX/RX -> POGO  | detect pin ->---+               | DC (data/command)  |
  |  - SPI pins ------------+               | I2C control -> +---> MCU SPI / GPIOs  |
  |  - I2C SDA, SCL ------------------------+ (SGTL5000 control) | RST & BUSY -> MCU  |
  |  - ADC input <- Volume slider (PTL...)  |                 |  (only ~6-8 MCU pins actually used)
  |  - GPIO <- Headphone detect <--- SJ1-3525NG-SW (switched jack)
  |  - Various GPIOs -> Keyboard matrix rows/columns
  |
  +--> USB D+ / D- differential pair (to ESP32 USB pins; route as 90 ohm diff pair)
  |
  +--> I2C bus  (SDA, SCL)
  |        |
  |        +--> SGTL5000 (I2C control for volume, power-down, etc.)
  |
  +--> I2S bus (BCLK, LRCK, DIN)
           |
           +--> SGTL5000 (I2S audio data in)
  |
  +--> SPI bus (MOSI, SCLK, CS)    [can be shared; use separate chip-selects]
           |
           +--> E-Ink (MOSI, SCLK, CS, DC, RST, BUSY)  [use SPI mode]
           +--> MicroSD (if using SPI mode)            [or use dedicated SDMMC pins]
  |
  +--> UART pins -> Pogo pads (TX, RX) for debug / external USB-UART
  |
  +--> Boot/EN Pads -> For entering bootloader / reset during debug

Other important connections:
- Charger STATUS pin(s) -> Red / Green LEDs (HSMF-C162, HSMF-C158) for charging / full indication
- SD card detect -> MCU GPIO (to detect insertion/removal)
- ESD diodes -> near USB-C, SD socket, and e-ink FPC connector
- Optional low-noise LDO -> powering analog supply pins of SGTL5000 (if required)

