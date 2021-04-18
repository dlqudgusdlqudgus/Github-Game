#include<bangtal.h>
#include<stdlib.h>
#include<time.h>
#include<stdio.h>

SceneID scene0, scene1, scene2;
ObjectID object1, gameStart, gameHow, back, showScore, gameTitle, restart, gamestop;
ObjectID obstacle;
TimerID timer1, timer2, gameScore;

int puX = 150, puY = 200, jcount = 0;
int obX = 1280, obY = 215;
int score = 0, puWalk = 1, obsType=1;

bool jumpT = false, gamePlay = false, puSlide = false;


void checkCrashed() {

	if (puSlide == true) {

		if ((obX >= puX +10  && obX <= puX + 115) && (obY >= puY + 10 && obY <= puY + 100)) {
			goto stop;
		}
		else if ((puX + 10 >= obX && puX + 10 <= obX + 30) && (puY + 10 >= obY && puY + 10 <= obY + 80)) {
			goto stop;
		}

	}
	else if (puSlide == false) {

		if ((obX >= puX + 8 && obX <= puX + 80) && (obY >= puY + 12 && obY <= puY + 120)) {
			goto stop;
		}
		else if ((puX + 8 >= obX && puX + 8 <= obX + 30) && (puY + 12 >= obY && puY + 12 <= obY + 80)) {
			goto stop;
		}

	}

	if (0) {
		stop:
		
		printf("!");

		gamePlay = false;
		showObject(restart);
		showObject(gamestop);
	}

}

void gameStartSetting() {

	jumpT = false;
	gamePlay = true;
	puSlide = false;
	puX = 150, puY = 200, jcount = 0;
	obX = 1280, obY = 215;
	score = 0, puWalk = 1, obsType = 1;

	enterScene(scene1);

	hideObject(restart);
	hideObject(gamestop);

	locateObject(obstacle, scene1, obX, obY);
	locateObject(object1, scene1, puX, puY);

	setTimer(gameScore, 0.0f);							//게임점수 초기화
	showTimer(gameScore);

	startTimer(timer1);									// 게임시작
	startTimer(timer2);

}

void locateObject(ObjectID object, SceneID scene, int x, int y, bool shown) {

	locateObject(object, scene, x, y);
	
	if (shown) {
		showObject(object);
	}
	else if (shown == false) {
		hideObject(object);
	}
}

void jump_move() {									// 점프

	if (jcount >= 0 && jcount < 12) {
		puY += 10;
		jcount++;
	}
	else if (jcount>=12 && jcount < 24) {
		puY -= 10;
		jcount++;
	}
	else if (jcount == 24) {
		jumpT = false;
		jcount = 0;
	}
}

void moveObstacle() {

	if (score < 3) {
		obX -= 10;
	}
	else if (score >= 3) {
		obX -= 15;
	}

	if (obsType == 1) {

		obY = 215;

		setObjectImage(obstacle, "obstacle1.png");
		locateObject(obstacle, scene1, obX, obY);

	}
	else if (obsType == 2) {

		obY = 310;

		setObjectImage(obstacle, "obstacle2.png");
		locateObject(obstacle, scene1, obX, obY);

	}
}

void mouseCallback(ObjectID object, int x, int y, MouseAction action) {

	if (object == gameStart || object == restart) {			//게임 시작 
		gameStartSetting();
	}
	else if (object == gameHow) {		//게임방법
		enterScene(scene2);
	}
	else if (object == back) {			//게임 방법에서 뒤로가기
		enterScene(scene0);
	}
	else if (object == gamestop) {		// 게임 끝
		endGame();
	}
}

void keyboardCallback(KeyCode key, KeyState state) {
	
	if (gamePlay == true) {
		if (key == KeyCode::KEY_SPACE && state == KeyState::KEY_PRESSED && puSlide == false) {
			jumpT = true;
		}

		if (key == KeyCode::KEY_DOWN_ARROW && state == KeyState::KEY_PRESSED && puSlide == false) {
			puSlide = true;
			setObjectImage(object1, "푸앙 누움.png");
		}
		else if (key == KeyCode::KEY_DOWN_ARROW && state == KeyState::KEY_RELEASED && puSlide == true) {
			puSlide = false;
			setObjectImage(object1, "푸앙21.png");
		}
	}
}

void timerCallBack(TimerID timer) {

	if (gamePlay == true) {
		if (timer == timer1) {

			moveObstacle();
			checkCrashed();

			if (obX <= 0) {
				increaseTimer(gameScore, 1.0f);

				obsType = rand() % 2 + 1;

				score++;
				obX = 1280;
			}

			if (puWalk == 1 && puSlide == false) {
				setObjectImage(object1, "푸앙22.png");
				puWalk = 2;
			}
			else if (puWalk == 2 && puSlide == false) {
				setObjectImage(object1, "푸앙21.png");
				puWalk = 1;
			}

			setTimer(timer1, 0.001f);
			startTimer(timer1);

			
		}

		if (timer == timer2) {
			if (jumpT) {
				jump_move();
				locateObject(object1, scene1, puX, puY);
			}

			setTimer(timer2, 0.01f);			//움직이는거 무한루프
			startTimer(timer2);
		}
	}
}

int main() {

	srand(time(NULL));

	setGameOption(GameOption::GAME_OPTION_INVENTORY_BUTTON, false);
	setGameOption(GameOption::GAME_OPTION_ROOM_TITLE, false);
	setGameOption(GameOption::GAME_OPTION_MESSAGE_BOX_BUTTON, false);


	setMouseCallback(mouseCallback);
	setTimerCallback(timerCallBack);
	setKeyboardCallback(keyboardCallback);

	scene0 = createScene("처음 화면", "배경.png");
	scene1 = createScene("게임 화면", "배경.png");
	scene2 = createScene("게임 방법", "게임방법2.jpg");

	object1 = createObject("푸앙21.png");
	obstacle = createObject("obstacle1.png");
	gameStart = createObject("게임시작.jpg");
	gameHow = createObject("게임방법.jpg");
	back = createObject("뒤로.jpg");
	showScore = createObject("점수.jpg");
	gameTitle = createObject("게임제목.jpg");
	restart = createObject("재시작.jpg");
	gamestop = createObject("게임멈춰.jpg");


	timer1 = createTimer(0.0001f);	
	timer2 = createTimer(0.001f);
	gameScore = createTimer(0.0f);
	
	locateObject(gameTitle, scene0, 240, 400, true);
	locateObject(gameStart, scene0, 575, 300, true);
	locateObject(gameHow, scene0, 575, 200, true);

	locateObject(object1, scene1, puX, puY, true);
	locateObject(obstacle, scene1, obX, obY, true);
	locateObject(showScore, scene1, 500, 630, true);
	locateObject(restart, scene1, 575, 200);
	locateObject(gamestop, scene1, 575, 100);

	locateObject(back, scene2, 0, 0, true);

	startGame(scene0);


}
