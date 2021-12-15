#include "CGAME.h"

CGAME::CGAME()  {
	isPause = false;
	isPlaying = false;
	isPlayed = false;
	mConsole = Console::getConsole(WINDOW_BUFFER_WIDTH, WINDOW_BUFFER_HEIGHT, FONT_WIDTH, FONT_HEIGHT);
	
	mLevel = 1;
	mMinEnemies = 1;
	mMaxEnemies = mMinEnemies;

	mScore = 0;
	mHightScore = 0;

	// Set obj
	setMainMenu();
	setSettingMenu();
	setPlayingArea(SCALE_X, SCALE_Y);
	setScoreBoard();
	setObjects();
	setPeople();
	setAlienShip();
}

CGAME* CGAME::getGame() {
	static CGAME mInstance;
	return &mInstance;
}

CGAME::~CGAME() {

}

void CGAME::setSettingMenu() {
	Texture gameTitle = \
		" ######  ######## ######## ######## #### ##    ##  ######\n"
		"##    ## ##          ##       ##     ##  ###   ## ##    ##\n"
		"##       ##          ##       ##     ##  ####  ## ##\n"
		" ######  ######      ##       ##     ##  ## ## ## ##   ####\n"
		"      ## ##          ##       ##     ##  ##  #### ##    ##\n"
		"##    ## ##          ##       ##     ##  ##   ### ##    ##\n"
		" ######  ########    ##       ##    #### ##    ##  ######";

	mSettingMenu.setMenuTitle(gameTitle, COLOUR::PINK);
	mSettingMenu.setMarginTop(4);
	mSettingMenu.addOption("Turn Off Music", bind(&CGAME::toggleMusic, this));
	mSettingMenu.addOption("Back to main menu", bind(&CCenterMenu::QuitMenu, &mSettingMenu));
}

void CGAME::setMainMenu() {
	Texture gameTitle = \
		" ######  ########   #######   ######   ######  #### ##    ##  ######      ########   #######     ###    ########\n"
		"##    ## ##     ## ##     ## ##    ## ##    ##  ##  ###   ## ##    ##     ##     ## ##     ##   ## ##   ##     ##\n"
		"##       ##     ## ##     ## ##       ##        ##  ####  ## ##           ##     ## ##     ##  ##   ##  ##     ##\n"
		"##       ########  ##     ##  ######   ######   ##  ## ## ## ##   ####    ########  ##     ## ##     ## ##     ##\n"
		"##       ##   ##   ##     ##       ##       ##  ##  ##  #### ##    ##     ##   ##   ##     ## ######### ##     ##\n"
		"##    ## ##    ##  ##     ## ##    ## ##    ##  ##  ##   ### ##    ##     ##    ##  ##     ## ##     ## ##     ##\n"
		" ######  ##     ##  #######   ######   ######  #### ##    ##  ######      ##     ##  #######  ##     ## ########\n";
	
	mMainMenu.setMenuTitle(gameTitle);
	mMainMenu.setMarginTop(4);
	mMainMenu.addOption("New Game", bind(&CGAME::StartGame, this));
	mMainMenu.addOption("Load Game", bind(&CGAME::loadGame, this));
	mMainMenu.addOption("Setting", bind(&CCenterMenu::Run, &mSettingMenu, ref(*mConsole)));
	mMainMenu.addOption("Quit", bind(&CGAME::exitGame, this));
}

void CGAME::setPlayingArea(float scaleX, float scaleY) {
	// Set the size of the playing area
	// 0,0 is the coord of top left
	int playingAreaWidth = mConsole->Width() * scaleX;
	int playingAreaHeight = mConsole->Height() * scaleY;

	// Set the top left point of the playing area
	mTopLeft = CPOINT2D(TOP_LEFT_X, TOP_LEFT_Y);
	mBottomRight = CPOINT2D(TOP_LEFT_X + playingAreaWidth - 1, TOP_LEFT_Y + playingAreaHeight - 1);
}

void CGAME::setScoreBoard()
{
	CPOINT2D topLeftCoord = CPOINT2D(mBottomRight.getX() + 1, mTopLeft.getY());
	int playingAreaWidth = mBottomRight.getX() - mTopLeft.getX() + 1;

	int scoreBoardWidth = mConsole->Width() - playingAreaWidth;
	int scoreBoarHeight = mConsole->Height() * SCALE_Y;

	mScoreBoard = CScoreBoard::getScoreBoard();
	mScoreBoard->setPosTopLeftCorner(topLeftCoord);
	mScoreBoard->resize(scoreBoardWidth, scoreBoarHeight);
}

