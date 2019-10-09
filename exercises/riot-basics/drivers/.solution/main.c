#include <stdio.h>
#include <string.h>

#include "xtimer.h"
#include "thread.h"

#include "board.h"

#include "lpsxxx.h"
#include "lpsxxx_params.h"

#include "lsm303dlhc.h"
#include "lsm303dlhc_params.h"

static char stack_lpsxxx[THREAD_STACKSIZE_MAIN];
static char stack_lsm303dlhc[THREAD_STACKSIZE_MAIN];

static lpsxxx_t lpsxxx_dev;
static lsm303dlhc_t lsm303dlhc_dev;

static void *lpsxxx_handler(void *arg)
{
    (void) arg;

    while (1) {
        uint16_t pres = 0;
        int16_t temp = 0;
        lpsxxx_read_temp(&lpsxxx_dev, &temp);
        lpsxxx_read_pres(&lpsxxx_dev, &pres);
        printf("Pressure: %uhPa, Temperature:%u.%u°C\n",
               pres, (temp / 100), (temp % 100));
        xtimer_sleep(2);
    }

    return NULL;
}

static void *lsm303dlhc_handler(void *arg)
{
    (void) arg;

    while (1) {
        lsm303dlhc_3d_data_t mag_value;
        lsm303dlhc_3d_data_t acc_value;
        lsm303dlhc_read_acc(&lsm303dlhc_dev, &acc_value);
        printf("Accelerometer x: %i y: %i z: %i\n",
               acc_value.x_axis, acc_value.y_axis, acc_value.z_axis);
        lsm303dlhc_read_mag(&lsm303dlhc_dev, &mag_value);
        printf("Magnetometer x: %i y: %i z: %i\n",
               mag_value.x_axis, mag_value.y_axis, mag_value.z_axis);
        xtimer_usleep(500 * US_PER_MS);
    }

    return NULL;
}


int main(void)
{
    puts("RIOT multi driver application");

    lpsxxx_init(&lpsxxx_dev, &lpsxxx_params[0]);
    lsm303dlhc_init(&lsm303dlhc_dev, lsm303dlhc_params);

    thread_create(stack_lpsxxx, sizeof(stack_lpsxxx),
                  THREAD_PRIORITY_MAIN - 1,
                  0,
                  lpsxxx_handler,
                  NULL, "lpsxxx");
    
    thread_create(stack_lsm303dlhc, sizeof(stack_lsm303dlhc),
                  THREAD_PRIORITY_MAIN - 1,
                  0,
                  lsm303dlhc_handler,
                  NULL, "lsm303dlhc");

    return 0;
}
