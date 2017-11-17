#include <iostream>
#include <fstream>
#include "minimization.hpp"

using namespace std;
using Vec2 = Vec<2>;
using Mat2 = Eigen::Matrix2d;

class StepRecorder {
	ostream& os_;
public:
	StepRecorder(ostream& os)
		:count_(0), os_(os)
	{}
	void operator()(const Vec2& x)
	{
		os_ << x.transpose() << endl;
		count_++;
	}
	int count_;
};

void QuadTest()
{
	auto f = [&](const Vec2& x)->double {
		return (x - Vec2::Ones() * 10).dot(x - Vec2::Ones());
	};

	auto grad = [&](const Vec2& x)->Vec2 {
		return Vec2(2 * x[0] - 10, 2 * x[1] - 1);
	};

	auto step = [&](const Vec2& x, const Vec2& gradx, const Vec2& d)->double {
		return 0.1;
	};

	Vec2 x0 = Vec2::Zero();
	double epsilon = 1e-5;
	int maxiterate = (int)1e5;

	Problem<2> problem(f, grad, step, x0, epsilon, maxiterate);
	
	SteepestDesent<2> minimizer;
	minimizer.postproc = [&](const Vec2& x) {
		cout << x.transpose() << endl;
	};

	Vec2 r = minimizer(problem);
	cout << "Result: " << r.transpose() << endl;
}

template <size_t N>
class QuadraticFormExactStep {
	Mat<N> A_;
public:
	QuadraticFormExactStep(const Mat<N>& A)
		:A_(A)
	{}

	double operator()(const Vec<N>& x, const Vec<N>& gradx, const Vec<N>& d)
	{
		return -(double(gradx.transpose()*d) / (d.transpose()*A_*d));
	}
};


void QuadraticFormTest()
{
	Mat2 A;
	Vec2 b;
	
	A << 3, 2,
		2, 6;

	b << 2,
		-8;

	double c = 0;
	auto f = [&](const Vec2& x)->double {
		return 1.0 / 2.0 * x.transpose() * A * x - double(b.transpose() * x) + c;
	};
	auto grad = [&](const Vec2& x)->Vec2 {
		return 1.0 / 2.0 * A.transpose() * x + 1.0 / 2.0 * A * x - b;
	};
	QuadraticFormExactStep<2> exactstep_strategy(A);

	Vec2 x0 = Vec2::Zero();
	double epsilon = 1e-5;
	int maxiterate = (int)1e5;

	Problem<2> problem(f, grad, std::ref(exactstep_strategy), x0, epsilon, maxiterate);
	
	ofstream ofs("steps.txt");
	StepRecorder recorder(ofs);
	SteepestDesent<2> minimizer;
	minimizer.postproc = std::ref(recorder);

	Vec2 r = minimizer(problem);
	cout << "Result: " << r.transpose() << endl;
	cout << "Step count: " << recorder.count_ << endl;

}

int main()
{
	QuadraticFormTest();
	return 0;
}