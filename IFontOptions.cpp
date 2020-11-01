#include "stdafx.h"
#include "IFontOptions.h"

struct CFontOptions : public IFontOptions
{
	CFontOptions()
	{
		RestoreDefaults();
	}
	virtual void Read(LPCTSTR file)
	{
		CPrivateIniFile ini;
		BOOL res = FALSE;

		if (ini.Create(file)) {
			m_UserNameFore = (COLORREF)ini.GetInt(_T("visual"), _T("user_fore"), Colors::BLUE);
			m_UserNameBack = (COLORREF)ini.GetInt(_T("visual"), _T("user_back"), Colors::WHITE);
			m_UserTextFore = (COLORREF)ini.GetInt(_T("visual"), _T("text_fore"), Colors::BLACK);
			m_UserTextBack = (COLORREF)ini.GetInt(_T("visual"), _T("text_back"), Colors::WHITE);
			meFore = (COLORREF)ini.GetInt(_T("visual"), _T("me_fore"),Colors::BLUE);
			meBack = (COLORREF)ini.GetInt(_T("visual"), _T("me_back"), Colors::WHITE);
			myTextFore = (COLORREF)ini.GetInt(_T("visual"), _T("mytext_fore"), Colors::BLACK);
			myTextBack = (COLORREF)ini.GetInt(_T("visual"), _T("mytext_back"), Colors::WHITE);
			topicFore = (COLORREF)ini.GetInt(_T("visual"), _T("topic_fore"), Colors::MARRON);
			topicBack = (COLORREF)ini.GetInt(_T("visual"), _T("topic_back"), Colors::WHITE);
			motdFore = (COLORREF)ini.GetInt(_T("visual"), _T("motd_fore"), Colors::GREEN);
			motdBack = (COLORREF)ini.GetInt(_T("visual"), _T("motd_back"), Colors::WHITE);
			userEnter = (COLORREF)ini.GetInt(_T("visual"), _T("user_enters"), Colors::GREEN);
			userLeave = (COLORREF)ini.GetInt(_T("visual"), _T("user_leaves"), Colors::GREEN);
		}
	}
	virtual void Write(LPCTSTR file)
	{
		CPrivateIniFile ini;
		BOOL res = FALSE;

		if (ini.Create(file)) {
			ini.WriteInt(_T("visual"), _T("user_fore"), m_UserNameFore);
			ini.WriteInt(_T("visual"), _T("user_back"), m_UserNameBack);
			ini.WriteInt(_T("visual"), _T("text_fore"), m_UserTextFore);
			ini.WriteInt(_T("visual"), _T("text_back"), m_UserTextBack);
			ini.WriteInt(_T("visual"), _T("me_fore"), meFore);
			ini.WriteInt(_T("visual"), _T("me_back"), meBack);
			ini.WriteInt(_T("visual"), _T("mytext_fore"), myTextFore);
			ini.WriteInt(_T("visual"), _T("mytext_back"), myTextBack);
			ini.WriteInt(_T("visual"), _T("topic_fore"), topicFore);
			ini.WriteInt(_T("visual"), _T("topic_back"), topicBack);
			ini.WriteInt(_T("visual"), _T("user_enters"), userEnter);
			ini.WriteInt(_T("visual"), _T("user_leaves"), userLeave);
		}
	}
	virtual void RestoreDefaults()
	{
		m_UserNameFore = Colors::BLUE;
		m_UserNameBack = Colors::WHITE;
		m_UserTextFore = Colors::BLACK;
		m_UserTextBack = Colors::WHITE;
		meFore = Colors::BLUE;
		meBack = Colors::WHITE;
		myTextFore = Colors::BLACK;
		myTextBack = Colors::WHITE;
		topicFore = Colors::MARRON;
		topicBack = Colors::WHITE;
		motdFore = Colors::GREEN;
		motdBack = Colors::WHITE;
		userEnter = Colors::GREEN;
		userLeave = Colors::GREEN;
	}
	virtual void GetUserName(CColor& fore, CColor& back)
	{
		fore = m_UserNameFore;
		back = m_UserNameBack;
	}
	virtual void GetUserNameDef(CColor& fore, CColor& back)
	{
		fore = Colors::BLUE;
		back = Colors::WHITE;
	}
	virtual void GetUserText(CColor& fore, CColor& back)
	{
		fore = m_UserTextFore;
		back = m_UserTextBack;
	}
	virtual void GetUserTextDef(CColor& fore, CColor& back)
	{
		fore = Colors::BLACK;
		back = Colors::WHITE;
	}
	virtual void SetUserName(CColor& fore, CColor& back)
	{
		m_UserNameFore = fore;
		m_UserNameBack = back;
	}
	virtual void SetUserNameDef()
	{
		CColor fore, back;
		GetUserNameDef(fore, back);
		SetUserName(fore, back);

	}
	virtual void SetUserText(CColor& fore, CColor& back)
	{
		m_UserTextFore = fore;
		m_UserTextBack = back;
	}
	virtual void SetUserTextDef()
	{
		CColor fore, back;
		GetUserTextDef(fore, back);
		SetUserText(fore, back);
	}
	virtual CColor MeFore(bool def,bool dummy) const
	{
		if (def) {
			return Colors::BLUE;
		}
		return meFore;
	}
	virtual CColor MeBack(bool def,bool dummy) const
	{
		if (def) {
			return Colors::WHITE;
		}
		return meBack;
	}
	virtual CColor MyTextFore(bool def,bool dummy) const
	{
		if (def) {
			return Colors::BLACK;
		}
		return myTextFore;
	}
	virtual CColor MyTextBack(bool def,bool dummy) const
	{
		if (def) {
			return Colors::WHITE;
		}
		return myTextBack;
	}
	virtual void MeFore(const CColor& fore)
	{
		meFore = fore;
	}
	virtual void MeBack(const CColor& back)
	{
		meBack = back;
	}
	virtual void MyTextFore(const CColor& fore)
	{
		myTextFore = fore;
	}
	virtual void MyTextBack(const CColor& back)
	{
		myTextBack = back;
	}
	virtual void Topic(CColor& fore, CColor& back, bool def) const
	{
		fore = def?Colors::MARRON:topicFore;
		back = def ? Colors::WHITE : topicBack;
	}
	virtual void Topic(const CColor& fore, const CColor& back)
	{
		topicFore = fore;
		topicBack = back;
	}
	virtual void motd(CColor& fore, CColor& back, bool def) const
	{
		fore = def ? Colors::GREEN : motdFore;
		back = def ? Colors::WHITE : motdBack;
	}
	virtual void motd(const CColor& fore, const CColor& back)
	{
		motdFore = fore;
		motdBack = back;
	}
	virtual CColor UserEnters(bool def, bool dummy) const
	{
		return def ? Colors::GREEN : userEnter;
	}
	virtual void UserEnters(const CColor& clr)
	{
		userEnter = clr;
	}
	virtual CColor UserLeaves(bool def, bool dummy) const
	{
		return def ? Colors::GREEN : userLeave;
	}
	virtual void UserLeaves(const CColor& clr)
	{
		userLeave = clr;
	}
	virtual void CopyTo(IFontOptions** ppOptions)
	{
		ATLASSERT(ppOptions && (*ppOptions));
		//if (*ppOptions)
		//	DestroyFontOptions(ppOptions);
		//CreateFontOptions(ppOptions);
		(*ppOptions)->SetUserName(m_UserNameFore, m_UserNameBack);
		(*ppOptions)->SetUserText(m_UserTextFore, m_UserTextBack);
		(*ppOptions)->MeFore(meFore);
		(*ppOptions)->MeBack(meBack);
		(*ppOptions)->MyTextFore(myTextFore);
		(*ppOptions)->MyTextBack(myTextBack);
		(*ppOptions)->Topic(topicFore, topicBack);
		(*ppOptions)->motd(motdFore, motdBack);
		(*ppOptions)->UserEnters(userEnter);
		(*ppOptions)->UserLeaves(userLeave);
	}
	CColor m_UserNameFore, m_UserNameBack, m_UserTextFore, m_UserTextBack;
	CColor meFore, meBack, myTextFore, myTextBack;
	CColor topicFore, topicBack, motdFore, motdBack;
	CColor userEnter, userLeave;
};

BOOL CreateFontOptions(IFontOptions** ppOptions)
{
	if (ppOptions) {
		*ppOptions = NULL;
		*ppOptions = new CFontOptions();
	}
	return TRUE;
}

void DestroyFontOptions(IFontOptions** ppOptions)
{
	if (ppOptions && *ppOptions) {
		CFontOptions* pfo = (CFontOptions *)*ppOptions;
		delete pfo;
		*ppOptions = NULL;
	}
}