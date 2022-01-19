#pragma once

/*
* 如何使用windowshook类设置windowshook,注意SetHookPrc函数不能在DLL外部使用,因为外部传递HOOK的回调函数的地址会异常,这个函数只是用来在HOOKDLL内部实现别的逻辑时使用的
1.DLL工程中添加导出接口:

    extern "C" __declspec(dllexport) WindowsHook* GetInstance()
    {
        WindowsHook* lpWindowsHookInstance = WindowsHook::CreateInstance(WH_CALLWNDPROCRET, HookCallBack);
        lpWindowsHookInstance.SetHookPrc(...,...)
        return lpWindowsHookInstance;
    }

注意:HookCallBack的定义如下(这只是个例子):

    LRESULT CALLBACK HookCallBack(int nCode, WPARAM wParam, LPARAM lParam)
    {
        LPCWPRETSTRUCT lpMsg = (LPCWPRETSTRUCT)lParam;
        if (NULL != lpMsg)
        {
            if ((WM_INITDIALOG == lpMsg->message))
            {

                WCHAR szCaption[200] = { 0 };
                GetWindowText(lpMsg->hwnd, szCaption, 200);

                CString buffer;
                buffer.Format(L"当前窗体名称为:%s", szCaption);
                WRITE_LOG(buffer);

                goto _EXIT;
            }
            goto _EXIT;
        }
        else
        {
            WRITE_LOG(L"指针为空");
            goto _EXIT;
        }

        goto _EXIT;

    _EXIT:
        return CallNextHookEx(NULL, nCode, wParam, lParam);
    }


int WindowsHookCode(idHook)的参数如下:
WH_CALLWNDPROC      在系统将消息发送至目标窗口处理过程之前,挂钩并对该消息进行监视
WH_CALLWNDPROCRET   在系统将消息发送至目标窗口处理过程之后,挂钩并对该消息进行监视
WH_CBT              挂钩CBT应用程序的消息
WH_DEBUG            安装一个挂钩处理过程以便对其他挂钩处理过程进行调试
WH_FOREGROUNDIDLE   当应用程序的前台线程即将进入空闲状态时被挂钩
WH_GETMESSAGE       对寄送至消息队列的消息进行挂钩并监视
WH_JOURNALPLAYBACK  对此前由WH_JOURNALRECORD挂钩处理过程纪录的消息进行寄送
WH_JOURNALRECORD    对寄送至系统消息队列的输入消息进行纪录
WH_KEYBOARD         对键盘消息进行挂钩并监视
WH_KEYBOARD_LL      只能在Windows NT中被安装,用来对底层的键盘输入事件进行挂钩并监视
WH_MOUSE            对鼠标消息进行挂钩并监视
WH_MOUSE_LL         能在Windows NT中被安装,用来对底层的鼠标输入事件进行挂钩并监视
WH_MSGFILTER        对 对话框、消息框、菜单条、或滚动条中的输入事件引发的消息挂钩并监视
WH_SHELL            对shell应用程序的消息进行挂钩并监视
WH_SYSMSGFILTER     对 对话框、消息框、菜单条、或滚动条中的输入事件引发的消息挂钩并监视,这个挂钩处理过程对系统中所有应用程序的这类消息都进行监视

2.将此类的声明windowsHook.h添加到应用程序工程文件中
3.在应用程序中调用:

    HMODULE hMod = LoadLibrary(L"Dll1.dll");
    using lpWindowsHook = WindowsHook* (*)();
    WindowsHook* lpWindowsHookInstance = NULL;
    lpWindowsHook lp_WindowsHook = (lpWindowsHook)GetProcAddress(hMod, "GetInstance");
    if (lp_WindowsHook)
    {
        lpWindowsHookInstance = lp_WindowsHook();
    }
    此时lpWindowsHookInstance仅可使用Install()或UnInstall()函数,其他函数禁止使用

*/



namespace windowshook_nsp
{
    HMODULE GetSelfModuleHandle();

    LRESULT CALLBACK Fun_HookCallBack(int nCode, WPARAM wParam, LPARAM lParam);
}


class WindowsHook
{
public:
    WindowsHook(int WindowsHookCode, HOOKPROC HookCallBack = NULL);

    ~WindowsHook();

    int SetHookPrc(int WindowsHookCode, HOOKPROC HookCallBack);

    //注意Install函数执行正确的返回值是FALSE,若返回了非0,则为lasterror
    int Install();

    //注意UnInstall函数执行正确的返回值是FALSE,若返回了非0,则为lasterror
    int UnInstall();

    static WindowsHook* CreateInstance(int WindowsHookCode, HOOKPROC HookCallBack = NULL);

    void ReleaseInstance();

public:
    //using lpCallBack = LRESULT(CALLBACK*)(int, WPARAM, LPARAM);
    typedef LRESULT(CALLBACK* lpCallBack)(int code, WPARAM wParam, LPARAM lParam); //为了兼容非CPP11的写法
    lpCallBack m_HookCallBack;  //hook回调的函数指针
    HMODULE m_hModule;  //100%确定是本DLL的句柄
    HHOOK m_hHook;  //hook句柄
    int WindowsHookCode;    //hook标识
    BOOL m_HasSetHook;  //是否已经设置hook

};