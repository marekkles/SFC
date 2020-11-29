#include <UI/MapWidget.hpp>

MapWidget::MapWidget(QWidget *parent)
: 
QWidget(parent),
_aco(nullptr),
_mapNodes(nullptr),
_pen(Qt::gray),
_brush(Qt::NoBrush),
_projectedPointX(0),
_projectedPointY(0),
_i1(0),
_i2(0)
{
    setMouseTracking(true);
    setMinimumWidth(400);
}

void MapWidget::recalculateScale()
{
    if(_mapTemp != nullptr)
    {
        int _padding = 10; 
        _w = width() - 2*_padding;
        _h = height() - 3*_padding;
        float xScale = _w/(_mapTemp->limits[2] - _mapTemp->limits[0]);
        float yScale = _h/(_mapTemp->limits[3] - _mapTemp->limits[1]);
        _scale = (xScale > yScale)?yScale:xScale;

        _xScew = -_mapTemp->limits[0]*_scale + _padding;
        _yScew = -_mapTemp->limits[1]*_scale - 2*_padding;
    }
}

void MapWidget::drawWatermark(QPainter & painter)
{
    painter.setPen(QPen(Qt::gray, 3));
    painter.drawText(QRectF(0, 0, _w, _h), Qt::AlignCenter, "No map is loaded");
}

void MapWidget::drawMap(QPainter & painter)
{
    QPen roadPen(Qt::gray, 10);
    roadPen.setCapStyle(Qt::RoundCap);
    QPen linePen(Qt::white, 2);
    linePen.setStyle(Qt::DashLine);

    
    for (auto &&edge : _mapTemp->edges)
    {
        int x1, y1, x2, y2;
        x1 = static_cast<int>(_mapTemp->nodes[edge[0]].x*_scale + _xScew);
        y1 = _h - static_cast<int>(_mapTemp->nodes[edge[0]].y*_scale + _yScew);
        x2 = static_cast<int>(_mapTemp->nodes[edge[1]].x*_scale + _xScew);
        y2 = _h - static_cast<int>(_mapTemp->nodes[edge[1]].y*_scale + _yScew);
        
        painter.setPen(roadPen);
        painter.drawLine(x1, y1, x2, y2);
        painter.setPen(linePen);
        painter.drawLine(x1, y1, x2, y2);
    }
}

void MapWidget::drawMarkedNodes(QPainter & painter)
{
    painter.setPen(QPen(Qt::blue, 3));
    painter.setBrush(Qt::white);
    
    int count = 0;
    for (auto &&i : _mapTemp->markedNodes)
    {
        
        int x1, y1;
        x1 = static_cast<int>(_mapTemp->nodes[i].x*_scale + _xScew);
        y1 = _h - static_cast<int>(_mapTemp->nodes[i].y*_scale + _yScew);
        painter.drawEllipse(x1-10, y1-10, 20, 20);
        QString number = QString::number(count);
        
        painter.drawText(QRectF(x1-10, y1-10, 20, 20), Qt::AlignCenter, number);
        count++;
    }
}

void MapWidget::drawProjectedPoint(QPainter & painter)
{
    if(_mapTemp != nullptr)
    {
        painter.setPen(QPen(Qt::darkGray, 3));
        painter.setBrush(Qt::white);
        int x1, y1;
        x1 = static_cast<int>(_projectedPointX*_scale + _xScew);
        y1 = _h - static_cast<int>(_projectedPointY*_scale + _yScew);
        
        painter.drawEllipse(x1-10, y1-10, 20, 20);
        QString number = QString::number(_mapTemp->markedNodes.size());
        painter.drawText(QRectF(x1-10, y1-10, 20, 20), Qt::AlignCenter, number);
    }
}
void MapWidget::drawSelectedEdge(QPainter & painter)
{
    painter.setPen(Qt::red);
    int x1, y1, x2, y2;
    x1 = static_cast<int>(_mapTemp->nodes[_i1].x*_scale + _xScew);
    y1 = _h - static_cast<int>(_mapTemp->nodes[_i1].y*_scale + _yScew);
    x2 = static_cast<int>(_mapTemp->nodes[_i2].x*_scale + _xScew);
    y2 = _h - static_cast<int>(_mapTemp->nodes[_i2].y*_scale + _yScew);
    painter.drawLine(x1, y1, x2, y2);
}

