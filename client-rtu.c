/*
 * Copyright © 2008-2014 Stéphane Raimbault <stephane.raimbault@gmail.com>
 *
 * rtu client 
 * read register and record in file 
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <modbus.h>

#include "unit-test.h"

#define _DEVELOPMENT_ 
const int EXCEPTION_RC = 2;

int equal_dword(uint16_t *tab_reg, const uint32_t value);

int equal_dword(uint16_t *tab_reg, const uint32_t value) {
    return ((tab_reg[0] == (value >> 16)) && (tab_reg[1] == (value & 0xFFFF)));
}

int main(int argc, char *argv[])
{
    uint16_t *tab_rp_registers = NULL;
    modbus_t *ctx_rtu  = NULL;
    int nb_points;
    int rc;
    int success = FALSE; 
    int i;

    uint16_t *product_id = NULL ;
    uint16_t *gross_wt = NULL ; 


//    ctx_rtu = modbus_new_rtu("/dev/ttyUSB1", 115200, 'N', 8, 1);
 
   for(;;){  
    ctx_rtu = modbus_new_tcp("192.168.1.4", 1502 ) ;

    fprintf(stderr," Connecting to /dev/tty/USB0 , 2400, N, 8, 1 ... \n" );
    if (ctx_rtu == NULL) {
        fprintf(stderr, "Unable to allocate libmodbus context\n");
        return -1;
    }
    modbus_set_debug(ctx_rtu, TRUE);
    modbus_set_error_recovery(ctx_rtu,
                              MODBUS_ERROR_RECOVERY_LINK |
                              MODBUS_ERROR_RECOVERY_PROTOCOL);

     modbus_set_slave(ctx_rtu, SERVER_ID);

    if (modbus_connect(ctx_rtu) == -1) {
        fprintf(stderr, "Connection failed: %s\n", modbus_strerror(errno));
        modbus_free(ctx_rtu);
        return -1;
    }
    fprintf(stderr," Connected ... \n" );

    printf("** GET data from RTU device     **\n");


    /* Allocate and initialize the memory to store the registers */
    nb_points =  20; 
    tab_rp_registers = (uint16_t *) malloc(nb_points * sizeof(uint16_t));
    memset(tab_rp_registers, 0, nb_points * sizeof(uint16_t));
    /* read registers */
    /* registers no 5,7,9, 15, 111 641 to be read */

    rc = modbus_read_registers(ctx_rtu, 0,10, tab_rp_registers);
    sleep (1);
//    ASSERT_TRUE(rc == -1 && errno == EMBMDATA, "");


close:
    /* Free the memory */
    free(tab_rp_registers);

    /* Close the connection */
    modbus_close(ctx_rtu);
    modbus_free(ctx_rtu);

   } 
    return (success) ? 0 : -1;
}

