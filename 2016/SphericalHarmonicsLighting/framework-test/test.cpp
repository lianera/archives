#include <iostream>
#include "graphics.h"
#include "framework.h"
#include "geometry.h"

using namespace std;

class TestApp
	:public fw::Application{
public:
private:

	void OnInit() override;
	//void OnInput(fw::InputEvent e) override;
	void OnUpdate(float dt) override;
	void OnShutdown() override;
};

void TestApp::OnInit()
{
}

void TestApp::OnUpdate(float dt)
{
}

void TestApp::OnShutdown()
{

}

int main(int argc, char *argv[])
{
	try{
		TestApp app;
		app.Run();
	}
	catch (exception e){
		cerr << e.what() << endl;
	}
	return 0;
}