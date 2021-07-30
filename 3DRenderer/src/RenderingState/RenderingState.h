#pragma once

class LabelHolder;
class Camera;
class Object;

#include "../Engine/State.h"
#include "../Math/Matrix4x4.h"

using Matrix4x4 = math::Matrix4x4;

namespace States
{
	class RenderingState : public State
	{
	public:
		// temporary (will be moved to renderer clas) //
		static sf::VertexArray Vertices;

		static Matrix4x4 ProjectionMatrix;
		static unsigned ClippedPolygons;
		static unsigned DrawnPolygons;
		static unsigned AllPolygons;
		static bool ClippingVisible;
		static bool MeshVisible;
	private:
		std::unique_ptr<LabelHolder> mLabelHolder;
		std::shared_ptr<Camera> mCamera;
		std::unique_ptr<Object> mObject;

	private:
		void init_labels(void);
		void init_resources(void);
	
	public:
		RenderingState(StateManager &stateManager, Context context);
		virtual ~RenderingState(void);

		virtual void render(void) override;
		virtual bool update_scene(sf::Time deltaTime) override;
		virtual bool handle_event(const sf::Event &event) override;

		void switch_mesh_status(void);
		void switch_clipping_hint_status(void);
	};
	
	inline void RenderingState::switch_mesh_status(void)
	{
		MeshVisible = MeshVisible ? false : true;
	}

	inline void RenderingState::switch_clipping_hint_status(void)
	{
		ClippingVisible = ClippingVisible ? false : true;
	}

}