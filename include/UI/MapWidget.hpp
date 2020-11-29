#include <QWidget>
#include <QApplication>
#include <QPushButton>
#include <QLabel>
#include <QPen>
#include <QBrush>
#include <QPainter>
#include <QMouseEvent>
#include <QFileDialog>
#include <memory>
#include <ACO/tree_nodes.hpp>
#include <ACO/aco_as.hpp>

#include <iostream>

#ifndef MapWidget_hpp
#define MapWidget_hpp

class MapWidget : public QWidget
{
    Q_OBJECT
signals:
    void mapUpdated();
public slots:
    void setMap(const std::shared_ptr<tree_nodes> map);
    void setResults(int index);
    void setAco(const std::shared_ptr<aco_as> as);
    void reSetAco();
public:
    void Redraw();
    MapWidget(QWidget *parent = nullptr);
protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
private:
    void recalculateScale();
    void drawWatermark(QPainter & painter);
    void drawMap(QPainter & painter);
    void drawMarkedNodes(QPainter & painter);
    void drawProjectedPoint(QPainter & painter);
    void drawSelectedEdge(QPainter & painter);
    void drawPath(QPainter & painter, std::vector<int> &path);
    void drawClosedPath(QPainter & painter, std::vector<int> &path);
    void drawAcoPath(QPainter & painter);

    int _resultIndex; 
    std::shared_ptr<tree_nodes> _mapNodes;
    std::shared_ptr<aco_as> _aco;
    std::shared_ptr<tree_nodes> _mapTemp;
    std::shared_ptr<aco_as> _acoTemp;
    QPen _pen;
    QBrush _brush;
    float _scale;
    float _xScew;
    float _yScew;
    float _w;
    float _h;

    float _mapX;
    float _mapY;
    float _projectedPointX;
    float _projectedPointY;
    int _i1;
    int _i2;
    int _acoPathIndex;
};

#endif