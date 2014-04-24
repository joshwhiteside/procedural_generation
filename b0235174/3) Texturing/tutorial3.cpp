#pragma comment(lib, "nclgl.lib")

#include "../../nclGL/window.h"
#include "Renderer.h"

int main() {
	Window w("Texturing!", 800,600,false);	 //This is all boring win32 window creation stuff!
	if(!w.HasInitialised()) {
		return -1;
	}
	
	Renderer renderer(w);	//This handles all the boring OGL 3.2 initialisation stuff, and sets up our tutorial!
	if(!renderer.HasInitialised()) {
		return -1;
	}

	float rotate = 0.0f;
	float scale = 1.0f;
	while(w.UpdateWindow() && !Window::GetKeyboard()->KeyDown(KEYBOARD_ESCAPE)){
		if(Window::GetKeyboard()->KeyDown(KEYBOARD_LEFT) ) {
			--rotate;
			renderer.UpdateTextureMatrix(rotate);
		}

		if(Window::GetKeyboard()->KeyDown(KEYBOARD_RIGHT) ) {
			++rotate;
			renderer.UpdateTextureMatrix(rotate);
		}

		if(Window::GetKeyboard()->KeyDown(KEYBOARD_UP) ) {
			++scale;
			renderer.UpdateTextureMatrix(rotate);
			renderer.ScaleTextureMatrix(scale);
		}

		if(Window::GetKeyboard()->KeyDown(KEYBOARD_DOWN) ) {
			--scale;
			renderer.UpdateTextureMatrix(rotate);
			renderer.ScaleTextureMatrix(scale);
		}

		if(Window::GetKeyboard()->KeyTriggered(KEYBOARD_1) ) {
			renderer.ToggleFiltering();
		}

		if(Window::GetKeyboard()->KeyTriggered(KEYBOARD_2) ) {
			renderer.ToggleRepeating();
		}

		
		if(Window::GetKeyboard()->KeyTriggered(KEYBOARD_3) ) {
			renderer.ToggleAnsiotropy();
		}

		if(Window::GetKeyboard()->KeyDown(KEYBOARD_PLUS))
			++scale;
		if(Window::GetKeyboard()->KeyDown(KEYBOARD_MINUS))
			--scale;


		renderer.UpdateScene(w.GetTimer()->GetTimedMS());
		renderer.RenderScene();
	}

	return 0;
}