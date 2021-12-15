#ifndef CLANE_H
#define CLANE_H

#include "HelpFunctions.h"
#include "CEnemyFactory.h"

#include "TrafficLightGreenState.h"
#include "TrafficLightRedState.h"

class CLANE {
private:
	int mY;
	int mSpeedOnLane;
	int mLeftLimit;
	int mRightLimit;

	std::vector<Movable*> mObjects;

	bool mEnabledTrafficLight;

	ITrafficLightState* mGreenLight;
	ITrafficLightState* mRedLight;
	ITrafficLightState* mCurrLightState;
public:
	CLANE();
	CLANE(int, int);
	CLANE(const CLANE&);
	~CLANE();

	// Operator overloading
	CLANE& operator= (const CLANE&);
	Movable& operator[](int);
	const Movable& operator[](int) const;

	// Setter
	void setY(int);
	void setLimit(int, int);
	void setSpeed(int);
	void setTrafficLightState(ITrafficLightState*);

	// Getter
	int getY() const;
	int getLimitLeft();
	int getSpeed() const;
	int size() const;
	ITrafficLightState* getTrafficLightState();
	ITrafficLightState* getRedLightState();
	ITrafficLightState* getGreenLightState();

	// Method
	void enableTrafficLight();
	void disableTrafficLight();
	void storeData(ofstream&);
	void loadData(ifstream&);
	void generateObjectsOnLane(const ENEMY&, int);
	void updateObjectsOnLane();
	void drawTrafficLight(Console&);
	void drawObjectsOnLane(Console&);
};


#endif // CLANE_H
