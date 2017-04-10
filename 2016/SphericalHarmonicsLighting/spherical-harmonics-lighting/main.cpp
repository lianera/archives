#include <array>
#include <iostream>
#include <fstream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "graphics.h"
#include "framework.h"
#include "geometry.h"
#include "inputs.h"

using namespace std;

class SHLightingApp
	:public fw::Application{
public:
	SHLightingApp(array<string, 6> cube_textures, string sh_coef_file, string objfile)
		:cube_textures_(cube_textures), sh_coef_file_(sh_coef_file), objfile_(objfile)
	{}

private:
	// parameters
	array<string, 6> cube_textures_;
	string sh_coef_file_;
	string objfile_;

	// objects
	fw::SkyBox* skybox_;
	shared_ptr<fw::Model> model_;
	GLuint model_program_;
	glm::vec3 coefs[16];

	fw::ObserverInput* input_proc_;
	
	glm::mat4 view, proj;
	GLuint skybox_textures_;

	void OnInit() override;
	void OnUpdate(float dt) override;
	void OnShutdown() override;
};

void SHLightingApp::OnInit()
{
	// load parameters
	ifstream ifs(sh_coef_file_);
	if (!ifs)
		throw runtime_error("open " + sh_coef_file_ + " failed");
	for (int i = 0; i < 16; i++){
		ifs >> coefs[i].r >> coefs[i].g >> coefs[i].b;
	}

	// input proc
	input_proc_ = new fw::ObserverInput({ 3.f, 3.f, 3.f }, { 0.f, 1.f, 0.f });
	this->SetInputProcessor(input_proc_);

	// sky box
	skybox_ = new fw::SkyBox(cube_textures_);

	// object
	model_ = fw::LoadModel(objfile_);	
	model_program_ = fw::CreateProgram("shaders/sh_lighting_vert.glsl", "shaders/sh_lighting_frag.glsl");

	// setup opengl
	glViewport(0, 0, WindowWidth(), WindowHeight());
	//glEnable(GL_CULL_FACE);
	//glCullFace(GL_BACK);
	//glFrontFace(GL_CCW);
	glEnable(GL_DEPTH_TEST);

}

void SHLightingApp::OnUpdate(float dt)
{
	glEnable(GL_DEPTH_TEST);

	view = input_proc_->GetCameraView();
	proj = glm::perspective(glm::radians(60.f), FrameRatio(), 0.1f, 100.f);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
	skybox_->Draw(proj*view);
	glUseProgram(model_program_);
	// compute transforms
	glm::mat4 model_trans = input_proc_->GetModelTransform();
	glm::mat4 model_view_proj = proj*view*model_trans;
	glm::mat4 normal_trans = glm::transpose(glm::inverse(model_trans));

	// delivery transforms
	glUniformMatrix4fv(glGetUniformLocation(model_program_, "model_view_proj"),
		1, false, glm::value_ptr(model_view_proj));
	glUniformMatrix4fv(glGetUniformLocation(model_program_, "normal_trans"),
		1, false, glm::value_ptr(normal_trans));
	glUniform3fv(glGetUniformLocation(model_program_, "coef"),
		16, (float*)(coefs));
	model_->Draw(model_program_);

}

void SHLightingApp::OnShutdown()
{
	delete input_proc_;
	delete skybox_;
	glDeleteTextures(1, &skybox_textures_);
	glDeleteProgram(model_program_);
}

int main(int argc, char *argv[])
{
	try{
		if (argc < 9)
			throw runtime_error("Usage: sh_lighting posx negx posy negy posz negz coefficients.txt model");
		array<string, 6> cube_textures;
		for (int i = 0; i < 6; i++)
			cube_textures[i] = argv[i + 1];
		string sh_coef_file = argv[7];
		string objfile = argv[8];

		SHLightingApp app(cube_textures, sh_coef_file, objfile);
		app.SetWindowSize(800, 600);
		app.SetWindowTitle("Spherical Harmonics Lighting");
		app.Run();
	}
	catch (exception e){
		cerr << e.what() << endl;
	}
	return 0;
}