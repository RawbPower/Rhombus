#include "rbpch.h"
#include "WindowsWindow.h"

#include "Rhombus/Events/ApplicationEvent.h"
#include "Rhombus/Events/MouseEvent.h"
#include "Rhombus/Events/KeyEvent.h"

#include "Platform/OpenGL/OpenGLContext.h"

namespace rhombus {

	static bool s_GLFWInitialized = false;

	const int SCREEN_WIDTH = 640;
	const int SCREEN_HEIGHT = 480;

	static void GLFWErrorCallback(int error, const char* description)
	{
		RB_CORE_ERROR("GLFW Error ({0}): {1}", error, description);
	}

	Window* Window::Create(const WindowParams& params)
	{
		return new WindowsWindow(params);
	}

	WindowsWindow::WindowsWindow(const WindowParams& params)
	{
		Init(params);

		Init_SDL();
		Update_SDL();
	}

	WindowsWindow::~WindowsWindow()
	{
		Shutdown();
		Shutdown_SDL();
	}

	void WindowsWindow::Init(const WindowParams& params)
	{
		// This is a virtual function and calls virtual functions in the constructor.
		// Seems like a no no but this code will be gone soon and is deprecated
		m_Data.Title = params.Title;
		m_Data.Width = params.Width;
		m_Data.Height = params.Height;

		RB_CORE_INFO("Creating window {0} ({1}, {2})", params.Title, params.Width, params.Height);

		if (!s_GLFWInitialized)
		{
			// TODO: glfwTerminate on system shutdown
			int success = glfwInit();
			RB_CORE_ASSERT(success, "Could not initialize GLFW!");
			glfwSetErrorCallback(GLFWErrorCallback);
			s_GLFWInitialized = true;
		}

		m_Window = glfwCreateWindow((int)params.Width, (int)params.Height, m_Data.Title.c_str(), nullptr, nullptr);
		// This could be changed from OpenGL to DirectX or Vulcan in the future
		m_Context = new OpenGLContext(m_Window);
		m_Context->Init();

		// A pointer to our Windows data struct
		glfwSetWindowUserPointer(m_Window, &m_Data);
		SetVSync(true);
	}

	void WindowsWindow::Init_SDL()
	{
		m_SDLWindow = NULL;
		m_SDLScreenSurface = NULL;
		m_SDLHelloWorld = NULL;

		//Initialize SDL
		if (SDL_Init(SDL_INIT_VIDEO) < 0)
		{
			RB_CORE_ERROR("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
		}
		else
		{
			//Create window
			m_SDLWindow = SDL_CreateWindow(m_Data.Title.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
			if (m_SDLWindow == NULL)
			{
				RB_CORE_ERROR("Window could not be created! SDL_Error: %s\n", SDL_GetError());
			}
			else
			{
				//Get window surface
				m_SDLScreenSurface = SDL_GetWindowSurface(m_SDLWindow);
			}
		}
	}

	void WindowsWindow::Shutdown()
	{
		glfwDestroyWindow(m_Window);
	}

	void WindowsWindow::Shutdown_SDL()
	{
		//Deallocate surface
		SDL_FreeSurface(m_SDLHelloWorld);
		m_SDLHelloWorld = NULL;

		//Destroy window
		SDL_DestroyWindow(m_SDLWindow);
		m_SDLWindow = NULL;

		//Quit SDL subsystems
		SDL_Quit();
	}

	void WindowsWindow::OnUpdate()
	{
		glfwPollEvents();
		m_Context->SwapBuffers();

		HandleEvents_SDL();

		//Apply the image
		SDL_BlitSurface(m_SDLHelloWorld, NULL, m_SDLScreenSurface, NULL);

		//Update the surface
		SDL_UpdateWindowSurface(m_SDLWindow);
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

	void WindowsWindow::Update_SDL()
	{
		//Load splash image
		m_SDLHelloWorld = SDL_LoadBMP("../Sandbox/assets/textures/hello_world.bmp");
		if (m_SDLHelloWorld == NULL)
		{
			RB_CORE_ERROR("Unable to load image %s! SDL Error: %s\n", "hello_world.bmp", SDL_GetError());
		}
	}

	void WindowsWindow::HandleEvents_SDL()
	{
		//Event handler
		SDL_Event e;

		//Handle events on queue
		while (SDL_PollEvent(&e) != 0)
		{
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
						/*float width = e.window.data1;
						float height = e.window.data2;
						m_Data.Width = width;
						m_Data.Height = height;

						WindowResizeEvent event(width, height);
						RB_CORE_WARN("{0}, {1}", width, height);
						// Dispatch it
						m_Data.EventCallback(event);*/
					}
					break;
				}
				case SDL_KEYDOWN:
				{
					KeyPressedEvent event(e.key.keysym.sym, 0);
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
				default:
					break;
			}
		}
	}

	void WindowsWindow::SetVSync(bool enabled)
	{
		if (enabled)
			glfwSwapInterval(1);
		else
			glfwSwapInterval(0);

		m_Data.VSync = enabled;
	}

	bool WindowsWindow::IsVSync() const
	{
		return m_Data.VSync;
	}
}