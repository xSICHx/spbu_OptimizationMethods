/*
    Copyright (c) 2008-2024 Jan W. Krieger (<jan@jkrieger.de>)

    

    This software is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License (LGPL) as published by
    the Free Software Foundation, either version 2.1 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License (LGPL) for more details.

    You should have received a copy of the GNU Lesser General Public License (LGPL)
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/



#include "jkqtplotter/jkqtpgraphsbase.h"
#include "jkqtplotter/jkqtpbaseplotter.h"
#include <stdlib.h>
#include <QDebug>
#include <iostream>
#include "jkqtplotter/jkqtptools.h"
#include "jkqtplotter/graphs/jkqtpimage.h"
#include "jkqtplotter/jkqtpbaseelements.h"
#include "jkqtplotter/jkqtplotter.h"
#include "jkqtpgraphsbase.h"


#define SmallestGreaterZeroCompare_xvsgz() if ((xvsgz>10.0*DBL_MIN)&&((smallestGreaterZero<10.0*DBL_MIN) || (xvsgz<smallestGreaterZero))) smallestGreaterZero=xvsgz;

JKQTPPlotElement::JKQTPPlotElement(JKQTBasePlotter* parent):
    QObject(parent), xAxisRef(JKQTPPrimaryAxis), yAxisRef(JKQTPPrimaryAxis)
{
    title="";
    visible=true;
    highlighted=false;
    parentPlotStyle=-1;
    setParent(parent);
}


JKQTPGraph::JKQTPGraph(JKQTBasePlotter* parent):
    JKQTPPlotElement(parent)
{
}


QImage JKQTPPlotElement::generateKeyMarker(QSize size)
{
    QImage img(size.width(),size.height(),QImage::Format_ARGB32);
    if (parent) img.fill(Qt::transparent);//->getKeyBackgroundColor());
    {
        JKQTPEnhancedPainter painter(&img);
        painter.setRenderHint(QPainter::Antialiasing, true);
        painter.setRenderHint(QPainter::TextAntialiasing, true);
        painter.setRenderHint(QPainter::SmoothPixmapTransform, true);
#if (QT_VERSION<QT_VERSION_CHECK(6, 0, 0))
        painter.setRenderHint(QPainter::HighQualityAntialiasing, true);
#endif
        QRectF rect(0,0,size.width(),size.height());
        drawKeyMarker(painter, rect);
    }
    return img;
}

void JKQTPPlotElement::setTitle(const QString &__value)
{
    this->title = __value;
}

QString JKQTPPlotElement::getTitle() const
{
    return this->title;
}

void JKQTPPlotElement::setVisible(bool __value)
{
    this->visible = __value;
}

bool JKQTPPlotElement::isVisible() const
{
    return this->visible;
}

void JKQTPPlotElement::setHighlighted(bool __value)
{
    highlighted=__value;
}

bool JKQTPPlotElement::isHighlighted() const
{
    return highlighted;
}





void JKQTPPlotElement::setParent(JKQTBasePlotter* parent) {
    this->parent=parent;
    QObject::setParent(parent);
}

void JKQTPPlotElement::setParent(JKQTPlotter *parent)
{
    setParent(parent->getPlotter());
}


bool JKQTPGraph::getDataMinMax(int column, double &minx, double &maxx, double &smallestGreaterZero)
{
    bool start=true;
    minx=0;
    maxx=0;
    smallestGreaterZero=0;

    if (parent==nullptr) return false;

    const JKQTPDatastore* datastore=parent->getDatastore();
    int imin=0;
    int imax=static_cast<int>(datastore->getRows(column));

    if (imin<0) imin=0;
    if (imax<0) imax=0;

    for (int i=imin; i<imax; i++) {
        double xv=datastore->get(column,static_cast<size_t>(i));
        if (start || xv>maxx) maxx=xv;
        if (start || xv<minx) minx=xv;
        double xvsgz;
        xvsgz=xv; SmallestGreaterZeroCompare_xvsgz();
        start=false;
    }
    return !start;
}

void JKQTPPlotElement::getOutsideSize(JKQTPEnhancedPainter& /*painter*/, int& leftSpace, int& rightSpace, int& topSpace, int& bottomspace) {
    leftSpace=0;
    rightSpace=0;
    topSpace=0;
    bottomspace=0;
}

void JKQTPPlotElement::drawOutside(JKQTPEnhancedPainter& /*painter*/, QRect /*leftSpace*/, QRect /*rightSpace*/, QRect /*topSpace*/, QRect /*bottomSpace*/) {

}


QString JKQTPPlotElement::formatHitTestDefaultLabel(double x, double y, int index, const JKQTPDatastore* datastore) const {
    const JKQTPXGraphErrorData* errgx=dynamic_cast<const JKQTPXGraphErrorData*>(this);
    QString xerrstr;
    // retrieve x-error data
    if (errgx && datastore) {
        if (errgx->getXErrorColumn()>=0) {
            if (errgx->getXErrorColumnLower()>=0) {
                xerrstr=QString("\\:+%1\\:-%2")
                            .arg(xFloatToString(datastore->get(errgx->getXErrorColumn(),static_cast<size_t>(index))))
                            .arg(xFloatToString(datastore->get(errgx->getXErrorColumnLower(),static_cast<size_t>(index))));
            } else {
                xerrstr=QString("{\\:}{\\pm}%1")
                            .arg(xFloatToString(datastore->get(errgx->getXErrorColumn(),static_cast<size_t>(index))));
            }
        }
    }

    // retrieve y-error data
    const JKQTPYGraphErrorData* errgy=dynamic_cast<const JKQTPYGraphErrorData*>(this);
    QString yerrstr;
    if (errgy && datastore) {
        if (errgy->getYErrorColumn()>=0) {
            if (errgy->getYErrorColumnLower()>=0) {
                yerrstr=QString("\\:+%1\\:-%2")
                            .arg(yFloatToString(datastore->get(errgy->getYErrorColumn(),static_cast<size_t>(index))))
                            .arg(yFloatToString(datastore->get(errgy->getYErrorColumnLower(),static_cast<size_t>(index))));
            } else {
                yerrstr=QString("{\\:}{\\pm}%1")
                            .arg(yFloatToString(datastore->get(errgy->getYErrorColumn(),static_cast<size_t>(index))));
            }
        }
    }
    return QString("\\ensuremath{\\left[{\\:}%1%3{\\;},{\\;}%2%4{\\:}\\right]}").arg(xFloatToString(x)).arg(yFloatToString(y)).arg(xerrstr).arg(yerrstr);

}

