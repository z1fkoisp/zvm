/*
 * Copyright 2021-2022 HNU-ESNL
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr.h>
#include <kernel.h>
#include <sys/printk.h>
#include <sys/mem_manage.h>
#include <shell/shell.h>
#include <shell/shell_uart.h>
#include <drivers/uart.h>
#include <kernel/thread.h>
#include <timing/timing.h>
#include <sys/time_units.h>

static void zvm_print_log(void)
{
	printk("\n");
	printk("\n");
	printk("█████████╗    ██╗     ██╗    ███╗    ███╗  \n");
	printk("╚════███╔╝    ██║     ██║    ████╗  ████║  \n");
	printk("   ███╔╝      ╚██╗   ██╔╝    ██╔ ████╔██║  \n");
	printk("  ██╔╝         ╚██  ██╔╝     ██║ ╚██╔╝██║  \n");
	printk("█████████╗      ╚████╔╝      ██║  ╚═╝ ██║  \n");
	printk("╚════════╝        ╚═╝        ╚═╝      ╚═╝  \n");

}

int main(int argc, char **argv)
{
	zvm_print_log();
	return 0;
}
