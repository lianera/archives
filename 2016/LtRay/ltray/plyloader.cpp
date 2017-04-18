#include <fstream>
#include "plyloader.h"
#include "ltray.h"

using namespace std;
using namespace ltray;

void PLYLoader::Load(const std::string& filename)
{
	// read ply file
	ifstream plyfs(filename_);
	if (!plyfs)
		throw Error("cannot open ply file");
	string str;
	int vertex_num = 0;
	int face_num = 0;
	enum PROP_TERM{ X, Y, Z, NX, NY, NZ, CONFIDENCE, INTENSITY, UNKNOW };
	vector<PROP_TERM> prop_terms;
	while (plyfs >> str && str != "end_header"){
		if (str == "element"){
			plyfs >> str;
			if (str == "vertex")
				plyfs >> vertex_num;
			else if (str == "face")
				plyfs >> face_num;
		}
		else if (str == "property"){
			string ntype;
			string term_str;
			plyfs >> ntype;	// only suport float currently 
			if (ntype == "list"){
				// ignore
				string s;
				plyfs >> s >> s >> s;
				continue;
			}
			plyfs >> term_str;
			PROP_TERM t;
			if (term_str == "x")		t = X;
			else if (term_str == "y")	t = Y;
			else if (term_str == "z")	t = Z;
			else if (term_str == "nx")	t = NX;
			else if (term_str == "ny")	t = NY;
			else if (term_str == "nz")	t = NZ;
			else if (term_str == "confidence")	t = CONFIDENCE;
			else if (term_str == "intensity")	t = INTENSITY;
			else
				t = UNKNOW;	// unplemented
			prop_terms.push_back(t);
		}
	}
	int prop_size = int(prop_terms.size());
	vector<Point> points;
	for (int i = 0; i < vertex_num; i++){
		float x, y, z;
		for (int i = 0; i < prop_size; i++){
			float val;
			plyfs >> val;
			switch (prop_terms[i]){
			case X:
				x = val;
				break;
			case Y:
				y = val;
			case Z:
				z = val;
			}
		}
		points.push_back(Point(x, y, z));
	}

	int vn;
	for (int j = 0; j < face_num; j++){
		plyfs >> vn;
		vector<int> vertex_indexes(vn);
		shared_ptr<SurfaceBase> surface;
		for (int iv = 0; iv < vn; iv++)
			plyfs >> vertex_indexes[iv];
		if (vn == 3){
			surface = make_shared<Triangle>(points[vertex_indexes[0]],
				points[vertex_indexes[1]],
				points[vertex_indexes[2]]);
		}
		else{
			vector<Point> polygon_vertexes(vn);
			for (int iv = 0; iv < vn; iv++)
				polygon_vertexes[iv] = points[vertex_indexes[iv]];
			surface = make_shared<Polygon>(polygon_vertexes);
		}
		objects_.push_back(make_shared<Object>(surface.get(), material_.get(), transform_.get()));
	}

}