QString JKQTPPlotElement::xFloatToString(double v, int past_comma) const
{
    if (past_comma<0) return getXAxis()->floattolabel(v);
    else return getXAxis()->floattolabel(v, past_comma);
}

QString JKQTPPlotElement::yFloatToString(double v, int past_comma) const
{
    if (past_comma<0) return getYAxis()->floattolabel(v);
    else return getYAxis()->floattolabel(v, past_comma);
}

double JKQTPPlotElement::hitTest(const QPointF & posSystem, QPointF* closestSpotSystem, QString* label, HitTestMode mode) const
{
    if (parent==nullptr) return JKQTP_NAN;

    int closest=-1;
    double closedist=JKQTP_NAN;
    double closedistsec=JKQTP_NAN;
    QPointF closestPos;
    QPointF posF=transform(posSystem);
    for (int i=0; i<m_hitTestData.size(); i++) {
        const QPointF x=m_hitTestData[i].pos;
        const QPointF xpix = transform(x);
        if (JKQTPIsOKFloat(xpix.x())&&JKQTPIsOKFloat(xpix.y())) {
            double d=0, dsecondary=0;
            switch (mode) {
                case HitTestXY: d=sqrt(jkqtp_sqr(xpix.x()-posF.x())+jkqtp_sqr(xpix.y()-posF.y())); dsecondary=0; break;
                case HitTestXOnly: d=fabs(xpix.x()-posF.x()); dsecondary=fabs(xpix.y()-posF.y()); break;
                case HitTestYOnly: d=fabs(xpix.y()-posF.y()); dsecondary=fabs(xpix.x()-posF.x()); break;
            }
            if (closest<0 || d<closedist || (jkqtp_approximatelyEqual(d,closedist) && dsecondary<closedistsec)) {
                closest=i;
                closedist=d;
                closedistsec=dsecondary;
                closestPos=x;
            }
        }
    }
    if (closest>=0) {
        if (closestSpotSystem) *closestSpotSystem=closestPos;
        if (label) *label=m_hitTestData[closest].label;
        return closedist;
    } else {
        return JKQTP_NAN;
    }
}

double JKQTPPlotElement::transformX(double x) const {
    return getXAxis()->x2p(x);
}

double JKQTPPlotElement::transformY(double y) const {
    return getYAxis()->x2p(y);
}

QVector<double> JKQTPPlotElement::transformX(const QVector<double>& x) const {
    QVector<double> res;
    res.resize(x.size());
    for (int i=0; i<x.size(); i++) {
        res[i]=getXAxis()->x2p(x[i]);
    }
    return res;
}

QVector<double> JKQTPPlotElement::transformY(const QVector<double>& y) const {
    QVector<double> res;
    res.resize(y.size());
    for (int i=0; i<y.size(); i++) {
        res[i]=getYAxis()->x2p(y[i]);
    }
    return res;
}

void JKQTPPlotElement::setXAxis(JKQTPCoordinateAxisRef ref)
{
    xAxisRef=ref;
}

void JKQTPPlotElement::setYAxis(JKQTPCoordinateAxisRef ref)
{
    yAxisRef=ref;
}

JKQTPCoordinateAxisRef JKQTPPlotElement::getXAxisRef() const
{
    return xAxisRef;
}

JKQTPCoordinateAxisRef JKQTPPlotElement::getYAxisRef() const
{
    return yAxisRef;
}


double JKQTPPlotElement::backtransformX(double x) const {
    return getXAxis()->p2x(x);
}

double JKQTPPlotElement::backtransformY(double y) const {
    return getYAxis()->p2x(y);
}


bool JKQTPGraph::usesColumn(int /*column*/) const
{
    return false;
}

void JKQTPGraph::drawErrorsBefore(JKQTPEnhancedPainter &)
{

}

void JKQTPGraph::drawErrorsAfter(JKQTPEnhancedPainter &)
{

}

QPolygonF JKQTPPlotElement::transform(const QPolygonF &x) const {
    QPolygonF res;
    for (int i=0; i<x.size(); i++) {
        res.append(transform(x[i]));
    }
    return res;
}

QPainterPath JKQTPPlotElement::transformToLinePath(const QPolygonF &x) const {
    QPainterPath res;
    if (x.size()>0) {
        res.moveTo(transform(x[0]));
        for (int i=1; i<x.size(); i++) {
            res.lineTo(transform(x[i]));
        }
    }
    return res;
}




JKQTPXYGraph::JKQTPXYGraph(JKQTBasePlotter* parent):
    JKQTPGraph(parent), xColumn(-1), yColumn(-1), sortData(Unsorted)
{

}


bool JKQTPXYGraph::getXMinMax(double& minx, double& maxx, double& smallestGreaterZero) {
    bool start=true;
    minx=0;
    maxx=0;
    smallestGreaterZero=0;

    if (parent==nullptr)  return false;

    const JKQTPDatastore* datastore=parent->getDatastore();
    int imin=0;
    int imax=0;
    getIndexRange(imin, imax);

    for (int i=imin; i<imax; i++) {
        double xv=datastore->get(static_cast<size_t>(xColumn),static_cast<size_t>(i));
        if (JKQTPIsOKFloat(xv)) {
            if (start || xv>maxx) maxx=xv;
            if (start || xv<minx) minx=xv;
            double xvsgz;
            xvsgz=xv; SmallestGreaterZeroCompare_xvsgz();
            start=false;
        }
    }
    return !start;
}

