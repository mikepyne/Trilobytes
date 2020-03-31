#ifndef ENERGY_H
#define ENERGY_H

#include <cstdint>

constexpr uint64_t operator ""_Mj(unsigned long long num) { return num * 1'000'000'000'000; }
constexpr uint64_t operator ""_kj(unsigned long long num) { return num * 1'000'000'000; }
constexpr uint64_t operator ""_j(unsigned long long num)  { return num * 1'000'000; }
constexpr uint64_t operator ""_mj(unsigned long long num) { return num * 1'000; }
constexpr uint64_t operator ""_uj(unsigned long long num) { return num * 1; }

constexpr uint64_t operator ""_Mj(long double num) { return static_cast<uint64_t>(num * 1'000'000'000'000); }
constexpr uint64_t operator ""_kj(long double num) { return static_cast<uint64_t>(num * 1'000'000'000); }
constexpr uint64_t operator ""_j(long double num)  { return static_cast<uint64_t>(num * 1'000'000); }
constexpr uint64_t operator ""_mj(long double num) { return static_cast<uint64_t>(num * 1'000); }
constexpr uint64_t operator ""_uj(long double num) { return static_cast<uint64_t>(num * 1); }

#endif // ENERGY_H
