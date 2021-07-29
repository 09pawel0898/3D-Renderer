#pragma once

#include "./Engine/ResourceManager.h"
#include "./Engine/StateManager.h"

using StateManager = States::StateManager;

class App : public sf::NonCopyable
{
private:
	static const sf::Time TIME_PER_FRAME;

public:
	static constexpr int WIDTH = 1280;
	static constexpr int HEIGHT = 720;
	static vec2i WindowCenter;
	static sf::Time DT;

private:
	sf::RenderWindow* mWindow;
	TextureManager mTextures;
	FontManager mFonts;
	StateManager* mStateManager;
	sf::Text mFpsLabel;
	std::size_t FPS;

private:
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
	~App(void);
	void run(void);
};