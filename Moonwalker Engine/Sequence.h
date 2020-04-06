#pragma once
#include <list>

template <typename T>
class Sequence {
public:
	Sequence() {
		sequence_count = 0;
	}
	void Push(const T &elem) {
		_base.push_back(elem);
		sequence_count++;
	}
	const T& operator [](int index) { 
		return At(index);
	}
	void Delete(int index) {
		if (index < 0 || index >= sequence_count) {
			throw std::exception("index out of range");
		}
		else {
			int i = 0;
			for (auto it = _base.begin(); it != _base.end(); *it++) {
				if (i == index) {
					_base.erase(it);
					sequence_count--;
					break;
				}
				i++;
			}
		}
	}
	T PopFront() {
		if (sequence_count > 0) {
			T ret = (*_base.front());
			_base.pop_front();
			return ret;
		}
		else {
			throw std::exception("nothing to pop");
			return NULL;
		}
	}
	T PopBack() {
		if (sequence_count > 0) {
			T ret = (*_base.end());
			_base.pop_back();
			return ret;
		}
		else {
			throw std::exception("nothing to pop");
			return NULL;
		}
	}
	int Count() {
		return sequence_count;
	}
private:
	std::list<T> _base;
	const T& At(int index) {
		int i = 0;
		for (auto it = _base.begin(); it != _base.end(); *it++) {
			if (i == index) {
				return *it;
			}
			i++;
		}
		return *_base.end();
	}
	int sequence_count;
};