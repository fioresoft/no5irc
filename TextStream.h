#pragma once
#include "stdafx.h"

static COLORREF ColorFromCode(int code)
{
	switch (code)
	{
	case 0:
		return RGB(255, 255, 255);
	case 1:
		return 0;
	case 2:
		return RGB(0, 0, 127);
	case 3:
		return RGB(0, 147, 0);
	case 4:
		return RGB(255, 0, 0);
	case 5:
		return RGB(127, 0, 0);
	case 6:
		return RGB(156, 0, 156);
	case 7:
		return RGB(252, 127, 0);
	case 8:
		return RGB(255, 255, 0);
	case 9:
		return RGB(0, 252, 0);
	case 10:
		return RGB(0, 147, 147);
	case 11:
		return RGB(0, 255, 255);
	case 12:
		return RGB(0, 0, 252);
	case 13:
		return RGB(255, 0, 255);
	case 14:
		return RGB(127, 127, 127);
	case 15:
		return RGB(210, 210, 210);
	default:
		return 0;
	}
}

class CTextAtom
{
public:
	union
	{
		LPCTSTR psz;
		//COLORREF clr;
		int color_code;
		bool on;
	};
	CString str;
	enum class type
	{
		TEXT,
		STR,
		COLOR_FORE,
		COLOR_BACK,
		NONE,
		CODE,
		BOLD,
		ITALIC,
		UNDERLINE,
		STRIKEOUT,
		RESET_COLOR,
		MONOSAPCE,
	};
	type ty;

	CTextAtom()
	{
		ty = type::NONE;
	}

	virtual ~CTextAtom()
	{
		switch (ty)
		{
		case type::TEXT:
			break;
		}
	}
};

class CTextStream
{
public:
	NO5TL::CPointerArray<CTextAtom> atoms;
	int color_fore;
	int color_back;
public:
	CTextStream()
	{
		color_fore = 0;
	}
	void AddText(LPCTSTR txt)
	{
		CTextAtom* atom = new CTextAtom();
		atom->psz = txt;
		atom->ty = CTextAtom::type::TEXT;
		atoms.Add(atom);
	}
	void AddStr(const CString& s)
	{
		CTextAtom* atom = new CTextAtom();
		atom->str = s;
		atom->ty = CTextAtom::type::STR;
		atoms.Add(atom);
	}
	void AddColorForeCode(int x)
	{
		CTextAtom* atom = new CTextAtom();
		atom->color_code = x;
		atom->ty = CTextAtom::type::COLOR_FORE;
		atoms.Add(atom);
		color_fore = x;
	}
	void AddColorBackCode(int x)
	{
		CTextAtom* atom = new CTextAtom();
		atom->color_code = x;
		atom->ty = CTextAtom::type::COLOR_BACK;
		atoms.Add(atom);
		color_back = x;
	}
	void AddBold(bool bOn)
	{
		CTextAtom* atom = new CTextAtom();
		atom->on = bOn;
		atom->ty = CTextAtom::type::BOLD;
		atoms.Add(atom);
	}
	void AddItalic(bool bOn)
	{
		CTextAtom* atom = new CTextAtom();
		atom->on = bOn;
		atom->ty = CTextAtom::type::ITALIC;
		atoms.Add(atom);
	}
	void AddUnderline(bool bOn)
	{
		CTextAtom* atom = new CTextAtom();
		atom->on = bOn;
		atom->ty = CTextAtom::type::UNDERLINE;
		atoms.Add(atom);
	}
	void AddStrike(bool bOn)
	{
		CTextAtom* atom = new CTextAtom();
		atom->on = bOn;
		atom->ty = CTextAtom::type::STRIKEOUT;
		atoms.Add(atom);
	}
	void AddMono(bool bOn)
	{
		CTextAtom* atom = new CTextAtom();
		atom->on = bOn;
		atom->ty = CTextAtom::type::MONOSAPCE;
		atoms.Add(atom);
	}

