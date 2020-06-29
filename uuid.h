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

#pragma once

#include <cstdint>
#include <iosfwd>

class Uuid
{
  std::uint64_t m_hi;
  std::uint64_t m_lo;

  friend std::ostream& operator<<(std::ostream& os, const Uuid& uuid);

public:
  Uuid();
  Uuid(std::uint64_t hi, std::uint64_t lo);

  std::uint64_t hi() const
  {
    return m_hi;
  }

  std::uint64_t lo() const
  {
    return m_lo;
  }
};

std::ostream& operator<<(std::ostream& os, const Uuid& uuid);
