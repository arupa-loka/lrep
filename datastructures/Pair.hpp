#ifndef _PAIR_HPP
#define _PAIR_HPP

template<class A, class B>
class Pair {
	public:
	Pair();
	Pair(A iFirst, B iSecond);
	Pair(const Pair<A,B> & iP);
	//Pair(const Pair<A,B> iP);
	const Pair<A,B>& operator=(const Pair<A,B>& iP);
	inline A first() const;
	inline B second() const;
	
	private:
	A _first;
	B _second;
};

template<class A, class B>
Pair<A,B>::Pair() {}

template<class A, class B>
Pair<A,B>::Pair(const Pair<A,B> & iP): _first(iP.first()), _second(iP.second()) {}

//template<class A, class B>
//Pair<A,B>::Pair(const Pair<A,B> iP): _first(iP.first()), _second(iP.second()) {}

template<class A, class B>
Pair<A,B>::Pair(A iFirst, B iSecond): _first(iFirst), _second(iSecond) {}

template<class A, class B>
const Pair<A,B>& Pair<A,B>::operator=(const Pair<A,B>& iP)
{
	if (this!=&iP) {
		_first=iP.first();	  
		_second = iP.second();
	}
	return *this;
}
template<class A, class B>
A Pair<A,B>::first() const { return _first; }

template<class A, class B> 
B Pair<A,B>::second() const { return _second; }

#endif

