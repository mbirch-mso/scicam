#include "DisplayContrast.h"
#include <iostream>



DisplayContrast::DisplayContrast(EdtQtImageItem *pWindow)
{
    m_channel = -1;
    m_pWindow = pWindow;
    m_nChannels = 0;
    m_mode = EdtLut::Gamma;
    m_gammaSliderValue = 1;

}

void DisplayContrast::set_current_window(EdtQtImageItem *pWindow)
{
    m_pWindow = pWindow;
}

void DisplayContrast::set_channel(int band)
{
    m_channel = band;
}

void DisplayContrast::set_map_type(EdtLut::MapType type)
{
    m_mode = type;

    if (m_mode == EdtLut::Copy)
        m_pWindow->ClearTransform();

    else
    {
        m_pWindow->SetTransformActive();
        std::cout << "setting" << std::endl;
    }

    if (m_pWindow->pLut)
        m_pWindow->pLut->SetMapType(type);

    update_display(true);

    update();
}

void DisplayContrast::update_lut_values()
{
    if(m_channel >= 0)
       m_pWindow->pLut->SetGamma(m_gammaSliderValue, m_channel);
    else
        for (int band=0; band<m_nChannels; band++)
            m_pWindow->pLut->SetGamma(m_gammaSliderValue, band);
}

void DisplayContrast::update(EdtQtImageItem *pWindow)
{
    if (pWindow && (pWindow != m_pWindow))
        m_pWindow = pWindow;

    EdtImage * pImage = m_pWindow->GetImage();

    m_nChannels = pImage->GetNColors();

}

void DisplayContrast::update_display(bool redraw_image)
{
    update_lut_values();

    m_pWindow->pLut->UpdateMap(this->m_pWindow->GetImage());

    if (redraw_image)
        m_pWindow->SetNeedsRedraw(redraw_image);
}

void DisplayContrast::set_gamma_slider_value(double value)
{
    m_gammaSliderValue = value;
}
