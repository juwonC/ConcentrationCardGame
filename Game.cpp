#include "Game.h"
#include <string>

namespace concentration
{
	void Game::Init(HWND hwnd)
	{
		mHwnd = hwnd;
		mBackground = std::make_unique<Gdiplus::Image>(L"Data/bg_blank.png");

		CreateCard();
	}

	void Game::Release()
	{
		mBackground.reset();

		mDeck.clear();
	}

	void Game::Draw(Gdiplus::Graphics& graphics)
	{
		graphics.DrawImage(mBackground.get(), 0, 0,
			mBackground->GetWidth(), mBackground->GetHeight());

		for (auto& card : mDeck)
		{
			card.Draw(graphics);
		}

		Gdiplus::PointF pos(895.0f, 20.0f);
		Gdiplus::SolidBrush brush(Gdiplus::Color(190, 70, 60));
		Gdiplus::Font font(L"±¼¸²", 20);

		graphics.DrawString(L"Å¬¸¯¼ö:", -1, &font, pos, &brush);

		Gdiplus::StringFormat format;
		format.SetAlignment(Gdiplus::StringAlignmentCenter);
		format.SetLineAlignment(Gdiplus::StringAlignmentCenter);
		graphics.DrawString(std::to_wstring(mFlipCount).c_str(), -1,
							&font, mCountRect, &format, &brush);
	}

	void Game::OnClick(int x, int y)
	{
		for (auto& card : mDeck)
		{
			if (card.CheckClicked(x, y))
			{
				// TODO : Check Card
			}
		}
	}

	void Game::CreateCard()
	{
		// TODO : Create Card

		mDeck.push_back(concentration::Card(mHwnd, concentration::Type::Wolf, 0, 0));
		mDeck.push_back(concentration::Card(mHwnd, concentration::Type::Bear, 120, 0));
		mDeck.push_back(concentration::Card(mHwnd, concentration::Type::Dragon, 240, 0));
	}
}