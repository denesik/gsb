#pragma once
#ifndef PerlinNoise_h__
#define PerlinNoise_h__



void init(void);
double noise1(double);
double noise2(double *);
double noise3(double *);
void normalize3(double *);
void normalize2(double *);

double PerlinNoise1D(double, double, double, int);
double PerlinNoise2D(double, double, double, double, int);
double PerlinNoise3D(double, double, double, double, double, int);


#endif // PerlinNoise_h__