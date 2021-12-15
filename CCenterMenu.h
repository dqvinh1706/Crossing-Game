#ifndef CCENTER_MENU_H
#define CCENTER_MENU_H

#include "Console.h"
#include <functional>

class CCenterMenu
{
	bool mQuit;

	Texture mMenuTitle;
	Texture mSelectorLeftSymbol;
	Texture mSelectorRightSymbol;
	vector<Texture> mOptions;
	vector<function <void()>> mOptionsFunc;

	int mHeight;
	int mWidth;

	int mCurrChoice;
	int mMarginTopMenuTitle;

	COLOUR mMenuTitleColor;
	COLOUR mMenuColor;
	COLOUR mMenuHoverOptionColor;
public:
	CCenterMenu();
	CCenterMenu(Console*);
	~CCenterMenu() = default;

	void setSelectorSymbol(Texture = "->", Texture = "<-");
	void setMenuTitle(Texture,COLOUR = COLOUR::CYAN);
	void setMarginTop(int);
	void setMenuColor(COLOUR);
	void setHoverOptionColor(COLOUR);
	void setOption(int, Texture);
	void setOption(int, Texture, function<void()>);

	bool isHasOption(const Texture&) const;

	void QuitMenu();
	KEY keyPressed();
	void insertOption(int,Texture,function<void()>);
	void addOption(Texture);
	void addOption(Texture, function<void()>);
	void removeOption(Texture);
	void drawMenu(Console&);

	void Run(Console&);
};

#endif // CCENTER_MENU_H