void CGAME::setObjects() {
	int left = mTopLeft.getX();
	int right = mBottomRight.getX();
	int y = mTopLeft.getY() + 1;

	for (const auto& LaneInfo : mLanes) {
		int randomQty = RandomInt(mMaxEnemies, mMinEnemies);
		CLANE mLane(left, right);
		mLane.setY(y);
		mLane.setSpeed(LaneInfo.first);
		mLane.generateObjectsOnLane(LaneInfo.second, randomQty);
		switch (LaneInfo.second)		
		{
		case ENEMY::CCAR:
		case ENEMY::CTRUCK:
			mLane.enableTrafficLight();
		default:
			break;
		}

		if(mLane.size() != 0)
			mLaneOfEnemies.push_back(mLane);
		y += LANE_SIZE;
	}
}

void CGAME::setPeople() {
	// Set the position of people
	// Generate randomly X by multiplying a floating number between 0.2 to 0.8
	float randomPercent = (float) RandomInt(8, 2) / 10.0f;
	int x = mBottomRight.getX() - mTopLeft.getX();
	int y = mBottomRight.getY() - mPeople.Height();

	// X is random, Y must at the end of the last square
	x = x * randomPercent;
	mPeople = CPEOPLE(x, y, mTopLeft, mBottomRight);
	mPeople.setState(true);
}

void CGAME::setAlienShip()
{
	int alienPosX = RandomInt(mBottomRight.getX(), mTopLeft.getY() + 1);
	int alienPosY = RandomInt(mTopLeft.getY(), mTopLeft.getY() - 10);
	mAlienShip.reset();
	mAlienShip.setXY(alienPosX, alienPosY);
	mAlienShip.setPeople(&mPeople);
}

void CGAME::drawPlayingArea() {
	// Draw the border line
	mConsole->DrawBorder(mTopLeft, mBottomRight, PLAYING_AREA_COLOUR);
}

void  CGAME::drawEnemies() {
	for (auto& Lane : mLaneOfEnemies) {
		Lane.drawObjectsOnLane(*mConsole);
	}
}

void CGAME::drawGame() {
	// Draw border
	drawPlayingArea();
	mScoreBoard->drawScoreBoard(this);

	if (!mPeople.isDead()) {
		// Draw people
		mPeople.drawPeople(*mConsole);

		// Draw object
		drawEnemies();
	}
}

