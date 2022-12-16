// Control font format and color in the terminal
#ifndef REDBUD_IO_COLOR_H_
#define REDBUD_IO_COLOR_H_

#include "../platform.h"
#if defined(REDBUD_LINUX) || defined(REDBUD_OSX)
    #include <unistd.h> //getenv
    #include <cstring>  //strstr
#elif defined(REDBUD_WIN)
    #include <Windows.h>
    #include <io.h>
#endif

#include <cstdlib>
#include <iostream>
#include <type_traits>

namespace redbud {
namespace io {
// Enumerates the ANSI escape code corresponding to the font attributes.
// Example:
//   using namespace redbud::io;
//   std::cout << fg::red << "This text has a red foreground color\n";
//   std::cout << bg::green << "This text has a green background color\n"


// Sets the text format, some of them is not widely supported.
enum class format {
    reset      = 0,  // All attributes off.
    bold       = 1,  // Bold or increased intensity.
    faint      = 2,  // Faint (decreased intensity).
    italic     = 3,  // Italian font.
    underline  = 4,  // Underline.
    blinkslow  = 5,  // Blink slowly.
    blinkrapid = 6,  // Blink quickly.
    inverse    = 7,  // Swap foreground and background.
    conceal    = 8,  // Hide the text.
    strikeline = 9   // Characters legible, but marked for deletion.
};

// Sets the foreground color.
enum class fg {
    black   = 30,
    red     = 31,
    green   = 32,
    yellow  = 33,
    blue    = 34,
    purple  = 35,
    cyan    = 36,
    white   = 37,
    reserve = 38,
    reset   = 39
};

// Sets the background color.
enum class bg
{
    black   = 40,
    red     = 41,
    green   = 42,
    yellow  = 43,
    blue    = 44,
    purple  = 45,
    cyan    = 46,
    white   = 47,
    reserve = 38,
    reset   = 39
};

// Sets the highlight foreground color.
enum class hfg
{
    black  = 90,
    red    = 91,
    green  = 92,
    yellow = 93,
    blue   = 94,
    purple = 95,
    cyan   = 96,
    white  = 97
};

// Sets the highlight background color.
enum class hbg
{
    black  = 100,
    red    = 101,
    green  = 102,
    yellow = 103,
    blue   = 104,
    purple = 105,
    cyan   = 106,
    white  = 107
};

// Sets the control state.
enum class state
{
    automatic = 0,  // Automatic control.
    manual    = 1   // Manual control.
};

namespace details {

inline const std::streambuf*& get_coutbuf() {
    static const std::streambuf* pout = std::cout.rdbuf();
    return pout;
}

inline const std::streambuf*& get_cerrbuf() {
    static const std::streambuf* perr = std::cerr.rdbuf();
    return perr;
}

inline const std::streambuf*& get_clogbuf() {
  static const std::streambuf* plog = std::clog.rdbuf();
  return plog;
}

// Gets an unique integer to use as index to iword()
inline int get_iword() {
    static int i = std::ios_base::xalloc();
    return i;
}

// Determines whether the terminal color of this system can be modified.
inline bool is_modifiable() {
#if defined(REDBUD_LINUX) || defined(REDBUD_OSX)
    static constexpr const char* terms[] = {
        "ansi", "color", "console", "cygwin", "gnome", "konsole", "kterm",
        "linux", "msys", "putty", "rxvt", "screen", "vt100", "xterm"
    };
    const char* penv = std::getenv("TERM");
    if (penv == nullptr) return false;
    bool result = false;
    for (const auto& t : terms) {
        if (std::strstr(penv, t) != nullptr) {
            result = true;
            break;
        }
    }
#elif defined(REDBUD_WIN)
    static constexpr bool result = true;
#endif
    return result;
}

/// Determines whether the buffer stream reaches the end.
inline bool is_terminal(const std::streambuf* buf) {
    if (buf == get_coutbuf()) {
#if defined(REDBUD_LINUX) || defined(REDBUD_OSX)
        return isatty(fileno(stdout)) ? true : false;
#elif defined(REDBUD_WIN)
        return _isatty(_fileno(stdout)) ? true : false;
#endif
    }

    if (buf == get_cerrbuf() || buf == get_clogbuf()) {
#if defined(REDBUD_LINUX) || defined(REDBUD_OSX)
        return isatty(fileno(stderr)) ? true : false;
#elif defined(REDBUD_WIN)
        return _isatty(_fileno(stderr)) ? true : false;
#endif
    }
}

// For overloading standard output stream.
template <typename T>
using color_return_t = typename std::enable_if<
    std::is_same<T, redbud::io::format>::value ||
    std::is_same<T, redbud::io::fg>::value ||
    std::is_same<T, redbud::io::bg>::value ||
    std::is_same<T, redbud::io::hfg>::value ||
    std::is_same<T, redbud::io::hbg>::value,
    std::ostream&>::type;

template <typename T>
using state_return_t = typename std::enable_if<
    std::is_same<T, redbud::io::state>::value,
    std::ostream&>::type;

// Sets the format and color of the text.
#if defined(REDBUD_LINUX) || defined(REDBUD_OSX)
template <typename T>
inline color_return_t<T> set_color(std::ostream& os, const T& value) {
    return os << "\033[" << static_cast<int>(value) << "m";
}
#endif

}// namespace details

// Overloads standard output stream to control the color of text.
template <typename T>
inline details::color_return_t<T>
operator << (std::ostream& os, const T& value) {
    return (os.iword(details::get_iword()) ||
            (details::is_modifiable() &&
            details::is_terminal(os.rdbuf())))
        ? details::set_color(os, value) : os; 
}

template <typename T>
inline details::state_return_t<T>
operator << (std::ostream& os, const T& value) {
    if (value == redbud::io::state::automatic) {
        os.iword(details::get_iword()) = 0;
    } else if (value == redbud::io::state::manual) {
        os.iword(details::get_iword()) = 1;
    }
    return os;
}

}// namespace io
}// namespace redbud
#endif // !REDBUD_IO_COLOR_H_