#ifndef __PTRTOOLS_UTILITY_HPP
#define __PTRTOOLS_UTILITY_HPP

#include <cstddef>
#include <cstdint>
#include <exception>

namespace ptrtools
{
   template <typename T>
   T align(T value, T boundary) {
      if (boundary == 0)
         throw std::runtime_error("divide by zero");

      if (value % boundary == 0)
         return value;
      else
         return value + (boundary - (value % boundary));
   }
}

#endif
