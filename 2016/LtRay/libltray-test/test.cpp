#include <gtest/gtest.h>
#include "ltray.h"

using namespace ltray;

TEST(GeometryTest, IntervalTest){
	// equal test
	EXPECT_EQ(Interval(1, 2), Interval(1, 2));
	EXPECT_NE(Interval(1, 2), Interval(0, 2));
	EXPECT_EQ(Interval(), Interval(2, 1));
	// NullSet test
	EXPECT_TRUE(Interval().IsNullSet());
	EXPECT_TRUE(Interval(1, -1).IsNullSet());
	EXPECT_TRUE(Interval::NullSet().IsNullSet());
	// Length test
	EXPECT_EQ(1, Interval(-1, 0).Length());
	EXPECT_EQ(0, Interval().Length());
	// infinity length
	EXPECT_EQ(INF, Interval::Real().Length());
	EXPECT_EQ(INF, Interval::NonNegtive().Length());
	EXPECT_EQ(1, Interval::Real().Intersect(Interval(1, 2)).Length());

	Interval a(-3, -1);
	Interval b(1, 3);
	Interval c(-2, 1);
	Interval d(-1, 0);
	// intersection
	EXPECT_EQ(Interval(), a.Intersect(b));
	EXPECT_EQ(Interval(-2, -1), a.Intersect(c));
	EXPECT_EQ(d, c.Intersect(d));
	//range
	EXPECT_EQ(Interval(-3, 3), a.Range(b));
	EXPECT_EQ(Interval(-3, 1), a.Range(c));
	EXPECT_EQ(a, Interval().Range(a));
}

TEST(GeometryTest, VectorTest){
	Vector a(-1, 2, 3);
	Vector b(4, -5, 6);
	Vector c;
	EXPECT_EQ(Vector(3, -3, 9), a + b);
	EXPECT_EQ(Vector(-5, 7, -3), a - b);
	EXPECT_FLOAT_EQ(4, a.Dot(b));
	EXPECT_EQ(Vector(-0.25f, -0.4f, 0.5f), a / b);
	EXPECT_EQ(Vector(-0.267261f, 0.534522f, 0.801784f), a.Normalized());
	EXPECT_EQ(Vector(-4, -10, 18), a * b);
	EXPECT_EQ(Vector(27, 18, -3), a.Cross(b));
	EXPECT_FLOAT_EQ(6, a.Norm1());
	EXPECT_FLOAT_EQ(3.7416575f, a.Norm2());

}

TEST(GeometryTest, PointTest){
	Point a(-1, 2, 3);
	Point b(4, -5, 6);
	Point c;
	Vector d(7, 8, 9);
	EXPECT_EQ(Vector(-5, 7, -3), a - b);
	EXPECT_EQ(Point(6, 10, 12), a + d);
	EXPECT_EQ(Point(-8, -6, -6), a - d);

}

TEST(GeometryTest, RayTest){
	Ray ray(Point(1, 1, 1), Point(1, 1, 1), Interval(0, 10));
	EXPECT_EQ(Point(2, 2, 2), ray.PointOnT(1));
	EXPECT_FLOAT_EQ(1, ray.TOnPoint(Point(2, 2, 2)));
	Transform t;
	Ray newray = t.Apply(ray);
	EXPECT_FLOAT_EQ(0, newray.range.lb);
	EXPECT_FLOAT_EQ(10, newray.range.ub);
}


TEST(GeometryTest, BoxTest){
	Box box(Point(-1, -2, -3), Point(1, 2, 3));
	Ray ray(Point(0, 0, 0), Vector(1, 1, 1).Normalized());
	EXPECT_TRUE(box.Inside(Point(0, 0, 0)));
	float hitr = box.Intersect(ray);
	EXPECT_TRUE(std::isfinite(hitr));
	
	EXPECT_EQ(Box(Point(-4, -4, -4), Point(1, 2, 3)),
		box.Union(Box(Point(-4, -4, -4), Point(0, 0, 0))));

}

TEST(GeometryTest, MatrixTest){
	EXPECT_EQ(Matrix::DiagMatrix(3.0f), Matrix::IdentityMatrix() * 3.0f);

}

