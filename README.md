# Zephyr Custom Driver for MCP41XXX

This project is a sample implementation of an "out-of-tree" driver for the **Zephyr RTOS**, focusing on integrating the **Microchip MCP41XXX** series (e.g., MCP41010, MCP41050) digital potentiometer with a Nordic microcontroller (nRF52832).

The primary goal is to demonstrate how to create a custom driver module that implements a custom, high-level API, communicates with the sensor via the SPI interface, and is fully devicetree-driven.

## 🎯 Key Features

* **Custom API:** Implements a clean, high-level API (`mcp41xxx.h`) to set resistance in Ohms (e.g., `mcp41xxx_set_resistance(dev, 5000)`) and a low-level one to set the raw tap value (`mcp41xxx_set_tap(dev, 128)`).
* **SPI Interface:** Utilizes the modern Zephyr SPIM API for communication.
* **Automatic CS Control:** Leverages the Zephyr `spi-dt-spec` and `cs-gpios` property, allowing the underlying SPIM driver to manage the Chip Select pin automatically.
* **Devicetree Driven:** Fully configured via Devicetree, including:
    * A custom binding (`microchip,mcp41xxx.yaml`).
    * An application overlay (`bruno_nrf52832.overlay`) that enables `spi1` and defines the device.
* **Sample Application:** The `main.c` file demonstrates how to initialize the potentiometer and continuously cycle through resistance steps (0%, 25%, 50%, 75%, 100%) based on the `resistance` property from the devicetree.

## 🛠️ Hardware Setup (Pinout)

This project is configured for an nRF52832 board. The expected hardware connection, as defined in `bruno_nrf52832.overlay`, is:

| nRF52832 Pin | Function | Connect to MCP41XXX Pin |
| :----------- | :---------- | :--------------------- |
| `P0.11` | `SPI1_SCK` | `SCK` |
| `P0.12` | `SPI1_MOSI` | `SDI` |
| `P0.13` | `SPI1_MISO` | `SDO` |
| `P0.10` | `CS` (GPIO) | `CS` |

*(Pin definitions are sourced from `pinctrl` and `cs-gpios` in the overlay file).*

## 🚀 How to Build and Use

This project is a Zephyr external module and should be placed in a suitable location (e.g., alongside your application folder).

1.  Clone this repository.
2.  Ensure your Zephyr environment (e.g., nRF Connect SDK) is set up.
3.  Ensure your app's `CMakeLists.txt` points to the module.
4.  Build and flash the project to your development board (e.g., `bruno_nrf52832` or `nrf52832_pca10040`):

    ```bash
    west build -b nrf52832_pca10040
    west flash
    ```

5.  Open a serial terminal (e.g., PuTTY, RealTerm) to view the log output with the resistance changes.

    ```
    *** Booting Zephyr OS build v3.5.99-ncs1 ***
    [00:00:00.375,555] <inf> MCP41XXX_Test: MCP41XXX Application started. Checking devices...
    [00:00:00.375,616] <inf> mcp41xxx: Device mcp41xxx_0 (10 kOhm) initialized.
    [00:00:00.375,769] <inf> MCP41XXX_Test: Setup complete. Starting resistance test...
    [00:00:00.375,769] <inf> MCP41XXX_Test: Potentiometer detected: 10 kOhms
    [00:00:00.375,800] <inf> MCP41XXX_Test: Setting resistance to: 0 Ohms
    [00:00:05.376,000] <inf> MCP41XXX_Test: Setting resistance to: 2500 Ohms
    [00:00:10.376,100] <inf> MCP41XXX_Test: Setting resistance to: 5000 Ohms
    [00:00:15.376,200] <inf> MCP41XXX_Test: Setting resistance to: 7500 Ohms
    [00:00:20.376,300] <inf> MCP41XXX_Test: Setting resistance to: 10000 Ohms
    [00:00:25.376,400] <inf> MCP41XXX_Test: Test cycle complete.
    [00:00:25.376,430] <inf> MCP41XXX_Test: Setting resistance to: 0 Ohms
    ```

## 📂 Project Structure

* `/app`: Contains the main application logic (`main.c`).
* `/custom_driver_module`: The custom driver module.
    * `drivers/pot/mcp41xxx.c`: The driver implementation.
    * `drivers/pot/mcp41xxx.h`: The public API header.
    * `drivers/pot/Kconfig.mcp41xxx`: The driver's Kconfig definitions.
    * `dts/bindings/microchip,mcp41xxx.yaml`: The devicetree binding.
    * `Kconfig` / `CMakeLists.txt`: Files that define the driver as a Zephyr module.
* `bruno_nrf52832.overlay`: The Devicetree overlay file for configuring board pins.
* `prj.conf`: The project's Kconfig configuration file.

## 📄 License

This project is licensed under the **Apache 2.0 License**.
