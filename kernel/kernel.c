#include <kernel/log.h>

#include <hal/terminal.h>
#include <mm/pmm.h>
#include <mm/vmm.h>
#include <stdio.h>

extern int ktgtinit(); // must be defined somewhere in the target specific code

void kinit() {
#ifdef KINIT_MM_FIRST // initialize MM first
    pmm_init();
    vmm_init();
#endif
    
    term_init();
    stdio_init();

    kinfo("SysX version 0.0.1 prealpha (compiled %s %s)", __DATE__, __TIME__);
    kinfo("Copyright <C> 2023 Thanh Vinh Nguyen (itsmevjnk)");

#ifndef KINIT_MM_FIRST
    kinfo("initializing physical memory management");
    pmm_init();
    kinfo("initializing virtual memory management");
    vmm_init();
#endif

    kinfo("invoking target-specific system initialization routine");
    if(ktgtinit()) {
        kinfo("ktgtinit() failed, committing suicide");
        return; // this should send us into an infinite loop prepared by the bootstrap code
    }

#ifndef TERM_NO_XY
    /* display terminal dimensions */
    size_t term_width, term_height;
    term_get_dimensions(&term_width, &term_height);
    kinfo("terminal size: %u x %u", term_width, term_height);
#endif
    
    while(1);
}
