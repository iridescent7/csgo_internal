#include "slider2.h"

template <typename T>
void Slider2<T>::Scale()
{
	width  = DYN_X(200);
	height = DYN_Y(36);

	_bar_width_offset = width - DYN_X(4);
	_bar_height = DYN_Y(7);
}

template <typename T>
void Slider2<T>::Paint()
{
	TLVertex* pVertices = new TLVertex[_valueB ? 12 : 6];

	if (_text_w < 0)
		g_renderer->GetTextSizeA(g_fonts.text, this->_text, &_text_w, nullptr);

	_bar_x  = x + DYN_X(2);
	_bar_y  = y + DYN_Y(10);
	_text_y = _bar_y + _bar_height - 2;

	// Bar
	g_renderer->DrawSoftFilledRect(x,     _bar_y,     width,     _bar_height, MENU_COLOR(g_colors.outline));
	g_renderer->DrawSoftFilledRect(x + 1, _bar_y + 1, width - 2, _bar_height - 2, MENU_COLOR(g_colors.foreground));

	// == VALUE A =============================================================================================================================================

	float _valueA_pX = _bar_x + (((float)*_valueA / (float)_max) * _bar_width_offset);
	float _textA_x = x;

	float _pointA_offset = DYN_X((_valueB ? 12 : 6));

	pVertices[0] = TLV_AUTO(_valueA_pX,                                          _bar_y + DYN_Y(6), MENU_COLOR(g_colors.outline));
	pVertices[1] = TLV_AUTO(_valueA_pX + (_valueB ? 0 : _pointA_offset),         _bar_y - DYN_Y(6), MENU_COLOR(g_colors.outline));
	pVertices[2] = TLV_AUTO(_valueA_pX - _pointA_offset,                         _bar_y - DYN_Y(6), MENU_COLOR(g_colors.outline));
	
	pVertices[3] = TLV_AUTO(_valueA_pX,                                          _bar_y + DYN_Y(5), MENU_COLOR(g_colors.foreground));
	pVertices[4] = TLV_AUTO(_valueA_pX + (_valueB ? 0 : _pointA_offset - 1) - 1, _bar_y - DYN_Y(5), MENU_COLOR(g_colors.foreground));
	pVertices[5] = TLV_AUTO(_valueA_pX - _pointA_offset + 2,                     _bar_y - DYN_Y(5), MENU_COLOR(g_colors.foreground));
	
	if (!_valueB)
	{
		g_renderer->GetTextSizeA(g_fonts.text, std::to_string(*_valueA), &_textA_x, nullptr);
		_textA_x = x + width - _textA_x;
	}

	g_renderer->DrawOutlinedText(g_fonts.text, std::to_string(*_valueA), _textA_x, _text_y, MENU_COLOR(g_colors.text), MENU_COLOR(g_colors.outline));

	// == VALUE B =============================================================================================================================================

	if (_valueB)
	{
		float _valueB_pX = _bar_x + (((float)*_valueB / (float)_max) * _bar_width_offset);
		
		pVertices[6]  = TLV_AUTO(_valueB_pX,             _bar_y + DYN_Y(6), MENU_COLOR(g_colors.outline));
		pVertices[7]  = TLV_AUTO(_valueB_pX + DYN_X(12), _bar_y - DYN_Y(6), MENU_COLOR(g_colors.outline));
		pVertices[8]  = TLV_AUTO(_valueB_pX,             _bar_y - DYN_Y(6), MENU_COLOR(g_colors.outline));
														 
		pVertices[9]  = TLV_AUTO(_valueB_pX,             _bar_y + DYN_Y(5), MENU_COLOR(g_colors.foreground));
		pVertices[10] = TLV_AUTO(_valueB_pX + DYN_X(10), _bar_y - DYN_Y(5), MENU_COLOR(g_colors.foreground));
		pVertices[11] = TLV_AUTO(_valueB_pX + 1,         _bar_y - DYN_Y(5), MENU_COLOR(g_colors.foreground));
		
		float _textB_w;
		g_renderer->GetTextSizeA(g_fonts.text, std::to_string(*_valueB), &_textB_w, nullptr);
		g_renderer->DrawOutlinedText(g_fonts.text, std::to_string(*_valueB), x + width - _textB_w, _text_y, MENU_COLOR(g_colors.text), MENU_COLOR(g_colors.outline));
	}
	// ========================================================================================================================================================

	g_renderer->DrawPrimitiveUP(D3DPT_TRIANGLELIST, _valueB ? 4 : 2, pVertices, sizeof(TLVertex));

	// Text
	g_renderer->DrawOutlinedText(g_fonts.text, _text, x + (_valueB ? (width / 2 - _text_w / 2) : 0), _text_y, MENU_COLOR(g_colors.text), MENU_COLOR(g_colors.outline));

	delete[] pVertices;
}

template <typename T>
bool Slider2<T>::Update(UINT msg, WPARAM wParam, LPARAM lParam)
{
	return false;
}