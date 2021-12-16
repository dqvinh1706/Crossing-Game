#ifndef CANIMAL_H
#define CANIMAL_H

#include "Console.h"
#include "Movable.h"

class CANIMAL : public Movable{
protected:
	// Current position
	CPOINT2D mCurrPos;

	// Width and height of vehicle
	int mHeight;
	int mWidth;

	// Animal form
	Texture mAnimalLeftForm;
	Texture mAnimalRightForm;
	Texture* mCurrAnimalForm;

	COLOUR mAnimalColour;
public:
	CANIMAL() = default;
	virtual ~CANIMAL() = default;
	virtual CANIMAL* Clone() = 0;

	// Setter
	void setX(int);
	void setY(int);
	void setXY(int, int);
	void setColour(COLOUR);

	// Getter
	int getX() const;
	int getY() const;
	int Width() const;
	int Height() const;

	// Update method
	void toggleForm(int);
	void Move(int, int);

	// Save load 
	void storeData(ofstream&);
	void loadData(ifstream&);

	// Sound
	virtual void Tell() = 0;

	// Render method
	void drawToConsole(Console&, int, int);
};

#endif // CANIMAL_H
