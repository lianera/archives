#include <QtDataVisualization/QValue3DAxis>
#include <QtDataVisualization/Q3DTheme>
#include <QtDataVisualization/QCustom3DItem>
#include <QtWidgets/QComboBox>

#include <QtGui/QImage>
#include <QtCore/qmath.h>
#include <iostream>
#include <sstream>

#include "mainwindow.h"
#include "minimization.hpp"

using namespace QtDataVisualization;

const int sampleCountX = 50;
const int sampleCountZ = 50;
const float sampleMin = -5.0f;
const float sampleMax = 5.0f;


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


class StepRecorder {
    std::vector<Vec2> rec_;
public:

    void operator()(const Vec2& x)
    {
        rec_.push_back(x);
    }
    std::vector<Vec2> record()
    {
        return rec_;
    }
};


SurfaceGraph::SurfaceGraph(Q3DSurface *surface)
    : m_graph(surface)
{

    m_graph->setAxisX(new QValue3DAxis);
    m_graph->setAxisY(new QValue3DAxis);
    m_graph->setAxisZ(new QValue3DAxis);

    m_quadformProxy = new QSurfaceDataProxy();
    m_quadformSeries = new QSurface3DSeries(m_quadformProxy);
}

SurfaceGraph::~SurfaceGraph()
{
    delete m_graph;
}

void SurfaceGraph::SetupProblem(Mat2 A, Vec2 b, double c)
{
    auto f = [=](const Vec2& x)->double {
        return 1.0 / 2.0 * x.transpose() * A * x - double(b.transpose() * x) + c;
    };
    auto grad = [=](const Vec2& x)->Vec2 {
        return 1.0 / 2.0 * A.transpose() * x + 1.0 / 2.0 * A * x - b;
    };
    cout << "Setup step" << endl;
    STEPFUNC<2> stepstrategy;
    std::string stepstr = stepbox_->currentText().toStdString();
    if(stepstr == "exact step"){
        cout << "exact step" << endl;
        static QuadraticFormExactStep<2> exactstep_strategy(A);
        stepstrategy = std::ref(exactstep_strategy);
    }else{
        std::string numstr(stepstr.begin() + stepstr.find_last_of(':') + 1, stepstr.end());
        cout << "Fixed step: " << numstr << endl;
        double steplen = std::stod(numstr);
        auto stepf = [=](const Vec2& x, const Vec2& gradx, const Vec2& d){
            //cout << "Steping: " << steplen << endl;
            return steplen;
        };
        stepstrategy = stepf;
    }

    Vec2 x0 = Vec2::Zero();
    double epsilon = 1e-5;
    int maxiterate = (int)1e3;


    Problem<2> problem(f, grad, stepstrategy, x0, epsilon, maxiterate);

    problem_ = problem;

}

void SurfaceGraph::StepChanged(const QString &str)
{
    this->ArgsChanged(0);
}

void SurfaceGraph::Solve()
{
    QPoint sel_p = this->m_quadformSeries->selectedPoint();
    // selection to coordination
    double realx1 = (double(sel_p.y())/(sampleCountZ-1))*(sampleMax - sampleMin) + sampleMin;
    double realx2 = (double(sel_p.x())/(sampleCountX-1))*(sampleMax - sampleMin) + sampleMin;

    cout << "Selected Grid:" << sel_p.x() << ',' << sel_p.y() << endl;
    cout << "Selection:" << realx1 << ',' << problem_.f_(Vec2(realx1, realx2)) << ',' << realx2 << endl;

    problem_.x0_ = Vec2(realx1, realx2);

    StepRecorder recorder;
    SteepestDesent<2> minimizer;
    minimizer.postproc = std::ref(recorder);

    Vec2 r = minimizer(problem_);
    records_ = recorder.record();

    std::stringstream ss;
    if(recorder.record().size() < 1000){
        ss << "Steps: " << endl;
        for(Vec2 p : records_)
            ss << p[0] << ',' << p[1] << endl;
        ss << "Result: " << r.transpose() << endl;
        ss << "Step count: " << recorder.record().size() << endl;
    }else{
        ss << "Cannot found minimization point!" << endl;
    }
    cout << ss.str() << endl;
    info_->setText(ss.str().c_str());

    float lastx = problem_.x0_[0];
    float lasty = problem_.f_(problem_.x0_);
    float lastz = problem_.x0_[1];

    for(int i = 0; i < records_.size(); i++){
        Vec2 rec = records_[i];
        double x = rec[0];
        double y = problem_.f_(rec);
        double z = rec[1];
        if(std::isnan(x) || std::isnan(y) || std::isnan(z) ||
                std::isinf(x) || std::isinf(y) || std::isinf(z))
            continue;
        // draw bar
        // cout << "xyz:" << x << ','  << y << ','  << z << endl;
        //if(i > 0){
        QVector3D barpos = QVector3D(x, y, z);
        double dx = x - lastx;
        double dy = y - lasty;
        double dz = z - lastz;
        //cout << "dxdydz:" << dx << ','  << dy << ','  << dz << endl;

        double caliy = dy/20;
        double len = sqrt(dx*dx + caliy*caliy + dz*dz);
        QVector3D dir = QVector3D(dx, caliy, -dz).normalized();
        QVector3D from(0,1,0);
        QImage barcolor = QImage(2, 2, QImage::Format_RGB32);
        barcolor.fill(Qt::black);
        QQuaternion quad = QQuaternion::rotationTo(from, dir);

        QCustom3DItem *baritem = new QCustom3DItem(":/obj/bar", barpos,
                                                   QVector3D(1,len/1.25,1),
                                                   quad,
                                                   barcolor);
        m_graph->addCustomItem(baritem);
        items_.push_back(baritem);
        // }
        lastx = x;
        lasty = y;
        lastz = z;
        // draw dot
        QVector3D pos = QVector3D(x, y, z);
        QImage color = QImage(2, 2, QImage::Format_RGB32);
        color.fill(Qt::red);
        QCustom3DItem *item = new QCustom3DItem(":/obj/dot", pos,
                                                QVector3D(0.05,0.05,0.05),
                                                QQuaternion(0,0,0,0),
                                                color);
        items_.push_back(item);
        m_graph->addCustomItem(item);
    }
}

