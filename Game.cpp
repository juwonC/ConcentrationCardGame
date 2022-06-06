#include <string>
#include <vector>
#include <random>
#include "Game.h"

namespace concentration
{
	HRESULT Game::Initialize(HINSTANCE hInstance, LPCWSTR title, UINT width, UINT height)
	{
		HRESULT hr;
		hr = D2DFramework::Initialize(hInstance, title, width, height);
		ThrowIfFailed(hr);

		mspBackground = std::make_unique<Actor>(this, L"Data/bg_blank.png", 0.0f, 0.0f);

		CreateCard();

		return S_OK;
	}

	void Game::Release()
	{
		for (auto& card : mDeck)
		{
			card.reset();
		}

		mDeck.clear();
		mspBackground.reset();

		D2DFramework::Release();
	}

	void Game::Render()
	{
		HRESULT hr;
		
		mspRenderTarget->BeginDraw();
		mspRenderTarget->Clear(D2D1::ColorF(0.0f, 0.2f, 0.4f, 1.0f));

		mspBackground->Draw();

		for (auto& card : mDeck)
		{
			card->Draw();
		}

		// TODO : DrawText

		static const WCHAR click[] = L"Clicks : ";

		D2D1_SIZE_F renderTargetSize = mspRenderTarget->GetSize();

		mspRenderTarget->DrawText(
			click,
			ARRAYSIZE(click) - 1,
			mspTextFormat.Get(), 
			D2D1::RectF(900.0f, 20.0f, renderTargetSize.width, renderTargetSize.height),
			mspBrush.Get()
		);
		//mspRenderTarget->DrawText(std::to_wstring(mFlipCount).c_str(),
		//	-1, mspTextFormat.Get(), mCountRect, mspBrush.Get());

		//DrawScore();

		hr = mspRenderTarget->EndDraw();
		if (hr == D2DERR_RECREATE_TARGET)
		{
			CreateDevicResources();
		}
	}

	//void Game::OnClick(int x, int y)
	//{
	//	// TODO : OnClick

	//	Card* pCard;

	//	//for (auto& card : mDeck)
	//	//{
	//	//	if (CheckClicked())
	//	//	{
	//	//		pCard = reinterpret_cast<Card*>(&card);
	//	//		break;
	//	//	}
	//	//}

	//	if (pCard)
	//	{
	//		mFlipCount++;

	//		RECT rct{
	//				static_cast<LONG>(mCountRect.left),
	//				static_cast<LONG>(mCountRect.top),
	//				static_cast<LONG>(mCountRect.right),
	//				static_cast<LONG>(mCountRect.bottom)
	//		};
	//		InvalidateRect(mHwnd, &rct, false);

	//		if (mpSelectedCard == nullptr)
	//		{
	//			mpSelectedCard = pCard;
	//		}
	//		else
	//		{
	//			if (mpSelectedCard == pCard)
	//			{
	//				mpSelectedCard = nullptr;
	//			}
	//			else
	//			{
	//				if (pCard->GetType() == mpSelectedCard->GetType())
	//				{
	//					pCard->Invalidate();
	//					mpSelectedCard->Invalidate();

	//					mDeck.remove_if([&](Card& card) {
	//						return (card.GetIndex() == mpSelectedCard->GetIndex() || card.GetIndex() == pCard->GetIndex());
	//					}
	//					);

	//					mpSelectedCard = nullptr;

	//					if (mCurrentPlayer == 1)
	//					{
	//						mPlayer1Score++;

	//						RECT rct{
	//								static_cast<LONG>(mScore1Rect.left),
	//								static_cast<LONG>(mScore1Rect.top),
	//								static_cast<LONG>(mScore1Rect.right),
	//								static_cast<LONG>(mScore1Rect.bottom)
	//						};
	//						InvalidateRect(mHwnd, &rct, false);
	//					}
	//					else
	//					{
	//						mPlayer2Score++;

