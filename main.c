#include <stdio.h>
#include <math.h>
#include <stdbool.h>
#include <unistd.h>

#include "logger.h"
#include "vector.h"
#include "triangle.h"
#include "input.h"

#define SCREENSIZE 40
#define WORLDSIZE 1
#define NUMSHADES 10
#define FACESIZE 0.5
#define FOV (M_PI/2.0)
#define DISTANCE 1.5

#define CLEARTERM() printf("\x1b[2J")
#define RESETCURSOR() printf("\x1b[H")
#define INVERSECOLORS() printf("\033[7m")
#define NORMALCOLORS() printf("\033[m")

int main(void)
{

	logger_setOutput("run.log");
	logger_setLevel(LOGTYPE_INFO);

	input_init();

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

	Vec3 pivot = { 0, DISTANCE, 0 };

	CLEARTERM();
	double period = 0.05;
	bool run = true;
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
		triangle_renderAll(numtriangles, triangles, SCREENSIZE, FOV);
		INVERSECOLORS();
		printf(" t=%.2fs ", t);

		if (angles.x < 0)
			printf("| ROTATING UP ");
		else if (angles.x > 0)
			printf("| ROTATING DOWN ");

		if (angles.z < 0)
			printf("| ROTATING LEFT ");
		else if (angles.z > 0)
			printf("| ROTATING RIGHT ");

		printf("\n Press 'q' or 'Esc' to exit, arrow keys to rotate ");
		NORMALCOLORS();
		fflush(stdout);

		usleep(period * 1000000);
	}

	CLEARTERM();
	RESETCURSOR();

	return 0;
}

