#pragma once
#include <raylib.h>
#include <math.h>

static inline Vector2	operator+(const Vector2& lhs, const Vector2& rhs)
{
	return { lhs.x + rhs.x, lhs.y + rhs.y };
}

static inline Vector2	operator-(const Vector2& lhs, const Vector2& rhs)
{
	return { lhs.x - rhs.x, lhs.y - rhs.y };
}

static inline bool	operator==(const Vector2& lhs, const Vector2& rhs)
{
	if (lhs.x == rhs.x && lhs.y == rhs.y)
		return true;
	else
		return false;
}

static inline float	dist(const Vector2& lhs, const Vector2& rhs)
{
	return sqrt((lhs.x - rhs.x) * (lhs.x - rhs.x) + (lhs.y - rhs.y) * (lhs.y - rhs.y));
}


typedef struct IVector2 {
	int x;
	int y;

	operator Vector2() {
		return	{ static_cast<float>(x), static_cast<float>(y) };
	}
} IVector2;

static inline IVector2	operator+(const IVector2& lhs, const IVector2& rhs)
{
	return { lhs.x + rhs.x, lhs.y + rhs.y };
}

static inline IVector2	operator-(const IVector2& lhs, const IVector2& rhs)
{
	return { lhs.x - rhs.x, lhs.y - rhs.y };
}

static inline bool	operator==(const IVector2& lhs, const IVector2& rhs)
{
	if (lhs.x == rhs.x && lhs.y == rhs.y)
		return true;
	else
		return false;
}

static inline bool	operator!=(const IVector2& lhs, const IVector2& rhs)
{
	if (lhs.x == rhs.x && lhs.y == rhs.y)
		return false;
	else
		return true;
}

static inline int		mht_dist(const IVector2& lhs, const IVector2& rhs)
{
	return abs(rhs.x - lhs.x) + abs(rhs.y - lhs.y);
}