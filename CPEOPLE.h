#ifndef CPEOPLE_H
#define CPEOPLE_H

#include "CLANE.h"

class CPEOPLE {
private:
	// Current position 2D
	CPOINT2D mCurrPos;

	// Limit zone
	CPOINT2D mTopLeft;
	CPOINT2D mBottomRight;

	// Height and width of people
	int mHeight;
	int mWidth;

	// State of alive
	bool mState;

	// People form in ascii
	Texture mPeopleLeftForm;
	Texture mPeopleRightForm;
	Texture* mCurrForm;
public:
	CPEOPLE();
	CPEOPLE(int, int);
	CPEOPLE(int, int, CPOINT2D, CPOINT2D);
	CPEOPLE(const CPEOPLE&);
	~CPEOPLE() = default;

	CPEOPLE& operator= (const CPEOPLE&);

	// Setter
	void setXY(int, int);
	void setLimitZone(CPOINT2D, CPOINT2D);
	void setState(bool);

	// Getter
	int getX() const;
	int getY() const;
	int Height() const;
	int Width() const;
	CPOINT2D getPos() const;

	// Moving function
	void Up(int);
	void Left(int);
	void Right(int);
	void Down(int);

	void Clip(); // Don't let the people go outside the playing area
	void Move(KEY, int);

	// Check impact
	bool isImpact(const CLANE&);

	void storeData(ofstream&);
	void loadData(ifstream&);

	// Handle the event
	bool isFinish() const;
	bool isDead() const;

	// Render people function
	void drawPeople(Console&);
};

#endif // CPEOPLE_H