bool JKQTPXYGraph::getYMinMax(double& miny, double& maxy, double& smallestGreaterZero) {
    bool start=true;
    miny=0;
    maxy=0;
    smallestGreaterZero=0;

    if (parent==nullptr) return false;

    const JKQTPDatastore* datastore=parent->getDatastore();
    int imin=0;
    int imax=0;
    getIndexRange(imin, imax);


    for (int i=imin; i<imax; i++) {
        double yv=datastore->get(static_cast<size_t>(yColumn),static_cast<size_t>(i));
        if (JKQTPIsOKFloat(yv)) {
            if (start || yv>maxy) maxy=yv;
            if (start || yv<miny) miny=yv;
            double xvsgz;
            xvsgz=yv; SmallestGreaterZeroCompare_xvsgz();
            start=false;
        }
    }
    return !start;
}

bool JKQTPXYGraph::usesColumn(int column) const
{
    return (column==xColumn)||(column==yColumn);
}

void JKQTPXYGraph::setXColumn(int __value)
{
    this->xColumn = __value;
}

int JKQTPXYGraph::getXColumn() const
{
    return this->xColumn;
}

void JKQTPXYGraph::setXColumn(size_t __value) {
    this->xColumn = static_cast<int>(__value);
}

void JKQTPXYGraph::setYColumn(int __value)
{
    this->yColumn = __value;
}

int JKQTPXYGraph::getYColumn() const
{
    return this->yColumn;
}

void JKQTPXYGraph::setYColumn(size_t __value) {
    this->yColumn = static_cast<int>(__value);
}

void JKQTPXYGraph::setKeyColumn(int __value)
{
    setXColumn(__value);
}

void JKQTPXYGraph::setValueColumn(int __value)
{
    setYColumn(__value);
}

void JKQTPXYGraph::setDataSortOrder(JKQTPXYGraph::DataSortOrder __value)
{
    this->sortData = __value;
}

JKQTPXYGraph::DataSortOrder JKQTPXYGraph::getDataSortOrder() const
{
    return this->sortData;
}

int JKQTPXYGraph::getKeyColumn() const
{
    return getXColumn();
}

int JKQTPXYGraph::getValueColumn() const
{
    return getYColumn();
}

void JKQTPXYGraph::setDataSortOrder(int __value) {
    sortData=static_cast<DataSortOrder>(__value);
}

void JKQTPXYGraph::setXYColumns(size_t xCol, size_t yCol)
{
    setXColumn(xCol);
    setYColumn(yCol);
}

void JKQTPXYGraph::setXYColumns(int xCol, int yCol)
{
    setXColumn(xCol);
    setYColumn(yCol);
}

#if QT_VERSION<QT_VERSION_CHECK(6,0,0)
void JKQTPXYGraph::setXYColumns(std::pair<int, int> xyColPair)
{
    setXColumn(xyColPair.first);
    setYColumn(xyColPair.second);
}

void JKQTPXYGraph::setXYColumns(std::pair<size_t, size_t> xyColPair)
{
    setXColumn(xyColPair.first);
    setYColumn(xyColPair.second);
}
#endif

void JKQTPXYGraph::setXYColumns(QPair<int, int> xyColPair)
{
    setXColumn(xyColPair.first);
    setYColumn(xyColPair.second);
}

void JKQTPXYGraph::setXYColumns(QPair<size_t, size_t> xyColPair)
{
    setXColumn(xyColPair.first);
    setYColumn(xyColPair.second);
}


double JKQTPXYGraph::hitTest(const QPointF &posSystem, QPointF *closestSpotSystem, QString *label, HitTestMode mode) const
{
    if (parent==nullptr) return JKQTP_NAN;

    // check base-class implementation and use it, if it returns a vaid value
    const double baseclassResult=JKQTPPlotElement::hitTest(posSystem, closestSpotSystem, label, mode);
    if (JKQTPIsOKFloat(baseclassResult)) return baseclassResult;

    const JKQTPDatastore* datastore=parent->getDatastore();
    int imin=0;
    int imax=0;
    if (!getIndexRange(imin, imax)) return JKQTP_NAN;


    int closest=-1;
    double closedist=JKQTP_NAN;
    double closedistsec=JKQTP_NAN;
    QPointF closestPos;
    QPointF posF=transform(posSystem);
    for (int i=imin; i<imax; i++) {
        const QPointF x(datastore->get(static_cast<size_t>(xColumn),static_cast<size_t>(i)), datastore->get(static_cast<size_t>(yColumn),static_cast<size_t>(i)));
        const QPointF xpix = transform(x);
        if (JKQTPIsOKFloat(xpix.x())&&JKQTPIsOKFloat(xpix.y())) {
            double d=0, dsecondary=0;
            switch (mode) {
                case HitTestXY: d=sqrt(jkqtp_sqr(xpix.x()-posF.x())+jkqtp_sqr(xpix.y()-posF.y())); dsecondary=0; break;
                case HitTestXOnly: d=fabs(xpix.x()-posF.x()); dsecondary=fabs(xpix.y()-posF.y()); break;
                case HitTestYOnly: d=fabs(xpix.y()-posF.y()); dsecondary=fabs(xpix.x()-posF.x()); break;
            }
            if (closest<0 || d<closedist || (jkqtp_approximatelyEqual(d,closedist) && dsecondary<closedistsec)) {
                closest=i;
                closedist=d;
                closedistsec=dsecondary;
                closestPos=x;
                //qDebug()<<"hitTest("<<posSystem<<"[="<<posF<<"pix]...): found closest="<<closest<<", closedist="<<closedist<<", closedistsec="<<closedistsec<<", closestPos="<<closestPos;
            }
        }
    }
    if (closest>=0) {
        if (label) *label=formatHitTestDefaultLabel(closestPos.x(), closestPos.y(), closest, datastore);
        if (closestSpotSystem) *closestSpotSystem=closestPos;
        return closedist;
    } else {
        return JKQTP_NAN;
    }
}




JKQTPSingleColumnGraph::JKQTPSingleColumnGraph(JKQTBasePlotter *parent):
    JKQTPGraph(parent), dataColumn(-1), dataDirection(DataDirection::Y), sortData(Unsorted)
{
}


void JKQTPSingleColumnGraph::setDataColumn(int __value)
{
    this->dataColumn = __value;
    if (this->title.size()==0 && parent && __value>=0) this->title=parent->getDatastore()->getColumnName(static_cast<size_t>(__value));
}

