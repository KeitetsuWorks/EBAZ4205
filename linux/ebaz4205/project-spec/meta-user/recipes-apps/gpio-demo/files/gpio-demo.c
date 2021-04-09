/**
 * @file        gpio-demo.c
 * @brief       GPIO Demo Application for Zynq EBAZ4205 Board
 * @see         https://xilinx-wiki.atlassian.net/wiki/spaces/A/pages/18842398/Linux+GPIO+Driver#LinuxGPIODriver-DemoApplication
 */

#include <fcntl.h>
#include <linux/input.h>
#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

/**
 * @name    Green LED (LED6G)
 */
/** @{ */
/**
 * @brief   Path to set source of LED events
 */
#define LED_TRIGGER_PATH "/sys/class/leds/led-green/trigger"

/**
 * @brief   Path to set LED brightness
 */
#define LED_BRIGHTNESS_PATH "/sys/class/leds/led-green/brightness"
/** @} */

/**
 * @brief   Path to read input event
 */
#define KEY_EVENT_PATH "/dev/input/event0"

#define LED_SPEED_MAX 9        /**< Maximum LED blinking speed */
#define LED_SPEED_DEFAULT 5    /**< Default LED blinking speed */
#define LED_PERIOD_COEFF 16000 /**< LED blinking period coefficient */

static unsigned int led_speed;         /**< LED blinking speed */
static pthread_mutex_t led_speed_lock; /**< Mutex */

static int blink_led_stop;  /**< LED blinking thread stop flag */
static int blink_led_error; /**< LED blinking thread error flag */

static int stop; /**< Stop flag */

/**
 * @brief   Signal handler
 * @param[in]       signum          Signal
 */
static void signal_handler(int signum);

/**
 * @brief   Initialize the LED
 * @retval          0               Initialized the LED
 * @retval          -1              Failed to initialize the LED
 */
static int init_led(void);

/**
 * @brief   Turn off the LED
 * @retval          0               Turned off the LED
 * @retval          -1              Failed to turn off the LED
 */
static int off_led(void);

/**
 * @brief   LED blinking thread function
 * @param           arg             Dummy argument
 */
static void *blink_led(void *arg);

/**
 * @brief   Main function
 * @param[in]       argc            Argument count
 * @param[in]       argv            Argument vector
 * @return          Exit status
 */
int main(int argc, char *argv[])
{
    int error;
    struct sigaction sa_stop;
    int key_event_fd;
    struct input_event key_event;
    size_t key_event_size;
    int key_code;
    pthread_t blink_led_pth;

    /* Initialize the flags */
    stop = 0;
    error = 0;

    /* Initialize the signal handler */
    memset(&sa_stop, 0, sizeof(sa_stop));
    sa_stop.sa_handler = signal_handler;
    sa_stop.sa_flags = SA_RESTART;
    /* Catch kill signal */
    if (sigaction(SIGTERM, &sa_stop, NULL) < 0) {
        fprintf(stderr, "Error: Failed to set action for SIGTERM signal\n");
        exit(EXIT_FAILURE);
    }
    /* Catch hang up signal */
    if (sigaction(SIGHUP, &sa_stop, NULL) < 0) {
        fprintf(stderr, "Error: Failed to set action for SIGHUP signal\n");
        exit(EXIT_FAILURE);
    }
    /* Catch quit signal */
    if (sigaction(SIGQUIT, &sa_stop, NULL) < 0) {
        fprintf(stderr, "Error: Failed to set action for SIGQUIT signal\n");
        exit(EXIT_FAILURE);
    }
    /* Catch a Ctrl-C signal */
    if (sigaction(SIGINT, &sa_stop, NULL) < 0) {
        fprintf(stderr, "Error: Failed to set action for SIGINT signal\n");
        exit(EXIT_FAILURE);
    }

    /* Initialize the LED */
    if (init_led() != 0) {
        exit(EXIT_FAILURE);
    }
    fprintf(stdout, "Ready to use the LED\n");

    /* Create thread */
    led_speed = LED_SPEED_DEFAULT;
    pthread_mutex_init(&led_speed_lock, NULL);
    blink_led_stop = 0;
    blink_led_error = 0;
    if (pthread_create(&blink_led_pth, NULL, blink_led, NULL) != 0) {
        fprintf(stderr, "Error: Failed to create new thread\n");
        exit(EXIT_FAILURE);
    }

    /* Open KEY_EVENT_PATH */
    key_event_fd = open(KEY_EVENT_PATH, O_RDONLY);
    if (key_event_fd == -1) {
        fprintf(stderr, "Error: Failed to open " KEY_EVENT_PATH "\n");
        error = 1;
    }
    key_event_size = sizeof(key_event);

    /* Read and parse event, update global variable */
    while (!stop && !error && !blink_led_error) {
        if (read(key_event_fd, &key_event, key_event_size) < key_event_size) {
            fprintf(stderr, "Error: Failed to read from " KEY_EVENT_PATH "\n");
            error = 1;
            break;
        }

        if (key_event.type == EV_KEY && key_event.value == 1) { /* Down press only */
            key_code = key_event.code;
            if (key_code == KEY_HOME) { /* Slow down */
                /* Protect from concurrent read/write */
                if (pthread_mutex_lock(&led_speed_lock) != 0) {
                    fprintf(stderr, "Error: Failed to lock the mutex\n");
                    error = 1;
                    break;
                }
                if (led_speed > 0) {
                    led_speed -= 1;
                }
                if (pthread_mutex_unlock(&led_speed_lock) != 0) {
                    fprintf(stderr, "Error: Failed to unlock the mutex\n");
                    error = 1;
                    break;
                }
            }
            else if (key_code == KEY_POWER) { /* Speed up */
                /* Protect from concurrent read/write */
                if (pthread_mutex_lock(&led_speed_lock) != 0) {
                    fprintf(stderr, "Error: Failed to lock the mutex\n");
                    error = 1;
                    break;
                }
                if (led_speed < LED_SPEED_MAX) {
                    led_speed += 1;
                }
                if (pthread_mutex_unlock(&led_speed_lock) != 0) {
                    fprintf(stderr, "Error: Failed to unlock the mutex\n");
                    error = 1;
                    break;
                }
            }
            fprintf(stdout, "Blinking Speed: %d\n", led_speed);
            usleep(1000);
        }
    }

    /* Close KEY_EVENT_PATH */
    if (key_event_fd != -1) {
        if (close(key_event_fd) == -1) {
            fprintf(stderr, "Error: Failed to close " KEY_EVENT_PATH "\n");
            error = 1;
        }
    }

    /* Join thread */
    blink_led_stop = 1;
    if (pthread_join(blink_led_pth, NULL) != 0) {
        fprintf(stderr, "Error: Failed to wait for the thread termination\n");
        error = 1;
    }
    if (pthread_mutex_destroy(&led_speed_lock) != 0) {
        fprintf(stderr, "Error: Failed to destroy the mutex\n");
        error = 1;
    }

    /* Turn off the LED */
    if (off_led() != 0) {
        error = 1;
    }

    if (error != 0) {
        exit(EXIT_FAILURE);
    }

    exit(EXIT_SUCCESS);
}

