#pragma once

#include <iostream>
#include <cmath>
#include <array>
#include <random>
#include <map>
#include <vector>
#include <limits>
#include <boost/multiprecision/cpp_int.hpp>
#include <boost/random.hpp>
#include <boost/multiprecision/miller_rabin.hpp>
namespace bm = boost::multiprecision;
typedef boost::multiprecision::cpp_int bigint;

int64_t powmod(int64_t a, int64_t x, int64_t p);
template <typename T>
T powmod(T a, T x, T p);
bigint mod_pow(bigint num, bigint degree, bigint p);
std::array<int64_t,3> Evklid(int64_t a, int64_t b);
std::array<bigint,3> Evklid(bigint a, bigint b);
template <typename T>
std::array<T,3> Evklid(T a, T b);
//bool MillerRabinTest(int64_t n, int k);
template <typename T>
bool MillerRabinTest(T n, int k);
int64_t BigPrimeNum();
void DiffHellm();
int64_t BSGS();
