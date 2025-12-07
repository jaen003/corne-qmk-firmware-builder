# QMK Keymap — Corne (crkbd) — Devorak (Spanish LATAM)

This repository contains my **custom keymap** for the **Corne (crkbd)** keyboard using **QMK Firmware**, with a **Devorak layout adapted for Spanish Latin America**. Features include productivity layers, layer locking, dual-computer switching, and a *Keep Awake* function (prevents system sleep).

## 📦 Requirements

- **Docker** (Docker Desktop for Windows/Mac or Docker Engine for Linux)

- **Visual Studio Code** with **Dev Containers extension** (`ms-vscode-remote.remote-containers`)  

- **Avrdude**

---

<a name="build"></a>
## 🔧 Build

1. Clone this project:

    ```bash 
    git clone https://github.com/jaen003/corne-qmk-firmware-builder
    ```

2. Open the folder in VS Code and choose Reopen in Container (Dev Containers extension).

3. Build the firmware:

    ```bash
    make compile
    ```
    > 💡 The generated firmware (`.hex`) will appear in the project root directory.

## 💾 Flashing

After completing the [build steps](#build), follow the instructions below to flash each half of the keyboard.

This guide assumes your Corne halves use an **ATmega32U4** microcontroller running the **Caterina / avr109 bootloader** (standard for Pro Micro-based CRKBD builds). Each half must be flashed independently start with the **master half** (usually the left).

1. Enter Bootloader Mode

    Choose one of the following methods:

    - **Using QK_BOOT:**  
    Press the key assigned to `QK_BOOT` (Layer 3 in this keymap).  
    The device will re-enumerate as a bootloader serial port.

    - **Using the physical RESET button:**  
    Press the reset/boot button on the PCB.

    Your OS should now expose a new serial device.

2. Identify the Bootloader Port

    **Linux**

    ```bash
    ls -l /dev/ttyACM* /dev/ttyUSB* 2>/dev/null
    ```

    **macOS**

    ```bash
    ls /dev/tty.usbmodem* /dev/tty.usbserial* 2>/dev/null
    ```

    **Windows**

    Check Device Manager → Ports (COM & LPT) and note the COM port (e.g. COM3).

3. Flash with avrdude

    **Linux / macOS**


    ```bash
    sleep 5 && avrdude -v -p atmega32u4 -c avr109 -P /dev/ttyACM0 -b 57600 -D -U flash:w:crkbd_rev1_custom.hex:i
    ```

    **Windows (PowerShell)**

    ```powershell
    Start-Sleep -Seconds 5; avrdude -v -p atmega32u4 -c avr109 -P COM3 -b 57600 -D -U flash:w:crkbd_rev1_custom.hex:i
    ```

    > ⚠️ Replace /dev/ttyACM0 or COM3 with the correct port detected in step 2.

4. Flash the Other Half:

    1. Disconnect the flashed half (or keep it connected if using separate cables).

    2. Connect the other half, put it in bootloader, detect port, and run the avrdude command again.

---

## 📜 License

This project is licensed under the GNU GPLv2.

