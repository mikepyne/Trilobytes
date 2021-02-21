#include "LineGraphContainerWidget.h"

#include <QCheckBox>
#include <QDoubleSpinBox>
#include <QLabel>
#include <QScrollArea>

LineGraphContainerWidget::LineGraphContainerWidget(QWidget* parent, Tril::Handle&& graphUpdaterHandle, LineGraph* lineGraph)
    : QWidget(parent)
    , lineGraph_(lineGraph)
    , graphUpdaterHandle_(std::move(graphUpdaterHandle))
{
    QScrollArea* settingsScrollArea = new QScrollArea;
    settingsScrollArea->setFrameShape(QFrame::NoFrame);
    settingsScrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    settingsScrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    settingsScrollArea->setWidgetResizable(true);

    QWidget* settings = new QWidget();
    settingsScrollArea->setWidget(settings);

    QCheckBox* xAxisMinEnable = new QCheckBox("x-axis Min");
    QCheckBox* xAxisMaxEnable = new QCheckBox("x-axis Max");
    QCheckBox* yAxisMinEnable = new QCheckBox("y-axis Min");
    QCheckBox* yAxisMaxEnable = new QCheckBox("y-axis Max");
    connect(xAxisMinEnable, &QCheckBox::toggled, lineGraph_, &LineGraph::EnableXAxisMinOverride);
    connect(xAxisMaxEnable, &QCheckBox::toggled, lineGraph_, &LineGraph::EnableXAxisMaxOverride);
    connect(yAxisMinEnable, &QCheckBox::toggled, lineGraph_, &LineGraph::EnableYAxisMinOverride);
    connect(yAxisMaxEnable, &QCheckBox::toggled, lineGraph_, &LineGraph::EnableYAxisMaxOverride);

    QDoubleSpinBox* xAxisMinSpin = new QDoubleSpinBox();
    QDoubleSpinBox* xAxisMaxSpin = new QDoubleSpinBox();
    QDoubleSpinBox* yAxisMinSpin = new QDoubleSpinBox();
    QDoubleSpinBox* yAxisMaxSpin = new QDoubleSpinBox();
    xAxisMinSpin->setEnabled(false);
    xAxisMaxSpin->setEnabled(false);
    yAxisMinSpin->setEnabled(false);
    yAxisMaxSpin->setEnabled(false);
    connect(xAxisMinEnable, &QCheckBox::toggled, xAxisMinSpin, &QDoubleSpinBox::setEnabled);
    connect(xAxisMaxEnable, &QCheckBox::toggled, xAxisMaxSpin, &QDoubleSpinBox::setEnabled);
    connect(yAxisMinEnable, &QCheckBox::toggled, yAxisMinSpin, &QDoubleSpinBox::setEnabled);
    connect(yAxisMaxEnable, &QCheckBox::toggled, yAxisMaxSpin, &QDoubleSpinBox::setEnabled);
    connect(xAxisMinEnable, &QCheckBox::toggled, [=](bool) { xAxisMinSpin->setRange(lineGraph_->GetXAxisMin(), lineGraph_->GetXAxisMax()); xAxisMinSpin->setValue(lineGraph_->GetXAxisMinOverride()); });
    connect(xAxisMaxEnable, &QCheckBox::toggled, [=](bool) { xAxisMaxSpin->setRange(lineGraph_->GetXAxisMin(), lineGraph_->GetXAxisMax()); xAxisMaxSpin->setValue(lineGraph_->GetXAxisMaxOverride()); });
    connect(yAxisMinEnable, &QCheckBox::toggled, [=](bool) { yAxisMinSpin->setRange(lineGraph_->GetYAxisMin(), lineGraph_->GetYAxisMax()); yAxisMinSpin->setValue(lineGraph_->GetYAxisMinOverride()); });
    connect(yAxisMaxEnable, &QCheckBox::toggled, [=](bool) { yAxisMaxSpin->setRange(lineGraph_->GetYAxisMin(), lineGraph_->GetYAxisMax()); yAxisMaxSpin->setValue(lineGraph_->GetYAxisMaxOverride()); });
    connect(xAxisMinSpin, static_cast<void(QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), lineGraph_, &LineGraph::SetXAxisMinOverride);
    connect(xAxisMaxSpin, static_cast<void(QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), lineGraph_, &LineGraph::SetXAxisMaxOverride);
    connect(yAxisMinSpin, static_cast<void(QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), lineGraph_, &LineGraph::SetYAxisMinOverride);
    connect(yAxisMaxSpin, static_cast<void(QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), lineGraph_, &LineGraph::SetYAxisMaxOverride);

    QSpinBox* plotDataCountSpin = new QSpinBox();
    plotDataCountSpin->setRange(1, std::numeric_limits<int>::max());
    plotDataCountSpin->setValue(lineGraph_->GetPlotDataPointCount());
    connect(plotDataCountSpin, static_cast<void(QSpinBox::*)(int)>(&QSpinBox::valueChanged), lineGraph_, &LineGraph::SetPlotDataPointCount);

    QCheckBox* graticuleHidden = new QCheckBox("Hide Graticule");
    graticuleHidden->setChecked(true);
    connect(graticuleHidden, &QCheckBox::toggled, lineGraph_, &LineGraph::SetGraticuleHidden);

    QGridLayout* settingsGrid = new QGridLayout();
    int row = 0;
    settingsGrid->addWidget(xAxisMinEnable, row, 0, 1, 1);
    settingsGrid->addWidget(xAxisMinSpin, row++, 1, 1, 1);
    settingsGrid->addWidget(xAxisMaxEnable, row, 0, 1, 1);
    settingsGrid->addWidget(xAxisMaxSpin, row++, 1, 1, 1);
    settingsGrid->addWidget(yAxisMinEnable, row, 0, 1, 1);
    settingsGrid->addWidget(yAxisMinSpin, row++, 1, 1, 1);
    settingsGrid->addWidget(yAxisMaxEnable, row, 0, 1, 1);
    settingsGrid->addWidget(yAxisMaxSpin, row++, 1, 1, 1);
    settingsGrid->addWidget(graticuleHidden, row++, 0, 1, 2);
    settingsGrid->addWidget(new QLabel("# Data Points"), row, 0, 1, 1);
    settingsGrid->addWidget(plotDataCountSpin, row++, 1, 1, 1);
    lineGraph_->ForEachPlot([=](const LineGraph::Plot& plot, size_t plotIndex) mutable
    {
        QCheckBox* plotHider = new QCheckBox(QString("Show %1").arg(plot.name_));
        plotHider->setChecked(!plot.hidden_);
        settingsGrid->addWidget(plotHider, row++, 0, 1, 2);
        connect(plotHider, &QCheckBox::toggled, [=](bool checked){ lineGraph_->SetPlotHidden(plotIndex, !checked); });
    });
    settings->setLayout(settingsGrid);

    QHBoxLayout* baseLayout = new QHBoxLayout(this);
    baseLayout->setContentsMargins(0, 0, 0, 0);
    baseLayout->setSpacing(0);
    baseLayout->addWidget(settingsScrollArea);
    baseLayout->addWidget(lineGraph_);
    baseLayout->setStretch(0, 0);
    baseLayout->setStretch(1, 1);
    setLayout(baseLayout);
}

void LineGraphContainerWidget::ResetGraph()
{
    lineGraph_->Reset();
}
