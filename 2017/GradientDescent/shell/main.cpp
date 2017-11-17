#include "mainwindow.h"
#include <QtWidgets/QApplication>
#include <QtWidgets/QWidget>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMessageBox>
#include <QtGui/QPainter>
#include <QtGui/QScreen>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    Q3DSurface *graph = new Q3DSurface();
    QWidget *container = QWidget::createWindowContainer(graph);

    if (!graph->hasContext()) {
        QMessageBox msgBox;
        msgBox.setText("Couldn't initialize the OpenGL context.");
        msgBox.exec();
        return -1;
    }

    QSize screenSize = graph->screen()->size();
    container->setMinimumSize(QSize(screenSize.width() / 2, screenSize.height() / 1.6));
    container->setMaximumSize(screenSize);
    container->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    container->setFocusPolicy(Qt::StrongFocus);

    QWidget *widget = new QWidget;
    QHBoxLayout *hLayout = new QHBoxLayout(widget);
    QVBoxLayout *vLayout = new QVBoxLayout();
    hLayout->addWidget(container, 1);
    hLayout->addLayout(vLayout);
    vLayout->setAlignment(Qt::AlignTop);

    widget->setWindowTitle(QStringLiteral("Gradient Descent"));
    widget->setWindowIcon(QIcon(QStringLiteral(":/images/logo")));

    QGroupBox *modelGroupBox = new QGroupBox(QStringLiteral("Function"));

    QRadioButton *sqrtSinModelRB = new QRadioButton(widget);
    sqrtSinModelRB->setText(QStringLiteral("Quadratic Form"));
    sqrtSinModelRB->setChecked(false);

    QVBoxLayout *modelVBox = new QVBoxLayout;
    modelVBox->addWidget(sqrtSinModelRB);

    SurfaceGraph *modifier = new SurfaceGraph(graph);

    ::QTextEdit info("Message:");
    modifier->SetInfobox(&info);

    QComboBox strategybox;
    strategybox.addItem("exact step");
    strategybox.addItem("fixed step:0.1");
    modelVBox->addWidget(&strategybox);
    modifier->SetStepbox(&strategybox);

    QSlider* quad_args[7];
    int initval[7] ={3, 2, 2, 6,
                2, -8, 0};

    for(int i = 0; i < 7; i++){
        QSlider*& arg = quad_args[i];
        arg = new QSlider(Qt::Horizontal, widget);
        arg->setMinimum(-10);
        arg->setMaximum(10);
        arg->setTickInterval(1);
        arg->setEnabled(true);
        arg->setValue(initval[i]);
        modelVBox->addWidget(arg);
        QObject::connect(arg, SIGNAL(valueChanged(int)),
                         modifier, SLOT(ArgsChanged(int)));
    }
    modifier->SetSlider(quad_args);
    modifier->ArgsChanged(0);

    QPushButton plotbutton("Solve", widget);



    modelVBox->addWidget(&plotbutton);
    modelVBox->addWidget(&info);
    modelGroupBox->setLayout(modelVBox);

    vLayout->addWidget(modelGroupBox);

    widget->show();


    QObject::connect(sqrtSinModelRB, &QRadioButton::toggled,
                     modifier, &SurfaceGraph::enablequadformModel);
    QObject::connect(&plotbutton, &QPushButton::clicked,
                     modifier, &SurfaceGraph::Solve);
    QObject::connect(&strategybox, &QComboBox::currentTextChanged,
                     modifier, &SurfaceGraph::StepChanged);
    strategybox.setEditable(true);
      strategybox.setCurrentIndex(0);
    sqrtSinModelRB->setChecked(true);

    return app.exec();

}
