#include "pch.h"
#include "App.h"
#include "Utility.h"
#include "./Engine/State.h"
#include "./Engine/StateIdentifiers.h"
#include "./RenderingState/RenderingState.h"
#include "./Engine/ResourceIdentifiers.h"


const sf::Time App::TIME_PER_FRAME = sf::seconds(1.0f / 60.0f);
int App::HEIGHT = 720;
int App::WIDTH = 1280;

App::App(void) 
	:	mTextures(),
		mFonts(),
		mFpsLabel(),
		FPS(0)
{
	mWindow = new sf::RenderWindow(sf::VideoMode(WIDTH, HEIGHT), "AStarSimulation", sf::Style::Titlebar | sf::Style::Close);
	mStateManager = new StateManager(States::State::Context(mWindow, &mTextures, &mFonts));

	init_fonts();
	init_window_settings();
	init_labels();
	register_states();
}

void App::render(void)
{
	mWindow->clear();
	mStateManager->render();
	mWindow->draw(mFpsLabel);
	mWindow->display();
}

void App::process_events(void)
{
	static sf::Event event;

	while (mWindow->pollEvent(event))
	{
		mStateManager->handle_event(event);

		if (event.type == sf::Event::Closed)
			mWindow->close();
	}
}

void App::update_scene(sf::Time deltaTime)
{
	mStateManager->update_scene(deltaTime);
}

void App::update_statistics(sf::Time elapsedTime)
{
	static sf::Time statisticsUpdateTime = sf::seconds(0);

	statisticsUpdateTime += elapsedTime;
	FPS++;

	if (statisticsUpdateTime >= sf::seconds(1.0f))
	{
		mFpsLabel.setString("FPS = " + Utility::to_string(FPS) + "\n");
		statisticsUpdateTime -= sf::seconds(1.0f);
		FPS = 0;
	}
}

void App::register_states(void)
{
	// registering all provided states 
	mStateManager->register_state<States::RenderingState>(States::ID::RENDERING);
	// initialize default state
	mStateManager->add_state(States::ID::RENDERING);	
}

void App::run(void)
{
	sf::Clock clock;
	sf::Time timeSinceLastUpdate = sf::Time::Zero;

	while (mWindow->isOpen())
	{
		sf::Time elapsedTime = clock.restart();
		timeSinceLastUpdate += elapsedTime;

		while (timeSinceLastUpdate > TIME_PER_FRAME)
		{
			timeSinceLastUpdate -= TIME_PER_FRAME;

			process_events();
			update_scene(TIME_PER_FRAME);
			if (mStateManager->is_empty())
				mWindow->close();
		}

		update_statistics(elapsedTime);
		render();
	}
}

void App::init_fonts(void)
{
	mFonts.load_resource(Fonts::ID::SANSATION, "assets/fonts/Sansation.ttf");
}

void App::init_window_settings(void)
{
	mWindow->setKeyRepeatEnabled(false);
	//mWindow->setFramerateLimit(100);
}

void App::init_labels(void)
{
	mFpsLabel.setFont(mFonts.get_resource(Fonts::ID::SANSATION));
	mFpsLabel.setPosition(5.0f, 5.0f);
	mFpsLabel.setCharacterSize(11);
	mFpsLabel.setFillColor(sf::Color::White);
}