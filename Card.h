#pragma once
#include <Windows.h>
#include <gdiplus.h>
#include <memory>

enum class Type
{
	Wolf,
	Dragon,
	Bear
};

class Card
{
private:
	std::unique_ptr<Gdiplus::Image> mBack;
	std::unique_ptr<Gdiplus::Image> mFront;

	HWND mHwnd;
	int mX;
	int mY;
	bool mIsFront;
	Type mType;

public:
	Card(HWND hwnd, Type type, int x, int y);

	bool CheckClicked();
	void Flip(bool isFront);
	void Draw(Gdiplus::Graphics& graphics);
};

