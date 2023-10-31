#pragma once

#include <iostream>
#include <cmath>
#include <array>
#include <random>
#include <map>
#include <vector>
#include <limits>

int64_t powmod(int64_t a, int64_t x, int64_t p);
std::array<int64_t,3> Evklid(int64_t a, int64_t b);
bool MillerRabinTest(int64_t n, int k);
int64_t BigPrimeNum();
void DiffHellm();
int64_t BSGS();
