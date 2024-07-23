#include <stdexcept>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

#include "Renderer.h"
#include "SceneManager.h"
#include "Texture.h"
#include "Font.h"
#include "imgui.h"
#include "backends/imgui_impl_sdl2.h"
#include "backends/imgui_impl_sdlrenderer2.h"
#include "implot.h"
#include "Engine.h"

using namespace Minigin;

class Renderer::Impl
{
public:
	explicit Impl();
	~Impl() = default;

	Impl(const Impl& other) = delete;
	Impl(Impl&& other) noexcept = delete;
	Impl& operator=(const Impl& other) = delete;
	Impl& operator=(Impl&& other) noexcept = delete;

	void Render() const;
	void Destroy();
	Texture* CreateTexture(const std::filesystem::path& path) const;
	Texture* CreateTexture(Font* font, const std::string& text);
	void RenderTexture(const Texture& texture, const Transform& transform) const;

private:
	SDL_Renderer* m_Renderer;
	SDL_Window* m_Window;
	SDL_Color m_ClearColor;
	ImGuiContext* m_ImGuiContext;
	ImPlotContext* m_ImPlotContext;

	int GetDriverIndex() const;

};

Renderer::Impl::Impl() :
	m_Renderer{},
	m_Window{ Engine::GetWindow() },
	m_ClearColor{},
	m_ImGuiContext{},
	m_ImPlotContext{}
{
	m_Renderer = SDL_CreateRenderer(m_Window, GetDriverIndex(), SDL_RENDERER_ACCELERATED);
	if (m_Renderer == nullptr)
	{
		throw std::runtime_error(std::string("SDL_CreateRenderer Error: ") + SDL_GetError());

	}

	m_ImGuiContext = ImGui::CreateContext();
	m_ImPlotContext = ImPlot::CreateContext();
	ImGui_ImplSDL2_InitForSDLRenderer(m_Window, m_Renderer);
	ImGui_ImplSDLRenderer2_Init(m_Renderer);
}

void Renderer::Impl::Render() const
{
	const auto& color = m_ClearColor;
	SDL_SetRenderDrawColor(m_Renderer, color.r, color.g, color.b, color.a);
	SDL_RenderClear(m_Renderer);

	ImGui_ImplSDL2_NewFrame();
	ImGui_ImplSDLRenderer2_NewFrame();
	ImGui::NewFrame();

	SceneManager::GetInstance().Render();
	ImGui::Render();
	ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData(), m_Renderer);

	SDL_RenderPresent(m_Renderer);
}

void Renderer::Impl::Destroy()
{
	ImGui_ImplSDLRenderer2_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImPlot::DestroyContext(m_ImPlotContext);
	ImGui::DestroyContext(m_ImGuiContext);

	if (m_Renderer != nullptr)
	{
		SDL_DestroyRenderer(m_Renderer);
		m_Renderer = nullptr;
	}
}

Texture* Renderer::Impl::CreateTexture(const std::filesystem::path& path) const
{
	SDL_Texture* texture{ IMG_LoadTexture(m_Renderer, path.generic_string().c_str()) };
	if (texture == nullptr) throw std::runtime_error(std::string("Renderer::Impl::CreateTexture() - ") + SDL_GetError());

	return new Texture{ texture };
}

Texture* Renderer::Impl::CreateTexture(Font* font, const std::string& text)
{
	const SDL_Color color{ 255,255,255,255 }; // only white text is supported now
	SDL_Surface* surface{ TTF_RenderText_Blended(font->GetFont(), text.c_str(), color) };
	if (surface == nullptr)	
	{
		throw std::runtime_error(std::string("Renderer::Impl::CreateTexture() - ") + SDL_GetError());
	}
	SDL_Texture* texture{ SDL_CreateTextureFromSurface(m_Renderer, surface) };	
	if (texture == nullptr)
	{
		throw std::runtime_error(std::string("Renderer::Impl::CreateTexture() - ") + SDL_GetError());
	}
	SDL_FreeSurface(surface);		

	return new Texture{ texture };	
}


void Renderer::Impl::RenderTexture(const Texture& texture, const Transform& transform) const
{
	SDL_Rect dst{};
	dst.x = static_cast<int>(transform.GetPosition().x);	
	dst.y = static_cast<int>(transform.GetPosition().y);

	SDL_QueryTexture(texture.GetTexture(), nullptr, nullptr, &dst.w, &dst.h);
	SDL_RenderCopy(m_Renderer, texture.GetTexture(), nullptr, &dst);
}

//void Renderer::Impl::RenderTexture(const Texture& texture, float x, float y, float width, float height) const
//{
//	SDL_Rect dst{};
//	dst.x = static_cast<int>(x);
//	dst.y = static_cast<int>(y);
//	dst.w = static_cast<int>(width);
//	dst.h = static_cast<int>(height);
//	SDL_RenderCopy(m_Renderer, texture.GetTexture(), nullptr, &dst);
//}

int Renderer::Impl::GetDriverIndex() const
{
	int index{ -1 };
	for (auto i = 0; i < SDL_GetNumRenderDrivers(); i++)
	{
		SDL_RendererInfo info{};
		if (!SDL_GetRenderDriverInfo(i, &info))
		{
			if (!strcmp(info.name, "opengl"))
			{
				index = i;
			}
		}
	}

	return index;
}

Renderer::Renderer() :
	m_Pimpl{ std::make_unique<Renderer::Impl>() }
{

}

Renderer::~Renderer() = default;

void Renderer::Render() const
{
	m_Pimpl->Render();
}

void Renderer::Destroy()
{
	m_Pimpl->Destroy();
}

Texture* Renderer::CreateTexture(const std::filesystem::path& path) const
{
	return m_Pimpl->CreateTexture(path);
}

Texture* Renderer::CreateTexture(Font* font, const std::string& text) const
{
	return m_Pimpl->CreateTexture(font, text);
}

void Renderer::RenderTexture(const Texture& texture, const Transform& transform) const
{
	m_Pimpl->RenderTexture(texture, transform);
}