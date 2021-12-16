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
	void enableTrafficLight(); // Bật đèn xanh đèn đỏ
	void disableTrafficLight(); // Tắt đèn xanh đèn đỏ
	void storeData(ofstream&); // Lưu đối tương CLANE
	void loadData(ifstream&); // Tải đối tượng CLANE
	// Hàm tạo các đối tượng bên trong CLANE bằng loại và số lượng
	void generateObjectsOnLane(const ENEMY&, int); 
	void updateObjectsOnLane(); // Hàm update vị trí các đối tượng trên CLANE
	void drawTrafficLight(Console&); // Hàm vẽ đèn xanh đèn đỏ
	// Hàm vẽ các đối tưởng trên 1 CLANE bao gồm đèn xanh đèn đỏ
	void drawObjectsOnLane(Console&); 
};

#endif // CLANE_H
