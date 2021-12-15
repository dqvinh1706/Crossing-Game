#include "CAlienShip.h"

CAlienShip::CAlienShip()
{
	mCurrPos = CPOINT2D(0, 0);

	mVehicleLeftForm = \
				"     ___\n"
				" ___/   \\___\n"
				"/   '---'   \\\n"
				"'--_______--'";
	mVehicleRightForm =  \
				"     ___\n"
				" ___/   \\___\n"
				"/   '---'   \\\n"
				"'--_______--'";

	mHeight = mVehicleRightForm.Height();
	mWidth = mVehicleRightForm.Width();

	mCurrVehicleForm = &mVehicleRightForm;
	mVehicleColour = TRUCK_COLOUR;

	mSpeed = 3;
	mIsReachPeople = false;
	mIsCapture = false;
	mIsFlyAway = false;
	mPeople = nullptr;
}

CAlienShip::CAlienShip(const CAlienShip& other)
{
	// Current position
	mCurrPos = other.mCurrPos;

	// Width and height of vehicle
	mHeight = other.mHeight;
	mWidth = other.mWidth;

	// Vehicle form
	mVehicleLeftForm = other.mVehicleLeftForm;
	mVehicleRightForm = other.mVehicleRightForm;

	// Assign pointer
	if (other.mCurrVehicleForm == &other.mVehicleLeftForm) {
		mCurrVehicleForm = &mVehicleLeftForm;
	}
	else {
		mCurrVehicleForm = &mVehicleRightForm;
	}

	// Color for vehicle
	mVehicleColour = other.mVehicleColour;
	mIsReachPeople = other.mIsReachPeople;
	mIsCapture = other.mIsCapture;
	mPeople = other.mPeople;
}

CAlienShip::~CAlienShip() {}

CVEHICLE* CAlienShip::Clone()
{
	return new CAlienShip(*this);
}

void CAlienShip::setPeople(CPEOPLE* people)
{
	mPeople = people;
}

bool CAlienShip::isReachPeople() const {
	return mIsReachPeople;
}

bool CAlienShip::isCapturePeople() const
{
	return mIsCapture;
}

bool CAlienShip::isFlyAway() const
{
	return mIsFlyAway;
}

void CAlienShip::reset()
{
	mIsReachPeople = false;
	mIsCapture = false;
	mIsFlyAway = false;
}

void CAlienShip::reachPeople()
{
	int desX = mPeople->getX() + (mPeople->Width() / 2);
	int desY = mPeople->getY();

	int srcX = getX() + (mWidth / 2);
	int srcY = getY() + mHeight;

	// Calc the angle base on two point
	double angle = atan2(desY - srcY, desX - srcX);

	double cosValue = cos(angle) * mSpeed;
	double sinValue = sin(angle) * mSpeed;
	
	srcX += cosValue;
	srcY += sinValue;
	mCurrPos.moveXY(cosValue, sinValue);

	CPOINT2D srcPoint(srcX, srcY);
	if (srcPoint.Distance(mPeople->getPos()) <= mSpeed) {
		setXY(desX - (mWidth / 2), desY - mHeight);
		mIsReachPeople = true;
	}
}

void CAlienShip::capturePeople()
{
	if (getY() + mHeight == mPeople->getY() + mPeople->Height()) {
		mIsCapture = true;
	}
	Move(0, 1);
}

void CAlienShip::flyAway(int y)
{
	if (getY() + mHeight < y)
		mIsFlyAway = true;
	int speed = mSpeed;
	Move(0, -speed);
}

void CAlienShip::Tell()
{
	PlaySound(TEXT("AlienShip.wav"), NULL, SND_FILENAME | SND_ASYNC);
	while (isFlyAway() == false) {}
	PlaySound(NULL, 0, 0);
}

