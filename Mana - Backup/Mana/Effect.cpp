#include "Effect.h"

std::ostream& operator<<( std::ostream &os, Effect &ef )
{
	os << ef.getName();
	return os;
}