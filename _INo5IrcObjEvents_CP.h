#pragma once

template<class T>
class CProxy_INo5IrcObjEvents :
	public ATL::IConnectionPointImpl<T, &__uuidof(_INo5IrcObjEvents)>
{
public:
	HRESULT Fire_OnChannelList(BSTR channel, BSTR users, BSTR topic)
	{
		HRESULT hr = S_OK;
		T* pThis = static_cast<T*>(this);
		int cConnections = m_vec.GetSize();

		for (int iConnection = 0; iConnection < cConnections; iConnection++)
		{
			pThis->Lock();
			CComPtr<IUnknown> punkConnection = m_vec.GetAt(iConnection);
			pThis->Unlock();

			IDispatch* pConnection = static_cast<IDispatch*>(punkConnection.p);

			if (pConnection)
			{
				CComVariant avarParams[3];
				avarParams[2] = channel;
				avarParams[2].vt = VT_BSTR;
				avarParams[1] = users;
				avarParams[1].vt = VT_BSTR;
				avarParams[0] = topic;
				avarParams[0].vt = VT_BSTR;
				CComVariant varResult;

				DISPPARAMS params = { avarParams, NULL, 3, 0 };
				hr = pConnection->Invoke(2, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &params, &varResult, NULL, NULL);
			}
		}
		return hr;
	}
	HRESULT Fire_OnChannelMode(BSTR channel, BSTR modes)
	{
		HRESULT hr = S_OK;
		T* pThis = static_cast<T*>(this);
		int cConnections = m_vec.GetSize();

		for (int iConnection = 0; iConnection < cConnections; iConnection++)
		{
			pThis->Lock();
			CComPtr<IUnknown> punkConnection = m_vec.GetAt(iConnection);
			pThis->Unlock();

			IDispatch* pConnection = static_cast<IDispatch*>(punkConnection.p);

			if (pConnection)
			{
				CComVariant avarParams[2];
				avarParams[1] = channel;
				avarParams[1].vt = VT_BSTR;
				avarParams[0] = modes;
				avarParams[0].vt = VT_BSTR;
				CComVariant varResult;

				DISPPARAMS params = { avarParams, NULL, 2, 0 };
				hr = pConnection->Invoke(3, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &params, &varResult, NULL, NULL);
			}
		}
		return hr;
	}
	HRESULT Fire_OnUserMode(BSTR user, BSTR modes)
	{
		HRESULT hr = S_OK;
		T* pThis = static_cast<T*>(this);
		int cConnections = m_vec.GetSize();

		for (int iConnection = 0; iConnection < cConnections; iConnection++)
		{
			pThis->Lock();
			CComPtr<IUnknown> punkConnection = m_vec.GetAt(iConnection);
			pThis->Unlock();

			IDispatch* pConnection = static_cast<IDispatch*>(punkConnection.p);

			if (pConnection)
			{
				CComVariant avarParams[2];
				avarParams[1] = user;
				avarParams[1].vt = VT_BSTR;
				avarParams[0] = modes;
				avarParams[0].vt = VT_BSTR;
				CComVariant varResult;

				DISPPARAMS params = { avarParams, NULL, 2, 0 };
				hr = pConnection->Invoke(4, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &params, &varResult, NULL, NULL);
			}
		}
		return hr;
	}
	HRESULT Fire_OnTopic(BSTR channel, BSTR topic)
	{
		HRESULT hr = S_OK;
		T* pThis = static_cast<T*>(this);
		int cConnections = m_vec.GetSize();

		for (int iConnection = 0; iConnection < cConnections; iConnection++)
		{
			pThis->Lock();
			CComPtr<IUnknown> punkConnection = m_vec.GetAt(iConnection);
			pThis->Unlock();

			IDispatch* pConnection = static_cast<IDispatch*>(punkConnection.p);

			if (pConnection)
			{
				CComVariant avarParams[2];
				avarParams[1] = channel;
				avarParams[1].vt = VT_BSTR;
				avarParams[0] = topic;
				avarParams[0].vt = VT_BSTR;
				CComVariant varResult;

				DISPPARAMS params = { avarParams, NULL, 2, 0 };
				hr = pConnection->Invoke(5, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &params, &varResult, NULL, NULL);
			}
		}
		return hr;
	}
	HRESULT Fire_OnNamesInChannel(BSTR channel, LPSAFEARRAY names)
	{
		HRESULT hr = S_OK;
		T* pThis = static_cast<T*>(this);
		int cConnections = m_vec.GetSize();

		for (int iConnection = 0; iConnection < cConnections; iConnection++)
		{
			pThis->Lock();
			CComPtr<IUnknown> punkConnection = m_vec.GetAt(iConnection);
			pThis->Unlock();

			IDispatch* pConnection = static_cast<IDispatch*>(punkConnection.p);

			if (pConnection)
			{
				CComVariant avarParams[2];
				avarParams[1] = channel;
				avarParams[1].vt = VT_BSTR;
				avarParams[0] = names;
				avarParams[0].vt = VT_SAFEARRAY | VT_BYREF;
				CComVariant varResult;

				DISPPARAMS params = { avarParams, NULL, 2, 0 };
				hr = pConnection->Invoke(6, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &params, &varResult, NULL, NULL);
			}
		}
		return hr;
	}
	HRESULT Fire_OnChannelMsg(BSTR channel, BSTR user, BSTR msg)
	{
		HRESULT hr = S_OK;
		T* pThis = static_cast<T*>(this);
		int cConnections = m_vec.GetSize();

		for (int iConnection = 0; iConnection < cConnections; iConnection++)
		{
			pThis->Lock();
			CComPtr<IUnknown> punkConnection = m_vec.GetAt(iConnection);
			pThis->Unlock();

			IDispatch* pConnection = static_cast<IDispatch*>(punkConnection.p);

			if (pConnection)
			{
				CComVariant avarParams[3];
				avarParams[2] = channel;
				avarParams[2].vt = VT_BSTR;
				avarParams[1] = user;
				avarParams[1].vt = VT_BSTR;
				avarParams[0] = msg;
				avarParams[0].vt = VT_BSTR;
				CComVariant varResult;

				DISPPARAMS params = { avarParams, NULL, 3, 0 };
				hr = pConnection->Invoke(7, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &params, &varResult, NULL, NULL);
			}
		}
		return hr;
	}
	HRESULT Fire_OnPrivateMsg(BSTR user, BSTR msg)
	{
		HRESULT hr = S_OK;
		T* pThis = static_cast<T*>(this);
		int cConnections = m_vec.GetSize();

		for (int iConnection = 0; iConnection < cConnections; iConnection++)
		{
			pThis->Lock();
			CComPtr<IUnknown> punkConnection = m_vec.GetAt(iConnection);
			pThis->Unlock();

			IDispatch* pConnection = static_cast<IDispatch*>(punkConnection.p);

			if (pConnection)
			{
				CComVariant avarParams[2];
				avarParams[1] = user;
				avarParams[1].vt = VT_BSTR;
				avarParams[0] = msg;
				avarParams[0].vt = VT_BSTR;
				CComVariant varResult;

				DISPPARAMS params = { avarParams, NULL, 2, 0 };
				hr = pConnection->Invoke(8, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &params, &varResult, NULL, NULL);
			}
		}
		return hr;
	}
	HRESULT Fire_OnUserQuit(BSTR channel, BSTR user, BSTR msg)
	{
		HRESULT hr = S_OK;
		T* pThis = static_cast<T*>(this);
		int cConnections = m_vec.GetSize();

		for (int iConnection = 0; iConnection < cConnections; iConnection++)
		{
			pThis->Lock();
			CComPtr<IUnknown> punkConnection = m_vec.GetAt(iConnection);
			pThis->Unlock();

			IDispatch* pConnection = static_cast<IDispatch*>(punkConnection.p);

			if (pConnection)
			{
				CComVariant avarParams[3];
				avarParams[2] = channel;
				avarParams[2].vt = VT_BSTR;
				avarParams[1] = user;
				avarParams[1].vt = VT_BSTR;
				avarParams[0] = msg;
				avarParams[0].vt = VT_BSTR;
				CComVariant varResult;

				DISPPARAMS params = { avarParams, NULL, 3, 0 };
				hr = pConnection->Invoke(9, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &params, &varResult, NULL, NULL);
			}
		}
		return hr;
	}
	HRESULT Fire_OnUserJoin(BSTR channel, BSTR user)
	{
		HRESULT hr = S_OK;
		T* pThis = static_cast<T*>(this);
		int cConnections = m_vec.GetSize();

		for (int iConnection = 0; iConnection < cConnections; iConnection++)
		{
			pThis->Lock();
			CComPtr<IUnknown> punkConnection = m_vec.GetAt(iConnection);
			pThis->Unlock();

			IDispatch* pConnection = static_cast<IDispatch*>(punkConnection.p);

			if (pConnection)
			{
				CComVariant avarParams[2];
				avarParams[1] = channel;
				avarParams[1].vt = VT_BSTR;
				avarParams[0] = user;
				avarParams[0].vt = VT_BSTR;
				CComVariant varResult;

				DISPPARAMS params = { avarParams, NULL, 2, 0 };
				hr = pConnection->Invoke(10, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &params, &varResult, NULL, NULL);
			}
		}
		return hr;
	}
	HRESULT Fire_OnMeJoin(BSTR channel, BSTR nick)
	{
		HRESULT hr = S_OK;
		T* pThis = static_cast<T*>(this);
		int cConnections = m_vec.GetSize();

		for (int iConnection = 0; iConnection < cConnections; iConnection++)
		{
			pThis->Lock();
			CComPtr<IUnknown> punkConnection = m_vec.GetAt(iConnection);
			pThis->Unlock();

			IDispatch* pConnection = static_cast<IDispatch*>(punkConnection.p);

			if (pConnection)
			{
				CComVariant avarParams[2];
				avarParams[1] = channel;
				avarParams[1].vt = VT_BSTR;
				avarParams[0] = nick;
				avarParams[0].vt = VT_BSTR;
				CComVariant varResult;

				DISPPARAMS params = { avarParams, NULL, 2, 0 };
				hr = pConnection->Invoke(15, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &params, &varResult, NULL, NULL);
				ATLASSERT(SUCCEEDED(hr));
			}
		}
		return hr;
	}
	HRESULT Fire_OnUserPart(BSTR channel, BSTR user, BSTR msg)
	{
		HRESULT hr = S_OK;
		T* pThis = static_cast<T*>(this);
		int cConnections = m_vec.GetSize();

		for (int iConnection = 0; iConnection < cConnections; iConnection++)
		{
			pThis->Lock();
			CComPtr<IUnknown> punkConnection = m_vec.GetAt(iConnection);
			pThis->Unlock();

			IDispatch* pConnection = static_cast<IDispatch*>(punkConnection.p);

			if (pConnection)
			{
				CComVariant avarParams[3];
				avarParams[2] = channel;
				avarParams[2].vt = VT_BSTR;
				avarParams[1] = user;
				avarParams[1].vt = VT_BSTR;
				avarParams[0] = msg;
				avarParams[0].vt = VT_BSTR;
				CComVariant varResult;

				DISPPARAMS params = { avarParams, NULL, 3, 0 };
				hr = pConnection->Invoke(11, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &params, &varResult, NULL, NULL);
			}
		}
		return hr;
	}
	HRESULT Fire_OnNotice(BSTR user, BSTR msg)
	{
		HRESULT hr = S_OK;
		T* pThis = static_cast<T*>(this);
		int cConnections = m_vec.GetSize();

		for (int iConnection = 0; iConnection < cConnections; iConnection++)
		{
			pThis->Lock();
			CComPtr<IUnknown> punkConnection = m_vec.GetAt(iConnection);
			pThis->Unlock();

			IDispatch* pConnection = static_cast<IDispatch*>(punkConnection.p);

			if (pConnection)
			{
				CComVariant avarParams[2];
				avarParams[1] = user;
				avarParams[1].vt = VT_BSTR;
				avarParams[0] = msg;
				avarParams[0].vt = VT_BSTR;
				CComVariant varResult;

				DISPPARAMS params = { avarParams, NULL, 2, 0 };
				hr = pConnection->Invoke(12, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &params, &varResult, NULL, NULL);
			}
		}
		return hr;
	}
	HRESULT Fire_OnAction(BSTR channel, BSTR from, BSTR msg)
	{
		HRESULT hr = S_OK;
		T* pThis = static_cast<T*>(this);
		int cConnections = m_vec.GetSize();

		for (int iConnection = 0; iConnection < cConnections; iConnection++)
		{
			pThis->Lock();
			CComPtr<IUnknown> punkConnection = m_vec.GetAt(iConnection);
			pThis->Unlock();

			IDispatch* pConnection = static_cast<IDispatch*>(punkConnection.p);

			if (pConnection)
			{
				CComVariant avarParams[3];
				avarParams[2] = channel;
				avarParams[2].vt = VT_BSTR;
				avarParams[1] = from;
				avarParams[1].vt = VT_BSTR;
				avarParams[0] = msg;
				avarParams[0].vt = VT_BSTR;
				CComVariant varResult;

				DISPPARAMS params = { avarParams, NULL, 3, 0 };
				hr = pConnection->Invoke(13, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &params, &varResult, NULL, NULL);
			}
		}
		return hr;
	}
	HRESULT Fire_OnUnknownCmd(BSTR line)
	{
		HRESULT hr = S_OK;
		T* pThis = static_cast<T*>(this);
		int cConnections = m_vec.GetSize();

		for (int iConnection = 0; iConnection < cConnections; iConnection++)
		{
			pThis->Lock();
			CComPtr<IUnknown> punkConnection = m_vec.GetAt(iConnection);
			pThis->Unlock();

			IDispatch* pConnection = static_cast<IDispatch*>(punkConnection.p);

			if (pConnection)
			{
				CComVariant avarParams[1];
				avarParams[0] = line;
				avarParams[0].vt = VT_BSTR;
				CComVariant varResult;

				DISPPARAMS params = { avarParams, NULL, 1, 0 };
				hr = pConnection->Invoke(14, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &params, &varResult, NULL, NULL);
			}
		}
		return hr;
	}
	HRESULT Fire_OnTimer(LONG timerid)
	{
		HRESULT hr = S_OK;
		T* pThis = static_cast<T*>(this);
		int cConnections = m_vec.GetSize();

		for (int iConnection = 0; iConnection < cConnections; iConnection++)
		{
			pThis->Lock();
			CComPtr<IUnknown> punkConnection = m_vec.GetAt(iConnection);
			pThis->Unlock();

			IDispatch* pConnection = static_cast<IDispatch*>(punkConnection.p);

			if (pConnection)
			{
				CComVariant avarParams[1];
				avarParams[0] = timerid;
				avarParams[0].vt;
				CComVariant varResult;

				DISPPARAMS params = { avarParams, NULL, 1, 0 };
				hr = pConnection->Invoke(16, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &params, &varResult, NULL, NULL);
			}
		}
		return hr;
	}
};
