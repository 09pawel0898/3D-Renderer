#include "../pch.h"
#include "./RenderingState.h"
#include "../Engine/ResourceManager.h"
#include "../App.h"


namespace States
{
	RenderingState::RenderingState(StateManager &stateManager, Context context) :
		State(stateManager, context)	
	{
		init_resources();
	}

	RenderingState::~RenderingState(void)
	{
	}

	void RenderingState::init_resources(void)
	{
		//context.mTextures->load_resource(Textures::ID::GRASS, "assets/img/grass.png");
		//context.mTextures->get_resource(Textures::ID::B_READY);

	}

	void RenderingState::render(void)
	{
		static sf::RenderWindow* window = get_context().mWindow;
		//window->draw(spr);
		//window->draw(*mWorld);	
	}

	bool RenderingState::update_scene(sf::Time deltaTime)
	{	
		static vec2i prevPointedCoord = vec2i();

		mMousePos = sf::Mouse::getPosition(*get_context().mWindow);
		bool cursorInWindow = (mMousePos.x > 0 &&  mMousePos.x < App::WIDTH*64 && mMousePos.y > 0 && mMousePos.y < App::HEIGHT*64) ? true : false;
		
		return true;
	}

	bool RenderingState::handle_event(const sf::Event &event)
	{
		static bool enable = true;

		if (event.type == sf::Event::MouseButtonReleased)
		{
			
		}
		else if (event.type == sf::Event::KeyPressed)
		{
			enable = false;
			switch (event.key.code)
			{
				case sf::Keyboard::Space: 
					//mWorld->change_path_visibility();
					break;
				
			}
		}
		else if (event.type == sf::Event::KeyReleased)
		{
			enable = true;
			mEndPointPositioning = false;
		}
		return true;
	}

}