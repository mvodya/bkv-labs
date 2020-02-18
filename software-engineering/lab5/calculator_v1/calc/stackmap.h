// Software engineering
// Lab 5

#include <string>
#include <map>

#include "error_handler.h"
#include "symbol.h"
#include "var.h"
#include "functions/function.h"
#include "functions/const.h"

// Functions includes
/*GEN_INCLUDES_AREA*/
#define HELP_DATA "\n"
/*GEN_INCLUDES_AREA_END*/

namespace calc {

std::map<std::string, Symbol*> stackMap = {
// Standart functions
/*GEN_FUNCTIONS_AREA*/

/*GEN_FUNCTIONS_AREA_END*/

// Standart constants
{"PI", new Const(M_PI)},
{"E", new Const(exp(1.0))},
{"PHI", new Const(1.618033988749894848)}

};

}