#include "Shader.h"

Shader::Shader(string vFile, string fFile, string gFile, string tcFile, string teFile)	{
	
	InitShader(vFile, fFile, gFile,tcFile,teFile);
	this->fFile = fFile;
	this->vFile = vFile;
	this->gFile = gFile;
}


Shader::Shader(Shader * shader)
{
	this->fFile = shader->fFile;
	this->vFile = shader->vFile;
	this->gFile = shader->gFile;
	this->tcFile = shader->tcFile;
	this->teFile = shader->teFile;

	InitShader(shader->vFile, shader->fFile, shader->gFile,shader->tcFile,shader->teFile);
}

void Shader::InitShader(string vertex, string fragment , string geometry, string tessControl, string tessEval)
{
	program		= glCreateProgram();
	objects[SHADER_VERTEX]		= GenerateShader(vertex	 ,GL_VERTEX_SHADER);
	objects[SHADER_FRAGMENT]	= GenerateShader(fragment,GL_FRAGMENT_SHADER);
	objects[SHADER_GEOMETRY]	= 0;

	if(!geometry.empty()) {
		objects[SHADER_GEOMETRY]	= GenerateShader(geometry,GL_GEOMETRY_SHADER);
 		glAttachShader(program,objects[SHADER_GEOMETRY]);
	}

	if(!tessControl.empty()) {
		objects[SHADER_CONTROL] = GenerateShader(tessControl,GL_TESS_CONTROL_SHADER );
		glAttachShader(program,objects[SHADER_CONTROL]);

	}
	if(!tessEval.empty()) {
		objects[SHADER_EVALUATION] = GenerateShader(tessEval,GL_TESS_EVALUATION_SHADER );
		glAttachShader(program,objects[SHADER_EVALUATION]);

	}

	glAttachShader(program,objects[SHADER_VERTEX]);
	glAttachShader(program,objects[SHADER_FRAGMENT]);
	

	SetDefaultAttributes();
}


Shader::~Shader(void)	{
	for(int i = 0; i < 3; ++i) {
		glDetachShader(program, objects[i]);
		glDeleteShader(objects[i]);
	}
	glDeleteProgram(program);
}

bool	Shader::LoadShaderFile(string from, string &into)	{
	ifstream	file;
	string		temp;

	cout << "Loading shader text from " << from << endl << endl;

	file.open(from.c_str());
	if(!file.is_open()){
		cout << "File does not exist!" << endl;
		return false;
	}

	while(!file.eof()){
		getline(file,temp);
		into += temp + "\n";
	}

	cout << into << endl << endl;

	file.close();
	cout << "Loaded shader text!" << endl << endl;
	return true;
}

GLuint	Shader::GenerateShader(string from, GLenum type)	{
	cout << "Compiling Shader..." << endl;

	string load;
	if(!LoadShaderFile(from,load)) {
		cout << "Compiling failed!" << endl;
		loadFailed = true;
		return 0;
	}

	GLuint shader = glCreateShader(type);

	const char *chars = load.c_str();
	glShaderSource(shader, 1, &chars, NULL);
	glCompileShader(shader);

	GLint status;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);

	if (status == GL_FALSE)	{
		cout << "Compiling failed!" << endl;
		char error[512];
		glGetInfoLogARB(shader, sizeof(error), NULL, error);
		cout << error;
		loadFailed = true;
		return 0;
	}
	cout << "Compiling success!" << endl << endl;
	loadFailed = false;
	return shader;
}

bool Shader::LinkProgram()	{
	if(loadFailed) {
		return false;
	}
	glLinkProgram(program); 

	GLint code;
	glGetProgramiv(program, GL_LINK_STATUS, &code);

	return code == GL_TRUE ?  true : false;
}

void	Shader::SetDefaultAttributes()	{
	glBindAttribLocation(program, VERTEX_BUFFER,  "position");
	glBindAttribLocation(program, COLOUR_BUFFER,  "colour");
	glBindAttribLocation(program, NORMAL_BUFFER,  "normal");
	glBindAttribLocation(program, TANGENT_BUFFER, "tangent");
	glBindAttribLocation(program, TEXTURE_BUFFER, "texCoord");
}

