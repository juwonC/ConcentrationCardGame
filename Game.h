#pragma once
#include <list>
#include "D2DFramework.h"
#include "Actor.h"
#include "ComException.h"
#include "Card.h"

namespace concentration
{
	class Game : public D2DFramework
	{
		const int BOARD_COLUMN = 8;
		const int BOARD_ROW = 5;
		const D2D1_RECT_F mCountRect{ 885.0f, 60.0f, 120.0f, 30.0f };
		const D2D1_RECT_F mScore1Rect{ 885.0f, 250.0f, 120.0f, 30.f };
		const D2D1_RECT_F mScore2Rect{ 885.0f, 450.0f, 120.0f, 30.f };

	private:
		std::unique_ptr<Actor> mspBackground;
		std::list<std::shared_ptr<Card>> mDeck;

		int mFlipCount{};
		int mCurrentPlayer{1};
		int mPlayer1Score{};
		int mPlayer2Score{};
		Card* mpSelectedCard{};

	public:
		virtual HRESULT Initialize(HINSTANCE hInstance, LPCWSTR title = L"Concentration",
			UINT width = 1024, UINT height = 800) override;
		
		virtual void Release() override;
		virtual void Render() override;
		void OnClick(int x, int y);

	private:
		void CreateCard();
		void DrawScore();
		void SwitchPlayer();
	};
}