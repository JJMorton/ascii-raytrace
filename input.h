#ifndef INPUT_H
#define INPUT_H

typedef enum
{
	KEY_UP,
	KEY_DOWN,
	KEY_LEFT,
	KEY_RIGHT,
	KEY_MINUS,
	KEY_PLUS,
	KEY_COMMA,
	KEY_FULLSTOP,
	KEY_BACKSPACE,
	KEY_ESC,
	KEY_Q,
	KEY_NONE
} Key;

void input_init(void);
void input_poll(void);
Key input_pop(void);

#endif // INPUT_H

