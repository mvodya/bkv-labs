// Software engineering
// Lab 4
// @mvodya 14/10/19
#include "simulator.h"
#include <iostream>

Simulator::Simulator(int x, int y) : size_x(x), size_y(y) {
  // Generate empty world
  world = new Voxel*[size_y];
  for (int i = 0; i < size_y; i++) world[i] = new Voxel[size_x];
  // Generate empty sub world
  subworld = new Voxel*[size_y];
  for (int i = 0; i < size_y; i++) subworld[i] = new Voxel[size_x];

  // Set default multipliers
  grass_grow = 0.01f;
  grass_expansion = 0.05f;
  rabbit_grow = 0.02f;
  rabbit_expansion = 0.05f;
  rabbit_eat = 0.05f;
  rabbit_death = 0.01f;
  fox_grow = 0.1f;
  fox_expansion = 0.1f;
  fox_eat = 0.02f;
  fox_death = 0.005f;

  tickCount = 0;
}

Simulator::~Simulator() {
  // Remove 2D voxels array
  for (int i = 0; i < size_y; i++) delete[] world[i];
  delete world;
  for (int i = 0; i < size_y; i++) delete[] subworld[i];
  delete subworld;
}

void Simulator::tick() {
  copy();
  // std::cout << "tick (" << tickCount << ")\n";

  for (int i = 0; i < size_y; i++) {
    for (int j = 0; j < size_x; j++) {
      Voxel* v = getVoxel(i, j);
      double r = v->fox();
      double g = v->grass();
      double b = v->rabbit();
      Voxel* sv = getSubVoxel(i, j);

      // Grass grow
      sv->grassAdd(g * grass_grow);
      sv->grassRemove(g * grass_expansion);
      sv->grassRemove(b * rabbit_eat);

      // Grass expansion
      double g_exp = g * grass_expansion / 4.f;
      getSubVoxel(i + 1, j)->grassAdd(g_exp);
      getSubVoxel(i, j + 1)->grassAdd(g_exp);
      getSubVoxel(i - 1, j)->grassAdd(g_exp);
      getSubVoxel(i, j - 1)->grassAdd(g_exp);

      // Rabbit grow
      sv->rabbitAdd(b * rabbit_grow * g);
      sv->rabbitRemove(b * rabbit_death);
      sv->rabbitRemove(r * fox_eat);

      // Rabbit expansion
      double b_exp[4];
      b_exp[0] = getSubVoxel(i + 1, j)->grass();
      b_exp[1] = getSubVoxel(i, j + 1)->grass();
      b_exp[2] = getSubVoxel(i - 1, j)->grass();
      b_exp[3] = getSubVoxel(i, j - 1)->grass();

      double e = (b_exp[0] + b_exp[1] + b_exp[2] + b_exp[3]);
      if (e > 0.001f)
        e = (b * rabbit_expansion) / e;
      else
        e = 0;

      sv->rabbitRemove((b_exp[0] + b_exp[1] + b_exp[2] + b_exp[3]) * e);

      getSubVoxel(i + 1, j)->rabbitAdd(b_exp[0] * e);
      getSubVoxel(i, j + 1)->rabbitAdd(b_exp[1] * e);
      getSubVoxel(i - 1, j)->rabbitAdd(b_exp[2] * e);
      getSubVoxel(i, j - 1)->rabbitAdd(b_exp[3] * e);

      // Fox grow
      sv->foxAdd(r * fox_grow * b);
      sv->foxRemove(r * fox_death);

      // fox expansion
      double r_exp[8];
      r_exp[0] = getSubVoxel(i + 1, j)->rabbit();
      r_exp[1] = getSubVoxel(i, j + 1)->rabbit();
      r_exp[2] = getSubVoxel(i - 1, j)->rabbit();
      r_exp[3] = getSubVoxel(i, j - 1)->rabbit();
      r_exp[4] = getSubVoxel(i + 1, j + 1)->rabbit();
      r_exp[5] = getSubVoxel(i - 1, j + 1)->rabbit();
      r_exp[6] = getSubVoxel(i - 1, j + 1)->rabbit();
      r_exp[7] = getSubVoxel(i - 1, j - 1)->rabbit();
      double sum_r_exp = 0;
      for (int i = 0; i < 8; i++) sum_r_exp += r_exp[i];

      e = sum_r_exp;
      if (e > 0.001f)
        e = (r * fox_expansion) / e;
      else
        e = 0;

      sv->foxRemove(sum_r_exp * e);

      getSubVoxel(i + 1, j)->foxAdd(r_exp[0] * e);
      getSubVoxel(i, j + 1)->foxAdd(r_exp[1] * e);
      getSubVoxel(i - 1, j)->foxAdd(r_exp[2] * e);
      getSubVoxel(i, j - 1)->foxAdd(r_exp[3] * e);
      getSubVoxel(i + 1, j + 1)->foxAdd(r_exp[4] * e);
      getSubVoxel(i - 1, j + 1)->foxAdd(r_exp[5] * e);
      getSubVoxel(i - 1, j + 1)->foxAdd(r_exp[6] * e);
      getSubVoxel(i - 1, j - 1)->foxAdd(r_exp[7] * e);
    }
  }

  commit();
  tickCount++;
}

inline Voxel* Simulator::getVoxel(int y, int x) {
  if (y < 0) y = size_y + y;
  if (x < 0) x = size_x + x;
  y %= size_y;
  x %= size_x;
  return &world[y][x];
}

inline Voxel* Simulator::getSubVoxel(int y, int x) {
  if (y < 0) y = size_y + y;
  if (x < 0) x = size_x + x;
  y %= size_y;
  x %= size_x;
  return &subworld[y][x];
}

void Simulator::copy() {
  for (int i = 0; i < size_y; i++) {
    for (int j = 0; j < size_x; j++) {
      subworld[i][j] = world[i][j];
    }
  }
}

void Simulator::commit() {
  for (int i = 0; i < size_y; i++) {
    for (int j = 0; j < size_x; j++) {
      world[i][j] = subworld[i][j];
    }
  }
}
