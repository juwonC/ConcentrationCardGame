#pragma once
#include "Card.h"

class Game
{
	const int BOARD_COLUMN = 8;
	const int BOARD_ROW = 5;
	const Gdiplus::RectF mCountRect{ 885.0f, 60.0f, 120.0f, 30.0f };

private:
	std::unique_ptr<Gdiplus::Image> mBackground;
	int mFlipCount{};
	HWND mHwnd{};
	Card* mpSelectedCard{};

public:
	void Init(HWND hwnd);
	void Release();

	void Draw(Gdiplus::Graphics& graphics);
	void OnClick();

private:
	void CreateCard();
};