TEST(GeometryTest, TransformTest){
	Transform tr;
	Box b = tr.Apply(Box(Point(-0.1f, -0.1f, -0.1f), Point(0.1f, 0.1f, 0.1f)));
	EXPECT_EQ(Box(Point(-0.1f, -0.1f, -0.1f), Point(0.1f, 0.1f, 0.1f)), b);
}




TEST(SurfaceTest, SphereTest){
	Sphere s(1.0f, -0.3f, 0.1f, PI);
	EXPECT_TRUE(std::isfinite(s.Hit(Ray(Point(0.0f, 2.0f, 0.0f), Vector(0.0f, -1.0f, 0.0f)))));
	EXPECT_FALSE(std::isfinite(s.Hit(Ray(Point(0.0f, 2.0f, -0.4f), Vector(0.0f, -1.0f, 0.0f)))));
}



// color test
TEST(SceneTest, ColorTest){
	Color color(0.5f, 0.5f, 0.5f);
	EXPECT_TRUE(color == color);
	EXPECT_TRUE(color != Color::Red());
	EXPECT_EQ(Color::White(), color * 2.0f);
	Color c2 = Color::Red();
	c2 += Color::Green();
	EXPECT_EQ(Color::Yellow(), c2);
}

TEST(SceneTest, CameraTest){
	Camera c(Point(1.0f, 1.0f, 1.0f),
		Point(0.0f, 0.0f, 0.0f), 
		Point(0.0f, 0.0f, 1.0f), 75.0f, false, 
		640, 480, Interval::NonNegtive());
	EXPECT_EQ(Vector(-1.0f, -1.0f, -1.0f).Normalized(), c.direction().Normalized());
	EXPECT_EQ(Point(1.0f, 1.0f, 1.0f), c.position());
	EXPECT_EQ(Point(0.0f,0.0f,0.0f), c.SceneToCamera()*Point(1.0f, 1.0f, 1.0f));
}

TEST(SceneTest, LightTest){
	Color c(1.f, 2.f, 3.f);
	EXPECT_EQ(c, DirectionLight(c).Radiance(Point()).color);
	EXPECT_EQ(c*(1.0f/3.0f),PointLight(c).Radiance(Point(1.0f, 1.0f, 1.0f)).color);
	EXPECT_EQ(
		Color(0.200712264f, 0.401424527f, 0.602136791f),
		SpotLight(c, PI / 6, PI / 12, 4).Radiance(Point(0.1f, 0.1f, 0.3f)).color);
}




TEST(TracingTest, DirectionTest) {
	Vector direction = Vector(-1, -1, -1).Normalized();
	Vector normal = Vector(0, 0, 1).Normalized();
	Vector reflected = Tracing::ReflectDirection(direction, normal);
	EXPECT_EQ(Vector(-1, -1, 1).Normalized(), reflected);

	const float  ior = 1.f;
	Vector refracted;
	Tracing::RefractDirection(direction, normal, ior, refracted);
	EXPECT_EQ(direction, refracted);
}


class VoxelTraverseTester : public VoxelProcesser{
public:
	virtual bool operator()(int id){
		vids.push_back(id);
		return true;
	}
	std::vector<int> vids;
};

TEST(TracingTest, VoxelDDATest){
	Box bound(Point(-2.0f, -2.0f, -2.0f), Point(2.0f, 2.0f, 2.0f));
	int voxelnum[3] = { 4, 4, 4 };
	VoxelGrid grid(bound, voxelnum);
	VoxelDDA dda(grid);
	
	// id test
	for (int i = 0; i < grid.VoxelNum(); i++){
		int x, y, z;
		grid.IdToVoxel(i, x, y, z);
		EXPECT_EQ(i, grid.VoxelId(x, y, z));
	}

	Ray ray(Point(-4.f, -4.f, 0.1f), Vector(8.f, 5.f, 0.f).Normalized(), Interval::NonNegtive());
	int i = 0;
	int points[4][3] = { { 1, 0, 2 }, { 2, 0, 2 }, { 2, 1, 2 }, { 3, 1, 2 } };
	VoxelTraverseTester vtester;
	dda.Traval(ray, vtester);

	for (int i = 0; i < 4; i++){
		int x, y, z;
		grid.IdToVoxel(vtester.vids[i], x, y, z);
		EXPECT_EQ(points[i][0], x);
		EXPECT_EQ(points[i][1], y);
		EXPECT_EQ(points[i][2], z);
	}
}



int main(int argc, char *argv[])
{
	testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}