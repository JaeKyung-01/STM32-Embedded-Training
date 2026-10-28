/*
 * ap_main.c
 *
 *  Created on: 2026. 10. 28.
 *      Author: kccistc
 */

#include "ap_main.h"
#include "service/temp_humid/temp_humid.h"
#include "service/display/display.h"


int ap_main(void)
{
	TH_init();
	display_init();

	while(1)
	{
		TH_task();
		display_task();
	}

	return 0;
}
