#pragma once
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

	Vec2(const sf::Vector2<T>& v) 
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

	T length() const
	{
		return (T)sqrtf((float)(x * x + y * y));
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
		return getSlope() == v.getSlope();
	}

	T x, y;
};
