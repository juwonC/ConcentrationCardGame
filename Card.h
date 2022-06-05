#pragma once
#include <Windows.h>
#include <gdiplus.h>
#include <memory>
#include "Actor.h"

namespace concentration
{
	enum class Type
	{
		Wolf,
		Dragon,
		Bear
	};

	class Card : public Actor
	{
	private:
		std::unique_ptr<Actor> mspBack;
		std::unique_ptr<Actor> mspFront;

		HWND mHwnd;
		int mIndex;
		int mX;
		int mY;
		bool mIsFront;
		Type mType;

	public:
		Card(HWND hwnd, int index, Type type, int x, int y);

		bool CheckClicked(POINT& pt);
		void Flip(bool isFront);
		virtual void Draw() override;
		void Invalidate();

		Type GetType() { return mType; }
		int GetIndex() { return mIndex; }
	};
}