	LPCTSTR GetIRCCode() const
	{
		CString res;
		CString code;

		for (int i = 0; i < atoms.GetSize(); i++)
		{
			CTextAtom* atom = atoms[i];
			switch (atom->ty)
			{
			case CTextAtom::type::TEXT:
				res += atom->psz;
				break;
			case CTextAtom::type::STR:
				res += atom->str;
				break;
			case CTextAtom::type::COLOR_FORE:
				//res += "\003" + atom->color_code;
				code.Format(L"\003%02d,%02d",atom->color_code,color_back);
				res += code;
				break;
			case CTextAtom::type::COLOR_BACK:
				//res += "\003" + color_fore;
				//res += "," + atom->color_code;
				code.Format(L"\003%02d,%02d", color_fore, atom->color_code);
				res += code;
		
				break;
			case CTextAtom::type::BOLD:
				res += "\002";
				break;
			case CTextAtom::type::ITALIC:
				res += "\x1d";
				break;
			case CTextAtom::type::UNDERLINE:
				res += "\x1f";
				break;
			case CTextAtom::type::STRIKEOUT:
				res += "\x1e";
				break;
			case CTextAtom::type::RESET_COLOR:
				res += "\003";
				break;
			case CTextAtom::type::MONOSAPCE:
				res += "\x11";
				break;

			}
		}
		return res;
	}
	void ParseIRCCode(LPCTSTR lpstrText)
	{
		const TCHAR* p = lpstrText;
		const TCHAR* pEnd = p;
		int fore, back;
		bool setfore = false, setback = false;

		while (*p) {
			if (*p == 0x3) {
				if (p > pEnd) {
					CString s(pEnd, p - pEnd);
					AddStr(s);
				}
				p++;
				if (*p >= '0' && *p <= '9') {
					fore = (*p - '0') * 10;
					setfore = true;
					p++;
					if (*p >= '0' && *p <= '9') {
						fore += (*p - '0');
						pEnd = ++p;
					}
					else {
						pEnd = p;
						fore /= 10;
					}
					if (*p == ',') {
						++p;
						if (*p >= '0' && *p <= '9') {
							back = (*p - '0') * 10;
							setback = true;
							p++;
							if (*p >= '0' && *p <= '9') {
								back += (*p - '0');
								pEnd = ++p;
							}
							else {
								pEnd = p;
								back /= 10;
							}
						}
					}
					else
						pEnd = p;
				}
				else {
					//ResetFormat();
					pEnd = p;
					continue;
				}
				if (setfore) {
					AddColorForeCode(fore);
					setfore = false;
				}
				if (setback) {
					AddColorBackCode(back);
					setback = false;
				}
			}
			else if (*p == 0x0f) { // reset
				if (p > pEnd) {
					CString s(pEnd, p - pEnd);
					AddStr(s);
				}
				ResetFormat();
				pEnd = ++p;
			}
			else if (*p == 0x11) { // monospaced
				if (p > pEnd) {
					CString s(pEnd, p - pEnd);
					AddStr(s);
				}
				AddMono(true);
				pEnd = ++p;
			}
			else if (*p == 0x1E) {
				if (p > pEnd) {
					CString s(pEnd, p - pEnd);
					AddStr(s);
				}
				AddStrike(true);
				pEnd = ++p;
			}
			else if (*p == 0x1F) {
				if (p > pEnd) {
					CString s(pEnd, p - pEnd);
					AddStr(s);
				}
				AddUnderline(true);
				pEnd = ++p;
			}
			else if (*p == 0x1D) {
				if (p > pEnd) {
					CString s(pEnd, p - pEnd);
					AddStr(s);
				}
				AddItalic(true);
				pEnd = ++p;
			}
			else if (*p == 0x2) {
				if (p > pEnd) {
					CString s(pEnd, p - pEnd);
					AddStr(s);
				}
				AddBold(true);
				pEnd = ++p;
			}
			else if (*(p + 1) == 0 && (p > pEnd))
			{
				CString s(pEnd, p - pEnd + 1);
				AddStr(s);
				//ResetFormat();
				p++;
			}
			else
				p++;
		}
	}
	void SendStreamToRichEdit(NO5TL::CNo5RichEdit& ctrl)
	{
		for (int i = 0; i < atoms.GetSize(); i++)
		{
			CTextAtom* atom = atoms[i];
			switch (atom->ty)
			{
			case CTextAtom::type::TEXT:
				ctrl.AppendText(atom->psz);
				break;
			case CTextAtom::type::STR:
				ctrl.AppendText(atom->str);
				break;
			case CTextAtom::type::COLOR_FORE:
				ctrl.SetTextColor(ColorFromCode(atom->color_code));
				break;
			case CTextAtom::type::COLOR_BACK:
				ctrl.SetTextBkColor(ColorFromCode(atom->color_code));
				break;
			case CTextAtom::type::BOLD:
				ctrl.SwitchBold();
				break;
			case CTextAtom::type::ITALIC:
				ctrl.SwitchItalic();
				break;
			case CTextAtom::type::UNDERLINE:
				ctrl.SwitchUnderline();
				break;
			case CTextAtom::type::STRIKEOUT:
				ctrl.SwitchStrike();
				break;
			case CTextAtom::type::RESET_COLOR:
				ctrl.SetTextBkColor(0xffffff);
				ctrl.SetTextColor(0);
				break;
			case CTextAtom::type::MONOSAPCE:
				ctrl.SetTextFontName(L"Courier", SCF_SELECTION);
				break;

			}
		}
	}
	void SendStreamToHtmlEdit(CHtmlCtrl & ctrl)
	{
		for (int i = 0; i < atoms.GetSize(); i++)
		{
			CTextAtom* atom = atoms[i];
			CString s;

			switch (atom->ty)
			{
			case CTextAtom::type::TEXT:
				s.Format(_T("<span style=\"color:#%06x;background-color:#%06x;\">%s"), ColorFromCode(color_fore),
					ColorFromCode(color_back), atom->psz);
				//ctrl.AppendText(atom->psz);
				ctrl.AppendHtmlText(s);
				break;
			case CTextAtom::type::STR:
				s.Format(_T("<span style=\"color:#%06x;background-color:#%06x;\">%s"), ColorFromCode(color_fore),
					ColorFromCode(color_back), atom->str);
				ctrl.AppendHtmlText(s);
				//ctrl.AppendText(atom->str);
				break;
			case CTextAtom::type::COLOR_FORE:
				//ctrl.SetTextColor(ColorFromCode(atom->color_code));
				// ctrl.AppendHtmlText(_T("<span style=\"color: #%06x"))
				color_fore = atom->color_code;
				break;
			case CTextAtom::type::COLOR_BACK:
				//ctrl.m_doc.ChangeStyleBkColor(_T("div"), ColorFromCode(atom->color_code));
				color_back = atom->color_code;
				break;
			case CTextAtom::type::BOLD:
				ctrl.AppendHtmlText(_T("<b>"));
				break;
			case CTextAtom::type::ITALIC:
				ctrl.AppendHtmlText(_T("<i>"));
				break;
			case CTextAtom::type::UNDERLINE:
				ctrl.AppendHtmlText(_T("<ins>"));
				break;
			case CTextAtom::type::STRIKEOUT:
				ctrl.AppendHtmlText(_T("<del>"));
				break;
			case CTextAtom::type::RESET_COLOR:
				break;
			case CTextAtom::type::MONOSAPCE:
				break;

			}
		}
	}
	CString GetPlainText()
	{
		CString res;

		for (int i = 0; i < atoms.GetSize(); i++)
		{
			CTextAtom* atom = atoms[i];
			switch (atom->ty)
			{
			case CTextAtom::type::TEXT:
				res += atom->psz;
				break;
			case CTextAtom::type::STR:
				res += atom->str;
				break;
			default:
				break;

			}
		}
		ATLTRACE((LPCTSTR)res);
		return res;

	}
	void ResetFormat()
	{
		CString text = GetPlainText();
		atoms.RemoveAll();
		AddStr(text);
	}
};