int JKQTPSingleColumnGraph::getDataColumn() const
{
    return this->dataColumn;
}

void JKQTPSingleColumnGraph::setDataColumn(size_t __value) {
    this->dataColumn = static_cast<int>(__value);
    if (this->title.size()==0 && parent) this->title=parent->getDatastore()->getColumnName(__value);
}

void JKQTPSingleColumnGraph::setDataSortOrder(JKQTPSingleColumnGraph::DataSortOrder __value)
{
    this->sortData = __value;
}

JKQTPSingleColumnGraph::DataSortOrder JKQTPSingleColumnGraph::getDataSortOrder() const
{
    return this->sortData;
}

void JKQTPSingleColumnGraph::setDataSortOrder(int __value) {
    sortData=static_cast<DataSortOrder>(__value);
    if (__value>0) sortData=Sorted;
}

void JKQTPSingleColumnGraph::setDataDirection(JKQTPSingleColumnGraph::DataDirection __value)
{
    this->dataDirection = __value;
}

JKQTPSingleColumnGraph::DataDirection JKQTPSingleColumnGraph::getDataDirection() const
{
    return this->dataDirection;
}

bool JKQTPSingleColumnGraph::usesColumn(int c) const
{
    return c==dataColumn;
}


void JKQTPSingleColumnGraph::intSortData()
{
    sortedIndices.clear();



    if (parent==nullptr)  return ;

    const JKQTPDatastore* datastore=parent->getDatastore();
    int imin=0;
    int imax=0;
    getIndexRange(imin, imax);

    QVector<double> datas;

    if (sortData==JKQTPSingleColumnGraph::Sorted) {

        for (int i=0; i<imax; i++) {
            double xv=datastore->get(dataColumn,static_cast<size_t>(i));
            sortedIndices<<i;
            datas<<xv;
        }

        jkqtpQuicksortDual(datas.data(), sortedIndices.data(), datas.size());


    }
}

bool JKQTPSingleColumnGraph::getIndexRange(int &imin, int &imax) const
{
    if (parent==nullptr)  return false;

    const JKQTPDatastore* datastore=parent->getDatastore();
    if (dataColumn>=0) {
        imin=0;
        imax=static_cast<int>(datastore->getRows(static_cast<size_t>(dataColumn)));
        if (imax<imin) {
            int h=imin;
            imin=imax;
            imax=h;
        }
        if (imin<0) imin=0;
        if (imax<0) imax=0;
        return true;
    }
    return false;
}















void JKQTPXYGraph::intSortData()
{
    sortedIndices.clear();



    if (parent==nullptr)  return ;
    if (sortData==JKQTPXYLineGraph::Unsorted) return ;

    JKQTPDatastore* datastore=parent->getDatastore();
    int imin=0;
    int imax=0;
    getIndexRange(imin, imax);

    QVector<double> datas;

    if (sortData==JKQTPXYLineGraph::SortedX) {

        for (int i=0; i<imax; i++) {
            double xv=datastore->get(static_cast<size_t>(xColumn),static_cast<size_t>(i));
            sortedIndices<<i;
            datas<<xv;
        }

        jkqtpQuicksortDual(datas.data(), sortedIndices.data(), datas.size());


    } else if (sortData==JKQTPXYLineGraph::SortedY) {

        for (int i=0; i<imax; i++) {
            double xv=datastore->get(static_cast<size_t>(yColumn),static_cast<size_t>(i));
            sortedIndices<<i;
            datas<<xv;
        }

        jkqtpQuicksortDual(datas.data(), sortedIndices.data(), datas.size());
    }
}

bool JKQTPXYGraph::getIndexRange(int& imin, int& imax) const
{
    if (parent==nullptr)  return false;
    if (xColumn>=0 && yColumn>=0) {
        JKQTPDatastore* datastore=parent->getDatastore();
        imin=0;
        imax=static_cast<int>(qMin(datastore->getRows(static_cast<size_t>(xColumn)), datastore->getRows(static_cast<size_t>(yColumn))));
        // ensure correct order, i.e. imin<=imax
        if (imax<imin) {
            int h=imin;
            imin=imax;
            imax=h;
        }
        // ranges are always >=0
        if (imin<0) imin=0;
        if (imax<0) imax=0;
        return true;
    }
    return false;
}



JKQTPGeometricPlotElement::JKQTPGeometricPlotElement(DrawMode drawMode, JKQTBasePlotter *parent):
    JKQTPPlotElement(parent), m_drawMode(drawMode)
{

}


void JKQTPGeometricPlotElement::setDrawMode(JKQTPGeometricPlotElement::DrawMode mode)
{
    m_drawMode=mode;
}

JKQTPGeometricPlotElement::DrawMode JKQTPGeometricPlotElement::getDrawMode() const
{
    return m_drawMode;
}

JKQTPGeometricPlotElement::~JKQTPGeometricPlotElement()
= default;








JKQTPXYYGraph::JKQTPXYYGraph(JKQTBasePlotter *parent):
    JKQTPXYGraph(parent), yColumn2(-1)
{

}

bool JKQTPXYYGraph::getYMinMax(double &miny, double &maxy, double &smallestGreaterZero)
{
    bool start=true;
    miny=0;
    maxy=0;
    smallestGreaterZero=0;

    if (parent==nullptr) return false;

    const JKQTPDatastore* datastore=parent->getDatastore();
    int imin=0;
    int imax=0;
    if (getIndexRange(imin, imax)) {


        for (int i=imin; i<imax; i++) {
            const double yv=datastore->get(static_cast<size_t>(yColumn),static_cast<size_t>(i));
            if (JKQTPIsOKFloat(yv)) {
                if (start || yv>maxy) maxy=yv;
                if (start || yv<miny) miny=yv;
                double xvsgz;
                xvsgz=yv; SmallestGreaterZeroCompare_xvsgz();
                start=false;
            }
            const double yv2=datastore->get(static_cast<size_t>(yColumn2),static_cast<size_t>(i));
            if (JKQTPIsOKFloat(yv2)) {
                if (start || yv2>maxy) maxy=yv2;
                if (start || yv2<miny) miny=yv2;
                double xvsgz;
                xvsgz=yv2; SmallestGreaterZeroCompare_xvsgz();
                start=false;
            }
        }
        return !start;
    }
    return false;
}

