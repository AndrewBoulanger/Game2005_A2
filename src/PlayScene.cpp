#include "PlayScene.h"
#include "Game.h"
#include "EventManager.h"

// required for IMGUI
#include "imgui.h"
#include "imgui_sdl.h"
#include "Renderer.h"

PlayScene::PlayScene()
{
	PlayScene::start();
}

PlayScene::~PlayScene()
= default;

void PlayScene::draw()
{
	TextureManager::Instance()->draw("background", -300, 0, 1500, 600, 0, 255, false);
	if(EventManager::Instance().isIMGUIActive())
	{
		GUI_Function();
	}

	SDL_RenderDrawLineF(Renderer::Instance()->getRenderer(), m_trianglePos.x, m_trianglePos.y, m_trianglePos.x, m_trianglePos.y - m_rise);
	SDL_RenderDrawLineF(Renderer::Instance()->getRenderer(), m_trianglePos.x, m_trianglePos.y, m_trianglePos.x + m_run, m_trianglePos.y);
	SDL_RenderDrawLineF(Renderer::Instance()->getRenderer(), m_trianglePos.x, m_trianglePos.y - m_rise, m_trianglePos.x + m_run, m_trianglePos.y);

	drawDisplayList();
	SDL_SetRenderDrawColor(Renderer::Instance()->getRenderer(),0,0,0,0);
	
}

void PlayScene::update()
{
	if (m_pLootbox->getTransform()->position.x >= m_trianglePos.x + m_run)
	{
		m_pLootbox->getRigidBody()->velocity.y = 0;
		m_pLootbox->getRigidBody()->acceleration.y = 0;
		m_pLootbox->getRigidBody()->acceleration.x = -m_pLootbox->getFriction();
		m_pLootbox->SetAngle(0.0f);
	
	}
	updateDisplayList();
}

void PlayScene::clean()
{
	removeAllChildren();
}

void PlayScene::handleEvents()
{
	EventManager::Instance().update();

	// handle player movement with GameController
	if (SDL_NumJoysticks() > 0)
	{
		if (EventManager::Instance().getGameController(0) != nullptr)
		{
			const auto deadZone = 10000;
			if (EventManager::Instance().getGameController(0)->LEFT_STICK_X > deadZone)
			{
				
			}
			else if (EventManager::Instance().getGameController(0)->LEFT_STICK_X < -deadZone)
			{
			
			}
			else
			{
			
			}
		}
	}


	// handle player movement if no Game Controllers found
	if (SDL_NumJoysticks() < 1)
	{
		if (EventManager::Instance().isKeyDown(SDL_SCANCODE_A))
		{
		
		}
		else if (EventManager::Instance().isKeyDown(SDL_SCANCODE_D))
		{
		
		}
		else
		{
		
		}
	}
	

	if (EventManager::Instance().isKeyDown(SDL_SCANCODE_ESCAPE))
	{
		TheGame::Instance()->quit();
	}

	if (EventManager::Instance().isKeyDown(SDL_SCANCODE_1))
	{
		TheGame::Instance()->changeSceneState(START_SCENE);
	}

	if (EventManager::Instance().isKeyDown(SDL_SCANCODE_2))
	{
		TheGame::Instance()->changeSceneState(END_SCENE);
	}
}

void PlayScene::start()
{
	TextureManager::Instance()->load("../Assets/textures/background.jpg", "background");
	// Set GUI Title
	m_guiTitle = "Play Scene";
	
	m_trianglePos.x = 100;
	m_trianglePos.y = 450;
	m_rise =300;
	m_run = 400;

	m_pLootbox = new Box();
	m_pLootbox->getTransform()->position = glm::vec2(m_trianglePos.x, m_trianglePos.y - m_rise);
	m_pLootbox->setWidth(50);
	m_pLootbox->setHeight(54);
	addChild(m_pLootbox);
	
	// Back Button
	m_pBackButton = new Button("../Assets/textures/backButton.png", "backButton", BACK_BUTTON);
	m_pBackButton->getTransform()->position = glm::vec2(300.0f, 500.0f);
	m_pBackButton->addEventListener(CLICK, [&]()-> void
	{
		m_pBackButton->setActive(false);
		TheGame::Instance()->changeSceneState(START_SCENE);
	});

	m_pBackButton->addEventListener(MOUSE_OVER, [&]()->void
	{
		m_pBackButton->setAlpha(128);
	});

	m_pBackButton->addEventListener(MOUSE_OUT, [&]()->void
	{
		m_pBackButton->setAlpha(255);
	});
	addChild(m_pBackButton);

	// Next Button
	m_pNextButton = new Button("../Assets/textures/nextButton.png", "nextButton", NEXT_BUTTON);
	m_pNextButton->getTransform()->position = glm::vec2(500.0f, 500.0f);
	m_pNextButton->addEventListener(CLICK, [&]()-> void
	{
		m_pNextButton->setActive(false);
		TheGame::Instance()->changeSceneState(END_SCENE);
	});

	m_pNextButton->addEventListener(MOUSE_OVER, [&]()->void
	{
		m_pNextButton->setAlpha(128);
	});

	m_pNextButton->addEventListener(MOUSE_OUT, [&]()->void
	{
		m_pNextButton->setAlpha(255);
	});

	addChild(m_pNextButton);

	/* Instructions Label */
	m_pInstructionsLabel = new Label("Press the backtick (`) character to toggle Debug View", "Consolas");
	m_pInstructionsLabel->getTransform()->position = glm::vec2(Config::SCREEN_WIDTH * 0.5f, 550.0f);

	addChild(m_pInstructionsLabel);
}

void PlayScene::GUI_Function() const
{
	
	// Always open with a NewFrame
	ImGui::NewFrame();

	// See examples by uncommenting the following - also look at imgui_demo.cpp in the IMGUI filter
	//ImGui::ShowDemoWindow();
	
	ImGui::Begin("Your Window Title Goes Here", NULL, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_MenuBar);

	if(ImGui::Button("activate"))
	{
		if (!m_pLootbox->IsActive())
		{
			m_pLootbox->toggleActive();
			m_pLootbox->setDiretion(glm::normalize(glm::vec2(m_run, m_rise)));
		}
		else
		{
			m_pLootbox->reset(m_trianglePos.x, m_trianglePos.y - m_rise);
		}
	}

	ImGui::Separator();

	static float height = 300;
	if (ImGui::SliderFloat("Height", &height, 1, 500)) {
		(float)m_rise = height;
		m_pLootbox->reset(m_trianglePos.x, m_trianglePos.y - m_rise);
	}
	static float length = 400;
	if (ImGui::SliderFloat("Length", &length, 1, 500)) {
		(float)m_run = length;
		m_pLootbox->reset(m_trianglePos.x, m_trianglePos.y - m_rise);
	}
	
	
	ImGui::End();

	// Don't Remove this
	ImGui::Render();
	ImGuiSDL::Render(ImGui::GetDrawData());
	ImGui::StyleColorsDark();
}
