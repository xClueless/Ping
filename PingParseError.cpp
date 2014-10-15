#include "PingParseError.hpp"

using namespace std;

PingParseError::PingParseError(string errorDesc) : runtime_error(errorDesc)
{
}