bool JKQTPXYYGraph::usesColumn(int column) const
{
    return (column==yColumn2) || JKQTPXYYGraph::usesColumn(column);
}

int JKQTPXYYGraph::getYColumn2() const
{
    return yColumn2;
}

double JKQTPXYYGraph::hitTest(const QPointF &posSystem, QPointF *closestSpotSystem, QString *label, JKQTPPlotElement::HitTestMode mode) const
{
    if (parent==nullptr) return JKQTP_NAN;

    // check base-class implementation and use it, if it returns a vaid value
    const double baseclassResult=JKQTPXYGraph::hitTest(posSystem, closestSpotSystem, label, mode);
    if (JKQTPIsOKFloat(baseclassResult)) return baseclassResult;

    const JKQTPDatastore* datastore=parent->getDatastore();
    int imin=0;
    int imax=0;
    if (!getIndexRange(imin, imax)) return JKQTP_NAN;


    int closest=-1;
    double closedist=JKQTP_NAN;
    double closedistsec=JKQTP_NAN;
    QPointF closestPos;
    QPointF posF=transform(posSystem);
    for (int i=imin; i<imax; i++) {
        const QPointF x(datastore->get(static_cast<size_t>(xColumn),static_cast<size_t>(i)), datastore->get(static_cast<size_t>(yColumn2),static_cast<size_t>(i)));
        const QPointF xpix = transform(x);
        if (JKQTPIsOKFloat(xpix.x())&&JKQTPIsOKFloat(xpix.y())) {
            double d=0, dsecondary=0;
            switch (mode) {
            case HitTestXY: d=sqrt(jkqtp_sqr(xpix.x()-posF.x())+jkqtp_sqr(xpix.y()-posF.y())); dsecondary=0; break;
            case HitTestXOnly: d=fabs(xpix.x()-posF.x()); dsecondary=fabs(xpix.y()-posF.y()); break;
            case HitTestYOnly: d=fabs(xpix.y()-posF.y()); dsecondary=fabs(xpix.x()-posF.x()); break;
            }
            if (closest<0 || d<closedist || (jkqtp_approximatelyEqual(d,closedist) && dsecondary<closedistsec)) {
                closest=i;
                closedist=d;
                closedistsec=dsecondary;
                closestPos=x;
                //qDebug()<<"hitTest("<<posSystem<<"[="<<posF<<"pix]...): found closest="<<closest<<", closedist="<<closedist<<", closedistsec="<<closedistsec<<", closestPos="<<closestPos;
            }
        }
    }
    if (closest>=0) {
        if (label) *label=formatHitTestDefaultLabel(closestPos.x(), closestPos.y(), closest, datastore);
        if (closestSpotSystem) *closestSpotSystem=closestPos;
        return closedist;
    } else {
        return JKQTP_NAN;
    }

}

void JKQTPXYYGraph::setXYYColumns(size_t xCol, size_t yCol, size_t y2Col)
{
    setXYColumns(xCol, yCol);
    yColumn2=static_cast<int>(y2Col);
}

void JKQTPXYYGraph::setXYYColumns(int xCol, int yCol, int y2Col)
{
    setXYColumns(xCol, yCol);
    yColumn2=y2Col;
}

void JKQTPXYYGraph::setYColumn2(int __value)
{
    yColumn2=__value;
}

void JKQTPXYYGraph::setYColumn2(size_t __value)
{
    yColumn2=static_cast<int>(__value);
}

int JKQTPXYYGraph::getValue2Column() const
{
    return getYColumn2();
}

void JKQTPXYYGraph::setValue2Column(int __value)
{
    setYColumn2(__value);
}

bool JKQTPXYYGraph::getIndexRange(int &imin, int &imax) const
{
    bool ok=JKQTPXYGraph::getIndexRange(imin, imax);
    if (ok) {
        if (parent==nullptr)  return false;
        if (yColumn2<0) return false;
        const JKQTPDatastore* datastore=parent->getDatastore();
        const int rows=static_cast<int>(datastore->getRows(static_cast<size_t>(yColumn2)));
        imax=qMin<int>(imax, rows);
    }
    return ok;
}







JKQTPXXYGraph::JKQTPXXYGraph(JKQTBasePlotter *parent):
    JKQTPXYGraph(parent), xColumn2(-1)
{

}

bool JKQTPXXYGraph::getXMinMax(double &minx, double &maxx, double &smallestGreaterZero)
{
    bool start=true;
    minx=0;
    maxx=0;
    smallestGreaterZero=0;

    if (parent==nullptr) return false;

    const JKQTPDatastore* datastore=parent->getDatastore();
    int imin=0;
    int imax=0;
    if (getIndexRange(imin, imax)) {


        for (int i=imin; i<imax; i++) {
            const double xv=datastore->get(static_cast<size_t>(xColumn),static_cast<size_t>(i));
            if (JKQTPIsOKFloat(xv)) {
                if (start || xv>maxx) maxx=xv;
                if (start || xv<minx) minx=xv;
                double xvsgz;
                xvsgz=xv; SmallestGreaterZeroCompare_xvsgz();
                start=false;
            }
            const double xv2=datastore->get(static_cast<size_t>(xColumn2),static_cast<size_t>(i));
            if (JKQTPIsOKFloat(xv2)) {
                if (start || xv2>maxx) maxx=xv2;
                if (start || xv2<minx) minx=xv2;
                double xvsgz;
                xvsgz=xv2; SmallestGreaterZeroCompare_xvsgz();
                start=false;
            }
        }
        return !start;
    }
    return false;
}

bool JKQTPXXYGraph::usesColumn(int column) const
{
    return (column==xColumn2) || JKQTPXXYGraph::usesColumn(column);
}

int JKQTPXXYGraph::getXColumn2() const
{
    return xColumn2;
}

int JKQTPXXYGraph::getKey2Column() const
{
    return getXColumn2();
}

void JKQTPXXYGraph::setKey2Column(int __value)
{
    setXColumn2(__value);
}

