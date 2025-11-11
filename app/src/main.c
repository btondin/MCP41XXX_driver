/*
 * Copyright (c) 2025 Bruno Rodriguez Tondin <btondin@gmail.com>
 *
 * SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
 */

#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/logging/log.h>

/* 1. Include the custom API for our new driver */
#include "mcp41xxx/mcp41xxx.h"

/* 2. Register the log module */
LOG_MODULE_REGISTER(MCP41XXX_Test, LOG_LEVEL_DBG);

/* 3. Get the potentiometer device (from 'mcp41xxx_0:' in the overlay) */
static const struct device *pot_dev = DEVICE_DT_GET(DT_NODELABEL(mcp41xxx_0));

/* 4. Define the devicetree node (to read properties) */
#define POT_NODE DT_NODELABEL(mcp41xxx_0)

/* 5. Get the LED for feedback */
const struct gpio_dt_spec ledspec = GPIO_DT_SPEC_GET(DT_NODELABEL(led0), gpios);


int main(void)
{
	int err; /* Variable to store error codes */
	
	LOG_INF("MCP41XXX Application started. Checking devices...");

	/* 6. Check if the potentiometer device is ready */
	if (!device_is_ready(pot_dev)) {
		LOG_ERR("Potentiometer device %s is not ready", pot_dev->name);
		return 0;
	}

	/* 7. Check if the LED is ready */
	if (!gpio_is_ready_dt(&ledspec)) {
		LOG_ERR("Error: LED GPIO device is not ready");
		return 0;
	}

	gpio_pin_configure_dt(&ledspec, GPIO_OUTPUT_ACTIVE);
	LOG_INF("Setup complete. Starting resistance test...");

    /*
     * 8. Dynamically define the resistance steps
     * based on the 'resistance:' value (in kOhms) from the overlay.
     */
    
    /* 8.a: Read the total resistance (in kOhms) from the overlay */
    const uint32_t total_res_kohm = DT_PROP(POT_NODE, resistance);
    /* 8.b: Convert to Ohms */
    const uint32_t total_res_ohms = total_res_kohm * 1000;

    LOG_INF("Potentiometer detected: %u kOhms", total_res_kohm);

    /* 8.c: Define 5 proportional steps (0%, 25%, 50%, 75%, 100%) */
	uint32_t resistance_steps[] = {
        0,
        (total_res_ohms * 1) / 4, /* 25% */
        (total_res_ohms * 2) / 4, /* 50% */
        (total_res_ohms * 3) / 4, /* 75% */
        total_res_ohms            /* 100% */
    };
	int num_steps = ARRAY_SIZE(resistance_steps);
	int i = 0;

	/* Main application loop */
	while (1) {
		
		uint32_t ohms = resistance_steps[i];

		LOG_INF("Setting resistance to: %u Ohms", ohms);
		
		/* 9. Call the new high-level API */
		err = mcp41xxx_set_resistance(pot_dev, ohms);
		if (err) {
			LOG_ERR("Failed to set resistance, error: %d", err);
		}

		/* 10. Toggle the LED on each change (as requested) */
		gpio_pin_toggle_dt(&ledspec);

		/* Move to the next step */
		i++;
		if (i >= num_steps) {
			i = 0;
			LOG_INF("Test cycle complete.");
		}

		k_sleep(K_MSEC(5000)); /* Wait 5 seconds */
	}
	return 0;
}