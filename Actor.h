#pragma once

#include "D2DFramework.h"

class Actor
{
protected:
	D2DFramework* mpFramework;
	ID2D1Bitmap* mpBitmap;

	float mX;
	float mY;
	float mOpacity;

public:
	Actor() = default;
	Actor(D2DFramework* pFramework, LPCWSTR filename);
	Actor(D2DFramework* pFramework, LPCWSTR filename, float x, float y, float opacity = 1.0f);

	virtual ~Actor();

	virtual void Draw();

private:
	void Draw(float x, float y, float opacity = 1.0f);
	// public 쪽의 함수들은 사용하기 편하게 매개변수들을 없애고 제대로된 함수는 private에 만든다

public:
	// 2개의 float으로 된 vector를 반환하는 타입
	inline D2D_VECTOR_2F GetPosition() const
	{
		return D2D_VECTOR_2F{ mX, mY };
	}
	inline void SetPosition(D2D_VECTOR_2F& position)
	{
		mX = position.x;
		mY = position.y;
	}
	inline void SetPosition(float x, float y)
	{
		mX = x;
		mY = y;
	}
};