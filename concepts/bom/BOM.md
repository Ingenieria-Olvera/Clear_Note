# 🧠 ClearNote — Bill of Materials (v2.0)

A refined, manufacturable BOM combining the original ClearNote design with updated power, audio, and protection circuitry.

| **Part # / Model** | **Component Name** | **Description / Function** | **Connection / Notes** | **URL / Datasheet** | **Qty** | **Unit Cost (USD)** | **Total** |
|--------------------|--------------------|-----------------------------|------------------------|---------------------|----------|----------------------|-----------|
| **ESP32-S3-WROOM-1-N16R8** | MCU Module | Wi-Fi/BLE + native USB microcontroller | Main system controller | [ESP32-S3-WROOM Datasheet](https://www.espressif.com/en/products/modules/esp32-s3-wroom-1) | 1 | 6.56 | 6.56 |
| **Adafruit 3.7" UC8253 E-Ink** | Tri-Color E-Ink Display | 416×240 e-ink panel (red/white/black) | SPI (UC8253 driver) | [Adafruit Product Page](https://www.adafruit.com/product/6394) | 1 | 24.50 | 24.50 |
| **528062410 (Molex)** | 24-Pin FPC Connector | E-Ink display interface | ESP32-S3 SPI → FPC | [Molex Datasheet](https://www.molex.com/molex/products/part-detail/528062410) | 1 | 0.89 | 0.89 |
| **SGTL5000XNLA3** | Audio Codec | Stereo DAC + headphone amp, I²C control | I²S bus + I²C from MCU | [NXP Datasheet](https://www.nxp.com/docs/en/data-sheet/SGTL5000.pdf) | 1 | 4.78 | 4.78 |
| **SJ1-3525NG-SW** | 3.5 mm Headphone Jack (Switched) | Headphone output with detect pin | From SGTL5000 line-out | [CUI Devices SJ1-3525NG-SW](https://www.cuidevices.com/product/audio-connectors/headphone-jacks/sj1-352xng-series) | 1 | 1.20 | 1.20 |
| **B3F-1000 (Omron)** | Tactile Switch | Key switches for compact keyboard | GPIO matrix input | [Omron B3F-1000 Datasheet](https://omronfs.omron.com/en_US/ecb/products/pdf/en-b3f.pdf) | 30 | 0.24 | 7.20 |
| **1N4148 SMD** | Keyboard Diode | Prevents ghosting in key matrix | One per key (SOD-323) | [1N4148WS Diode](https://www.digikey.com/en/products/detail/onsemi/1N4148WS/458603) | 30 | 0.05 | 1.50 |
| **MCP73871-2AAI/ML** | Li-Ion Charger / Power-Path IC | Handles USB charging + system supply | USB-C VBUS → Battery + 3.3 V rail | [MCP73871 Datasheet](https://www.microchip.com/en-us/product/MCP73871) | 1 | 2.41 | 2.41 |
| **DW01 + FS8205A** | Li-Ion Protection Combo | Over-charge/discharge/short protection | Inline with battery | [DW01/FS8205A Application Note](https://datasheet.lcsc.com/lcsc/1806122323_ABLIC-DW01A-G_C130747.pdf) | 1 set | 0.50 | 0.50 |
| **SI2301-TP** | P-Channel MOSFET | Soft-latch power switch FET | High-side soft-latch circuit | [SI2301 Datasheet](https://www.diodes.com/assets/Datasheets/ds31086.pdf) | 1 | 0.37 | 0.37 |
| **Momentary Pushbutton (SMD)** | Power / Function Button | Turns system on via soft-latch | GPIO hold + latch | — | 1 | 0.15 | 0.15 |
| **PTL20-10G0-103B2 (Bourns)** | Slide Potentiometer (10 kΩ)** | Volume control slider | ADC → MCU → SGTL5000 digital gain | [Bourns PTL20-10G0-103B2](https://www.bourns.com/products/potentiometers/slide-potentiometers/ptl20-series) | 1 | 2.46 | 2.46 |
| **AP2112K-3.3TRG1** | 3.3 V LDO Regulator | Converts battery → 3.3 V logic rail | Powers ESP32-S3, display, codec | [Diodes Inc. AP2112K-3.3](https://www.diodes.com/assets/Datasheets/AP2112.pdf) | 1 | 0.22 | 0.22 |
| **MSD-11-A** | Micro SD Card Socket | Storage for notes and audio | SPI/SDMMC bus | [MSD-11-A Datasheet](https://www.molex.com/molex/products/part-detail/5025700893) | 1 | 0.36 | 0.36 |
| **USB-C Receptacle (105444-0001)** | USB-C Connector | Power and data | USB-C to MCU + charger | [Molex USB-C 105444-0001](https://www.molex.com/molex/products/part-detail/usb_type-c/1054440001) | 1 | 2.66 | 2.66 |
| **USBLC6-2SC6** | ESD Protection Array | Protects USB D+ / D- lines | Place near USB-C port | [STMicroelectronics USBLC6-2SC6](https://www.st.com/resource/en/datasheet/usblc6-2.pdf) | 1 | 0.40 | 0.40 |
| **TVS Diode (SMBJ5.0A)** | USB VBUS Surge Protection | Protects against power surges | Inline with VBUS | [SMBJ5.0A Datasheet](https://www.littelfuse.com/products/tvs-diodes/smbj/smbj5-0a.aspx) | 1 | 0.30 | 0.30 |
| **HSMF-C162** | Red LED | Charging / Low-battery indicator | GPIO or charger STAT pin | [Broadcom HSMF-C162](https://docs.broadcom.com/doc/AV02-4186EN) | 1 | 1.01 | 1.01 |
| **HSMF-C158** | Green LED | Power-on / Full-charge indicator | GPIO or charger STAT pin | [Broadcom HSMF-C158](https://docs.broadcom.com/doc/AV02-4186EN) | 1 | 1.08 | 1.08 |
| **Pogo Pads (4-Pin Set)** | Programming / Debug Pads | UART TX/RX/3.3V/GND | Flash & serial logging | [Generic Example](https://www.adafruit.com/product/5357) | 1 | 0.50 | 0.50 |
| **Boot/EN Test Pads** | Boot Mode Access | For flashing and bring-up | EN + IO0 + UART pads | — | 1 set | 0.10 | 0.10 |
| **GRM033C71C104KE14D** | 0.1 µF Ceramic Capacitor | Decoupling caps | Near all IC power pins | [Murata GRM033C71C104KE14D](https://www.murata.com/en-us/products/productdata/8790467706910) | 5 | 0.08 | 0.40 |
| **PFR05S-221-FNH** | Resistor Pack (220 Ω) | General pull-ups / limit resistors | Keyboard, logic, LEDs | [Yageo PFR05S-221-FNH](https://www.yageo.com/en/Product/Resistor/Array/PFR/PFR05S221FNH) | 5 | 0.10 | 0.50 |

**_Estimated total: ≈ $61 – $65 USD (prototype, 1 unit)_**

---

### 💡 Notes
- **Soft-Power Circuit:** Uses SI2301 P-FET with a momentary pushbutton; MCU holds gate high to maintain power until shutdown.  
- **Charging:** MCP73871 provides USB charging *and* power-path (you can run and charge simultaneously).  
- **Protection:** DW01 + FS8205A protects Li-Ion cell; ESD diodes protect USB, SD, and FPC connectors.  
- **Volume:** Bourns 10 kΩ slide pot → ESP32 ADC → SGTL5000 digital gain.  
- **Keyboard:** Omron B3F + 1N4148 diodes prevent ghosting.  
- **Debug:** Pogo pads + Boot/EN test pads simplify flashing and logging.

---

### 📎 Resources
- [ESP32-S3 Reference Design Guide (Espressif)](https://www.espressif.com/en/support/documents/technical-documents)
- [NXP SGTL5000 App Note](https://www.nxp.com/docs/en/application-note/AN3660.pdf)
- [Adafruit UC8253 Library (GitHub)](https://github.com/adafruit/Adafruit_EPD)
- [Microchip MCP73871 Reference Design](https://ww1.microchip.com/downloads/en/AppNotes/01349b.pdf)
- [ST USBLC6 ESD Protection Guide](https://www.st.com/resource/en/application_note/cd00002307.pdf)

---
