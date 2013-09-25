#ifndef __BASIC_OP_HPP
#define __BASIC_OP_HPP

template< typename T >
void swap(T & a, T & b)
{
	T t = a;
	a = b;
	b = t;
}

template< typename R, typename W >
void copy(R start, R end, W result)
{
	while (start!=end)
		*result++ = *start++;	  
}
#endif
