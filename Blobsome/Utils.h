#pragma once
#ifndef __utils_h__
#define __utils_h__

// Colors
// line parameters
double lRed,lGreen,lBlue;
// fill parameters
double fRed,fGreen,fBlue;
// background parameters
double bRed,bGreen,bBlue;

// width and height of window
int width;
int height;

// local coordinate system
double xCenter;
double yCenter;

// mouse
int mouseX;
int mouseY;

// refresh miliseconds for time
int refreshMs;

// utility functions
void init();
void size(int width, int height);
void stroke(int index);
void fill(int index);
void setBackground(int index);
void setTimer(int milisec);
void ellipse(double x, double y,double width, double height);
void line(double x, double y, double width, double height);
void rect(double x, double y, double width, double height);
void translate(double xCenter, double yCenter);

int random(int max);
int random(int min, int max);

#endif