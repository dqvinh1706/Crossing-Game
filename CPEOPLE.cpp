#include "CPEOPLE.h"

CPEOPLE::CPEOPLE() {
	mState = true;

	mPeopleLeftForm = \
		"\\o~\n"
		" |\\\n"
		" | \n"
		"/ \\";

	mPeopleRightForm = \
		"~o/\n"
		"/| \n"
		" | \n"
		"/ \\";

	mHeight = mPeopleLeftForm.Height();
	mWidth = mPeopleLeftForm.Width();

	mCurrForm = &mPeopleLeftForm;
}

CPEOPLE::CPEOPLE(int x, int y) : CPEOPLE() {
	setXY(x, y);
}

CPEOPLE::CPEOPLE(int x, int y, CPOINT2D topLeft, CPOINT2D bottomRight) : CPEOPLE(x, y) {
	setLimitZone(topLeft, bottomRight);
}

CPEOPLE::CPEOPLE(const CPEOPLE& other) {
	mCurrPos = other.mCurrPos;

	mTopLeft = other.mTopLeft;
	mBottomRight = other.mBottomRight;

	mState = other.mState;

	mPeopleLeftForm = other.mPeopleLeftForm;

	mPeopleRightForm = other.mPeopleRightForm;

	mHeight = other.mHeight;
	mWidth = other.mWidth;

	if(other.mCurrForm == &other.mPeopleLeftForm)
		mCurrForm = &mPeopleLeftForm;
	else
		mCurrForm = &mPeopleRightForm;
}

CPEOPLE& CPEOPLE::operator= (const CPEOPLE& other) {
	if (this == &other)
		return *this;

	mCurrPos = other.mCurrPos;

	mTopLeft = other.mTopLeft;
	mBottomRight = other.mBottomRight;

	mState = other.mState;

	mPeopleLeftForm = other.mPeopleLeftForm;

	mPeopleRightForm = other.mPeopleRightForm;

	mHeight = other.mHeight;
	mWidth = other.mWidth;

	if (other.mCurrForm == &other.mPeopleLeftForm)
		mCurrForm = &mPeopleLeftForm;
	else
		mCurrForm = &mPeopleRightForm;

	return *this;
}

void CPEOPLE::setXY(int x, int y) {
	mCurrPos.setX(x);
	mCurrPos.setY(y);
}

void CPEOPLE::setLimitZone(CPOINT2D topLeft, CPOINT2D bottomRight) {
	mTopLeft = topLeft;
	mBottomRight = bottomRight;
}

void CPEOPLE::setState(bool isAlive)
{
	mState = isAlive;
}

int CPEOPLE::getX() const {
	return mCurrPos.getX();
}

int CPEOPLE::getY() const {
	return mCurrPos.getY();
}

int CPEOPLE::Height() const {
	return mHeight;
}

int CPEOPLE::Width() const {
	return mWidth;
}

CPOINT2D CPEOPLE::getPos() const
{
	return mCurrPos;
}

void CPEOPLE::Up(int delta) {
	mCurrPos.moveY(-delta);
}

void CPEOPLE::Left(int delta) {
	mCurrPos.moveX(-delta);
}

void CPEOPLE::Right(int delta) {
	mCurrPos.moveX(delta);
}

void CPEOPLE::Down(int delta) {
	mCurrPos.moveY(delta);
}

void CPEOPLE::Clip() {
	int currX = mCurrPos.getX();
	int currY = mCurrPos.getY();

	int minX = mTopLeft.getX() + 1;
	int minY = mTopLeft.getY() + 1;

	int maxX = mBottomRight.getX() - mWidth;
	int maxY = mBottomRight.getY() - mHeight;

	// Not allow the people move to outside
	if (currX <= minX)
		currX = minX;
	if (currX >= maxX)
		currX = maxX;

	if (currY >= maxY)
		currY = maxY;
	if (currY <= minY)
		currY = minY;

	mCurrPos.setXY(currX, currY);
}

void CPEOPLE::Move(KEY direction, int delta) {
	switch (direction) {
	case KEY::UP:
		Up(delta);
		break;

	case KEY::DOWN:
		Down(delta);
		break;

	case KEY::LEFT:
		Left(mWidth);
		mCurrForm = &mPeopleLeftForm;
		break;

	case KEY::RIGHT:
		Right(mWidth);
		mCurrForm = &mPeopleRightForm;
		break;
	default:
		return;
	}

	mHeight = mCurrForm->Height();
	mWidth = mCurrForm->Width();

	Clip();
}

bool CPEOPLE::isImpact(const CLANE& Lane) {
	bool isImpact = false;

	for (int i = 0; i < Lane.size(); i++) {
		if (getX() + Width() >= Lane[i].getX() && getX() <= Lane[i].getX() + Lane[i].Width()) {
			isImpact = true;
			break;
		}
	}

	// If impact with object, people will die
	if (isImpact)
		mState = false;
	return isImpact;
}

void CPEOPLE::storeData(ofstream& ofs)
{
	ofs.write(reinterpret_cast<char*>(&mCurrPos), sizeof(CPOINT2D));

	bool state = false;
	if (mCurrForm == &mPeopleLeftForm) {
		state = true;
	}
	ofs.write(reinterpret_cast<char*>(&state), sizeof(bool));
}

void CPEOPLE::loadData(ifstream& ifs)
{
	ifs.read(reinterpret_cast<char*>(&mCurrPos), sizeof(CPOINT2D));

	bool state;
	ifs.read(reinterpret_cast<char*>(&state), sizeof(bool));

	mCurrForm = (state) ? &mPeopleLeftForm : &mPeopleRightForm;
}

bool  CPEOPLE::isFinish() const {
	if (mCurrPos.getY() <= mTopLeft.getY() + 1)
		return true;

	return false;
}

bool  CPEOPLE::isDead() const {
	return !mState;
}

void CPEOPLE::drawPeople(Console& console){
	int x = mCurrPos.getX();
	int y = mCurrPos.getY();

	console.DrawObject(x, y, *mCurrForm, PEOPLE_COLOUR);
}