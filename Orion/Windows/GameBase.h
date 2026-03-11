class GameBase
{
public:
	GameBase();
	~GameBase();

	void			processMouseInput(int button, int state, int x, int y);
	void			processKeyPress(unsigned char key, int speckey);
	void			keyDown(unsigned char key);
	void			keyUp(unsigned char key);
	void			mouseDown(int x, int y);
	void			mouseUp(int x, int y);
	void			mouseDrag(int x, int y);
	void			init(int w, int h);
	void			reshape(int w, int h);
	void			run(unsigned long timer);
	void			display(void);

	unsigned long	m_ulTimer;
	bool			m_bGameOver;

private:
};