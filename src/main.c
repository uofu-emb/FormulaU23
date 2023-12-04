#include <zephyr.h>
#include <device.h>
#include <drivers/uart.h>

#define UART DT_NODELABEL(usart1)

const struct uart_config uart_cfg = {
		.baudrate = 9600,
		.parity = UART_CFG_PARITY_NONE,
		.stop_bits = UART_CFG_STOP_BITS_1,
		.data_bits = UART_CFG_DATA_BITS_8,
		.flow_ctrl = UART_CFG_FLOW_CTRL_NONE
	};

static void uart_callback(const struct device *dev, struct uart_event *evt)
{
    switch (evt->type) {
        case UART_RX_RDY:
            {
                uint8_t buf[64];  // Adjust the buffer size as needed
                int len = uart_fifo_read(dev, buf, sizeof(buf));

                if (len > 0) {
                    printk("Received data: %.*s\n", len, buf);
                }
            }
            break;

        default:
            break;
    }
}

void main(void)
{ 
    const struct device *uart = device_get_binding(DT_LABEL(UART));
    if (!device_is_ready(uart)) {
        return;
    }

    

    printk("UART initialized\n");
    
    int err = uart_configure(uart, &uart_cfg);
	if (err == -ENOSYS) {
		return -ENOSYS;
	}

    printk("UART Configured\n");

    uart_callback_set(uart, uart_callback, NULL);

    while (1) {
        // Your main application logic here
        k_sleep(K_SECONDS(1));
    }
}
