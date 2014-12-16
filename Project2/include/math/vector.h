#pragma once
#include <cmath>
#include <SFML/Graphics.hpp>

template<typename T>
class Vec2
{
public:
	Vec2()
	{
		x = (T)0;
		y = (T)0;
	}

	Vec2(const Vec2<T>& v) 
	{ 
		x = v.x;
		y = v.y;
	}

	template<typename U>
	Vec2(const sf::Vector2<U>& v)
	{
		x = (T)v.x;
		y = (T)v.y;
	}

	Vec2<T>(const sf::Vector2<T>& v) 
	{ 
		x = v.x;
		y = v.y;
	}

	Vec2(const T& x, const T& y)
	{
		this->x = x;
		this->y = y;
	}

	Vec2<T>& operator+=(const Vec2<T>& _vec)
	{
		x += _vec.x;
		y += _vec.y;
		return *this;
	}

	Vec2<T>& operator-=(const Vec2<T>& _vec)
	{
		x -= _vec.x;
		y -= _vec.y;
		return *this;
	}

	Vec2<T>& operator*=(const T& n)
	{
		x *= n;
		y *= n;
		return *this;
	}

	Vec2<T>& operator/=(const T& n)
	{
		x /= n;
		y /= n;
		return *this;
	}

	Vec2<T> operator+(const Vec2<T>& _vec)
	{
		Vec2<T> tmp;
		return tmp += _vec;
	}

	Vec2<T> operator-(const Vec2<T>& _vec)
	{
		Vec2<T> tmp;
		return tmp -= _vec;

	}

	Vec2<T> operator*(const T& n)
	{
		Vec2<T> tmp;
		return tmp *= n;
	}

	Vec2<T> operator/(const T& n)
	{
		Vec2<T> tmp;
		return tmp *= n;
	}

	float dot(const Vec2<T>& v)
	{
		return (float)(x * v.x + y * v.y);
	} 

	bool operator==(const Vec2<T>& v) const
	{
		return x == v.x && y == v.y;
	}

	T lengthSquared() const
	{
		return x * x + y * y;
	}

	T length() const
	{
		return (T)sqrtf((float)lengthSquared());
	}

	void normalize()
	{
		*this *= length();
	}

	float getSlope() const
	{
		return (float)x / (float)y;
	}

	bool isCollinear(const Vec2<T>& v) const
	{
		return getSlope() == v.getSlope() || getSlope() == -v.getSlope();
	}

	bool isPerpendicular(const Vec2<T>& v) const
	{
		return dot(v) == (T)0;
	}

	template<typename U>
	sf::Vector2<U> sfmlVector() const
	{
		return sf::Vector2<U>((U)x, (U)y);
	}

	T x, y;
};

typedef Vec2<double> Vec2d;
typedef Vec2<float> Vec2f;
typedef Vec2<int> Vec2i;
typedef Vec2<unsigned int> Vec2u;
typedef Vec2<unsigned char> Vec2u8;
typedef Vec2<unsigned short> Vec2u16;