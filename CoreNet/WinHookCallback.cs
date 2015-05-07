using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;

namespace CoreNet
{
    /// <summary>
    /// 表示Window钩子回调委托
    /// 返回true则中止钩子传播
    /// </summary>
    /// <param name="nCode">nCode值</param>
    /// <param name="wParam">wParam值</param>
    /// <param name="lParam">lParam值</param>
    /// <returns></returns>
    [UnmanagedFunctionPointer(CallingConvention.StdCall)]
    public delegate bool WinHookCallback(int nCode, int wParam, int lParam);
}