static void signal_handler(int signum)
{
    switch (signum) {
        case SIGTERM:
        case SIGHUP:
        case SIGQUIT:
        case SIGINT:
            stop = 1;
            break;
        default:
            break;
    }

    return;
}

static int init_led(void)
{
    int led_trigger_fd;

    led_trigger_fd = open(LED_TRIGGER_PATH, O_WRONLY);
    if (led_trigger_fd == -1) {
        fprintf(stderr, "Error: Failed to open " LED_TRIGGER_PATH "\n");
        return -1;
    }
    if (write(led_trigger_fd, "default-on", 11) != 11) {
        fprintf(stderr, "Error: Failed to set source of LED events\n");
        if (close(led_trigger_fd) == -1) {
            fprintf(stderr, "Error: Failed to close " LED_TRIGGER_PATH "\n");
        }
        return -1;
    }
    if (close(led_trigger_fd) == -1) {
        fprintf(stderr, "Error: Failed to close " LED_TRIGGER_PATH "\n");
        return -1;
    }
    return 0;
}

static int off_led(void)
{
    int led_brightness_fd;

    led_brightness_fd = open(LED_BRIGHTNESS_PATH, O_WRONLY);
    if (led_brightness_fd == -1) {
        fprintf(stderr, "Error: Failed to open " LED_BRIGHTNESS_PATH "\n");
        return -1;
    }
    if (write(led_brightness_fd, "0", 2) != 2) {
        fprintf(stderr, "Error: Failed to set LED brightness\n");
        if (close(led_brightness_fd) == -1) {
            fprintf(stderr, "Error: Failed to close " LED_BRIGHTNESS_PATH "\n");
        }
        return -1;
    }
    if (close(led_brightness_fd) == -1) {
        fprintf(stderr, "Error: Failed to close " LED_BRIGHTNESS_PATH "\n");
        return -1;
    }
    return 0;
}

static void *blink_led(void *arg)
{
    unsigned int led_period;
    int led_brightness_fd;

    led_brightness_fd = open(LED_BRIGHTNESS_PATH, O_WRONLY);
    if (led_brightness_fd == -1) {
        fprintf(stderr, "Failed to open " LED_BRIGHTNESS_PATH "\n");
        blink_led_error = 1;
        return (void *)&blink_led_error;
    }
    while (!blink_led_stop) {
        if (pthread_mutex_lock(&led_speed_lock) != 0) {
            fprintf(stderr, "Error: Failed to lock the mutex\n");
            blink_led_error = 1;
            break;
        }
        led_period = ((LED_SPEED_MAX + 1) - led_speed) * LED_PERIOD_COEFF;
        if (pthread_mutex_unlock(&led_speed_lock) != 0) {
            fprintf(stderr, "Error: Failed to unlock the mutex\n");
            blink_led_error = 1;
            break;
        }

        if (write(led_brightness_fd, "255", 4) != 4) {
            fprintf(stderr, "Error: Failed to set LED brightness\n");
            blink_led_error = 1;
            break;
        }
        usleep(led_period);
        if (write(led_brightness_fd, "0", 2) != 2) {
            fprintf(stderr, "Error: Failed to set LED brightness\n");
            blink_led_error = 1;
            break;
        }
        usleep(led_period);
    }
    if (close(led_brightness_fd) == -1) {
        fprintf(stderr, "Error: Failed to close " LED_BRIGHTNESS_PATH "\n");
        blink_led_error = 1;
    }
    return (void *)&blink_led_error;
}
