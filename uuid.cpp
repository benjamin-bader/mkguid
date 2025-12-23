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

#include <chrono>
#include <iostream>
#include <random>

using namespace std;

namespace {

static int g_uuidFormatFlag = ios_base::xalloc();

enum class Fmt : long
{
  Standard = 0,
  Com = 1,
};

void SetFormat(ostream& os, Fmt fmt)
{
  os.iword(g_uuidFormatFlag) = static_cast<long>(fmt);
}

Fmt GetAndClearFormat(ostream& os)
{
  Fmt result = static_cast<Fmt>(os.iword(g_uuidFormatFlag));
  SetFormat(os, Fmt::Standard);
  return result;
}

constexpr const char* const kHexAlphabet = "0123456789abcdef";
constexpr const uint64_t kVariantFlag = 0x8000000000000000UL;
constexpr const uint64_t kVariantMask = 0x3FFFFFFFFFFFFFFFUL;

constexpr const uint64_t kVersionFlag = 0x0000000000004000UL;
constexpr const uint64_t kVersionMask = 0xFFFFFFFFFFFF0FFFUL;

template <int N>
constexpr uint64_t GetVersionFlag() {
  static_assert(N >= 1 && N <= 8);
  return static_cast<uint64_t>(N & 0xFF) << 12;
}

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

Uuid Uuid::v4()
{
  return {};
}

Uuid Uuid::v7()
{
  /*
    UUIDv7 bitwise layout from https://datatracker.ietf.org/doc/rfc9562/

    0                   1                   2                   3
    0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
   |                           unix_ts_ms                          |
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
   |          unix_ts_ms           |  ver  |       rand_a          |
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
   |var|                        rand_b                             |
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
   |                            rand_b                             |
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

  */
  random_device dev;
  mt19937_64 dist(dev());

  auto now = chrono::system_clock::now();
  auto epoch = chrono::duration_cast<chrono::milliseconds>(now.time_since_epoch()).count();

  uint64_t hi = (epoch << 16) | (dist() & 0xFFFFULL);
  uint64_t lo = dist();

  hi = (hi & kVersionMask) | GetVersionFlag<7>();
  lo = (lo & kVariantMask) | kVariantFlag;

  return {hi, lo};
}

Uuid::Uuid()
{
  random_device dev;
  mt19937_64 dist(dev());
  m_hi = (dist() & kVersionMask) | GetVersionFlag<4>();
  m_lo = (dist() & kVariantMask) | kVariantFlag;
}

ostream& operator<<(ostream& os, const Uuid& uuid)
{
  Fmt fmt = GetAndClearFormat(os);
 
  if (fmt == Fmt::Com)
  {
    os << "{";
  }

  print_bytes(4, os, uuid.m_hi >> 32);
  os << "-";
  print_bytes(2, os, uuid.m_hi >> 16);
  os << "-";
  print_bytes(2, os, uuid.m_hi);
  os << "-";
  print_bytes(2, os, uuid.m_lo >> 48);
  os << "-";
  print_bytes(6, os, uuid.m_lo);

  if (fmt == Fmt::Com)
  {
    os << "}";
  }

  return os;
}

namespace UuidFormat {

ostream& Standard(ostream& os)
{
  SetFormat(os, Fmt::Standard);
  return os;
}

ostream& Com(ostream& os)
{
  SetFormat(os, Fmt::Com);
  return os;
}

}