	//						RECT rct{
	//								static_cast<LONG>(mScore2Rect.left),
	//								static_cast<LONG>(mScore2Rect.top),
	//								static_cast<LONG>(mScore2Rect.right),
	//								static_cast<LONG>(mScore2Rect.bottom)
	//						};
	//						InvalidateRect(mHwnd, &rct, false);
	//					}

	//					if (mDeck.empty())
	//					{
	//						if (mPlayer1Score > mPlayer2Score)
	//						{
	//							MessageBox(nullptr, L"Player1 Wins", L"Congratulation!", MB_OK);
	//						}
	//						else
	//						{
	//							MessageBox(nullptr, L"Player2 Wins", L"Congratulation!", MB_OK);
	//						}
	//					}
	//				}
	//				else
	//				{
	//					UpdateWindow(mHwnd);
	//					Sleep(500);
	//					pCard->Flip(false);
	//					mpSelectedCard->Flip(false);

	//					mpSelectedCard = nullptr;

	//					SwitchPlayer();
	//				}
	//			}
	//		}
	//	}
	//}
	

	void Game::CreateCard()
	{
		// TODO : CreateCard
		RECT rect{};
		GetClientRect(mHwnd, &rect);

		std::vector<Type> types;
		while (types.size() < static_cast<size_t>(BOARD_COLUMN * BOARD_ROW))
		{
			int temp = types.size() % 6;

			switch (temp)
			{
				case 0 :
					types.push_back(Type::Bear);
					types.push_back(Type::Bear);
					break;

				case 2:
					types.push_back(Type::Wolf);
					types.push_back(Type::Wolf);
					break;

				case 4:
					types.push_back(Type::Dragon);
					types.push_back(Type::Dragon);
					break;
			}
		}

		std::random_device rd;
		std::mt19937 gen(rd());
		std::shuffle(types.begin(), types.end(), gen);

		int posX{ 15 }, posY{ 10 };
		int index{};
		for (int x = 0; x < BOARD_COLUMN; ++x)
		{
			posY = 10;

			for (int y = 0; y < BOARD_ROW; ++y)
			{				
				mDeck.push_back(std::make_shared<Card>(this, L"Data/card_back.png", mHwnd, index, types[index++], posX, posY));
				posY += 140 + 10 + rect.bottom;
			}
			posX += 100 + 10 + rect.right;
		}

	}

	void Game::DrawScore()
	{
		// TODO : DrawScore

		//Gdiplus::SolidBrush brush(Gdiplus::Color(190, 70, 60));
		//Gdiplus::Font font(L"±¼¸²", 20);

		//Gdiplus::PointF player1(895.0f, 200.0f);
		//graphics.DrawString(L"Player1: ", -1, &font, player1, &brush);

		//Gdiplus::PointF player2(895.0f, 400.0f);
		//graphics.DrawString(L"Player2: ", -1, &font, player2, &brush);

		//Gdiplus::StringFormat format1;
		//format1.SetAlignment(Gdiplus::StringAlignmentCenter);
		//format1.SetLineAlignment(Gdiplus::StringAlignmentCenter);
		//graphics.DrawString(std::to_wstring(mPlayer1Score).c_str(), -1,
		//	&font, mScore1Rect, &format1, &brush);

		//Gdiplus::StringFormat format2;
		//format2.SetAlignment(Gdiplus::StringAlignmentCenter);
		//format2.SetLineAlignment(Gdiplus::StringAlignmentCenter);
		//graphics.DrawString(std::to_wstring(mPlayer2Score).c_str(), -1,
		//	&font, mScore2Rect, &format2, &brush);
	}

	void Game::SwitchPlayer()
	{
		++mCurrentPlayer;

		if (mCurrentPlayer > 2)
		{
			mCurrentPlayer = 1;
		}

		if (mCurrentPlayer == 1)
		{
			MessageBox(nullptr, L"Player1's Turn", L"Alarm", MB_OK);
		}
		else
		{
			MessageBox(nullptr, L"Player2's Turn", L"Alarm", MB_OK);
		}
	}
}