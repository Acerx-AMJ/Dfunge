#ifndef FORMAT_HPP
#define FORMAT_HPP

#include <iostream>
#include <sstream>
#undef assert

// String conversion functions

template<typename T>
inline std::string toString(const T &value) {
   std::stringstream s;
   s << std::boolalpha << value;
   return s.str();
}

// Format functions

template<typename... Args>
std::string format(const char *base, const Args&...args) {
   std::string result = base;

   size_t pos = 0;
   (( pos = result.find("{}", pos),
      pos != std::string::npos
         ? result = result.replace(pos, 2, toString(args))
         : result
   ), ...);
   return result;
}

template<typename... Args>
void warn(const char *base, const Args&...args) {
   std::cout << "WARNING: " << format(base, args...) << "\n";
}

template<typename... Args>
[[noreturn]] void raise(const char *base, const Args&...args) {
   std::cout << "ERROR: " << format(base, args...) << '\n';
   std::exit(-1);
}

template<typename...Args>
void check(bool condition, const char *base, const Args&...args) {
   if (!condition) {
      warn(base, args...);
   }
}

template<typename... Args>
void assert(bool condition, const char *base, const Args&...args) {
   if (!condition) {
      raise(base, args...);
   }
}

#endif
