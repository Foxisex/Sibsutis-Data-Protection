#pragma once

#include <libdp/lab2.hpp>

#include <filesystem>
#include <libdp/picosha2.h>
#include <sstream>

std::string read_file(std::string &filename);
std::array<int64_t, 2> RSA_sign(std::string &filename, std::string &data);
bool RSA_valid(std::string &filename, int64_t &d, int64_t &N, uint64_t &size);
std::array<int64_t, 3> Elgamal_sign(std::string &filename, std::string &data);
bool Elgamal_valid(std::string &filename, std::array<int64_t, 3> Pgy, uint64_t &size);
std::array<int64_t, 4> GOST_sign(std::string &filename, std::string &data);
bool GOST_valid(std::string &filename, std::array<int64_t, 4> pqay, uint64_t &size);