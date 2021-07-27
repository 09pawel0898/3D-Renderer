#pragma once

#include "../Engine/State.h"


namespace States
{
	class RenderingState : public State
	{
	private:
		void init_resources(void);
		vec2i mMousePos;
		bool mEndPointPositioning = false;
	public:
		RenderingState(StateManager &stateManager, Context context);
		virtual ~RenderingState(void);

		virtual void render(void) override;
		virtual bool update_scene(sf::Time deltaTime) override;
		virtual bool handle_event(const sf::Event &event) override;
	};
}