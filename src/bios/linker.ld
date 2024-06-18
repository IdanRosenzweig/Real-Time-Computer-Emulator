ENTRY(start)

SECTIONS {
/* make sure to load the startup.asm code at the start of the file*/
    . = 0x0;
    .start : {
        main/startup.o
    }

}