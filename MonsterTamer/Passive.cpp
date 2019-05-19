#include "Passive.h"

Passive::Passive()
{
	mIndex = -1;
	mName = "NULL PASSIVE";

	mEffect = 0;
}

std::istream& operator>>(std::istream& is, Passive& passive)
{
	is >> passive.mIndex >> std::quoted(passive.mName) >> passive.mEffect >> std::quoted(passive.mDescription);

	return is;
}

std::ostream& operator<<(std::ostream& os, const Passive passive)
{
	os << passive.mIndex << " " << std::quoted(passive.mName) << " " << passive.mEffect << " " << std::quoted(passive.mDescription);

	return os;
}