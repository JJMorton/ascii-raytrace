#include <X11/Xlib.h>
#include <limits.h>
#include <termios.h>
#include <stdlib.h>
#include <unistd.h>

#include "input.h"
#include "logger.h"

#define MAX_KEYS_AT_ONCE 8

Display *input_xdpy = NULL;
Key input_keybuffer[MAX_KEYS_AT_ONCE] = { KEY_NONE };

// X key codes corresponding to keys in `Key` enum
// Needs the zero at the end corresponding to KEY_NONE to mark the end of the array
const int XKeyCodes[] =
{
	111, 116, 113, 114, 24, 9, 0
};

struct termios orig_termios;

// Re-enable echoing at exit
void enableEcho(void)
{
	tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios);
}

// To stop user's keypresses appearing in the terminal window
// https://viewsourcecode.org/snaptoken/kilo/02.enteringRawMode.html
void disableEcho(void)
{
	struct termios raw;
	tcgetattr(STDIN_FILENO, &raw);
	orig_termios = raw;
	raw.c_lflag &= ~(ECHO);
	tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
	atexit(enableEcho);
}

void input_init(void)
{
	input_xdpy = XOpenDisplay(NULL);
	if (input_xdpy == NULL)
	{
		logger_log(LOGTYPE_ERROR, "Failed to open X11 display");
	}
	else
	{
		logger_log(LOGTYPE_INFO, "Opened X11 display \"%s\" successfully", XDisplayString(input_xdpy));
		disableEcho();
	}
}

void input_poll(void)
{
	if (input_xdpy == NULL)
	{
		logger_log(LOGTYPE_ERROR, "input_poll() called before successfully initialising display");
		return;
	}

	for (int i = 0; i < MAX_KEYS_AT_ONCE; i++) input_keybuffer[i] = KEY_NONE;

	// Get the pressed keys from x11, each bit corresponds with one key
	char keys[32];
	XQueryKeymap(input_xdpy, keys);

	// Loop through all the key codes of interest and find the relevant bit
	const int *keyCode = XKeyCodes;
	Key key = (Key) 0;
	int keybufferIndex = 0;
	while (*keyCode != 0)
	{
		if (keys[*keyCode / CHAR_BIT] & (1 << *keyCode % CHAR_BIT))
		{
			logger_log(LOGTYPE_LOG, "Key pressed with key code %i", *keyCode);
			if (keybufferIndex >= MAX_KEYS_AT_ONCE)
			{
				logger_log(LOGTYPE_INFO,"Exceeded maximum number of keys pressed at once, some keypresses will be lost");
			}
			else
			{
				input_keybuffer[keybufferIndex++] = key;
			}
		}
		keyCode++;
		key = (Key) (key + 1);
	}
}

Key input_pop(void)
{
	Key key = input_keybuffer[0];
	for (int i = 0; i < MAX_KEYS_AT_ONCE - 1; i++)
	{
		input_keybuffer[i] = input_keybuffer[i + 1];
	}
	input_keybuffer[MAX_KEYS_AT_ONCE - 1] = KEY_NONE;
	return key;
}

