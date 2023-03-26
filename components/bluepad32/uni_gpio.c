
/****************************************************************************
http://retro.moe/unijoysticle2

Copyright 2022 Ricardo Quesada

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
****************************************************************************/

#include "uni_gpio.h"

#include <argtable3/argtable3.h>
#include <driver/adc.h>
#include <driver/gpio.h>
#include <driver/ledc.h>
#include <esp_console.h>
#include <esp_log.h>

#include "sdkconfig.h"
#include "uni_common.h"
#include "uni_log.h"

static char buf_gpio_get[16];
static char buf_gpio_set[16];

static struct {
    struct arg_int* gpio_num;
    struct arg_end* end;
} gpio_get_args;

static struct {
    struct arg_int* gpio_num;
    struct arg_int* value;
    struct arg_end* end;
} gpio_set_args;

int uni_gpio_analog_write(gpio_num_t pin, uint8_t value) {
    ARG_UNUSED(pin);
    ARG_UNUSED(value);

    return 1;
}

uint16_t uni_gpio_analog_read(gpio_num_t pin) {
    ARG_UNUSED(pin);

    return 0;
}

static int cmd_gpio_get(int argc, char** argv) {
    int gpio_num, value;

    if (argc == 1) {
        for (int i = 0; i < GPIO_NUM_MAX; i++) {
            int value = gpio_get_level(i);
            logi("GPIO %d = %d\n", i, value);
        }
        return 0;
    }

    int nerrors = arg_parse(argc, argv, (void**)&gpio_get_args);
    if (nerrors != 0) {
        arg_print_errors(stderr, gpio_get_args.end, argv[0]);
        return 1;
    }

    gpio_num = gpio_get_args.gpio_num->ival[0];
    if (gpio_num < 0 || gpio_num >= GPIO_NUM_MAX)
        return 1;

    value = gpio_get_level(gpio_num);
    logi("GPIO %d = %d\n", gpio_num, value);
    return 0;
}

static int cmd_gpio_set(int argc, char** argv) {
    int gpio_num, value;

    int nerrors = arg_parse(argc, argv, (void**)&gpio_set_args);
    if (nerrors != 0) {
        arg_print_errors(stderr, gpio_set_args.end, argv[0]);
        return 1;
    }

    gpio_num = gpio_set_args.gpio_num->ival[0];
    value = gpio_set_args.value->ival[0];

    if (gpio_num < 0 || gpio_num >= GPIO_NUM_MAX)
        return 1;

    if (gpio_set_level(gpio_num, value) != ESP_OK)
        return 1;
    return 0;
}

void uni_gpio_register_cmds(void) {
    snprintf(buf_gpio_get, sizeof(buf_gpio_get) - 1, "<0 - %d, none>", GPIO_NUM_MAX - 1);
    snprintf(buf_gpio_set, sizeof(buf_gpio_set) - 1, "<0 - %d>", GPIO_NUM_MAX - 1);

    gpio_get_args.gpio_num = arg_int1(NULL, NULL, buf_gpio_get, "GPIO number, or none for all GPIOs");
    gpio_get_args.end = arg_end(2);

    gpio_set_args.gpio_num = arg_int1(NULL, NULL, buf_gpio_set, "GPIO number");
    gpio_set_args.value = arg_int1(NULL, NULL, "<0 | 1>", "GPIO value: 0=Low, 1=High");
    gpio_set_args.end = arg_end(3);

    const esp_console_cmd_t gpio_get = {
        .command = "gpio_get",
        .help = "Gets the level for a given GPIO",
        .hint = NULL,
        .func = &cmd_gpio_get,
        .argtable = &gpio_get_args,
    };

    const esp_console_cmd_t gpio_set = {
        .command = "gpio_set",
        .help = "Sets the level for a given GPIO",
        .hint = NULL,
        .func = &cmd_gpio_set,
        .argtable = &gpio_set_args,
    };

    ESP_ERROR_CHECK(esp_console_cmd_register(&gpio_get));
    ESP_ERROR_CHECK(esp_console_cmd_register(&gpio_set));
}
