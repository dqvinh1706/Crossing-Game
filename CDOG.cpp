#include "CDOG.h"

CDOG::CDOG() {
	mCurrPos = CPOINT2D(0, 0);

	mAnimalLeftForm = "  __      _\n"
					"o'')}____//\n"
					" `_/      )\n"
					" (_(_/-(_/\n";

	mAnimalRightForm = "_      __  \n"
					"\\\____{(''o\n"
					"(       \\_' \n"
					"\\_)-\\\_)_)\n";

	mHeight = mAnimalLeftForm.Height();
	mWidth = mAnimalLeftForm.Width();

	mCurrAnimalForm = &mAnimalLeftForm;
	mAnimalColour = DOG_COLOUR;
}

CDOG::CDOG(const CDOG& other) {
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

CANIMAL* CDOG::Clone() {
	return new CDOG(*this);
}

void CDOG::Tell()
{
	PlaySound(TEXT("Dog.wav"), NULL, SND_FILENAME);
}
