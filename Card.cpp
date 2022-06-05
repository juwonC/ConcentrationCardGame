#include <string>
#include "Card.h"

namespace concentration
{
	Card::Card(HWND hwnd, int index, Type type, int x, int y) :
		mHwnd{ hwnd },
		mIndex{ index },
		mX{ x },
		mY{ y },
		mIsFront{ false },
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

		// TODO : Card Initialize?

		mspBack = std::make_unique<Actor>(this, L"Data/card_back.png", x, y);
		mspFront = std::make_unique<Actor>(this, filename.c_str(), x, y);
	}

	bool Card::CheckClicked(POINT& pt)
	{
		auto size{ mpBitmap->GetPixelSize() };
		
		D2D1_RECT_F rect{
		mX, mY,
		static_cast<float>(mX + size.width),
		static_cast<float>(mY + size.height)
		};

		if(pt.x >= rect.left && pt.x <= rect.right &&
			pt.y >= rect.top && pt.y <= rect.bottom)
		{
			Flip(!mIsFront);
			return true;
		}

		return false;
	}

	void Card::Flip(bool isFront)
	{
		mIsFront = isFront;
		Invalidate();
	}

	void Card::Draw()
	{
		// TODO : Draw?

		if (mIsFront)
		{
			mspFront->Draw();
		}
		else
		{
			mspBack->Draw();
		}
	}
	void Card::Invalidate()
	{
		// TODO : Invalidate?

		auto pRT = mpFramework->GetRenderTarget();
		auto size = mpBitmap->GetPixelSize();

		RECT rct{ mX, mY, 
			static_cast<LONG>(mX + size.width), 
			static_cast<LONG>(mY + size.height) };

		InvalidateRect(mHwnd, &rct, false);
	}
}