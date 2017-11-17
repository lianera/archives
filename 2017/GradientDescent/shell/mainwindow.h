#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtDataVisualization/Q3DSurface>
#include <QtDataVisualization/QSurfaceDataProxy>
#include <QtDataVisualization/QHeightMapSurfaceDataProxy>
#include <QtDataVisualization/QSurface3DSeries>
#include <QtWidgets/QSlider>
#include <QtWidgets/QTextEdit>
#include <vector>
#include <array>
#include <memory>
#include <QtWidgets/QComboBox>

#include "minimization.hpp"
using namespace std;
using Vec2 = Vec<2>;
using Mat2 = Eigen::Matrix2d;


using namespace QtDataVisualization;

class SurfaceGraph : public QObject
{
    Q_OBJECT
public:
    explicit SurfaceGraph(Q3DSurface *surface);
    ~SurfaceGraph();

    void enablequadformModel(bool enable);

    void toggleModeNone() { m_graph->setSelectionMode(QAbstract3DGraph::SelectionNone); }
    void toggleModeItem() { m_graph->setSelectionMode(QAbstract3DGraph::SelectionItem); }
    void toggleModeSliceRow() { m_graph->setSelectionMode(QAbstract3DGraph::SelectionItemAndRow
                                                          | QAbstract3DGraph::SelectionSlice); }
    void toggleModeSliceColumn() { m_graph->setSelectionMode(QAbstract3DGraph::SelectionItemAndColumn
                                                             | QAbstract3DGraph::SelectionSlice); }

    void setBlackToYellowGradient();
    void setGreenToRedGradient();

public Q_SLOTS:
    void changeTheme(int theme);

    void SetSlider(QSlider* args[7])
    {
        for(int i = 0; i < 7; i++)
            args_[i] = args[i];
    }

    void ArgsChanged(int val);
    void Solve();
    void SetStepbox(QComboBox *stepbox)
    {
        stepbox_ = stepbox;
    }
    void SetInfobox(QTextEdit *info)
    {
        info_ = info;
    }

    void StepChanged(const QString &str);
private:
    void SetupProblem(Mat2 A, Vec2 b, double c);
    Problem<2> problem_;
    std::vector<Vec2> records_;
    std::vector<QCustom3DItem*> items_;
    QSlider *args_[7];
    void ClearItems();
    QComboBox *stepbox_;
    QTextEdit *info_;
private:
    Q3DSurface *m_graph;
    QHeightMapSurfaceDataProxy *m_heightMapProxy;
    QSurfaceDataProxy *m_quadformProxy;
    QSurface3DSeries *m_quadformSeries;

    float m_rangeMinX;
    float m_rangeMinZ;
    float m_stepX;
    float m_stepZ;

    void setAxisXRange(float min, float max);
    void setAxisZRange(float min, float max);
    void fillquadformProxy();
};


#endif // MAINWINDOW_H
