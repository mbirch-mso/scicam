// RGBLut.h: interface for the CRGBLut class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RGBLUT_H__D5E81102_824F_11D3_8BD5_00104B357776__INCLUDED_)
#define AFX_RGBLUT_H__D5E81102_824F_11D3_8BD5_00104B357776__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CRGBLut  
{

	int m_nSize;
	RGBQUAD * m_pRGB;
	BOOL m_bOwned;

public:

	CRGBLut(int length = 0, RGBQUAD *pRGB = NULL)
	{
		m_pRGB = NULL;
		m_bOwned = FALSE;
		m_nSize = 0;
		Create(length, pRGB);

	}

	// Copy constructor

	CRGBLut(CRGBLut &RGBLut)
	{
		m_pRGB = NULL;
		m_nSize = 0;
		m_bOwned = FALSE;
		Copy(RGBLut);
	}

	void Copy(CRGBLut &RGBLut)
	{
		if (RGBLut.GetSize())
		{
			if (!GetRGB() || GetSize() != RGBLut.GetSize())
				Create(RGBLut.GetSize(),NULL);

			memcpy(m_pRGB, RGBLut.GetRGB(), RGBLut.GetSize() * sizeof(RGBQUAD));

		}
	}

	virtual ~CRGBLut()
	{
		Create(0,0);
	}

	void Create(int length = 256, RGBQUAD *pRGB = NULL)
	{
		if (m_pRGB && m_bOwned)
		{
			delete m_pRGB;
			m_bOwned = FALSE;
		}

		m_nSize = length;
		if (pRGB)
		{
			m_pRGB = pRGB;
			m_bOwned = FALSE;
		}
		else
		{
			if (m_nSize)
			{
				// initialize to 0
				m_pRGB = new RGBQUAD[m_nSize];
				memset(m_pRGB, 0, m_nSize * sizeof(RGBQUAD));
			}

			else
				m_pRGB = NULL;
			m_bOwned = TRUE;
		}
	}

	RGBQUAD * GetRGB()
	{
		return m_pRGB;
	}

	int GetSize() const
	{
		return m_nSize;
	}


};

#endif // !defined(AFX_RGBLUT_H__D5E81102_824F_11D3_8BD5_00104B357776__INCLUDED_)
