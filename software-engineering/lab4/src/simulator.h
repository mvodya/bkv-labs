// Software engineering
// Lab 4
// @mvodya 14/10/19
#pragma once
#include "voxel.h"

class Simulator {
 private:
  // 2D voxels array
  // World for sub operations
  Voxel** subworld;

 public:
  // 2D voxels array
  Voxel** world;
  // World sizes
  const int size_x, size_y;
  // Ticks count from simulation started
  unsigned long long int tickCount;

  // Get voxel
  inline Voxel* getVoxel(int y, int x);
  // Get voxel from sub world
  inline Voxel* getSubVoxel(int y, int x);

  // Copy world to subworld
  void copy();
  // Commit changes from sub world to world
  void commit();

 public:
  Simulator(int x, int y);
  ~Simulator();

  /* Multipliers */
  double grass_grow;
  double grass_expansion;
  double rabbit_grow;
  double rabbit_expansion;
  double rabbit_eat;
  double rabbit_death;
  double fox_grow;
  double fox_expansion;
  double fox_eat;
  double fox_death;

  // Step update world
  void tick();

  unsigned long long int currentTick() { return tickCount; }
};