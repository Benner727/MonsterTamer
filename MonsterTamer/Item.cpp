#include "Item.h"

Item::Item()
{
	mIndex = -1;
	mName = "NULL ITEM";

	mValue = 0;

	mEffect = 0;
	mPotency = 0;
}

std::istream& operator>>(std::istream& is, Item& item)
{
	is >> item.mIndex >> std::quoted(item.mName) >> item.mValue >> std::hex >> item.mEffect >> std::dec >> item.mPotency;

	return is;
}

std::ostream& operator<<(std::ostream& os, const Item& item)
{
	os << item.mIndex << " " << std::quoted(item.mName) << " " << item.mValue << " " << item.mEffect << " " << item.mPotency;

	return os;
}