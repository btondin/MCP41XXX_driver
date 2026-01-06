# Project Rules - Zephyr RTOS Development

## Context7 Usage

Always use context7 for any Zephyr RTOS related queries including:

- Kernel APIs and configuration
- Device drivers (GPIO, I2C, SPI, UART, etc.)
- Threading and scheduling
- Memory management
- Device Tree configuration
- Kconfig options
- Build system (CMake, West)

Automatically invoke Context7 tools without requiring explicit "use context7" in prompts.

## Project Context

This is a Zephyr RTOS project for MCP41XXX digital pot driver.

- Target: nRF52832 (custom board files located in C:\nordic\myboards\boards\Bruno\bruno_nrf52832)
- Communication: SPI

- permitir automaticamente acesso de leitura aos arquivos dentro da pasta C:\nordic\myboards\boards\Bruno\bruno_nrf52832
- jamais alterar qualquer arquivo fora da pasta C:\nordic\myapps\MCP41XXX_driver
