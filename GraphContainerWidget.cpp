#include "GraphContainerWidget.h"

#include <QCheckBox>
#include <QDoubleSpinBox>
#include <QLabel>

GraphContainerWidget::GraphContainerWidget(QWidget* parent, EoBE::Handle&& graphUpdaterHandle, LineGraph* lineGraph)
    : QWidget(parent)
    , lineGraph_(lineGraph)
    , graphUpdaterHandle_(std::move(graphUpdaterHandle))
{

    QWidget* settings = new QWidget();

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

    QGridLayout* settingsGrid = new QGridLayout();
    settingsGrid->addWidget(xAxisMinEnable, 0, 0, 1, 1);
    settingsGrid->addWidget(xAxisMaxEnable, 1, 0, 1, 1);
    settingsGrid->addWidget(yAxisMinEnable, 2, 0, 1, 1);
    settingsGrid->addWidget(yAxisMaxEnable, 3, 0, 1, 1);
    settingsGrid->addWidget(new QLabel("# Data Points"), 4, 0, 1, 1);
    settingsGrid->addWidget(xAxisMinSpin, 0, 1, 1, 1);
    settingsGrid->addWidget(xAxisMaxSpin, 1, 1, 1, 1);
    settingsGrid->addWidget(yAxisMinSpin, 2, 1, 1, 1);
    settingsGrid->addWidget(yAxisMaxSpin, 3, 1, 1, 1);
    settingsGrid->addWidget(plotDataCountSpin, 4, 1, 1, 1);
    lineGraph_->ForEachPlot([=, row = 5](const LineGraph::Plot& plot, size_t plotIndex) mutable
    {
        QCheckBox* plotHider = new QCheckBox(QString("Show %1").arg(plot.name_));
        plotHider->setChecked(!plot.hidden_);
        settingsGrid->addWidget(plotHider, row++, 0, 1, 2);
        connect(plotHider, &QCheckBox::toggled, [=](bool checked){ lineGraph_->SetPlotHidden(plotIndex, !checked); });
    });
    settings->setLayout(settingsGrid);

    QHBoxLayout* baseLayout = new QHBoxLayout(this);
    baseLayout->setMargin(0);
    baseLayout->setSpacing(0);
    baseLayout->addWidget(settings);
    baseLayout->addWidget(lineGraph_);
    baseLayout->setStretch(0, 0);
    baseLayout->setStretch(1, 1);
    setLayout(baseLayout);
}

void GraphContainerWidget::ResetGraph()
{
    lineGraph_->Reset();
}
