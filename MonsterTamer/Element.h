#pragma once

#include <string>
#include <vector>
#include <iomanip>

class Element
{
public:
	Element();
	
private:
	int mIndex;
	std::string mName;

	std::vector<int> mWeakTo;
	std::vector<int> mResists;
	std::vector<int> mImmuneTo;

public:
	int Index() const { return mIndex; }
	std::string Name() const { return mName; }

	bool WeakTo(int index) const;
	bool Resists(int index) const;
	bool ImmuneTo(int index) const;

	inline bool operator==(const Element& other) { return (mIndex == other.mIndex); }

	friend std::istream& operator>>(std::istream& is, Element& element);
	friend std::ostream& operator<<(std::ostream& os, const Element& element);
};