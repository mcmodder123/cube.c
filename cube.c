#include <math.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

float A, B, C;
float cubeWidth;
int width = 140, height = 44;
float zBuffer[140 * 44];
char buffer[140 * 44];
int bgChar = ' ';
int camDistance = 100;
float horizontalOffset;
float K1 = 40;

float incrementSpeed = 0.6;

float x, y, z;
float oz;
int xp, yp;
int buffer_index;

float calculateX(int i, int j, int k) {
  return j * sin(A) * sin(B) * cos(C) - k * cos(A) * sin(B) * cos(C) +
         j * cos(A) * sin(C) + k * sin(A) * sin(C) + i * cos(B) * cos(C);
}

float calculateY(int i, int j, int k) {
  return j * cos(A) * cos(C) + k * sin(A) * cos(C) -
         j * sin(A) * sin(B) * sin(C) + k * cos(A) * sin(B) * sin(C) -
         i * cos(B) * sin(C);
}

float calculateZ(int i, int j, int k) {
  return k * cos(A) * cos(B) - j * sin(A) * cos(B) + i * sin(B);
}

void calculatePoint(float i, float j, float k, int ch) {
  x = calculateX(i, j, k);
  y = calculateY(i, j, k);
  z = calculateZ(i, j, k) + camDistance;

  oz = 1 / z;

  xp = (int)(width / 2 + horizontalOffset + K1 * oz * x * 2);
  yp = (int)(height / 2 + K1 * oz * y);

  buffer_index = xp + yp * width;
  if (buffer_index >= 0 && buffer_index < width * height) {
    if (oz > zBuffer[buffer_index]) {
      zBuffer[buffer_index] = oz;
      buffer[buffer_index] = ch;
    }
  }
}

int main(int argc, char *argv[]) {
  printf("\x1b[2J");
  while (1) {
    memset(buffer, bgChar, width * height);
    memset(zBuffer, 0, width * height * 4);
    cubeWidth = 15;
    horizontalOffset = -2 * cubeWidth;

    for (float i = -cubeWidth; i < cubeWidth; i += incrementSpeed) {
      for (float j = -cubeWidth; j < cubeWidth; j += incrementSpeed) {
        calculatePoint(i, j, -cubeWidth, '@');
        calculatePoint(cubeWidth, j, i, '$');
        calculatePoint(-cubeWidth, j, -i, '~');
        calculatePoint(-i, j, cubeWidth, '#');
        calculatePoint(i, -cubeWidth, -j, ';');
        calculatePoint(i, cubeWidth, j, '+');
      }
    }

    printf("\x1b[H");
    for (int k = 0; k < width * height; k++) {
      putchar(k % width ? buffer[k] : 10);
    }

    A += 0.05;
    B += 0.05;
    C += 0.01;
    usleep(16000);
  }
  return 0;
}
