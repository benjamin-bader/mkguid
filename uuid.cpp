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

#include "uuid.h"

#include <iostream>
#include <random>

using namespace std;

namespace {

constexpr const char* const kHexAlphabet = "0123456789abcdef";
constexpr const uint64_t kVariantFlag = 0x8000000080000000UL;
constexpr const uint64_t kVariantMask = 0x3FFFFFFFFFFFFFFFUL;

constexpr const uint64_t kVersionFlag = 0x0000000000004000UL;
constexpr const uint64_t kVersionMask = 0xFFFFFFFFFFFF0FFFUL;

void print_bytes(int N, ostream& out, uint64_t n)
{
  for (int shifts = N - 1; shifts >= 0; shifts--)
  {
    auto b = (n >> (shifts * 8)) & 0xFF;
    auto hi = (b & 0xFF) >> 4;
    auto lo = (b & 0x0F);
    out << kHexAlphabet[hi] << kHexAlphabet[lo];
  }
}

}

Uuid::Uuid()
{
  random_device dev;
  mt19937_64 dist(dev());
  m_hi = (dist() & kVersionMask) | kVersionFlag;
  m_lo = (dist() & kVariantMask) | kVariantFlag;
}

Uuid::Uuid(uint64_t hi, uint64_t lo)
{
  m_hi = (hi & kVersionMask) | kVersionFlag;
  m_lo = (lo & kVariantMask) | kVariantFlag;
}

ostream& operator<<(ostream& os, const Uuid& uuid)
{
  print_bytes(4, os, uuid.m_hi >> 32);
  os << "-";
  print_bytes(2, os, uuid.m_hi >> 16);
  os << "-";
  print_bytes(2, os, uuid.m_hi);
  os << "-";
  print_bytes(2, os, uuid.m_lo >> 48);
  os << "-";
  print_bytes(6, os, uuid.m_lo);
  return os;
}
