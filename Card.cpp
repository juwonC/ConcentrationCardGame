#include <string>
#include "Card.h"
#include "BitmapManager.h"

namespace concentration
{
	Card::Card(D2DFramework* pFramework, int index, Type type, float x, float y) : Actor(pFramework, L"Data/card_back.png")
	{
		mIndex = index;
		mType = type;
		mX = x;
		mY = y;
		mIsFront = false;
		
		std::wstring filename(L"Data/card_creature_");

		switch (type)
		{
			case Type::Wolf:
				filename = L"wolf.png";
				break;

			case Type::Dragon:
				filename = L"dragon.png";
				break;

			case Type::Bear:
				filename = L"bear.png";
				break;
		}

		mpFront = BitmapManager::Instance().LoadBitmap(filename);
	}

	bool Card::CheckClicked(float x, float y)
	{
		if(x >= mX && x <= mX + mpBitmap->GetSize().width &&
			y >= mY && y <= mY + mpBitmap->GetSize().height)
		{
			Flip(!mIsFront);
			return true;
		}

		return false;
	}

	void Card::Flip(bool isFront)
	{
		mIsFront = isFront;
		//Invalidate();
	}

	void Card::Draw()
	{
		auto pRT = mpFramework->GetRenderTarget();
		auto size = mpBitmap->GetPixelSize();
		D2D1_RECT_F rect{
		0.0f, 0.0f,
		static_cast<float>(0.0f + size.width),
		static_cast<float>(0.0f + size.height)
		};
		auto matTranslate = D2D1::Matrix3x2F::Translation(mX, mY);
		pRT->SetTransform(matTranslate);

		if (mIsFront)
		{
			pRT->DrawBitmap(mpFront, rect, mOpacity);
		}
		else
		{
			pRT->DrawBitmap(mpBitmap, rect, mOpacity);
		}
	}

	//void Card::Invalidate()
	//{
	//	// TODO : Invalidate?

	//	auto pRT = mpFramework->GetRenderTarget();
	//	auto size = mpBitmap->GetPixelSize();

	//	RECT rct{ mX, mY,
	//		static_cast<LONG>(mX + size.width),
	//		static_cast<LONG>(mY + size.height) };

	//	InvalidateRect(mHwnd, &rct, false);
	//}
}