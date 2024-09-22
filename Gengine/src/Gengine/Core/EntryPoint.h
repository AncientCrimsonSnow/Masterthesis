#pragma once

#ifdef GG_PLATFORM_WINDOWS

extern Gengine::Application* Gengine::CreateApplication();

int main(int argc, char** argv) {
	Gengine::Log::Init();

	var app = Gengine::CreateApplication();
	app->Run();
	delete app;
}

#endif