void CGAME::musicThread()
{
	vector<CANIMAL*> listAnimalSound = { new CDOG(), new CBIRD() };
	bool isRunningBgMusic = false;

	while (isTurnOnMusic) {
		if (!isRunningBgMusic) {
			PlaySound(TEXT("BgMusic.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
		}
		isRunningBgMusic = true;

		if (!mPeople.isFinish()) {
			if (!mPeople.isDead()) {
				if (!isPause) {
				/*	int randomAnimalIndex = RandomInt(listAnimalSound.size() - 1, 0);
					listAnimalSound[randomAnimalIndex]->Tell();

					isRunningBgMusic = false;*/
				}
			}
			else {
				if (mAlienShip.isFlyAway() == false) {
					mAlienShip.Tell();
					isRunningBgMusic = false;
				}
			}
		}
		else {
			PlaySound(TEXT("upSound.wav"), NULL, SND_FILENAME);
			isRunningBgMusic = false;
		}
	}

	for (auto& animal : listAnimalSound) {
		delete animal;
		animal = nullptr;
	}
	listAnimalSound.clear();
}

void CGAME::renderWhenPlayerDie() {
	while (true) {
		mConsole->ClearScreen();

		if (!mAlienShip.isReachPeople()) {
			mAlienShip.reachPeople();
			// Draw people
			mPeople.drawPeople(*mConsole);
		}
		else {
			if (!mAlienShip.isCapturePeople()) {
				mAlienShip.capturePeople();
			}
			else {
				mAlienShip.flyAway(mTopLeft.getY());

				if (mAlienShip.isFlyAway()) {
					break;
				}
			}
		}

		// Draw border
		drawPlayingArea();
		mScoreBoard->drawScoreBoard(this);
	
		mAlienShip.drawToConsole(*mConsole, mTopLeft.getX(), mBottomRight.getX());
		
		mConsole->Render();
		Sleep(30);
	}
}

void CGAME::renderGameThread(KEY* MOVING) {
	while (isPlaying) {
		// Clear the old screen
		mConsole->ClearScreen();

		if (!isPause) {
			if (!mPeople.isDead()) {
				updatePosPeople(*MOVING);
				updatePosEnemies();

				if (mPeople.isImpact(getEnemyLane())) {
					mPeople.Dead();
					thread t = thread(&CGAME::renderWhenPlayerDie, this);
					t.join();
				}

				if (mPeople.isFinish()) {
					nextLevel();
				}
			}
			else {
				Texture deadText = \
					"               You are DEAD                 \n"
					"Press Y to play again. Press any key to exit";
				mConsole->DrawObject(30, 25, deadText, COLOUR::RED);
			}
		}
		
		*MOVING = KEY::SPACE;
		drawGame();

		if (isPause) {
			Texture deadText = \
				"      Game is PAUSE\n"
				"Press any key to continue";
			mConsole->DrawObject(45, 25, deadText, COLOUR::RED);
		}
		
		// Render out console
		mConsole->Render();
		Sleep(30);
	}
}

void CGAME::turnOnMusic()
{
	isTurnOnMusic = true;
	mMusicThread = thread(&CGAME::musicThread, this);
}

void CGAME::turnOffMusic()
{
	isTurnOnMusic = false;
	mMusicThread.join();
	PlaySound(NULL, 0, 0);
}

void CGAME::toggleMusic()
{
	if (isTurnOnMusic) {
		mSettingMenu.setOption(0, "Turn On Music");
		turnOffMusic();
	}
	else {
		mSettingMenu.setOption(0, "Turn Off Music");
		turnOnMusic();
	}
}

void CGAME::resetGame() {
	mLaneOfEnemies.clear();

	mLevel = 1;
	mMinEnemies = 1;
	mMaxEnemies = mMinEnemies;

	mScore = 0;
	
	setObjects();
	setPeople();
	setAlienShip();
}

void CGAME::nextLevel() {
	if (mLevel == 3){
		mLevel = 1;
		mMinEnemies = 1;
		mMaxEnemies = mMinEnemies;
	}
	else {
		mLevel++;
		mMinEnemies++;
		mMaxEnemies = mMinEnemies + 1;
	}
	
	mLaneOfEnemies.clear();
	
	setObjects();
	setPeople();
	mScore += 100;
	mHightScore += 100;
}

void CGAME::playGame() {
	isPlaying = true;
	isPause = false;
	isPlayed = true;

	KEY MOVING = KEY::SPACE;
	KEY temp;

	mRenderGame = thread(&CGAME::renderGameThread, this, &MOVING);

	while (isPlaying) {
		if (!getPeople().isDead()) {
			if (_kbhit()) {
				temp = (KEY)toupper(_getch());
				switch (temp)
				{
				case KEY::SAVE_GAME:
					pauseGame();
					saveGame();
					resumeGame();
					break;
				case KEY::LOAD_GAME:
					pauseGame();
					loadGame();
					resumeGame();
					break;
				case KEY::PAUSE_GAME:
					pauseGame();
					break;
				case KEY::ESC:
					goBackMainMenu();
					return;
				default:
					resumeGame();
					MOVING = (KEY)temp;
					break;
				}
			}
		}
		else {
			// Check if the alien ship has fly out console screen
			if (mAlienShip.isFlyAway()) {
				if (_kbhit()) {
					// Catch the key press
					temp = (KEY)toupper(_getch());

					// Check the key
					if (temp == KEY::YES) {
						resetGame();
					}
					else {
						goBackMainMenu();
						return;
					}
				}
			}
		}
	}
}

void CGAME::pauseGame() {
	isPause = true;
}

void CGAME::resumeGame() {
	isPause = false;
}

void CGAME::goBackMainMenu()
{
	if (isPlaying) {
		if (mPeople.isDead()) {
			isPause = false;
			isPlayed = false;
			resetGame();
			mMainMenu.removeOption("Continue");
		}
		isPlaying = false;
		mRenderGame.join();
	}
}

void CGAME::exitGame() {
	if (isTurnOnMusic)
		turnOffMusic();

	mMainMenu.QuitMenu();
}

void CGAME::updatePosPeople(KEY direction) {
	mPeople.Move(direction, LANE_SIZE);
}

void CGAME::updatePosEnemies() {
	for (auto& Lane : mLaneOfEnemies) {
		Lane.updateObjectsOnLane();
	}
}

void CGAME::StartGame() {
	if (isPlayed) {
		int result = MessageBox(
			NULL,
			L"You will lost all the unsaved process.\nContine to create new game???", // Text in msg box
			L"New Game", // Msg box title
			MB_YESNO | MB_ICONWARNING | MB_TOPMOST | MB_APPLMODAL // Mode ask yes no
		);

		if (result == IDYES) {
			resetGame();
			playGame();
		}
	}else {
		if (mMainMenu.isHasOption("Continue") == false)
			mMainMenu.insertOption(1, "Continue", bind(&CGAME::playGame, this));
		playGame();
	}
}

CPEOPLE CGAME::getPeople() const {
	return mPeople;
}

CLANE CGAME::getEnemyLane()
{
	for (const auto& Lane : mLaneOfEnemies) {
		if (Lane.getY() == mPeople.getY())
			return Lane;
	}
	return CLANE();
}

unsigned int CGAME::getLevel() const
{
	return mLevel;
}

unsigned int CGAME::getScore() const
{
	return mScore;
}

unsigned int CGAME::getHighScore() const {
	return mHightScore;

}

string CGAME::getHelp() const
{
	string helpText = \
		"Press W,S,D,A to move player\n"
		"Press L to load game\n"
		"Press T to save game\n"
		"Press P to pause game\n"
		"Press ESC to go back main menu\n";

	return helpText;
}

Console* CGAME::getConsole()
{
	return mConsole;
}

void CGAME::loadGame() {
	string filePath = mConsole->getFilePathToLoad();
	ifstream ifs(filePath.c_str(), ios::binary | ios::out);
	if (!ifs)
		return;
	
	// Load file save here
	ifs.read(reinterpret_cast<char*> (&isPlaying), sizeof(bool));
	ifs.read(reinterpret_cast<char*> (&isPause), sizeof(bool));
	ifs.read(reinterpret_cast<char*> (&isPlayed), sizeof(bool));

	if (isPlayed) {
		if (mMainMenu.isHasOption("Continue") == false)
			mMainMenu.insertOption(1, "Continue", bind(&CGAME::playGame, this));
	}

	ifs.read(reinterpret_cast<char*> (&mScore), sizeof(unsigned int));
	ifs.read(reinterpret_cast<char*> (&mHightScore), sizeof(unsigned int));
	ifs.read(reinterpret_cast<char*> (&mLevel), sizeof(short));
	ifs.read(reinterpret_cast<char*> (&mMaxEnemies), sizeof(short));
	ifs.read(reinterpret_cast<char*> (&mMinEnemies), sizeof(short));

	mPeople.loadData(ifs);

	for (auto& Lane : mLaneOfEnemies) {
		Lane.loadData(ifs);
	}
}

void CGAME::saveGame() {
	string filePath = mConsole->getFilePathToSave();
	ofstream ofs(filePath.c_str(),ios::binary | ios::out);
	if (!ofs)
		return;

	// Save game here
	ofs.write(reinterpret_cast<char*> (&isPlaying), sizeof(bool));
	ofs.write(reinterpret_cast<char*> (&isPause), sizeof(bool));
	ofs.write(reinterpret_cast<char*> (&isPlayed), sizeof(bool));

	ofs.write(reinterpret_cast<char*> (&mScore), sizeof(unsigned int));
	ofs.write(reinterpret_cast<char*> (&mHightScore), sizeof(unsigned int));
	ofs.write(reinterpret_cast<char*> (&mLevel), sizeof(short));
	ofs.write(reinterpret_cast<char*> (&mMaxEnemies), sizeof(short));
	ofs.write(reinterpret_cast<char*> (&mMinEnemies), sizeof(short));


	mPeople.storeData(ofs);

	for (auto& Lane : mLaneOfEnemies) {
		Lane.storeData(ofs);
	}
}


void CGAME::Run() {
	turnOnMusic();
	mMainMenu.Run(*mConsole);
}
