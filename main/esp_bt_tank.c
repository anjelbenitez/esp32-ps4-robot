//
// Created by abenitez on 3/28/23.
//
#include "PS4Controller.h"
#include "ps4_controller_buttons.h"

#include "esp_log.h"
#include "esp_err.h"
#include "driver/gpio.h"
#include "driver/ledc.h"

#define TAG   __FUNCTION__

#define SET_LOW                     0
#define SET_HIGH                    1

/* Define Right Motor pins */
#define R_MOTOR_PIN_1               GPIO_NUM_16
#define R_MOTOR_PIN_2               GPIO_NUM_17
#define R_MOTOR_ENABLE_PIN          GPIO_NUM_22

/* Define Left Motor pins */
#define L_MOTOR_PIN_1               GPIO_NUM_18
#define L_MOTOR_PIN_2               GPIO_NUM_19
#define L_MOTOR_ENABLE_PIN          GPIO_NUM_23

#define LEDC_FREQUENCY              1000 //1 kHz

/* Define macros */
#define constrain(amt, low, high) ((amt) < (low) ? (low) : ((amt) > (high) ? (high) : (amt)))
#define get_signal_idx(x) (((x / 8) ? LEDC_LS_SIG_OUT0_IDX : LEDC_HS_SIG_OUT0_IDX) + (x % 8))

typedef struct {
    ledc_timer_t timer;
    ledc_mode_t mode;
    ledc_timer_bit_t duty_res;
    ledc_channel_t channel;
} motor_ledc_t;

static motor_ledc_t r_motor_pwm;
static motor_ledc_t l_motor_pwm;

void send_motor_rotate_cmd(int r_motor_speed, int l_motor_speed) {
    /**********************************
     * pin1: low, pin2: high = forward
     * pin1: low, pin2: high = reverse
     * pin1 == pin2 = stop
     *********************************/
    if (r_motor_speed < 0) {
        gpio_set_level(R_MOTOR_PIN_1, SET_LOW);
        gpio_set_level(R_MOTOR_PIN_2, SET_HIGH);
    }

    else if (r_motor_speed > 0) {
        gpio_set_level(R_MOTOR_PIN_1, SET_HIGH);
        gpio_set_level(R_MOTOR_PIN_2, SET_LOW);
    }

    else {
        gpio_set_level(R_MOTOR_PIN_1, SET_LOW);
        gpio_set_level(R_MOTOR_PIN_2, SET_LOW);
    }

    if (l_motor_speed < 0) {
        gpio_set_level(L_MOTOR_PIN_1, SET_LOW);
        gpio_set_level(L_MOTOR_PIN_2, SET_HIGH);
    }

    else if (l_motor_speed > 0) {
        gpio_set_level(L_MOTOR_PIN_1, SET_HIGH);
        gpio_set_level(L_MOTOR_PIN_2, SET_LOW);
    }

    else {
        gpio_set_level(L_MOTOR_PIN_1, SET_LOW);
        gpio_set_level(L_MOTOR_PIN_2, SET_LOW);
    }

    // set duty cycle and call update to activate changes
    ledc_set_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_4, abs(r_motor_speed));
    ledc_update_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_4);
    ledc_set_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_5, abs(l_motor_speed));
    ledc_update_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_5);
}

/* Scales one range to another for precise pwm control
 * Ref: https://cdn.arduino.cc/reference/en/language/functions/math/map/ */
long map(long x, long in_min, long in_max, long out_min, long out_max) {
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

void cmd_cb() {
    int r_motor_speed, l_motor_speed;

    /* Map joystick y-axis values to pwm output range for more precision
     * and then constrain values to within range */

    // get translated right motor speed, neg values = reverse
    r_motor_speed = map(RStickY(), -127, 127, -255, 255);
    r_motor_speed = constrain(r_motor_speed, -255, 255);

    // get translated left motor speed, neg values = reverse
    l_motor_speed = map(LStickY(), -127, 127, -255, 255);
    l_motor_speed = constrain(l_motor_speed, -255, 255);

    /* uncomment to test individual motor speed */
//    ESP_LOGI(TAG, "Right motor speed: %d", r_motor_speed);
//    ESP_LOGI(TAG, "Left motor speed: %d", l_motor_speed);

    // send command to rotate each motor
    send_motor_rotate_cmd(r_motor_speed, l_motor_speed);
}

void connect_cb() {
    ESP_LOGI(TAG, "Connection was successful");
}

void disconnect_cb() {
    // stop motors
    send_motor_rotate_cmd(0, 0);
    ESP_LOGI(TAG, "Successfully disconnected");
}

void apply_pwm_motor_control(motor_ledc_t motor_ledc) {
    // Set up and apply the LEDC PWM timer configuration
    ledc_timer_config_t ledc_timer = {
            .speed_mode       = motor_ledc.mode,
            .timer_num        = motor_ledc.timer,
            .duty_resolution  = motor_ledc.duty_res,
            .freq_hz          = LEDC_FREQUENCY,
            .clk_cfg          = LEDC_AUTO_CLK
    };
    ESP_ERROR_CHECK(ledc_timer_config(&ledc_timer));
}

void motor_pwm_init() {
    r_motor_pwm.timer = LEDC_TIMER_0;
    r_motor_pwm.mode = LEDC_HIGH_SPEED_MODE;
    r_motor_pwm.duty_res = LEDC_TIMER_8_BIT;
    r_motor_pwm.channel = LEDC_CHANNEL_4;

    l_motor_pwm.timer = LEDC_TIMER_1;
    l_motor_pwm.mode = LEDC_HIGH_SPEED_MODE;
    l_motor_pwm.duty_res = LEDC_TIMER_8_BIT;
    l_motor_pwm.channel = LEDC_CHANNEL_5;
}

void set_motor_pins_direction() {
    gpio_set_direction(R_MOTOR_ENABLE_PIN, GPIO_MODE_OUTPUT);
    gpio_set_direction(R_MOTOR_PIN_1, GPIO_MODE_OUTPUT);
    gpio_set_direction(R_MOTOR_PIN_2, GPIO_MODE_OUTPUT);

    gpio_set_direction(L_MOTOR_ENABLE_PIN, GPIO_MODE_OUTPUT);
    gpio_set_direction(L_MOTOR_PIN_1, GPIO_MODE_OUTPUT);
    gpio_set_direction(L_MOTOR_PIN_2, GPIO_MODE_OUTPUT);
}

void app_main() {
    ctx.ps4 = calloc(sizeof(ps4_controller_t), 1);
    assert(ctx.ps4 != NULL);

    // init left and right motor pwm settings
    motor_pwm_init();
    set_motor_pins_direction();

    // configure LEDC PWM timer for both motors
    apply_pwm_motor_control(r_motor_pwm);
    apply_pwm_motor_control(l_motor_pwm);

    // set signal output to left & right motor pwm pins
    uint32_t r_signal_idx = get_signal_idx(r_motor_pwm.channel);
    uint32_t l_signal_idx = get_signal_idx(l_motor_pwm.channel);
    gpio_matrix_out(R_MOTOR_ENABLE_PIN, r_signal_idx, false, false);
    gpio_matrix_out(L_MOTOR_ENABLE_PIN, l_signal_idx, false, false);

    // init motor speeds to 0 until new cmd received
    send_motor_rotate_cmd(0, 0);

    // set up callbacks
    attach(cmd_cb);
    attachOnConnect(connect_cb);
    attachOnDisconnect(disconnect_cb);

    // establish bt connection with ps4 controller
    begin(NULL);
    ESP_LOGI(TAG, "All systems ready");
}
