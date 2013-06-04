/*
 * tasmania.c
 * 
 * detect keyboard stream
 *
 * 2013 Gabriel Alonso <gbr.alonso@gmail.com>
 * 
 */

#include <stdio.h>
#include <string.h>
#include "get_keyboard.h"

int get_keyboard(char *buffer, size_t buffer_len)
{

	FILE *devices;

	char events[128];
	char handlers[128];
	char *handlerSelected;
	char eventSelected[128];

	char returnV[] = "/dev/input/";

	int i;

	devices = fopen("/proc/bus/input/devices", "r");

	if (!devices) {
		perror("fopen");
		return -1;
	}

	while (fgets(events, sizeof(events), devices)) {
		if (strstr(events, "H: Handlers=") == events)
		{
			strcpy(handlers, events);
		}else if (!strcmp(events, "B: EV=120013\n")){

			handlerSelected = strstr(handlers, "event");

			for(i=0;i<=strlen("event");i++)
				eventSelected[i] = handlerSelected[i];

			return snprintf(buffer, buffer_len, "%s", strcat(returnV, eventSelected));
		}
	}

	return -1;
}