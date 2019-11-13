#include <stdio.h>
#include <string.h>

#include "xtimer.h"
#include "thread.h"

#include "lpsxxx.h"
#include "lpsxxx_params.h"

#include "lsm303dlhc.h"
#include "lsm303dlhc_params.h"

static char stack_lpsxxx[THREAD_STACKSIZE_MAIN];
static char stack_lsm303dlhc[THREAD_STACKSIZE_MAIN];

/* Declare device descriptors of drivers here */

static void *lpsxxx_handler(void *arg)
{
    (void) arg;

    /* Loop for reading and displaying temperature and pressure sensor values */

    return NULL;
}

static void *lsm303dlhc_handler(void *arg)
{
    (void) arg;

    /* Loop for reading and displaying accelerometer and magnetometer values */

    return NULL;
}


int main(void)
{
    puts("RIOT multi drivers application");

    /* Initialize drivers here  */

    thread_create(stack_lpsxxx, sizeof(stack_lpsxxx),
                  THREAD_PRIORITY_MAIN - 1,
                  0,
                  lpsxxx_handler,
                  NULL, "lps331ap");
    
    thread_create(stack_lsm303dlhc, sizeof(stack_lsm303dlhc),
                  THREAD_PRIORITY_MAIN - 1,
                  0,
                  lsm303dlhc_handler,
                  NULL, "lsm303ldhc");

    return 0;
}
