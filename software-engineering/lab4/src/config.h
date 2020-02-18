// Software engineering
// Lab 4
// @mvodya 14/10/19

// REQUIRE GUI LIBRARY: FLTK 1.3
#pragma once

// Fix FLTK fuck problem with Fl_Text_Display.H
#ifdef _WIN32
#define WIN32
#define _CRT_SECURE_NO_WARNINGS
#endif  // _WIN32

// Very small voxel values clipping to avoid undead problem
static const double LOW_VALUE_REMOVE = 0.000001;

// Simulation view window size
static const int VIEW_WINDOW_SIZE = 650;

// Use this for off special info in console
// I have no idea who can found this thing useful ¯\_(ツ)_/¯
//#define LESS_UI_MODE