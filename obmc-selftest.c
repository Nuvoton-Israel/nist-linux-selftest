/*
 * obmc-selftest.c
 *
 * Copyright (C) 2019 NUVOTON
 *
 * KW Liu <kwliu@nuvoton.com>
 *
 *  This is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This software is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this software; If not, see <http://www.gnu.org/licenses/>
 */
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdint.h>
#include <linux/reboot.h>

#define INTCR2 0xf0800060
#define INTCR2_SELFTESTREQ 0x400
#define INTCR2_SELFTESTPASS 0x800
#define INTCR2_WDC 0x200000

#define SELFTESTREQ 10
#define SELFTESTPASS 11
#define WDC 21

int main(int argc,char** argv)
{
    int fd;
    void *map_base, *virt_addr;
    uint8_t rSelfTestRequest, wdc;
    off_t target = INTCR2;
    uint32_t page_size = getpagesize();
    uint32_t intcr2;

    fd = open("/dev/mem" ,(O_RDWR | O_SYNC));

    map_base = mmap(NULL,
			page_size * 2,
			(PROT_READ | PROT_WRITE),
			MAP_SHARED,
			fd,
			target & ~(off_t)(page_size - 1));

    if (map_base == MAP_FAILED) {
        printf("MAP_FAILED \n");
        return -1;
    }

    virt_addr = (char*)map_base + (target & (page_size - 1));
    intcr2 = *(volatile uint32_t*)virt_addr;

    rSelfTestRequest = (intcr2 & INTCR2_SELFTESTREQ) >> SELFTESTREQ;
    wdc = (intcr2 & INTCR2_WDC) >> WDC;

    printf("INTCR2 0x%x \n", intcr2);
    printf("rSelfTestRequest %d \n", rSelfTestRequest);
    printf("wdc %d \n", wdc);

    if (rSelfTestRequest) {
        printf("start self test.... \n");


        /* do self test */

        /* if failed */
        /* return reboot(LINUX_REBOOT_CMD_RESTART); */

        /* if self test pass */
        printf("self test pass!\n");

        /* clear rSelfTestRequest */
        intcr2 &= (~INTCR2_SELFTESTREQ);

        /* set WDC to 0 */
        intcr2 &= (~INTCR2_WDC);

        /* set rSelfTestPass */
        intcr2 |= INTCR2_SELFTESTPASS;

        *(volatile uint32_t*)virt_addr = intcr2;
        intcr2 = *(volatile uint32_t*)virt_addr;
        printf("updated INTCR2 0x%x \n", intcr2);

        printf("reboot system \n");
        return reboot(LINUX_REBOOT_CMD_RESTART);
    } else {
        printf("no need self test \n");

        /* set WDC to 0 */
        intcr2 &= (~INTCR2_WDC);

        *(volatile uint32_t*)virt_addr = intcr2;
        intcr2 = *(volatile uint32_t*)virt_addr;
        printf("updated INTCR2 0x%x \n", intcr2);
    }

    munmap(map_base, page_size * 2);
    close(fd);
    return(0);
}
