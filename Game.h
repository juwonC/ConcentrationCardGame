#pragma once
#include <list>
#include "Card.h"

namespace concentration
{
	class Game
	{
		const int BOARD_COLUMN = 8;
		const int BOARD_ROW = 5;
		const Gdiplus::RectF mCountRect{ 885.0f, 60.0f, 120.0f, 30.0f };
		const Gdiplus::RectF mScore1Rect{ 885.0f, 250.0f, 120.0f, 30.f };
		const Gdiplus::RectF mScore2Rect{ 885.0f, 450.0f, 120.0f, 30.f };

	private:
		std::unique_ptr<Gdiplus::Image> mBackground;
		std::list<Card> mDeck;
		int mFlipCount{};
		int mCurrentPlayer{1};
		int mPlayer1Score{};
		int mPlayer2Score{};
		HWND mHwnd{};
		Card* mpSelectedCard{};

	public:
		void Init(HWND hwnd);
		void Release();

		void Draw(Gdiplus::Graphics& graphics);
		void OnClick(int x, int y);

	private:
		void CreateCard();
		void DrawScore(Gdiplus::Graphics& graphics);
		void SwitchPlayer();
	};
}