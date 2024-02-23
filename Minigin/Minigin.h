#ifndef MINIGIN
#define MINIGIN

#include <string>
#include <functional>
#include <chrono>

class TextObject;

class Minigin
{
public:

	explicit Minigin(const std::string& dataPath);
	~Minigin();

	Minigin(const Minigin& other) = delete;
	Minigin(Minigin&& other) = delete;
	Minigin& operator=(const Minigin& other) = delete;
	Minigin& operator=(Minigin&& other) = delete;

	void Run(const std::function<void()>& load);

private:

	SDL_Window* m_Window;
	const int m_MaxFrameRate;
	const std::chrono::milliseconds m_MinFrameDuration;
	const std::chrono::milliseconds m_FixedDuration;
	std::shared_ptr<TextObject> m_FPSCounter;

	std::chrono::milliseconds CalculateMinFrameDuration(int frameRate);
};

#endif