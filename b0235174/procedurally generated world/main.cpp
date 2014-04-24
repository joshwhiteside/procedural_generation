#include "../../nclGL/window.h"
#include "Renderer.h"

#pragma comment(lib, "nclgl.lib")

int main() {	
	Window w("Alien world", 1280,720,false);
	if(!w.HasInitialised()) {
		return -1;
	}
	
	Renderer renderer(w);
	if(!renderer.HasInitialised()) {
		return -1;
	}

	w.LockMouseToWindow(true);
	w.ShowOSPointer(false);

	 
	
	float timeSinceLastFrame = w.GetTimer()->GetTimedMS();

	while(w.UpdateWindow() && !Window::GetKeyboard()->KeyDown(KEYBOARD_ESCAPE)){
	
		timeSinceLastFrame = w.GetTimer()->GetTimedMS();

		renderer.UpdateScene(timeSinceLastFrame);
		renderer.RenderScene();

		//renderer.fps=1000.0f/timeSinceLastFrame;

	}

	return 0;
}