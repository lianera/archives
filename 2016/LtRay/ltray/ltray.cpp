#include <string>
#include <cctype>
#include <fstream>
#include <memory>
#include "ltray.h"
#include "io.h"
#include "worldloader.h"

using namespace std;
using namespace ltray;

class ResultProc{
	Canvas* canvas_;
//	ImageWriter* imgwriter_;
//	PLYWriter* plywriter_;
public:
	/*ResultProc(Canvas* canvas, ImageWriter* imgwriter, PLYWriter* plywriter)
		:canvas_(canvas), imgwriter_(imgwriter), plywriter_(plywriter)
	{}*/

	ResultProc(Canvas* canvas)
		:canvas_(canvas)
	{}

	void operator()(int image_x, int image_y,
		float red, float green, float blue,
		float point_x, float point_y, float point_z)
	{
		if (canvas_)
			canvas_->setPixel(image_x, image_y, red, green, blue);
	/*	if (imgwriter_)
			imgwriter_->setPixel(image_x, image_y, red, green, blue);
		if (plywriter_)
			plywriter_->addPoint(point_x, point_y, point_z, red, green, blue);
			*/
	}
	void Done()
	{
		/*
		if (imgwriter_)	imgwriter_->Write();
		if (plywriter_) plywriter_->Write();
		*/
	}
};

int main(int argc, char *argv[])
{
	// parsing command line options
	if (argc < 2){
		//cout << "Usage: ltray world.toml output.png [output.ply]" << endl;
		cout << "Usage: ltray world.toml" << endl;
		return 0;
	}
	string worldfile = argv[1];
	//string imgfile = argv[2];
	//string plyfile = argc > 3 ? argv[3] : "";

	try{
		cout << "Reading world: " << worldfile << endl;
		WorldLoader world_loader(worldfile);

		auto tracer = world_loader.tracer();
		auto shader = world_loader.shader();
		auto camera = world_loader.camera();
		auto scene = world_loader.scene();

		unique_ptr<Canvas> canvas(new Canvas(camera->pixel_nx(), camera->pixel_ny(), shader->grayscale()));
		
		/*cout << "Output image: " + imgfile << endl;
		unique_ptr<ImageWriter> imgwriter(new ImageWriter(imgfile, camera->pixel_nx(), camera->pixel_ny(), shader->grayscale()));
		unique_ptr<PLYWriter> plywriter;
		if (!plyfile.empty()) {
				cout << "Output Point cloud: " + plyfile << endl;
				plywriter.reset(new PLYWriter(plyfile));
		}*/
			
		//ResultProc result_proc(canvas.get(), imgwriter.get(), plywriter.get());
		ResultProc result_proc(canvas.get());
		cout << "Tracing ... " << endl;
		tracer->Trace(*scene, *camera, result_proc);
		result_proc.Done();
		cout << "Tracing Done" << endl;
	}
	catch (Error e){
		cerr << "An Error occurred:" << endl;
		cerr << e.what() << endl;
		return 1;
	}
	catch (...){
		cerr << "Unknow error" << endl;
		return 1;
	}
	return 0;
}
