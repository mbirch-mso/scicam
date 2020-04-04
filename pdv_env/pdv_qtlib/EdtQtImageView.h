#ifndef EDTQTIMAGEVIEW_H
#define EDTQTIMAGEVIEW_H


#include "EdtQtImageScene.h"
#include "EdtQtImageItem.h"

#include <QGraphicsView>
class EdtQtImageView : public QGraphicsView
{
	Q_OBJECT
 
    EdtQtImageScene *scene;

    double base_zoom;
    bool auto_scale;

public:
	EdtQtImageView(QWidget *parent = 0);
	virtual ~EdtQtImageView();

    virtual void Setup();

    EdtQtImageItem *ImageWindow(const int index = 0);

    virtual EdtQtImageItem *AddImageItem();

    void Refresh();

    void resizeEvent(QResizeEvent *event);

    double GetMeanUpdate();

    void GetCursorValue(QPoint &pt, QString &tag);

    bool GetAutoScale();

    void SetAutoScale(const bool ascale);

    void SetZoom(const double scale);

    void Scale(const double scale);

    double GetZoom();

    void SetDataImage(EdtImage *img);

    EdtImage *GetDataImage();

    EdtQtImageScene * Scene()
    {
        return scene;
    }

};

#endif // EDTQTIMAGEVIEW_H
