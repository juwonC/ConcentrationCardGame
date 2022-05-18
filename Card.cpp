#include <string>
#include "Card.h"

Card::Card(HWND hwnd, Type type, int x, int y) :
	mHwnd(hwnd),
	mX(x), 
	mY(y),
	mIsFront(false),
	mType{ type }
{
	std::wstring filename(L"Data/card_creature_");

	switch (type)
	{
		case Type::Wolf:
			filename += L"wolf.png";
			break;

		case Type::Dragon:
			filename += L"dragon.png";
			break;

		case Type::Bear:
			filename += L"bear.png";
			break;
	}

	mBack = std::make_unique<Gdiplus::Image>(L"Data/card_back.png");
	mFront = std::make_unique<Gdiplus::Image>(filename.c_str());
}

bool Card::CheckClicked()
{
	return false;
}

void Card::Flip(bool isFront)
{
	mIsFront = isFront;
}

void Card::Draw(Gdiplus::Graphics& graphics)
{
	if (mIsFront)
	{
		graphics.DrawImage(mFront.get(), mX, mY,
			mFront->GetWidth(), mFront->GetHeight());
	}
	else
	{
		graphics.DrawImage(mBack.get(), mX, mY,
			mBack->GetWidth(), mBack->GetHeight());
	}
}
