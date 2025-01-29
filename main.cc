#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "pico/stdio.h"
#include "pico/binary_info.h"
#include "pico/time.h"
#include "hardware/gpio.h"
#include "hardware/pwm.h"
 
#define GPIO_PWM_NUM 17

int main() {
 
    bi_decl(bi_program_description("PWM demonstration"));
    bi_decl(bi_1pin_with_func(GPIO_PWM_NUM, GPIO_FUNC_PWM));
    bi_decl(bi_2pins_with_func(PICO_DEFAULT_I2C_SDA_PIN,
                               PICO_DEFAULT_I2C_SCL_PIN, GPIO_FUNC_I2C));
    stdio_init_all();
    sleep_ms(1000);

    // initialize PWM on the GPIO pin
    gpio_set_function(GPIO_PWM_NUM, GPIO_FUNC_PWM);
    uint slice_num = pwm_gpio_to_slice_num(GPIO_PWM_NUM);
    uint channel = pwm_gpio_to_channel(GPIO_PWM_NUM);

    // System clock is 125MHz -- divide down to use 1MHz for PWM generation
    pwm_set_clkdiv_int_frac(slice_num, 125, 0);

    // A servo frame is 50ms = (1 / (1MHz / 20000))
    pwm_set_wrap(slice_num, 20000);

    // Start with servo centered which is a high time of 1500us
    pwm_set_chan_level(slice_num, channel, 1500);

    // start PWM
    pwm_set_enabled(slice_num, true);

    // loop accepting a new PWM high time in us
    while (1) {
        printf("> ");
        char buffer[20];
        fgets(buffer, sizeof(buffer), stdin);
        buffer[sizeof(buffer) - 1] = '\0';

        // trim leading white space
        int i;
        for (i = 0; isspace(buffer[i]); i++) {}
        if (i != 0) {
            memmove(buffer, &buffer[i], strlen(buffer) - i + 1);
        }

        // trim trailing white space
        for (i = strlen(buffer) - 1; (i >= 0) && isspace(buffer[i]); i--) {}
        buffer[i + 1] = '\0';

        i = 0;

        // check for empty string
        if (buffer[0] == '\0') {
            continue;
        }

        // convert input string to number
        char *cp;
        unsigned long new_high_time = strtoul(buffer, &cp, 10);
        if (*cp != '\0') {
            printf("Not a number\n");
            continue;
        }

        // Number must be between 800 and 2200 us
        if (new_high_time < 800) {
            printf("Number must be >= 800\n");
            continue;
        }
        if (new_high_time > 2200) {
            printf("Number must be <= 2200\n");
            continue;
        }

        // Assign new high time
        puts("OK");
        pwm_set_chan_level(slice_num, channel, new_high_time);
    }
    return 0;
}