double JKQTPXXYGraph::hitTest(const QPointF &posSystem, QPointF *closestSpotSystem, QString *label, JKQTPPlotElement::HitTestMode mode) const
{
    if (parent==nullptr) return JKQTP_NAN;

    // check base-class implementation and use it, if it returns a vaid value
    const double baseclassResult=JKQTPXYGraph::hitTest(posSystem, closestSpotSystem, label, mode);
    if (JKQTPIsOKFloat(baseclassResult)) return baseclassResult;

    const JKQTPDatastore* datastore=parent->getDatastore();
    int imin=0;
    int imax=0;
    if (!getIndexRange(imin, imax)) return JKQTP_NAN;


    int closest=-1;
    double closedist=JKQTP_NAN;
    double closedistsec=JKQTP_NAN;
    QPointF closestPos;
    QPointF posF=transform(posSystem);
    for (int i=imin; i<imax; i++) {
        const QPointF x(datastore->get(static_cast<size_t>(xColumn2),static_cast<size_t>(i)), datastore->get(static_cast<size_t>(yColumn),static_cast<size_t>(i)));
        const QPointF xpix = transform(x);
        if (JKQTPIsOKFloat(xpix.x())&&JKQTPIsOKFloat(xpix.y())) {
            double d=0, dsecondary=0;
            switch (mode) {
            case HitTestXY: d=sqrt(jkqtp_sqr(xpix.x()-posF.x())+jkqtp_sqr(xpix.y()-posF.y())); dsecondary=0; break;
            case HitTestXOnly: d=fabs(xpix.x()-posF.x()); dsecondary=fabs(xpix.y()-posF.y()); break;
            case HitTestYOnly: d=fabs(xpix.y()-posF.y()); dsecondary=fabs(xpix.x()-posF.x()); break;
            }
            if (closest<0 || d<closedist || (jkqtp_approximatelyEqual(d,closedist) && dsecondary<closedistsec)) {
                closest=i;
                closedist=d;
                closedistsec=dsecondary;
                closestPos=x;
                //qDebug()<<"hitTest("<<posSystem<<"[="<<posF<<"pix]...): found closest="<<closest<<", closedist="<<closedist<<", closedistsec="<<closedistsec<<", closestPos="<<closestPos;
            }
        }
    }
    if (closest>=0) {
        if (label) *label=formatHitTestDefaultLabel(closestPos.x(), closestPos.y(), closest, datastore);
        if (closestSpotSystem) *closestSpotSystem=closestPos;
        return closedist;
    } else {
        return JKQTP_NAN;
    }

}

void JKQTPXXYGraph::setXXYColumns(size_t xCol, size_t x2Col, size_t yCol)
{
    setXYColumns(xCol, yCol);
    xColumn2=static_cast<int>(x2Col);
}

void JKQTPXXYGraph::setXXYColumns(int xCol, int x2Col, int yCol)
{
    setXYColumns(xCol, yCol);
    xColumn2=x2Col;
}

void JKQTPXXYGraph::setXColumn2(int __value)
{
    xColumn2=__value;
}

void JKQTPXXYGraph::setXColumn2(size_t __value)
{
    xColumn2=static_cast<int>(__value);
}

bool JKQTPXXYGraph::getIndexRange(int &imin, int &imax) const
{
    bool ok=JKQTPXYGraph::getIndexRange(imin, imax);
    if (ok) {
        if (parent==nullptr)  return false;
        if (xColumn2<0) return false;
        const JKQTPDatastore* datastore=parent->getDatastore();
        const int rows=static_cast<int>(datastore->getRows(static_cast<size_t>(xColumn2)));
        imax=qMin<int>(imax, rows);
    }
    return ok;
}

JKQTPXYBaselineGraph::JKQTPXYBaselineGraph(JKQTBasePlotter *parent):
    JKQTPXYGraph(parent), m_baseline(0.0)
{

}

double JKQTPXYBaselineGraph::getBaseline() const
{
    return m_baseline;
}

void JKQTPXYBaselineGraph::setBaseline(double __value)
{
    m_baseline=__value;
}

bool JKQTPXYBaselineGraph::getMinMaxWithBaseline(int dataColumn, double &minv, double &maxv, double &smallestGreaterZero)
{
    minv=0;
    maxv=0;
    smallestGreaterZero=0;
    if (getBaseline()>0) {
        smallestGreaterZero=getBaseline();
        minv=getBaseline();
        maxv=getBaseline();
    }

    if (parent==nullptr) return false;

    JKQTPDatastore* datastore=parent->getDatastore();
    int imax=0;
    int imin=0;
    if (getIndexRange(imin, imax)) {


        for (int i=imin; i<imax; i++) {
            double yv=getBaseline();
            if (JKQTPIsOKFloat(yv)) {
                if (yv>maxv) maxv=yv;
                if (yv<minv) minv=yv;
                double xvsgz;
                xvsgz=yv; SmallestGreaterZeroCompare_xvsgz();
            }
            yv=datastore->get(static_cast<size_t>(dataColumn),static_cast<size_t>(i));
            if (JKQTPIsOKFloat(yv)) {
                if (yv>maxv) maxv=yv;
                if (yv<minv) minv=yv;
                double xvsgz;
                xvsgz=yv; SmallestGreaterZeroCompare_xvsgz();
            }
        }
        return true;
    }
    return false;
}

bool JKQTPXYBaselineGraph::getMinMaxWithErrorsAndBaseline(int dataColumn, int errorColumn, int errorColumnLower, bool errorSymmetric, double &minv, double &maxv, double &smallestGreaterZero)
{
    bool start=false;
    minv=getBaseline();
    maxv=getBaseline();
    smallestGreaterZero=0;
    if (getBaseline()>0) {
        smallestGreaterZero=getBaseline();
        minv=getBaseline();
        maxv=getBaseline();
    }

    if (parent==nullptr) return false;

    const JKQTPDatastore* datastore=parent->getDatastore();
    int imax=0;
    int imin=0;
    if (getIndexRange(imin, imax)) {


        for (int i=imin; i<imax; i++) {
            const double yv=datastore->get(static_cast<size_t>(dataColumn),static_cast<size_t>(i))+getErrorU(i, datastore, errorColumn);
            const double yvv=datastore->get(static_cast<size_t>(dataColumn),static_cast<size_t>(i))-getErrorL(i, datastore, errorColumn, errorColumnLower, errorSymmetric);
            if (JKQTPIsOKFloat(yv) && JKQTPIsOKFloat(yvv) ) {
                if (start || yv>maxv) maxv=yv;
                if (start || yv<minv) minv=yv;
                double xvsgz;
                xvsgz=yv; SmallestGreaterZeroCompare_xvsgz();
                if (start || yvv>maxv) maxv=yvv;
                if (start || yvv<minv) minv=yvv;
                xvsgz=yvv; SmallestGreaterZeroCompare_xvsgz();
                start=false;
            }
        }
        return !start;
    }
    return false;
}

