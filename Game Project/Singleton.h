#pragma once

// A base class which will only allow us to have one of each derived type.
// This will be used for Stage, UI, Window, etc.
// Things that we only need one copy of.
template <class T>
class Singleton
{
public:
	// Virtual destructor.
	virtual ~Singleton() {}

	// Method to statically access this class.
	static T& Get()
	{
		return instance_;	// Return reference to instance of type T.
	}

private:
	T& operator = (const T&) = delete;	// Cannot modify this.
	static T instance_;
};

template <class T>
T Singleton<T>::instance_;