#include "CCenterMenu.h"

int findMenuWidth(const vector<Texture>& optionsList) {
	int maxWidth = 0;
	for (const auto& option : optionsList) {
		maxWidth = max(option.Width(), maxWidth);
	}

	return maxWidth;
}

CCenterMenu::CCenterMenu()
{
	mQuit = false;

	mMenuTitle = "";
	setSelectorSymbol("->","<-");

	mHeight = 0;
	mWidth = 0;

	mCurrChoice = 0;
	mMarginTopMenuTitle = 0;

	mMenuTitleColor = COLOUR::CYAN;
	setMenuColor(COLOUR::WHITE);
	setHoverOptionColor(COLOUR::CYAN);
}

void CCenterMenu::setSelectorSymbol(Texture leftSymbol,Texture rightSymbol) {
	mSelectorLeftSymbol = leftSymbol;
	mSelectorRightSymbol = rightSymbol;
}

void CCenterMenu::setMenuTitle(Texture menuTitle,COLOUR color)
{
	mMenuTitle = menuTitle;
	mMenuTitleColor = color;
}

void CCenterMenu::setMarginTop(int marginTop) {
	if (marginTop < 0)
		return;

	mMarginTopMenuTitle = marginTop;
}

void CCenterMenu::setMenuColor(COLOUR color)
{
	mMenuColor = color;
}

void CCenterMenu::setHoverOptionColor(COLOUR color)
{
	mMenuHoverOptionColor = color;
}

void CCenterMenu::setOption(int pos, Texture option) {
	if (mOptions.size() < pos) {
		return;
	}
	mOptions[pos] = option;
}

void CCenterMenu::setOption(int pos, Texture option, function<void()> func) {
	if (mOptions.size() < pos) {
		return;
	}
	mOptions[pos] = option;
	mOptionsFunc[pos] = func;
}

bool CCenterMenu::isHasOption(const Texture& inOption) const {
	for (const auto& option : mOptions) {
		if (inOption == option)
			return true;
	}
	return false;
}

void CCenterMenu::QuitMenu() {
	mQuit = true;
}

KEY CCenterMenu::keyPressed() {
	KEY choice = KEY::NULL_CHAR;
	while (true)
	{
		if (_kbhit()) {
			choice = (KEY)toupper(_getch());
			switch (choice)
			{
			case KEY::UP:
				--mCurrChoice;
				mCurrChoice = (mCurrChoice + mHeight) % mHeight;
				break;
			case KEY::DOWN:
				++mCurrChoice;
				mCurrChoice %= mHeight;
				break;
			default:
				break;
			}
			break;
		}
	}

	return choice;
}

void CCenterMenu::insertOption( int pos, Texture option, function<void()> func)
{
	if (mOptions.size() < pos) {
		return;
	}
	mOptions.insert(mOptions.begin() + pos, option);
	mOptionsFunc.insert(mOptionsFunc.begin() + pos, func);

	mHeight += 1;
	mWidth = findMenuWidth(mOptions);
}

void CCenterMenu::addOption(Texture option)
{
	mOptions.push_back(option);
	mOptionsFunc.resize(mOptions.size());
	mHeight += 1;
	mWidth = findMenuWidth(mOptions);
}

void CCenterMenu::addOption(Texture option, function<void()> func)
{
	mOptions.push_back(option);
	mOptionsFunc.push_back(func);
	mHeight += 1;
	mWidth = findMenuWidth(mOptions);
}

void CCenterMenu::removeOption(Texture option)
{
	int pos = 0;
	int n = mOptions.size();
	for (pos = 0; pos < n; pos++) {
		if (mOptions[pos] == option) {
			// Erase option out
			mOptions.erase(mOptions.begin() + pos);
			mOptionsFunc.erase(mOptionsFunc.begin() + pos);

			mHeight -= 1;
			mWidth = findMenuWidth(mOptions);
			return;
		}
	}
	
}

void CCenterMenu::drawMenu(Console& console)
{
	int screenHeight = console.Height();
	int screenWidth = console.Width();
	int titleHeight = mMenuTitle.Height();
	int titleWidth = mMenuTitle.Width();

	int titleX = 0;
	int titleY = 0;

	if (titleHeight != 0) {
		titleX = (screenWidth - titleWidth) / 2;
		titleY = (screenHeight - titleHeight) / 2 - mHeight;
		console.DrawObject(titleX,titleY, mMenuTitle,mMenuTitleColor);
	}

	int x = (screenWidth - mOptions[0].Width()) / 2;
	int y = titleY + titleHeight + mMarginTopMenuTitle;
	for (int index = 0; index < mHeight; index++) {
		COLOUR textColor = mMenuColor;
		if (index == mCurrChoice) {
			textColor = mMenuHoverOptionColor;
			console.DrawObject(x - mSelectorLeftSymbol.Width() - 1, y, mSelectorLeftSymbol, textColor);
			console.DrawObject(x + mOptions[index].Width() + 1, y, mSelectorRightSymbol, textColor);

		}
		console.DrawObject(x, y, mOptions[index], textColor);
		y+=mOptions[index].Height();
	}
}

void  CCenterMenu::Run(Console& console) {
	KEY isPressed;
	do
	{
		console.ClearScreen();
		drawMenu(console);
		console.Render();

		isPressed = keyPressed();
		if (isPressed == KEY::RETURN) {
			if(mOptionsFunc[mCurrChoice])
				mOptionsFunc[mCurrChoice]();
		}
		if (mQuit) {
			mQuit = false;
			break;
		}
	} while (mOptionsFunc[mCurrChoice] || (isPressed != KEY::RETURN));
}




