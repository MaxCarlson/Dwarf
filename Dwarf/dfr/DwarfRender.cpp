#include "DwarfRender.h"
#include "Terminal.h"

#include <imgui.h>
#include <imgui-sfml.h>

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>

#include <memory>
#include <chrono>

namespace dfr
{
	std::unique_ptr<sf::RenderWindow> mainWindow;
	std::unique_ptr<Terminal> terminal;

	sf::RenderWindow * getWindow()
	{
		return dfr::mainWindow.get();
	}

	void init(config_r config)
	{
		
		Font * font = getFont(config.fontTag);

		mainWindow = std::make_unique<sf::RenderWindow>(sf::VideoMode(config.width * font->characterSize.first, config.height * font->characterSize.second), 
			config.title, sf::Style::Titlebar | sf::Style::Resize | sf::Style::Close);

		mainWindow->setVerticalSyncEnabled(true);

		sf::Vector2u sizePixel = mainWindow->getSize();
		
		terminal = std::make_unique<Terminal>(config.fontTag, 0, 0);
		terminal->resizePix(sizePixel.x, sizePixel.y);
	}

	void run(std::function<void(double)>& onTick)
	{
		resetMouseState();

		ImGui::SFML::Init(*mainWindow);

		sf::Clock deltaClock;
		while (mainWindow->isOpen())
		{
			sf::Event event;

			while (mainWindow->pollEvent(event))
			{
				ImGui::SFML::ProcessEvent(event);

				if (event.type == sf::Event::Closed)
					mainWindow->close();

				else if (event.type == sf::Event::Resized)
				{
					mainWindow->setView(sf::View(sf::FloatRect(0.f, 0.f, static_cast<float>(event.size.width), static_cast<float>(event.size.height))));

					terminal->resizePix(event.size.width, event.size.height);
				}

				else if (event.type == sf::Event::LostFocus)
					setWindowFocus(false);

				else if (event.type == sf::Event::GainedFocus)
					setWindowFocus(true);

				else if (event.type == sf::Event::MouseButtonPressed)
					setMouseState(event.mouseButton.button, true);

				else if (event.type == sf::Event::MouseButtonReleased)
					setMouseState(event.mouseButton.button, false);

				else if (event.type == sf::Event::MouseMoved)
					setMousePosition(event.mouseMove.x, event.mouseMove.y);

				else if (event.type == sf::Event::KeyPressed)
					keyPressed(event);

				else if (event.type == sf::Event::MouseWheelMoved)
				{
					if (event.mouseWheel.delta < 0)
						setMouseState(Button::WHEEL_UP, true);

					else if(event.mouseWheel.delta > 0)
						setMouseState(Button::WHEEL_DOWN, true);
				}
			}

			ImGui::SFML::Update(*mainWindow, deltaClock.restart());

			/*
			ImGui::Begin("Sample Window"); // Imugi example

			sf::Color bgColor;

			float color[3] = { 0.f, 0.f, 0.f };
			char windowTitle[255] = "ImGui + SFML = <3";

			if (ImGui::ColorEdit3("Background color", color)) {
				// this code gets called if color value changes, so
				// the background color is upgraded automatically!
				bgColor.r = static_cast<sf::Uint8>(color[0] * 255.f);
				bgColor.g = static_cast<sf::Uint8>(color[1] * 255.f);
				bgColor.b = static_cast<sf::Uint8>(color[2] * 255.f);
			}

			// Window title text edit
			ImGui::InputText("Window title", windowTitle, 255);

			if (ImGui::Button("Update window title")) {
				// this code gets if user clicks on the button
				// yes, you could have written if(ImGui::InputText(...))
				// but I do this to show how buttons work :)
				mainWindow->setTitle(windowTitle);
			}


			ImGui::End(); // end window
			*/

			mainWindow->clear();

			onTick(double{});


			terminal->render(*mainWindow);
			ImGui::SFML::Render(*mainWindow);

			mainWindow->display();
		}
	}
}


