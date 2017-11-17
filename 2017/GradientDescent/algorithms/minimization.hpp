#ifndef MINIMIZATION_H
#define MINIMIZATION_H

#include <functional>
#include <Eigen/Eigen>

template <size_t N>
using Vec = Eigen::Matrix<double, N, 1>;
template <size_t N>
using Mat = Eigen::Matrix<double, N, N>;

template <size_t N>
using FUNC = std::function<double(const Vec<N>&)>;
template <size_t N>
using GRAD = std::function<Vec<N>(const Vec<N>&)>;

// arguments: x, gradient on x, direction
template <size_t N>
using STEPFUNC = std::function<double(const Vec<N>&, const Vec<N>&, const Vec<N>&)>;


template <size_t N>
class Problem {
public:
	Problem(){}
	Problem(FUNC<N> f, GRAD<N> g, STEPFUNC<N> step,
		const Vec<N>& x0, double epsilon, int maxiterate)
		:f_(f), grad_(g), step_(step), x0_(x0),
		epsilon_(epsilon), maxiterate_(maxiterate)
	{}
	FUNC<N> f_;
	GRAD<N> grad_;
	Vec<N> x0_;
	double epsilon_;
	int maxiterate_;
	STEPFUNC<N> step_;
};

template <size_t N>
class Desent {
public:
    Vec<N> operator()(const Problem<N>& p)const
	{
        Vec<N> x = p.x0_;
        for (int i = 0; i < p.maxiterate_; i++) {
            Vec<N> g = p.grad_(x);
            if (g.norm() <= p.epsilon_)
				break;	// converge
			Vec<N> d = this->dir(p, x);
            double steplength = p.step_(x, g, d);
			x += steplength * d;
			if (postproc)
				postproc(x);
		}
		return x;
	}
	std::function<void(const Vec<N>&)> postproc;
protected:
    virtual Vec<N> dir(const Problem<N>& p, const Vec<N>& x) const= 0;
};

template <size_t N>
class SteepestDesent
	: public Desent<N>{
public:
protected:
    virtual Vec<N> dir(const Problem<N>& p, const Vec<N>& x)const
	{
        return -p.grad_(x);
	}
};


#endif
