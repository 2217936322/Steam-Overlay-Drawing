#include "CDraw.h"

#define SAFEDRAW()														\
if (!pDevice)															\
{																		\
	gBase.Fatal("CDrawManager recieved an invalid drawing device.");	\
	return;																\
}

void CDrawManager::CreateFonts()
{
	SAFEDRAW();

	HRESULT result = D3DXCreateFont(pDevice, 20, 0, FW_NORMAL, 1, false, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, ANTIALIASED_QUALITY, DEFAULT_PITCH, "Tahoma", &Tahoma);
	if (!SUCCEEDED(result))
		gBase.Fatal("Couldn't initialize fonts with the drawing device.");
}

void CDrawManager::Rect(long x, long y, long w, long h, Color color)
{
	SAFEDRAW();
	
	D3DRECT pos = { x, y, x + w, y + h };
	pDevice->Clear(1, &pos, D3DCLEAR_TARGET | D3DCLEAR_TARGET, color.ARGB(), 0, 0);
}

void CDrawManager::OutlineRect(long x, long y, long w, long h, Color color)
{
	SAFEDRAW();

	Line(x, y, x + w, y, color);
	Line(x, y, x, y + h, color);
	Line(x, y + h, x + w, y + h, color);
	Line(x + w, y, x + w, y + h, color);
}

void CDrawManager::Line(long x1, long y1, long x2, long y2, Color color, float width)
{
	SAFEDRAW();

	static ID3DXLine* line = nullptr;
	if (!line)
		D3DXCreateLine(pDevice, &line);

	D3DXVECTOR2 points[2] = { { float(x1), float(y1) }, { float(x2), float(y2) } };
	line->SetWidth(width);
	line->Draw(points, 2, color.ARGB());
}

RECT CDrawManager::Text(long x, long y, const char* szText, Color color, ID3DXFont* Font)
{
	if (!pDevice)
	{
		gBase.Fatal("CDrawManager recieved an invalid drawing device.");
		return RECT();
	}

	if (!Font)
		Font = Tahoma;

	RECT pos = { x, y, x, y };
	Font->DrawTextA(NULL, szText, -1, &pos, DT_CALCRECT, color.ARGB());
	Font->DrawTextA(NULL, szText, -1, &pos, DT_LEFT, color.ARGB());
	return pos;
}

RECT CDrawManager::TextSize(const char* szText, ID3DXFont* Font)
{
	if (!pDevice)
	{
		gBase.Fatal("CDrawManager recieved an invalid drawing device.");
		return RECT();
	}

	if (!Font)
		Font = Tahoma;

	RECT pos = { 0, 0, 0, 0};
	Font->DrawTextA(NULL, szText, -1, &pos, DT_CALCRECT, D3DCOLOR_XRGB(0, 0, 0));
	return pos;
}