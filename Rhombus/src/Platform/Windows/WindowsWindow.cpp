#include "rbpch.h"
#include "WindowsWindow.h"

#include "Rhombus/Core/Application.h"
#include "Rhombus/Core/Input.h"
#include "Rhombus/Events/ApplicationEvent.h"
#include "Rhombus/Events/MouseEvent.h"
#include "Rhombus/Events/KeyEvent.h"
#include "Rhombus/Events/GamepadEvent.h"

#include "Rhombus/Renderer/Renderer.h"

#include "imgui.h"
#include "backends/imgui_impl_sdl2.h"

#include "Platform/OpenGL/OpenGLContext.h"

namespace rhombus 
{
	Window* Window::Create(const WindowParams& params)
	{
		return new WindowsWindow(params);
	}

	WindowsWindow::WindowsWindow(const WindowParams& params)
	{
		//RB_PROFILE_FUNCTION();
		Init(params);
	}

	WindowsWindow::~WindowsWindow()
	{
		RB_PROFILE_FUNCTION();

		Shutdown();
	}

	void WindowsWindow::Init(const WindowParams& params)
	{
		RB_PROFILE_FUNCTION();

		// This is a virtual function and calls virtual functions in the constructor.
		// Seems like a no no but this code will be gone soon and is deprecated
		m_Data.Title = params.Title;
		m_Data.Width = params.Width;
		m_Data.Height = params.Height;

		RB_CORE_INFO("Creating window {0} ({1}, {2})", params.Title, params.Width, params.Height);

		m_Window = NULL;
		m_IconSurface = NULL;

		//Initialize SDL
		bool sdlFail = false;
		{
			RB_PROFILE_SCOPE("SDL_Init");
			sdlFail = SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK) < 0;
		}
		if (sdlFail)
		{
			RB_CORE_ERROR("SDL could not initialize! SDL_Error: {0}\n", SDL_GetError());
		}
		else
		{
			// SDL_GL_LoadLibrary(NULL);
			
			//Use OpenGL 3.1 core
			SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
			SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
			SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
			SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
			// Also request a depth buffer
			SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
			SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
			SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
			SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
			//SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 0);
			//SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);

			#if defined(RB_DEBUG)
				if (Renderer::GetAPI() == RendererAPI::API::OpenGL)
					SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_DEBUG_FLAG);
			#endif

			// Initialize joystick is present
			m_Joystick = nullptr;
			if (SDL_NumJoysticks() > 0)
			{
				SDL_JoystickEventState(SDL_ENABLE);
				m_Joystick = SDL_JoystickOpen(0);
			}

