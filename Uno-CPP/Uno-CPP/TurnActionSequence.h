#pragma once

#include <vector>
#include <map>

template <class T>
class TurnActionSequence
{
public:
	TurnActionSequence() = default;
	virtual ~TurnActionSequence() = default;

private:
	std::vector<T*> _unorderedSequence;
	T* _currentAction;
	std::map<std::string, int> _storedData;
};

