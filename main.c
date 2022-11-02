#include <stdio.h>
#include "pico/stdlib.h"

#include "uart_tx.pio.h"
#include "uart_rx.pio.h"
#include "pico/multicore.h"

unsigned int SERIAL_BAUD = 115200;
unsigned int PIN_RX = 16;
unsigned int PIN_TX = 15;
PIO pio = pio0;

void core1_main() {
    
    unsigned int sm_rx = pio_claim_unused_sm(pio, true);
    unsigned int offset_rx = pio_add_program(pio, &uart_rx_program);
    uart_rx_program_init(pio, sm_rx, offset_rx, PIN_RX, SERIAL_BAUD);

    while (true) {
        char c = uart_rx_program_getc(pio, sm_rx);
        putchar(c);
    }
}

int main() {
    stdio_init_all();


    unsigned int sm_tx = pio_claim_unused_sm(pio, true);
    unsigned int offset_tx = pio_add_program(pio, &uart_tx_program);
    uart_tx_program_init(pio, sm_tx, offset_tx, PIN_TX, SERIAL_BAUD);


    multicore_launch_core1(core1_main);

    while (true) {
        uart_tx_program_puts(pio, sm_tx, "hi\n");
        sleep_ms(20);
    }
}
