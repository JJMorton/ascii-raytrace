#include <stdio.h>
#include <math.h>
#include <stdbool.h>
#include <unistd.h>

#include "logger.h"
#include "vector.h"
#include "triangle.h"
#include "input.h"

#define SCREENSIZE 40
#define FACESIZE 0.5
#define DISTANCE 1.5

#define CLEARTERM() printf("\033[2J")
#define RESETCURSOR() printf("\033[H")
#define INVERSECOLORS() printf("\033[7m")
#define NORMALCOLORS() printf("\033[m")

void fillEndWithChar(size_t strlen, char str[strlen], char c);

int main(void)
{

	logger_setOutput("run.log");
	logger_setLevel(LOGTYPE_INFO);

	size_t numtriangles = 6;
	Vec3 vertices[] =
	{
		// Front
		{ 0, DISTANCE, FACESIZE },
		{ -FACESIZE, DISTANCE - FACESIZE, -FACESIZE },
		{ +FACESIZE, DISTANCE - FACESIZE, -FACESIZE },

		// Back
		{ 0, DISTANCE, FACESIZE },
		{ -FACESIZE, DISTANCE + FACESIZE, -FACESIZE },
		{ +FACESIZE, DISTANCE + FACESIZE, -FACESIZE },

		// Left
		{ 0, DISTANCE, FACESIZE },
		{ -FACESIZE, DISTANCE + FACESIZE, -FACESIZE },
		{ -FACESIZE, DISTANCE - FACESIZE, -FACESIZE },

		// Right
		{ 0, DISTANCE, FACESIZE },
		{ +FACESIZE, DISTANCE + FACESIZE, -FACESIZE },
		{ +FACESIZE, DISTANCE - FACESIZE, -FACESIZE },

		// Bottom
		{ -FACESIZE, DISTANCE - FACESIZE, -FACESIZE },
		{ -FACESIZE, DISTANCE + FACESIZE, -FACESIZE },
		{ +FACESIZE, DISTANCE - FACESIZE, -FACESIZE },

		{ +FACESIZE, DISTANCE + FACESIZE, -FACESIZE },
		{ -FACESIZE, DISTANCE + FACESIZE, -FACESIZE },
		{ +FACESIZE, DISTANCE - FACESIZE, -FACESIZE }
	};

	Triangle triangles[numtriangles];
	for (int i = 0; i < numtriangles; i++)
	{
		triangles[i] = triangle_create(vertices[i * 3], vertices[i * 3 + 1], vertices[i * 3 + 2]);
	}
	logger_log(LOGTYPE_INFO, "Created triangles");

	input_init();

	Vec3 pivot = { 0, DISTANCE, 0 };
	double fov = M_PI / 2.0;
	double cameradistance = 0.0;
	double period = 0.05;
	bool run = true;
	CLEARTERM();
	for (double t = 0; run; t += period)
	{
		input_poll();
		Key key;
		Vec3 angles = { 0, 0, 0 };
		while ((key = input_pop()) != KEY_NONE)
		{
			switch (key)
			{
				case KEY_UP:
					angles.x -= period; break;
				case KEY_DOWN:
					angles.x += period; break;
				case KEY_LEFT:
					angles.z -= period; break;
				case KEY_RIGHT:
					angles.z += period; break;
				case KEY_MINUS:
					cameradistance += period; break;
				case KEY_PLUS:
					cameradistance -= period; break;
				case KEY_COMMA:
					fov = fmax(M_PI / 12.0, fov - period * M_PI / 2.0); break;
				case KEY_FULLSTOP:
					fov = fmin(M_PI, fov + period * M_PI / 2.0); break;
				case KEY_BACKSPACE:
					CLEARTERM(); break;
				case KEY_Q:
				case KEY_ESC:
					run = false; break;
			}
		}

		for (int i = 0; i < numtriangles; i++)
		{
			triangles[i] = triangle_rotate(triangles[i], pivot, angles.x, angles.y, angles.z);
		}

		RESETCURSOR();
		triangle_renderAll(numtriangles, triangles, SCREENSIZE, fov, cameradistance);
		INVERSECOLORS();

		char statustext[SCREENSIZE * 2] = { 0 };
		snprintf(statustext, SCREENSIZE * 2, " FOV=%.1f | DISTANCE=%.1f%s%s ",
				fov * 180 / M_PI,
				cameradistance + DISTANCE,
				angles.x < 0 ? " | ROTATING UP" : (angles.x > 0 ? " | ROTATING DOWN" : ""),
				angles.z < 0 ? " | ROTATING LEFT" : (angles.z > 0 ? " | ROTATING RIGHT" : "")
			);
		fillEndWithChar(SCREENSIZE * 2, statustext, ' ');
		printf("%s", statustext);
		NORMALCOLORS();
		fflush(stdout);

		usleep(period * 1000000);
	}

	CLEARTERM();
	RESETCURSOR();

	return 0;
}

void fillEndWithChar(size_t strlen, char str[strlen], char c)
{
	for (char *end = str + strlen; str < end - 1; str++)
	{
		if (*str == '\0') *str = c;
	}
}

