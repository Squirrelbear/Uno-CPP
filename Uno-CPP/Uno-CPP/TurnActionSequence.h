#pragma once

#include <vector>
#include <map>

template <class T>
class TurnActionSequence
{
public:
	TurnActionSequence() = default;
	virtual ~TurnActionSequence() = default;

	/**
	 * Stores the specified data into the storedData map to be used for future iterations.
	 *
	 * @param key Key to store at in the storedData map.
	 * @param value Value to associate with the key.
	 */
	void injectProperty(const std::string& key, const int value);

	// Gets the value at the specified position, returns -1 if the key could not be found.
	int getPropertyValue(const std::string& key) const;

private:
	std::vector<T*> _unorderedSequence;
	T* _currentAction;
	std::map<std::string, int> _storedData;
};

template<class T>
inline void TurnActionSequence<T>::injectProperty(const std::string & key, const int value)
{
	_storedData[key] = value;
}

template<class T>
inline int TurnActionSequence<T>::getPropertyValue(const std::string & key) const
{
	if (_storedData.find(key) == _storedData.end()) {
		return -1;
	}

	return _storedData.at(key);
}
