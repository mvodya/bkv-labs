// Software engineering
// Lab 5

#pragma once
#include <string>

namespace calc {

namespace error_handler {

static bool error;
static std::string error_message;

void set(std::string m) {
  error = true;
  error_message = m;
}

}

}