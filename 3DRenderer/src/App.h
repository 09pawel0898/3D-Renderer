#pragma once

#include "./Engine/ResourceManager.h"
#include "./Engine/StateManager.h"

using StateManager = States::StateManager;

class App : public sf::NonCopyable
{
private:
	static const sf::Time TIME_PER_FRAME;

public:
	static int WIDTH;
	static int HEIGHT;

private:

	sf::RenderWindow* mWindow;
	TextureManager mTextures;
	FontManager mFonts;
	StateManager* mStateManager;

private:
	sf::Text mFpsLabel;
	std::size_t FPS;

	void update_statistics(sf::Time elapsedTime);
	void register_states(void);

	void render(void);
	void process_events(void);
	void update_scene(sf::Time elapsedTime);

	void init_fonts(void);
	void init_window_settings(void);
	void init_labels(void);
public:
	App(void);
	void run(void);
};