#include"CBIRD.h"

CBIRD::CBIRD() {
	mCurrPos = CPOINT2D(0, 0);

	mAnimalLeftForm = \
		" ,_ \n"
		">' )\n"
		"( ( \\\n"
		"''|\\";

	mAnimalRightForm = \
		"  _. \n"
		" ( '<\n"
		"/ ) )\n"
		" /|''";

	mHeight = mAnimalLeftForm.Height();
	mWidth = mAnimalLeftForm.Width();

	mCurrAnimalForm = &mAnimalLeftForm;

	mAnimalColour = BIRD_COLOUR;
}

CBIRD::CBIRD(const CBIRD& other) {
	// Current position
	mCurrPos = other.mCurrPos;

	// Width and height of animal
	mHeight = other.mHeight;
	mWidth = other.mWidth;

	// Animal form
	mAnimalLeftForm = other.mAnimalLeftForm;
	mAnimalRightForm = other.mAnimalRightForm;

	// Assign pointer
	if (other.mCurrAnimalForm == &other.mAnimalLeftForm) {
		mCurrAnimalForm = &mAnimalLeftForm;
	}
	else {
		mCurrAnimalForm = &mAnimalRightForm;
	}

	// Color for animal
	mAnimalColour = other.mAnimalColour;
}

CANIMAL* CBIRD::Clone() {
	return new CBIRD(*this);
}

void CBIRD::Tell()
{
	PlaySound(TEXT("Bird.wav"), NULL, SND_FILENAME | SND_SYNC);
}
