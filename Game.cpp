#include "Game.h"

void Game::Init(HWND hwnd)
{
	mHwnd = hwnd;
	mBackground = std::make_unique<Gdiplus::Image>(L"Data/bg_blank.png");
}

void Game::Release()
{
	mBackground.reset();
}

void Game::Draw(Gdiplus::Graphics& graphics)
{
	graphics.DrawImage(mBackground.get(), 0, 0,
		mBackground->GetWidth(), mBackground->GetHeight());
}

void Game::OnClick()
{
}

void Game::CreateCard()
{
}