void MapWidget::drawPath(QPainter & painter, std::vector<int> &path)
{
    painter.setPen(QPen(Qt::blue, 3));
    for (size_t i = 1; i < path.size(); i++)
    {
        int x1, y1, x2, y2;
        x1 = static_cast<int>(_mapTemp->nodes[path[i-1]].x*_scale + _xScew);
        y1 = _h - static_cast<int>(_mapTemp->nodes[path[i-1]].y*_scale + _yScew);
        x2 = static_cast<int>(_mapTemp->nodes[path[i]].x*_scale + _xScew);
        y2 = _h - static_cast<int>(_mapTemp->nodes[path[i]].y*_scale + _yScew);

        painter.drawLine(x1, y1, x2, y2);
    }
}
void MapWidget::drawClosedPath(QPainter & painter, std::vector<int> &path)
{
    drawPath(painter, path);
    int i1 = path.front();
    int i2 = path.back();
    int x1, y1, x2, y2;
    x1 = static_cast<int>(_mapTemp->nodes[i1].x*_scale + _xScew);
    y1 = _h - static_cast<int>(_mapTemp->nodes[i1].y*_scale + _yScew);
    x2 = static_cast<int>(_mapTemp->nodes[i2].x*_scale + _xScew);
    y2 = _h - static_cast<int>(_mapTemp->nodes[i2].y*_scale + _yScew);
    painter.drawLine(x1, y1, x2, y2);
}
void MapWidget::drawAcoPath(QPainter & painter)
{
    if (_acoTemp != nullptr && _mapTemp != nullptr)
    {
        if (_acoTemp->T.size() > _resultIndex && _acoTemp->T.size() != 0)
        {
            std::vector<int> &acoPath(_acoTemp->T[_resultIndex]);
            for (size_t i = 1; i < acoPath.size(); i++)
            {
                std::vector<int> path;
                _mapTemp->getMarkedPath(acoPath[i], acoPath[i-1], path);
                drawPath(painter, path);
            }
            if(acoPath.size() != 0)
            {
                std::vector<int> path;
                _mapTemp->getMarkedPath(acoPath.front(), acoPath.back(), path);
                drawPath(painter, path);
            } 
        }
    }
}

void MapWidget::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setBrush(_brush);

    if (_mapTemp != nullptr)
    {
        recalculateScale();
        drawMap(painter);
        drawAcoPath(painter);
        drawSelectedEdge(painter);
        drawProjectedPoint(painter);
        drawMarkedNodes(painter);
    }
    else
    {
        drawWatermark(painter);
    }
    _mapTemp = static_cast<const std::shared_ptr<tree_nodes>>(_mapNodes);
    _acoTemp = static_cast<const std::shared_ptr<aco_as>>( _aco);
}

void MapWidget::mousePressEvent(QMouseEvent *event)
{
    if(_mapTemp != nullptr)
    {
        _mapTemp->addMarkedNode(_projectedPointX, _projectedPointY);
        repaint();
        emit mapUpdated();
    }
    _mapTemp = static_cast<const std::shared_ptr<tree_nodes>>(_mapNodes);
}

void MapWidget::mouseMoveEvent(QMouseEvent *event)
{
    if(_mapTemp != nullptr)
    {
        auto pos = event->pos();
        int x = pos.x();
        int y = pos.y();
        float _mapX = (x - _xScew)/_scale;
        float _mapY = (y - _h + _yScew)/(-_scale);

        int closestEdge = _mapTemp->findClosestEdge(_mapX, _mapY);
        if (closestEdge != -1)
        {
            _i1 = _mapTemp->edges[closestEdge][0];
            _i2 = _mapTemp->edges[closestEdge][1];
            _mapTemp->projectOntoEdge(_i1, _i2, _mapX, _mapY, _projectedPointX,  _projectedPointY);

            //std::cout << x << "\t" << y << "\t->\t" << _mapX << "\t" << _mapY << "\t->\t" << _projectedPointX << "\t" << _projectedPointX << std::endl;
        }
        repaint();
    }
    _mapTemp = static_cast<const std::shared_ptr<tree_nodes>>(_mapNodes);
}

void MapWidget::setMap(const std::shared_ptr<tree_nodes> map)
{
    reSetAco();
    _mapNodes = map;
    repaint();
}
void MapWidget::setResults(int index)
{
    _resultIndex = index;
    repaint();
}
void MapWidget::setAco(const std::shared_ptr<aco_as> as)
{
    _aco = as;
    repaint();
}
void MapWidget::reSetAco()
{
    _aco.reset();
    repaint();
}
