ENTRY(start)

/*OS_BOOT_ADDR = 0x7c00;*/
OS_ADDR = 0x10000;

SECTIONS {
/*    . = OS_BOOT_ADDR;*/
/*    .os_boot : {*/
/*        boot.o*/
/*    }*/

    . = OS_ADDR;
    .os : {
        startup.o
    }

}