double JKQTPXYBaselineGraph::getErrorU(int i, const JKQTPDatastore *ds, int xErrorColumn) const
{
    if (ds && xErrorColumn>=0 && i>=0 && i<static_cast<int>(ds->getRows(xErrorColumn))) {
        return ds->get(xErrorColumn, static_cast<size_t>(i));
    }
    return 0.0;
}

double JKQTPXYBaselineGraph::getErrorL(int i, const JKQTPDatastore *ds, int xErrorColumn, int xErrorColumnLower, bool xErrorSymmetric) const
{
    if (ds) {
        if (xErrorSymmetric) {
            if (xErrorColumn>=0 && i>=0 && i<static_cast<int>(ds->getRows(xErrorColumn))) return ds->get(xErrorColumn, static_cast<size_t>(i));
        } else {
            if (xErrorColumnLower>=0 && i>=0 && i<static_cast<int>(ds->getRows(xErrorColumnLower))) return ds->get(xErrorColumnLower, static_cast<size_t>(i));
        }
    }
    return 0.0;
}


JKQTPPlotAnnotationElement::JKQTPPlotAnnotationElement(JKQTBasePlotter *parent):
    JKQTPPlotElement(parent)
{

}

JKQTPPlotAnnotationElement::~JKQTPPlotAnnotationElement()
{

}

JKQTPXYAndVectorGraph::JKQTPXYAndVectorGraph(JKQTBasePlotter *parent):
    JKQTPXYGraph(parent),
    vectorDataLayout(DefaultVectorDataLayout),
    dxColumn(-1), dyColumn(-1),
    angleColumn(-1), lengthColumn(-1)
{

}

bool JKQTPXYAndVectorGraph::getXMinMax(double &minx, double &maxx, double &smallestGreaterZero)
{
    return JKQTPXYGraph::getXMinMax(minx,maxx,smallestGreaterZero);
}

bool JKQTPXYAndVectorGraph::getYMinMax(double &miny, double &maxy, double &smallestGreaterZero)
{
    return JKQTPXYGraph::getYMinMax(miny,maxy,smallestGreaterZero);
}

bool JKQTPXYAndVectorGraph::usesColumn(int column) const
{
    if (vectorDataLayout==DeltaXDeltaYLayout) {
        return (column==dxColumn) || (column==dyColumn) || JKQTPXYGraph::usesColumn(column);
    } else {
        return (column==angleColumn) || (column==lengthColumn) || JKQTPXYGraph::usesColumn(column);
    }
}

int JKQTPXYAndVectorGraph::getDxColumn() const
{
    return dxColumn;
}

int JKQTPXYAndVectorGraph::getDyColumn() const
{
    return dyColumn;
}

int JKQTPXYAndVectorGraph::getAngleColumn() const
{
    return angleColumn;
}

int JKQTPXYAndVectorGraph::getLengthColumn() const
{
    return lengthColumn;
}

JKQTPXYAndVectorGraph::VectorDataLayout JKQTPXYAndVectorGraph::getVectorDataLayout() const
{
    return vectorDataLayout;
}

double JKQTPXYAndVectorGraph::hitTest(const QPointF &posSystem, QPointF *closestSpotSystem, QString *label, HitTestMode mode) const
{
    return JKQTPXYGraph::hitTest(posSystem, closestSpotSystem, label, mode);
}

QString JKQTPXYAndVectorGraph::formatHitTestDefaultLabel(double x, double y, int i, const JKQTPDatastore *datastore) const
{
    QString vec="\\mbox{no vector data}";
    if (datastore) {
        switch(vectorDataLayout) {
        case DeltaXDeltaYLayout: {
            const double dx=datastore->get(static_cast<size_t>(dxColumn),static_cast<size_t>(i));
            const double dy=datastore->get(static_cast<size_t>(dyColumn),static_cast<size_t>(i));
            vec="\\vec{\\mbox{\\delta}}=("+jkqtp_floattolatexqstr(dx, 3)+"{\\;},{\\;}"+jkqtp_floattolatexqstr(dy, 3)+")";
        } break;
        case AngleAndLengthLayout: {
            const double a=datastore->get(static_cast<size_t>(angleColumn),static_cast<size_t>(i));
            const double l=(lengthColumn<0) ? 1.0 : datastore->get(static_cast<size_t>(lengthColumn),static_cast<size_t>(i));
            vec= "\\angle="+jkqtp_floattolatexqstr(a/M_PI*180.0, 1)+"\\degree{\\;},{\\;}l="+jkqtp_floattolatexqstr(l, 3);
        } break;
        }
    }
    return QString("\\ensuremath{\\left|\\stackrel{x=({\\:}%1{\\;},{\\;}%2{\\:}),}{ %3 }\\right.").arg(jkqtp_floattolatexqstr(x, 3)).arg(jkqtp_floattolatexqstr(y, 3)).arg(vec);
}

void JKQTPXYAndVectorGraph::setDxColumn(int col)
{
    dxColumn=col;
    vectorDataLayout=DeltaXDeltaYLayout;
}

void JKQTPXYAndVectorGraph::setDyColumn(int col)
{
    dyColumn=col;
    vectorDataLayout=DeltaXDeltaYLayout;
}

void JKQTPXYAndVectorGraph::setDxDyColumn(int colDx, int colDy)
{
    dxColumn=colDx;
    dyColumn=colDy;
    vectorDataLayout=DeltaXDeltaYLayout;

}

