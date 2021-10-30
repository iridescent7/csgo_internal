#include "textbox.h"

void Textbox::Scale()
{
	width           = DYN_X(200);
	height          = DYN_Y(24);
	_text_padding_x = DYN_X(4);

	g_renderer->GetTextSizeA(g_fonts.text, "A", &_char_w, nullptr);
}

void Textbox::Paint()
{
	
	g_renderer->GetTextSizeA(g_fonts.text, _text, &_text_w, &_text_h);

	if (_text.empty())
	{
		_tb_width = width;
		_tb_x     = x;
	}
	else
	{
		_tb_width = width - _text_w + _text_padding_x,
		_tb_x     = x     + _text_w + _text_padding_x;
		g_renderer->DrawOutlinedText(g_fonts.text, _text, x, _text_y, MENU_COLOR(g_colors.text), MENU_COLOR(g_colors.outline));
	}

	_text_y = y + (height / 2 - _text_h / 2);

	// Draw the input field
	g_renderer->DrawSoftFilledRect(_tb_x,     y,     _tb_width,     height,     MENU_COLOR(g_colors.outline));
	g_renderer->DrawSoftFilledRect(_tb_x + 1, y + 1, _tb_width - 2, height - 2, MENU_COLOR(g_colors.depth));
	g_renderer->DrawSoftFilledRect(_tb_x + 2, y + 2, _tb_width - 4, height - 4, MENU_COLOR(g_colors.foreground));

	// Display buffer
	int bufferDisplayLimit = static_cast<int>(_tb_width / _char_w);
	_max_char = ((bufferDisplayLimit * 2) - 2);

	int caret_idx          = _outputBuffer.length() - _caret_pos_offset;
	_outputBuffer = *inputBuffer;

	// overflow (needs improvement)
	if (_outputBuffer.length() > bufferDisplayLimit)
		_outputBuffer = _outputBuffer.substr(caret_idx - (caret_idx - bufferDisplayLimit + _caret_pos_offset), bufferDisplayLimit);

	g_renderer->DrawOutlinedText(g_fonts.text, _outputBuffer, _tb_x + DYN_X(4), _text_y, MENU_COLOR(g_colors.text), MENU_COLOR(g_colors.outline));

	// Display Caret
	if (g_focus == this)
	{
		UINT CurrentTick = _GetTickCount();

		if (CurrentTick - _last_tick_delta > 360)
		{
			_last_tick_delta = CurrentTick;
			_caret_display = !_caret_display;
		}

		if (_caret_display)
		{
			float caretPos_x = 0.f;

			if (_outputBuffer.empty())
				caretPos_x = _tb_x + DYN_X(5);
			else
			{
				g_renderer->GetTextSizeA(g_fonts.text, _outputBuffer.substr(0, caret_idx), &caretPos_x, nullptr);

				if (_caret_pos_offset == 0)
				{
					for (int x = _outputBuffer.length() - 1; x >= 0; x--)
					{
						if (_outputBuffer.at(x) == ' ')
							caretPos_x += DYN_X(6);
						else
							break;
					}
				}

				caretPos_x += _tb_x + DYN_X(5);
			}

			TLVertex _vert_caret[] =
			{
				TLV_AUTO(caretPos_x, _text_y,           MENU_COLOR(g_colors.background)),
				TLV_AUTO(caretPos_x, _text_y + _text_h, MENU_COLOR(g_colors.background))
			};

			g_renderer->DrawPrimitiveUP(D3DPT_LINELIST, 1, _vert_caret, sizeof(TLVertex));
		}
	}
}

