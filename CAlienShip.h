#ifndef CALIEN_SHIP_H
#define CALIEN_SHIP_H

#include "CVEHICLE.h"
#include "CPEOPLE.h"

class CAlienShip : public CVEHICLE
{
private:
	CPEOPLE* mPeople;
	unsigned int mSpeed;
	bool mIsReachPeople;
	bool mIsCapture;
	bool mIsFlyAway;
	bool mPlaySound;
public:
	CAlienShip();
	CAlienShip(const CAlienShip&);
	~CAlienShip();
	virtual CVEHICLE* Clone();

	void setPeople(CPEOPLE*);

	bool isReachPeople() const;
	bool isCapturePeople() const;
	bool isFlyAway() const;

	void reset();
	void reachPeople();
	void capturePeople();
	void flyAway(int);
	void Tell();
};

#endif // !CALIEN_SHIP_H


