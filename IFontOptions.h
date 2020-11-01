#pragma once
struct IFontOptions
{
	virtual void Read(LPCTSTR file) = 0;
	virtual void Write(LPCTSTR file) = 0;
	virtual void RestoreDefaults() = 0;
	virtual void GetUserName(CColor &fore, CColor &back) = 0;
	virtual void GetUserNameDef(CColor& fore, CColor& back) = 0;
	virtual void SetUserNameDef() = 0;
	virtual void SetUserName(CColor& fore, CColor& back) = 0;
	virtual void GetUserText(CColor &fore, CColor &back) = 0;
	virtual void GetUserTextDef(CColor& fore, CColor& back) = 0;
	virtual void SetUserText(CColor& fore, CColor& back) = 0;
	virtual void SetUserTextDef() = 0;
	virtual CColor MeFore(bool def,bool bdummy) const = 0;
	virtual CColor MeBack(bool def,bool dummy) const = 0;
	virtual CColor MyTextFore(bool def,bool dummy) const = 0;
	virtual CColor MyTextBack(bool def,bool dummy) const = 0;
	virtual void MeFore( const CColor &fore) = 0;
	virtual void MeBack( const CColor &back) = 0;
	virtual void MyTextFore(const CColor &fore) = 0;
	virtual void MyTextBack( const CColor &back) = 0;
	virtual void Topic(CColor& fore, CColor& back, bool def) const = 0;
	virtual void Topic(const CColor& fore,const CColor &back) = 0;
	virtual void motd(CColor& fore, CColor& back, bool def) const = 0;
	virtual void motd(const CColor& fore, const CColor& back) = 0;
	virtual CColor UserEnters(bool def, bool dummy) const = 0;
	virtual void UserEnters(const CColor& clr) = 0;
	virtual CColor UserLeaves(bool def, bool dummy) const = 0;
	virtual void UserLeaves(const CColor& clr) = 0;
	virtual void CopyTo(IFontOptions** ppOptions) = 0;
};

 BOOL CreateFontOptions(IFontOptions** ppOptions);
 void DestroyFontOptions(IFontOptions** ppOptions);
