#pragma once
#include "stdafx.h"

struct MarqueeOptions
{
	COLORREF fore;
	COLORREF back;
	int Elapse;
	VARIANT_BOOL bLoop;
	MarqueeOptions()
	{
		fore = 0xffffff;
		back = 0x008000;
		Elapse = 4;
		bLoop = VARIANT_FALSE;
	}
	MarqueeOptions(MarqueeOptions& mo)
	{
		if (this != &mo) {
			fore = mo.fore;
			back = mo.back;
			Elapse = mo.Elapse;
		}
	}
};
