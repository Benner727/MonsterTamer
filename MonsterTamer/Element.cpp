#include "Element.h"

#include <sstream>

Element::Element()
{
	mIndex = -1;
	mName = "NULL ELEMENT";
}

bool Element::WeakTo(int index) const
{
	return (std::find(mWeakTo.begin(), mWeakTo.end(), index) != mWeakTo.end());
}

bool Element::Resists(int index) const
{
	return (std::find(mResists.begin(), mResists.end(), index) != mResists.end());
}

bool Element::ImmuneTo(int index) const
{
	return (std::find(mImmuneTo.begin(), mImmuneTo.end(), index) != mImmuneTo.end());
}

std::istream& operator>>(std::istream& is, Element& element)
{
	std::string line;
	std::getline(is, line);
	std::stringstream ss(line);

	ss >> element.mIndex >> std::quoted(element.mName);

	char delim;

	element.mWeakTo.clear();
	int weakId;
	while (ss.peek() != ',' && ss >> weakId)
		element.mWeakTo.push_back(weakId);

	ss >> delim;

	element.mResists.clear();
	int resistId;
	while (ss.peek() != ',' && ss >> resistId)
		element.mResists.push_back(resistId);
	
	ss >> delim;

	element.mImmuneTo.clear();
	int immuneId;
	while (ss >>immuneId)
		element.mImmuneTo.push_back(immuneId);

	return is;
}

std::ostream& operator<<(std::ostream& os, const Element& element)
{
	os << element.mIndex << " " << std::quoted(element.mName);

	for (const auto& weak : element.mWeakTo)
		os << " " << weak;
	os << ',';
	for (const auto& resist : element.mResists)
		os << " " << resist;
	os << ',';
	for (const auto& immune : element.mImmuneTo)
		os << " " << immune;

	return os;
}