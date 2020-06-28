// mkguid - program to generate GUIDs.
// Copyright (C) 2020 Benjamin Bader
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include <cstdint>
#include <iostream>
#include <random>
#include <type_traits>

using namespace std;

random_device r;
mt19937_64 e(r());

static_assert(sizeof(e()) == 8, "Random number generator must yield a 64-bit number");

constexpr const char* const kHexAlphabet = "0123456789abcdef";
constexpr const decltype(e()) kVariantFlag = 0x8000000080000000UL;
constexpr const decltype(e()) kVariantMask = 0x3FFFFFFFFFFFFFFFUL;

constexpr const decltype(e()) kVersionFlag = 0x0000000000004000UL;
constexpr const decltype(e()) kVersionMask = 0xFFFFFFFFFFFF0FFFUL;


template <typename Int>
ostream& print_hex_byte(ostream& out, Int n)
{
  auto hi = (n & 0xFF) >> 4;
  auto lo = (n & 0x0F);
  return out << kHexAlphabet[hi] << kHexAlphabet[lo];
}

template <typename Int>
ostream& print_uuid(ostream& out, Int hi, Int lo)
{
  static_assert(std::is_integral_v<Int> && sizeof(Int) == 8, "Ints mustust be unsigned and 64-bits");
  print_hex_byte(out, hi >> 56);
  print_hex_byte(out, hi >> 48);
  print_hex_byte(out, hi >> 40);
  print_hex_byte(out, hi >> 32);
  out << "-";
  print_hex_byte(out, hi >> 24);
  print_hex_byte(out, hi >> 16);
  out << "-";
  print_hex_byte(out, hi >> 8);
  print_hex_byte(out, hi >> 0);
  out << "-";
  print_hex_byte(out, lo >> 56);
  print_hex_byte(out, lo >> 48);
  out << "-";
  print_hex_byte(out, lo >> 40);
  print_hex_byte(out, lo >> 32);
  print_hex_byte(out, lo >> 24);
  print_hex_byte(out, lo >> 16);
  print_hex_byte(out, lo >> 8);
  print_hex_byte(out, lo >> 0);
}

int main()
{
  auto hi = (e() & kVersionMask) | kVersionFlag;
  auto lo = (e() & kVariantMask) | kVariantFlag;

  print_uuid(cout, hi, lo);
  cout << endl;
}
