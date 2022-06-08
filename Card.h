#pragma once
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
		ID2D1Bitmap* mpFront;

		//HWND mHwnd;
		int mIndex;
		bool mIsFront;
		Type mType;

	public:
		Card(D2DFramework* pFramework, int index, Type type, float x, float y);

		bool CheckClicked(float x, float y);
		void Flip(bool isFront);
		virtual void Draw() override;
		//void Invalidate();

		Type GetType() { return mType; }
		int GetIndex() { return mIndex; }
	};
}
