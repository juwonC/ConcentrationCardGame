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
		std::shared_ptr<Actor> mspBack;
		std::shared_ptr<Actor> mspFront;

		HWND mHwnd;
		int mIndex;
		bool mIsFront;
		Type mType;

	public:
		Card(D2DFramework* pFramework, LPCWSTR filename, HWND hwnd, int index, Type type, int x, int y);

		bool CheckClicked(POINT& pt);
		void Flip(bool isFront);
		virtual void Draw() override;
		void Invalidate();

		Type GetType() { return mType; }
		int GetIndex() { return mIndex; }
	};
}
