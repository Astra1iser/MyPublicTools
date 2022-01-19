#include <windowsHook.h>

HMODULE windowshook_nsp::GetSelfModuleHandle()
{
    MEMORY_BASIC_INFORMATION mbi;

    return ((::VirtualQuery(GetSelfModuleHandle, &mbi, sizeof(mbi)) != 0) ? (HMODULE)mbi.AllocationBase : NULL);
}

LRESULT CALLBACK windowshook_nsp::Fun_HookCallBack(int nCode, WPARAM wParam, LPARAM lParam)
{
    return CallNextHookEx(NULL, nCode, wParam, lParam);
}

WindowsHook::WindowsHook(int WindowsHookCode, HOOKPROC HookCallBack):m_hModule(NULL), m_hHook(NULL), m_HasSetHook(FALSE)
{
    if (!m_hModule)
    {
        m_hModule = windowshook_nsp::GetSelfModuleHandle();
    }

    this->WindowsHookCode = WindowsHookCode;
    if (NULL != HookCallBack)
    {
        this->m_HookCallBack = HookCallBack;
    }
    else
    {
        this->m_HookCallBack = windowshook_nsp::Fun_HookCallBack; //如果不设置回调,则默认callnexthookex
    }
}

WindowsHook::~WindowsHook()
{
    UnInstall();
}

int WindowsHook::SetHookPrc(int WindowsHookCode, HOOKPROC HookCallBack)
{
    if (FALSE == UnInstall())
    {
        this->WindowsHookCode = WindowsHookCode;
        if (NULL != HookCallBack)
        {
            this->m_HookCallBack = HookCallBack;
        }
        else
        {
            this->m_HookCallBack = windowshook_nsp::Fun_HookCallBack; //如果不设置回调,则默认callnexthookex
        }
    }
    return FALSE;
}

//注意Install函数执行正确的返回值是FALSE,若返回了非0,则为lasterror
int WindowsHook::Install()
{
    if (m_HasSetHook)
        return -1;   //已经设置了钩子,不能重复设置

    m_hHook = SetWindowsHookEx(WindowsHookCode, (HOOKPROC)m_HookCallBack, m_hModule, 0);
    if (m_hHook)
        return FALSE;
    else
    {
        return GetLastError();
    }
}

//注意UnInstall函数执行正确的返回值是FALSE,若返回了非0,则为lasterror
int WindowsHook::UnInstall()
{
    if (m_hHook)
    {
        if (UnhookWindowsHookEx(m_hHook))
        {
            m_hHook = NULL;
            m_HasSetHook = FALSE;

            return FALSE;
        }
        else
        {
            return GetLastError();
        }
    }
    return FALSE;
}

WindowsHook* WindowsHook::CreateInstance(int WindowsHookCode, HOOKPROC HookCallBack)
{
    return new WindowsHook(WindowsHookCode, HookCallBack);
}

void WindowsHook::ReleaseInstance()
{
    delete this;
}