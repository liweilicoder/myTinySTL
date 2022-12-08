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

}// namespace details
}// namespace io
}// namespace redbud
#endif // !REDBUD_IO_COLOR_H_