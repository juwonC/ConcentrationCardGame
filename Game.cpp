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

		DrawScore();

		hr = mspRenderTarget->EndDraw();
		if (hr == D2DERR_RECREATE_TARGET)
		{
			CreateDevicResources();
		}
	}

	void Game::OnClick(int x, int y)
	{
		Card* pCard{};

		for (auto& card : mDeck)
		{
			if (card->CheckClicked(
				static_cast<float>(x), 
				static_cast<float>(y)))
			{
				pCard = card.get();
				break;
			}
		}

		if (pCard)
		{
			mFlipCount++;

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
						mDeck.remove_if([&](auto& card) {
							return (card->GetIndex() == mpSelectedCard->GetIndex() || card->GetIndex() == pCard->GetIndex());
						}
						);

						mpSelectedCard = nullptr;

						if (mCurrentPlayer == 1)
						{
							mPlayer1Score++;
						}
						else
						{
							mPlayer2Score++;
						}

						if (mDeck.empty())
						{
							if (mPlayer1Score > mPlayer2Score)
							{
								MessageBox(nullptr, L"Player1 Wins", L"Congratulation!", MB_OK);
							}
							else
							{
								MessageBox(nullptr, L"Player2 Wins", L"Congratulation!", MB_OK);
							}
						}
					}
					else
					{
						Render();
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
				mDeck.push_back(std::make_shared<Card>(this, index, types[index++], 
					static_cast<float>(posX), 
					static_cast<float>(posY)));
				posY += 140 + 10;
			}
			posX += 100 + 10;
		}

	}

	void Game::DrawScore()
	{
		const WCHAR click[] = L"Clicks : ";
		const WCHAR player1[] = L"Player1 : ";
		const WCHAR player2[] = L"Player2 : ";

		D2D1_SIZE_F rtSize = mspRenderTarget->GetSize();

		mspRenderTarget->DrawText(
			click,
			ARRAYSIZE(click) - 1,
			mspTextFormat.Get(),
			D2D1::RectF(900.0f, 20.0f, rtSize.width, rtSize.height),
			mspBrush.Get()
		);

		mspRenderTarget->DrawText(
			player1,
			ARRAYSIZE(player1) - 1,
			mspTextFormat.Get(),
			D2D1::RectF(900.0f, 200.0f, rtSize.width, rtSize.height),
			mspBrush.Get()
		);

		mspRenderTarget->DrawText(
			player2,
			ARRAYSIZE(player1) - 1,
			mspTextFormat.Get(),
			D2D1::RectF(900.0f, 400.0f, rtSize.width, rtSize.height),
			mspBrush.Get()
		);

		std::wstring flipCount = std::to_wstring(mFlipCount).c_str();
		std::wstring player1Score = std::to_wstring(mPlayer1Score).c_str();
		std::wstring player2Score = std::to_wstring(mPlayer2Score).c_str();


		mspRenderTarget->DrawTextW(
			std::to_wstring(mFlipCount).c_str(),
			flipCount.size(), 
			mspTextFormat.Get(), 
			D2D1::RectF(980.0f, 20.0f, rtSize.width, rtSize.height),
			mspBrush.Get()
		);

		mspRenderTarget->DrawTextW(
			std::to_wstring(mPlayer1Score).c_str(),
			player1Score.size(), 
			mspTextFormat.Get(), 
			D2D1::RectF(980.0f, 200.0f, rtSize.width, rtSize.height),
			mspBrush.Get()
		);

		mspRenderTarget->DrawTextW(
			std::to_wstring(mPlayer2Score).c_str(),
			player2Score.size(), 
			mspTextFormat.Get(),
			D2D1::RectF(980.0f, 400.0f, rtSize.width, rtSize.height),
			mspBrush.Get()
		);
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