void SurfaceGraph::ClearItems()
{
    cout << items_.size() << endl;
    for(QCustom3DItem* d : items_){
        //Note: Removing a custom item from the graph also deletes it. If you want to preserve the item, you need to use releaseCustomItem() method instead.

        m_graph->removeCustomItem(d);
    }
    items_.clear();
}

void SurfaceGraph::ArgsChanged(int val)
{
    double a[7];
    for(int i = 0; i < 7; i++){
        a[i] = args_[i]->value();
    }
    Mat2 A;
    A << a[0], a[1],
         a[2], a[3];
    Vec2 b;
    b << a[4], a[5];
    double c = a[6];
    SetupProblem(A, b, c);
    fillquadformProxy();

    ClearItems();
}

void SurfaceGraph::fillquadformProxy()
{

    float stepX = (sampleMax - sampleMin) / float(sampleCountX - 1);
    float stepZ = (sampleMax - sampleMin) / float(sampleCountZ - 1);

    QSurfaceDataArray *dataArray = new QSurfaceDataArray;
    dataArray->reserve(sampleCountZ);
    for (int i = 0 ; i < sampleCountZ ; i++) {
        QSurfaceDataRow *newRow = new QSurfaceDataRow(sampleCountX);
        // Keep values within range bounds, since just adding step can cause minor drift due
        // to the rounding errors.
        float z = qMin(sampleMax, (i * stepZ + sampleMin));
        int index = 0;
        for (int j = 0; j < sampleCountX; j++) {
            float x = qMin(sampleMax, (j * stepX + sampleMin));
            Vec2 vx;
            vx << x ,z;
            float y = problem_.f_(vx);
            //float R = qSqrt(z * z + x * x) + 0.01f;
            //float y = (qSin(R) / R + 0.24f) * 1.61f;
            (*newRow)[index++].setPosition(QVector3D(x, y, z));
        }
        *dataArray << newRow;
    }


    m_quadformProxy->resetArray(dataArray);
}

void SurfaceGraph::enablequadformModel(bool enable)
{
    if (enable) {
       // m_quadformSeries->setDrawMode(QSurface3DSeries::DrawSurfaceAndWireframe);
         m_quadformSeries->setDrawMode(QSurface3DSeries::DrawWireframe);
        m_quadformSeries->setFlatShadingEnabled(true);

        m_graph->axisX()->setLabelFormat("%.2f");
        m_graph->axisZ()->setLabelFormat("%.2f");
        m_graph->axisX()->setRange(sampleMin, sampleMax);
        m_graph->axisY()->setRange(sampleMin*10, sampleMax*10);
        m_graph->axisZ()->setRange(sampleMin, sampleMax);
        m_graph->axisX()->setLabelAutoRotation(30);
        m_graph->axisY()->setLabelAutoRotation(90);
        m_graph->axisZ()->setLabelAutoRotation(30);

        m_graph->addSeries(m_quadformSeries);

        // Reset range sliders for Sqrt&Sin
        m_rangeMinX = sampleMin;
        m_rangeMinZ = sampleMin;
        m_stepX = (sampleMax - sampleMin) / float(sampleCountX - 1);
        m_stepZ = (sampleMax - sampleMin) / float(sampleCountZ - 1);
    }
}

void SurfaceGraph::changeTheme(int theme)
{
    m_graph->activeTheme()->setType(Q3DTheme::Theme(theme));
}

void SurfaceGraph::setBlackToYellowGradient()
{
    QLinearGradient gr;
    gr.setColorAt(0.0, Qt::black);
    gr.setColorAt(0.33, Qt::blue);
    gr.setColorAt(0.67, Qt::red);
    gr.setColorAt(1.0, Qt::yellow);

    m_graph->seriesList().at(0)->setBaseGradient(gr);
    m_graph->seriesList().at(0)->setColorStyle(Q3DTheme::ColorStyleRangeGradient);
}

void SurfaceGraph::setGreenToRedGradient()
{
    QLinearGradient gr;
    gr.setColorAt(0.0, Qt::darkGreen);
    gr.setColorAt(0.5, Qt::yellow);
    gr.setColorAt(0.8, Qt::red);
    gr.setColorAt(1.0, Qt::darkRed);

    m_graph->seriesList().at(0)->setBaseGradient(gr);
    m_graph->seriesList().at(0)->setColorStyle(Q3DTheme::ColorStyleRangeGradient);
}