			//Create window
			{
				RB_PROFILE_SCOPE("SDL_CreateWindow");
				uint32_t flags = SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE;
				if (params.Fullscreen)
				{
					flags |= SDL_WINDOW_MAXIMIZED;
				}
				m_Window = SDL_CreateWindow(m_Data.Title.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, m_Data.Width, m_Data.Height, flags);
				//m_Window = SDL_CreateWindow(m_Data.Title.c_str(), SDL_WINDOWPOS_CENTERED_DISPLAY(1), SDL_WINDOWPOS_CENTERED_DISPLAY(1), m_Data.Width, m_Data.Height, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
			}
			//m_SDLWindow = SDL_CreateWindow(m_Data.Title.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, m_Data.Width, m_Data.Height, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
			if (m_Window == NULL)
			{
				RB_CORE_ERROR("Window could not be created! SDL_Error: {0}\n", SDL_GetError());
			}
			else
			{
				m_IconSurface = SDL_LoadBMP(Application::Get().GetPathRelativeToEngineDirectory("resources/icons/RhombusIconBlueLarge.bmp").c_str());
				if (m_IconSurface == NULL)
				{
					RB_CORE_ERROR("Unable to load image {0}! SDL Error: {1}\n", "Resourcse/Icons/RhombusIconBlueLarge.bmp", SDL_GetError());
				}
				else
				{
					SDL_SetWindowIcon(m_Window, m_IconSurface);
				}

				m_Context = new OpenGLContext(m_Window);
				m_Context->Init();
			}
		}
	}

	void WindowsWindow::Shutdown()
	{
		RB_PROFILE_FUNCTION();

		//Deallocate surface
		SDL_FreeSurface(m_IconSurface);
		m_IconSurface = NULL;

		//Destroy window
		SDL_DestroyWindow(m_Window);
		m_Window = NULL;

		// Destroy joystick
		SDL_JoystickClose(m_Joystick);
		m_Joystick = NULL;

		//Quit SDL subsystems
		SDL_Quit();
	}

	void WindowsWindow::OnUpdate()
	{
		RB_PROFILE_FUNCTION();

		m_Context->SwapBuffers();

		HandleEvents();

		//Apply the image
		//SDL_BlitSurface(m_SDLHelloWorld, NULL, m_SDLScreenSurface, NULL);

		//Update the surface
		//SDL_UpdateWindowSurface(m_SDLWindow);
	}

	SDL_Surface* loadSurface(std::string path)
	{
		//Load image at specified path
		SDL_Surface* loadedSurface = SDL_LoadBMP(path.c_str());
		if (loadedSurface == NULL)
		{
			printf("Unable to load image %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
		}

		return loadedSurface;
	}

	void WindowsWindow::HandleEvents()
	{
		//Event handler
		SDL_Event e;

		//Handle events on queue
		while (SDL_PollEvent(&e) != 0)
		{
			ImGui_ImplSDL2_ProcessEvent(&e);
			switch (e.type)
			{
				//User requests quit
				case SDL_QUIT:
				{
					WindowCloseEvent event;
					m_Data.EventCallback(event);
					break;
				}
				case SDL_WINDOWEVENT:
				{
					if (e.window.event == SDL_WINDOWEVENT_SIZE_CHANGED)
					{
						// Don't call this until we update the rendering context
						int width = e.window.data1;
						int height = e.window.data2;
						m_Data.Width = width;
						m_Data.Height = height;

						WindowResizeEvent event(width, height);
						RB_CORE_INFO("{0}, {1}", width, height);
						// Dispatch it
						m_Data.EventCallback(event);
					}
					else if (e.window.event == SDL_WINDOWEVENT_MOVED)
					{
						WindowMovedEvent event;
						RB_CORE_INFO("Window Moved");
						m_Data.EventCallback(event);
					}
					break;
				}
				case SDL_KEYDOWN:
				{
					KeyPressedEvent event(e.key.keysym.sym, e.key.repeat);
					m_Data.EventCallback(event);
					break;
				}
				case SDL_KEYUP:
				{
					KeyReleasedEvent event(e.key.keysym.sym);
					m_Data.EventCallback(event);
					break;
				}
				case SDL_TEXTINPUT:
				{
					KeyTypedEvent event(std::string(e.text.text));
					m_Data.EventCallback(event);
					break;
				}
				case SDL_MOUSEBUTTONDOWN:
				{
					MouseButtonPressedEvent event(e.button.button);
					m_Data.EventCallback(event);
					break;
				}
				case SDL_MOUSEBUTTONUP:
				{
					MouseButtonReleasedEvent event(e.button.button);
					m_Data.EventCallback(event);
					break;
				}
				case SDL_MOUSEWHEEL:
				{
					MouseScrolledEvent event(e.wheel.x, e.wheel.y);
					m_Data.EventCallback(event);
					break;
				}
				case SDL_MOUSEMOTION:
				{
					MouseMovedEvent event(e.motion.x, e.motion.y);
					m_Data.EventCallback(event);
					break;
				}
				case SDL_JOYAXISMOTION:
				{
					if ((e.jaxis.value < -3200) || (e.jaxis.value > 3200))
					{
						float valueNorm = e.jaxis.value > 0 ? ((float)e.jaxis.value) / SDL_JOYSTICK_AXIS_MAX : ((float)e.jaxis.value) / SDL_JOYSTICK_AXIS_MIN;
						GamepadAxisEvent event(e.jaxis.axis, valueNorm);
						m_Data.EventCallback(event);
					}
					break;
				}
				case SDL_JOYBUTTONDOWN:
				{
					GamepadButtonDownEvent event(e.jbutton.button);
					m_Data.EventCallback(event);
					break;
				}
				default:
					break;
			}
		}
	}

	bool WindowsWindow::IsVSync() const
	{
		return m_Data.VSync;
	}
}