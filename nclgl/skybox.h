#pragma once


class skybox {
public:
	skybox();
	~skybox();
	void draw();


protected:
	GLuint textures[6];

}