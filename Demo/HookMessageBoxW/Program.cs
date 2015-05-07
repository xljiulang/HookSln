using CoreNet;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Runtime.InteropServices;
using System.IO;
using System.Windows.Forms;

namespace HookMessageBoxW
{
    public class Program
    {
        /// <summary>
        /// MessageBoxW委托
        /// </summary>
        /// <param name="hwnd"></param>
        /// <param name="text"></param>
        /// <param name="caption"></param>
        /// <param name="type"></param>
        /// <returns></returns>
        [UnmanagedFunctionPointer(CallingConvention.StdCall, CharSet = CharSet.Unicode)]
        private delegate int MessageBoxW(IntPtr hwnd, string text, string caption, int type);

        private static HookResult<MessageBoxW> messageBoxW;

        /// <summary>
        /// 程序集入口函数
        /// </summary>
        /// <param name="args"></param>
        public static void Main(string[] args)
        {            
            messageBoxW = Hook.HookApi<MessageBoxW>("user32.dll", "MessageBoxW", MessageBoxW_Proxy);            
        }

        private static int MessageBoxW_Proxy(IntPtr hwnd, string text, string caption, int type)
        {
            return messageBoxW.TargetApi.Invoke(hwnd, "[Hook]" + text, "[Hook]" + caption, type == 0 ? 1 : type);
        }
    }
}
