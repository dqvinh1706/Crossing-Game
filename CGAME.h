﻿#include "CPEOPLE.h"
#include "CTRUCK.h"

using namespace std;

#ifndef CGAME_H
#define CGAME_H

// Singleton Design Pattern

class CGAME {
	static CGAME* m_pGame;

	// Game object
	CVEHICLE* m_pAllVehicles;
	CTRUCK* m_pTrucks;

	// Player
	CPEOPLE mPeople;

	// Console Buffer
	int mWindowHeight, mWindowWidth;

	// Playing area setting
	CPOINT2D mTopLeft, mBottomRight;
	int mPlayingAreaHeight, mPlayingAreaWdth;

	// Road lane
	int mNumberOfLane;
	int gapBetween2Lane;
	vector<short>lanePosY;
	vector<bool>isHasObject;

	int mNumberOfVehicles;

	CGAME(); // Chuẩn bị dữ liệu cho tất cả các đối tượng
public:
	~CGAME(); // Hủy tài nguyên đã cấp phát
	static CGAME* getGame();

	// Post process
	void setRandomPostionTrucks(const int&);
	void setPeople();
	void setAllVehicles();
	void setPlayingArea();

	CPEOPLE getPeople() const; // Lấy thông tin người
	CVEHICLE* getVehicle(); // Lấy danh sách các xe
	CANIMAL* getAnimal(); // Lấy danh sách các thú

	void drawLane() const; // Vẽ lane đường
	void drawPlayingArea() const; // Vẽ khu vực chơi
	void drawVehicles() const; // Vẽ xe
	void drawAnimals(); // Vẽ thú
	void drawGame() const; // Thực hiện vẽ trò chơi ra màn hình sau khi có dữ liệu

	void resetGame(); // Thực hiện thiết lập lại toàn bộ dữ liệu như lúc đầu
	void startGame(); // Thực hiện bắt đầu vào trò chơi

	void loadGame(istream); // Thực hiện tải lại trò chơi đã lưu
	void saveGame(istream); // Thực hiện lưu lại dữ liệu trò chơi

	void pauseGame(HANDLE) const; // Tạm dừng Thread
	void resumeGame(HANDLE) const; // Quay lai Thread
	void exitGame(HANDLE); // Thực hiện thoát Thread

	void updatePosPeople(DIRECTION); // Thực hiện điều khiển di chuyển của CPEOPLE
	void updatePosVehicle(); // Thực hiện cho CTRUCK & CCAR di chuyển
	void updatePosAnimal(); // Thực hiện cho CDINAUSOR & CBIRD di chuyển
};

#endif // CGAME_H
