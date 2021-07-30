#include "../pch.h"
#include "./RenderingState.h"
#include "../Engine/ResourceManager.h"
#include "../App.h"
#include "../Engine/LabelHolder.h"
#include "Camera.h"
#include "Object.h"
#include "Mesh.h"

sf::VertexArray States::RenderingState::Vertices = sf::VertexArray();
Matrix4x4 States::RenderingState::ProjectionMatrix = Matrix4x4();

unsigned States::RenderingState::ClippedPolygons = 0;
unsigned States::RenderingState::DrawnPolygons = 0;
unsigned States::RenderingState::AllPolygons = 0;
bool States::RenderingState::ClippingVisible = false;
bool States::RenderingState::MeshVisible = false;

namespace States
{
	RenderingState::RenderingState(StateManager &stateManager, Context context) 
		:	State(stateManager, context),	
			mLabelHolder(std::make_unique<LabelHolder>()),
			mCamera(std::make_shared<Camera>(Vec3f(0.0f, 0.0f, -10.0f))),
			mObject(std::make_unique<Object>(mCamera))
	{
		math::init_projection_matrix(ProjectionMatrix,App::HEIGHT, App::WIDTH);
		init_labels();
		init_resources();

		mObject->mMesh->load_obj("teapot.obj");
		Vertices.setPrimitiveType(sf::Triangles);
		Vertices.resize(mObject->mMesh->mTrisCount * 3);
	}

	RenderingState::~RenderingState(void)
	{
	}

	void RenderingState::init_labels(void)
	{
		mLabelHolder->add_label(LabelHolder::ID::INFO,vec2f(10, 10),sf::Text(	" E - change rotation | Space - color | Q - Clipping visible",
																				get_context().mFonts->get_resource(Fonts::ID::SANSATION),20));
		mLabelHolder->add_label(LabelHolder::ID::NUM_CLIPPED_TRIS, vec2f(10, 30),	sf::Text(" Clipped polygons : ",
																				get_context().mFonts->get_resource(Fonts::ID::SANSATION),20));
		mLabelHolder->add_label(LabelHolder::ID::NUM_DRAWN_TRIS, vec2f(10, 50), sf::Text(" Drawn polygons : ",
																				get_context().mFonts->get_resource(Fonts::ID::SANSATION),20));
		mLabelHolder->add_label(LabelHolder::ID::NUM_MESH_POLYGONS, vec2f(10, 70),	sf::Text(" Mesh polygons : ",
																				get_context().mFonts->get_resource(Fonts::ID::SANSATION),20));
	}

	void RenderingState::init_resources(void)
	{}

	void RenderingState::render(void)
	{
		static sf::RenderWindow* window = get_context().mWindow;
		window->draw(*mLabelHolder);
		//mObject->mMesh->update_rotation(RotationType::NONE, App::DT.asSeconds());
		window->draw(*mObject);
	}

	bool RenderingState::update_scene(sf::Time deltaTime)
	{	
		//MousePos = sf::Mouse::getPosition();

		//auto cursorInWindow = [this]()
		//{ return  (MousePos.x > 0 && MousePos.x < App::WIDTH&& MousePos.y > 0 && MousePos.y < App::HEIGHT) ? true : false; };
		
		mLabelHolder->get_label(LabelHolder::ID::NUM_CLIPPED_TRIS).setString(" Clipped polygons : " + std::to_string(ClippedPolygons));
		mLabelHolder->get_label(LabelHolder::ID::NUM_DRAWN_TRIS).setString(" Drawn polygons : " + std::to_string(DrawnPolygons));
		mLabelHolder->get_label(LabelHolder::ID::NUM_MESH_POLYGONS).setString(" Mesh polygons : " + std::to_string(AllPolygons));

		return true;
	}

	bool RenderingState::handle_event(const sf::Event &event)
	{
		static bool keyPressed = false;
		static sf::Keyboard::Key lastPressedKey = sf::Keyboard::Unknown;

		mCamera->handle_camera_event(lastPressedKey, keyPressed);

		switch (event.type)
		{
			case sf::Event::KeyPressed: 
				keyPressed = true;
				lastPressedKey = event.key.code;

				switch (event.key.code)
				{
					case sf::Keyboard::Space: switch_mesh_status(); break;
					//case sf::Keyboard::E: nextRotationType(); break;
					case sf::Keyboard::Q: switch_clipping_hint_status();  break;
				}
				break;
			case sf::Event::KeyReleased: keyPressed = false; break;
		}
		return true;
	}

}