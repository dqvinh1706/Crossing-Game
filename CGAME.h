#ifndef CGAME_H
#define CGAME_H

#include "CCenterMenu.h"
#include "CScoreBoard.h"
#include "CAlienShip.h"
#include "CPEOPLE.h"
#include "CLANE.h"

class CGAME {
private:
	Console* mConsole;

	// Game nenu
	CCenterMenu mMainMenu;
	CCenterMenu mSettingMenu;

	// Score
	CScoreBoard* mScoreBoard;

	// Enemies obj
	vector<CLANE> mLaneOfEnemies;

	CAlienShip mAlienShip;

	// Player
	CPEOPLE mPeople;

	// Playing area setting
	CPOINT2D mTopLeft;
	CPOINT2D mBottomRight;

	// Render game thread
	thread mRenderGame;

	// Music thread
	thread mMusicThread;

	// Game state
	bool isPlaying; // True when player is playing
	bool isPause; // True when player pause game
	bool isPlayed; // True when player played game
	bool isTurnOnMusic;

	short mLevel;
	short mMaxEnemies;
	short mMinEnemies;

	unsigned int mScore;
	unsigned int mHightScore;

	// Default game setting 
	vector<pair<float, ENEMY>> mLanes = {
		{0.0f,ENEMY::NO_ENEMY},
		{-2.0f,ENEMY::CDOG},
		{1.0f,ENEMY::CBIRD},
		{-1.0f,ENEMY::CDOG},
		{2.0f,ENEMY::CBIRD},
		{0.0f,ENEMY::NO_ENEMY},
		{-1.0f,ENEMY::CCAR},
		{1.0f,ENEMY::CTRUCK},
		{-1.0f,ENEMY::CCAR},
		{2.0f,ENEMY::CTRUCK},
		{-1.0f,ENEMY::CCAR},
		{0.0f,ENEMY::NO_ENEMY},
	};
private:	
	CGAME(); // Chuẩn bị dữ liệu cho tất cả các đối tượng
	CGAME(const CGAME&);
	CGAME& operator=(const CGAME&);

	// Post process
	void setSettingMenu();
	void setMainMenu();
	void setPlayingArea(float = 1.0f, float = 1.0f);
	void setScoreBoard();
	void setObjects();
	void setPeople();
	void setAlienShip();

	// Draw part
	void drawPlayingArea(); // Vẽ khu vực chơi
	void drawEnemies(); // Vẽ xe
	void drawGame(); // Thực hiện vẽ trò chơi ra màn hình sau khi có dữ liệu

	// Game flow
	void musicThread();
	void renderWhenPlayerDie();
	void renderGameThread(KEY*);

	void turnOnMusic();
	void turnOffMusic();
	void toggleMusic();

	void resetGame(); // Thực hiện thiết lập lại toàn bộ dữ liệu như lúc đầu
	void nextLevel();
	void playGame(); // Trong qua trinh choi

	void pauseGame(); // Tạm dừng Thread
	void resumeGame(); // Quay lai Thread
	void goBackMainMenu();
	void exitGame(); // Thực hiện thoát Thread

	void updatePosPeople(KEY); // Thực hiện điều khiển di chuyển của CPEOPLE
	void updatePosEnemies(); // Thực hiện cho tất cả các xe va thú di chuyển

	void StartGame(); // Thực hiện bắt đầu vào trò chơi
public:
	static CGAME* getGame();
	~CGAME(); // Hủy tài nguyên đã cấp phát

	// Getter
	unsigned int getLevel() const;
	unsigned int getScore() const;
	unsigned int getHighScore() const;
	string getHelp() const;
	Console* getConsole();
	CPEOPLE getPeople() const; // Lấy thông tin người
	CLANE getEnemyLane(); // Lấy danh sách các đồ vật trên 1 lane

	// Save and load game
	void loadGame(); // Thực hiện tải lại trò chơi đã lưu
	void saveGame(); // Thực hiện lưu lại dữ liệu trò chơi
	void Run();
};

#endif // CGAME_H
