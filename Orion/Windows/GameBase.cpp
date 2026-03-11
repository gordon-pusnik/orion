#include "stdafx.h"
#include <GL/glew.h>
#include "GameBase.h"
#include "Game.h"

Game* game = NULL;

GameBase::GameBase()
{
	m_bGameOver = 0;
	m_ulTimer = 0;

	game = new Game();
}

void GameBase::init(int w, int h)
{
	GLenum err = glewInit();
	if (GLEW_OK == err) {
		if (game) {
			game->init((float) w, (float) h);
		}
	}
	else {
		delete game;
		game = NULL;
	}

	return;
}

GameBase::~GameBase()
{
	if (game) {
		delete game;
	}
}

void GameBase::mouseDown(int x, int y)
{
	if (game) {
		game->touchesBegan(x, y);
	}
}

void GameBase::mouseUp(int x, int y)
{
	if (game) {
		game->touchesEnded(x, y);
	}
}

void GameBase::mouseDrag(int x, int y)
{
	if (game) {
		game->touchesMoved(x, y);
	}
}

void GameBase::processMouseInput(int button, int state, int x, int y)
{
	// Add methods to act on mouse press

	return;
}

void GameBase::processKeyPress(unsigned char key, int speckey)
{
	if (key) {
//		game->ProcessKeyPress(key);
	}
	else {
		switch (speckey) {
		default:
			break;
		}
	}

	return;
}

void GameBase::keyDown(unsigned char key)
{
	if (game) {
		game->keyDown((short)key);
	}
}

void GameBase::keyUp(unsigned char key)
{
	if (game) {
		game->keyUp((short)key);
	}
}

void GameBase::run(unsigned long timer)
{
	if (game) {
		m_ulTimer = timer;
		game->update((long) m_ulTimer);

//	if (game->m_bGameOver) 
//		m_bGameOver = 1;

	}

	return;
}

void GameBase::display(void)
{
	if (game) {
		game->render();
	}

	return;
}

void GameBase::reshape(int w, int h) 
{
}