void JKQTPXYAndVectorGraph::setAngleColumn(int col)
{
    angleColumn=col;
    vectorDataLayout=AngleAndLengthLayout;
}

void JKQTPXYAndVectorGraph::setAngleAndLengthColumn(int colAngle, int colLength)
{
    angleColumn=colAngle;
    lengthColumn=colLength;
    vectorDataLayout=AngleAndLengthLayout;
}

void JKQTPXYAndVectorGraph::setLengthColumn(int col)
{
    lengthColumn=col;
    vectorDataLayout=AngleAndLengthLayout;
}

QPointF JKQTPXYAndVectorGraph::getVectorDxDy(int i) const
{

    const JKQTPDatastore* datastore=parent->getDatastore();
    if (datastore) {
        switch(vectorDataLayout) {
        case DeltaXDeltaYLayout: {
            const double dx=datastore->get(static_cast<size_t>(dxColumn),static_cast<size_t>(i));
            const double dy=datastore->get(static_cast<size_t>(dyColumn),static_cast<size_t>(i));
            return QPointF(dx,dy);
        } break;
        case AngleAndLengthLayout: {
            const double a=datastore->get(static_cast<size_t>(angleColumn),static_cast<size_t>(i));
            const double l=(lengthColumn<0) ? 1.0 : datastore->get(static_cast<size_t>(lengthColumn),static_cast<size_t>(i));
            return QPointF(l*cos(a),l*sin(a));
        } break;
        }
    }
    return QPointF(JKQTP_NAN, JKQTP_NAN);
}

bool JKQTPXYAndVectorGraph::getIndexRange(int &imin, int &imax) const
{
    bool ok=JKQTPXYGraph::getIndexRange(imin, imax);
    if (ok) {
        if (vectorDataLayout==DeltaXDeltaYLayout) {
            if (parent==nullptr)  return false;
            if (dxColumn<0||dyColumn<0) return false;
            const JKQTPDatastore* datastore=parent->getDatastore();
            const int rowsDX=static_cast<int>(datastore->getRows(static_cast<size_t>(dxColumn)));
            const int rowsDY=static_cast<int>(datastore->getRows(static_cast<size_t>(dyColumn)));
            imax=qMin<int>(imax, qMin<int>(rowsDX,rowsDY));
        } else  if (vectorDataLayout==AngleAndLengthLayout) {
            if (parent==nullptr)  return false;
            if (angleColumn<0 && lengthColumn<0) return false;
            if (angleColumn<0) return false;
            const JKQTPDatastore* datastore=parent->getDatastore();
            const int rowsA=static_cast<int>(datastore->getRows(static_cast<size_t>(angleColumn)));
            const int rowsL=(lengthColumn>=0)?static_cast<int>(datastore->getRows(static_cast<size_t>(lengthColumn))):0;
            imax=qMin<int>(imax, qMin<int>(rowsA,rowsL));
        }
    }
    return ok;
}

JKQTPXGraph::JKQTPXGraph(JKQTBasePlotter *parent):
    JKQTPGraph(parent), xColumn(-1), sortData(Unsorted)
{

}

bool JKQTPXGraph::getXMinMax(double &minx, double &maxx, double &smallestGreaterZero)
{
    bool start=true;
    minx=0;
    maxx=0;
    smallestGreaterZero=0;

    if (parent==nullptr)  return false;

    const JKQTPDatastore* datastore=parent->getDatastore();
    int imin=0;
    int imax=0;
    getIndexRange(imin, imax);

    for (int i=imin; i<imax; i++) {
        double xv=datastore->get(static_cast<size_t>(xColumn),static_cast<size_t>(i));
        if (JKQTPIsOKFloat(xv)) {
            if (start || xv>maxx) maxx=xv;
            if (start || xv<minx) minx=xv;
            double xvsgz;
            xvsgz=xv; SmallestGreaterZeroCompare_xvsgz();
            start=false;
        }
    }
    return !start;
}

bool JKQTPXGraph::usesColumn(int column) const
{
    return (column==xColumn);
}

int JKQTPXGraph::getXColumn() const
{
    return xColumn;
}

JKQTPXGraph::DataSortOrder JKQTPXGraph::getDataSortOrder() const
{
    return sortData;
}

int JKQTPXGraph::getKeyColumn() const
{
    return getXColumn();
}

void JKQTPXGraph::setDataSortOrder(int __value)
{
    sortData=static_cast<DataSortOrder>(__value);
}

void JKQTPXGraph::setDataSortOrder(DataSortOrder __value)
{
    sortData=__value;
}

void JKQTPXGraph::setXColumn(int __value)
{
    xColumn = static_cast<int>(__value);
}

void JKQTPXGraph::setXColumn(size_t __value)
{
    xColumn = static_cast<int>(__value);
}

void JKQTPXGraph::setKeyColumn(int __value)
{
    setXColumn(__value);
}

void JKQTPXGraph::intSortData()
{
    sortedIndices.clear();



    if (parent==nullptr)  return ;
    if (sortData==JKQTPXGraph::Unsorted) return ;

    JKQTPDatastore* datastore=parent->getDatastore();
    int imin=0;
    int imax=0;
    getIndexRange(imin, imax);

    QVector<double> datas;

    if (sortData==SortedX) {

        for (int i=0; i<imax; i++) {
            double xv=datastore->get(static_cast<size_t>(xColumn),static_cast<size_t>(i));
            sortedIndices<<i;
            datas<<xv;
        }

        jkqtpQuicksortDual(datas.data(), sortedIndices.data(), datas.size());

    }
}

bool JKQTPXGraph::getIndexRange(int &imin, int &imax) const
{
    if (parent==nullptr)  return false;
    if (xColumn>=0) {
        JKQTPDatastore* datastore=parent->getDatastore();
        imin=0;
        imax=static_cast<int>(datastore->getRows(static_cast<size_t>(xColumn)));
        // ensure correct order, i.e. imin<=imax
        if (imax<imin) qSwap(imin,imax);
        // ranges are always >=0
        if (imin<0) imin=0;
        if (imax<0) imax=0;
        return true;
    }
    return false;
}
