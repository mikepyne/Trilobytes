#ifndef LINEGRAPHCONTAINERWIDGET_H
#define LINEGRAPHCONTAINERWIDGET_H

#include "LineGraph.h"
#include "Utility/AutoClearingContainer.h"

#include <QWidget>
#include <QLayout>

class LineGraphContainerWidget : public QWidget {
    Q_OBJECT
public:
    LineGraphContainerWidget(QWidget* parent, Tril::Handle&& graphUpdaterHandle, LineGraph* lineGraph);
    void ResetGraph();

private:
    LineGraph* lineGraph_;
    Tril::Handle graphUpdaterHandle_;
};

#endif // LINEGRAPHCONTAINERWIDGET_H
