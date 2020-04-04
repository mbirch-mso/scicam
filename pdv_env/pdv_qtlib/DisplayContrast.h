#ifndef DISPLAYCONTRAST_H
#define DISPLAYCONTRAST_H

#include "edtimage/EdtImage.h"
#include "EdtQtLiveDisplay.h"
#include "EdtQtImageItem.h"
#include "edtimage/EdtLut.h"



class DisplayContrast
{

public:
    DisplayContrast(EdtQtImageItem * pWindow);
    void update(EdtQtImageItem * pWindow = NULL);
    void set_current_window(EdtQtImageItem * pWindow);
    void set_map_type(EdtLut::MapType type);
    void update_lut_values(void);
    void update_display(bool redraw_image);
    void set_channel(int band);
    void set_gamma_slider_value(double value);

private:
    EdtQtImageItem * m_pWindow;
    EdtImage * m_pImage;

    int m_channel;
    int m_nChannels;
    double m_gammaSliderValue;

    EdtLut::MapType m_mode;
};

#endif //DISPLAYCONTRAST_H 