bool Textbox::Update(UINT msg, WPARAM wParam, LPARAM lParam)
{
	
	if (msg == WM_LBUTTONDOWN || msg == WM_LBUTTONDBLCLK)
	{
		float mx = (float)GET_X_LPARAM(lParam),
			  my = (float)GET_Y_LPARAM(lParam);

		if (mx >= _tb_x && mx <= _tb_x + _tb_width
		&&  my >= y     && my <= y + height)
		{
			g_focus = this;
			return true;
		}
	}

	if (g_focus == this)
	{
		#define AUTO_INSERT(x) InsertToBuffer((x)); return true

		static bool isShiftDown = false;

		auto InsertToBuffer = [&, this](std::string input)
		{
			if (inputBuffer->length() >= _max_char)
				return;

			inputBuffer->insert((inputBuffer->length() - _caret_pos_offset), input);
		};

		switch (msg)
		{
			case WM_KEYUP:
			{
				switch (wParam)
				{
					case VK_SHIFT:
						isShiftDown = false;
						return true;
				}

				return true;
			}
			
			case WM_KEYDOWN:
			{
				switch (wParam)
				{
					
					case VK_SHIFT:
						isShiftDown = true;
						return true;
					
					case VK_ESCAPE:
						g_focus = nullptr;
						return true;

					case VK_RETURN:
						if (_enterCallback)
							_enterCallback(this);
						return true;

					case VK_BACK:
					{
						if (inputBuffer->empty())
							return true;

						int erasepos = inputBuffer->length() - 1 - _caret_pos_offset;

						if (erasepos < 0)
							return true;

						inputBuffer->erase(erasepos, 1);
						return true;
					}

					case VK_LEFT:
					{
						// seples is a joke.
						if (static_cast<int>(inputBuffer->length() - _caret_pos_offset - 1) < 0)
							return true;

						_caret_pos_offset++;
						return true;
					}

					case VK_RIGHT:
					{
						if ((inputBuffer->length() - _caret_pos_offset + 1) > inputBuffer->length())
							return true;

						_caret_pos_offset--;
						return true;
					}

					case VK_TAB:
						AUTO_INSERT("    ");

					case VK_SPACE:
						AUTO_INSERT(" ");

					case VK_MULTIPLY:   AUTO_INSERT(strenc("*"));
					case VK_ADD:        AUTO_INSERT(strenc("+"));
					case VK_SEPARATOR:  AUTO_INSERT(strenc("?"));
					case VK_SUBTRACT:   AUTO_INSERT(strenc("-"));
					case VK_DECIMAL:    AUTO_INSERT(strenc("."));
					case VK_DIVIDE:     AUTO_INSERT(strenc("/"));
   
					case VK_OEM_1:      AUTO_INSERT( isShiftDown ? strenc(":")  : strenc(";")  );
					case VK_OEM_PLUS:   AUTO_INSERT( isShiftDown ? strenc("+")  : strenc("=")  );
					case VK_OEM_MINUS:  AUTO_INSERT( isShiftDown ? strenc("_")  : strenc("-")  );
					case VK_OEM_PERIOD: AUTO_INSERT( isShiftDown ? strenc(">")  : strenc(".")  );
					case VK_OEM_COMMA:  AUTO_INSERT( isShiftDown ? strenc("<")  : strenc(",")  );
		   											 						   				  
					case VK_OEM_2:      AUTO_INSERT( isShiftDown ? strenc("?")  : strenc("/")  );    
					case VK_OEM_3:      AUTO_INSERT( isShiftDown ? strenc("~")  : strenc("`")  );    
					case VK_OEM_4:      AUTO_INSERT( isShiftDown ? strenc("{")  : strenc("[")  );    
					case VK_OEM_5:      AUTO_INSERT( isShiftDown ? strenc("|")  : strenc("\\") );   
					case VK_OEM_6:      AUTO_INSERT( isShiftDown ? strenc("}")  : strenc("]")  );    
					case VK_OEM_7:      AUTO_INSERT( isShiftDown ? strenc("\"") : strenc("'")  );    
				}

				if (wParam >= '0' && wParam <= '9')
				{
					if (isShiftDown)
					{
						switch (wParam)
						{
							case '0': AUTO_INSERT(")");
							case '1': AUTO_INSERT("!");
							case '2': AUTO_INSERT("@");
							case '3': AUTO_INSERT("#");
							case '4': AUTO_INSERT("$");
							case '5': AUTO_INSERT("%");
							case '6': AUTO_INSERT("^");
							case '7': AUTO_INSERT("&");
							case '8': AUTO_INSERT("*");
							case '9': AUTO_INSERT("(");
						}
					}
					else
					{
						AUTO_INSERT(std::to_string(wParam - '0'));
					}
				}

				if (wParam >= VK_NUMPAD0 && wParam <= VK_NUMPAD9)
				{
					AUTO_INSERT(std::to_string(wParam - VK_NUMPAD0 + 1));
				}

				std::string buf;

				if (wParam >= '0' && wParam <= 'Z')
				{
					AUTO_INSERT(buf = static_cast<char>(wParam) + (isShiftDown ? 0 : 'a' - 'A'));
				}

			}
		}

	}

	return false;
}