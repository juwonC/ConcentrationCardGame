#include "Game.h"
#include <string>
#include <vector>
#include <random>

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

		DrawScore(graphics);
	}

	void Game::OnClick(int x, int y)
	{
		Card* pCard{};
		
		for (auto& card : mDeck)
		{
			if (card.CheckClicked(x, y))
			{
				pCard = &card;
				break;
			}
		}

		if (pCard)
		{
			mFlipCount++;
			RECT rct{ 
					static_cast<LONG>(mCountRect.GetLeft()),
					static_cast<LONG>(mCountRect.GetTop()),
					static_cast<LONG>(mCountRect.GetRight()),
					static_cast<LONG>(mCountRect.GetBottom())
			};
			InvalidateRect(mHwnd, &rct, false);

			if (mpSelectedCard == nullptr)
			{
				mpSelectedCard = pCard;
			}
			else
			{
				if (mpSelectedCard == pCard)
				{
					mpSelectedCard = nullptr;
				}
				else
				{
					if (pCard->GetType() == mpSelectedCard->GetType())
					{
						pCard->Invalidate();
						mpSelectedCard->Invalidate();

						mDeck.remove_if([&](Card& card) { 
								return (card.GetIndex() == mpSelectedCard->GetIndex() || card.GetIndex() == pCard->GetIndex());
							}
						);

						mpSelectedCard = nullptr;
						
						if (mCurrentPlayer == 1)
						{
							mPlayer1Score++;

							RECT rct{
									static_cast<LONG>(mScore1Rect.GetLeft()),
									static_cast<LONG>(mScore1Rect.GetTop()),
									static_cast<LONG>(mScore1Rect.GetRight()),
									static_cast<LONG>(mScore1Rect.GetBottom())
							};
							InvalidateRect(mHwnd, &rct, false);
						}
						else
						{
							mPlayer2Score++;

							RECT rct{
									static_cast<LONG>(mScore2Rect.GetLeft()),
									static_cast<LONG>(mScore2Rect.GetTop()),
									static_cast<LONG>(mScore2Rect.GetRight()),
									static_cast<LONG>(mScore2Rect.GetBottom())
							};
							InvalidateRect(mHwnd, &rct, false);
						}
					}
					else
					{
						UpdateWindow(mHwnd);
						Sleep(500);
						pCard->Flip(false);
						mpSelectedCard->Flip(false);

						mpSelectedCard = nullptr;

						SwitchPlayer();
					}
				}
			}
		}
	}

	void Game::CreateCard()
	{
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
				mDeck.push_back(Card(mHwnd, index, types[index++], posX, posY));
				posY += 140 + 10;
			}
			posX += 100 + 10;
		}

	}

	void Game::DrawScore(Gdiplus::Graphics& graphics)
	{
		Gdiplus::SolidBrush brush(Gdiplus::Color(190, 70, 60));
		Gdiplus::Font font(L"±¼¸²", 20);

		Gdiplus::PointF player1(895.0f, 200.0f);
		graphics.DrawString(L"Player1: ", -1, &font, player1, &brush);

		Gdiplus::PointF player2(895.0f, 400.0f);
		graphics.DrawString(L"Player2: ", -1, &font, player2, &brush);

		Gdiplus::StringFormat format1;
		format1.SetAlignment(Gdiplus::StringAlignmentCenter);
		format1.SetLineAlignment(Gdiplus::StringAlignmentCenter);
		graphics.DrawString(std::to_wstring(mPlayer1Score).c_str(), -1,
			&font, mScore1Rect, &format1, &brush);

		Gdiplus::StringFormat format2;
		format2.SetAlignment(Gdiplus::StringAlignmentCenter);
		format2.SetLineAlignment(Gdiplus::StringAlignmentCenter);
		graphics.DrawString(std::to_wstring(mPlayer2Score).c_str(), -1,
			&font, mScore2Rect, &format2, &brush);
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