using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;

namespace CoreNet
{
    /// <summary>
    /// 表示外部实现的Api
    /// </summary>
    internal static class Extern
    {
        /// <summary>
        /// 设置要加载的程序集
        /// </summary>
        /// <param name="assemblyName">要加载的程序集</param>      
        [DllImport("Core.dll")]
        public static extern void SetAssembly(string assemblyName);

        /// <summary>
        /// 等待程序集被宿主进程加载完成
        /// </summary>
        /// <param name="dwTimeout"></param>
        [DllImport("Core.dll")]
        public static extern void WaitForLoadAssembly(int dwTimeout);

        /// <summary>
        /// 查找函数
        /// 返回函数指针
        /// </summary>
        /// <param name="moduleName">函数所在模块名</param>
        /// <param name="funcName">函数名</param>
        /// <returns></returns>
        [DllImport("Core.dll")]
        public static extern IntPtr FindFunction(string moduleName, string funcName);

        /// <summary>
        /// Hook函数 
        /// </summary>
        /// <param name="targetFuncPointer">目标函数指针</param>
        /// <param name="proxyFuncPointer">代理函数指针</param>
        /// <returns></returns>
        [DllImport("Core.dll")]
        public static extern bool HookFunction(ref IntPtr targetFuncPointer, IntPtr proxyFuncPointer);

        /// <summary>
        /// 取消函数的Hook
        /// </summary>
        /// <param name="targetFuncPointer">目标函数指针</param>
        /// <param name="proxyFuncPointer">代理函数指针</param>
        /// <returns></returns>
        [DllImport("Core.dll")]
        public static extern bool UnHookFunction(ref IntPtr targetFuncPointer, IntPtr proxyFuncPointer);

        /// <summary>
        /// 创建进程并注入
        /// </summary>
        /// <param name="fileName">进程文件名</param>
        /// <param name="argument">启动参数</param>
        /// <param name="dllName">要注入的dll名</param>
        /// <returns></returns>
        [DllImport("Core.dll")]
        public static extern bool CreateProcessWithDll(string fileName, string argument, string dllName);

        /// <summary>
        /// 创建远程线程并注入
        /// </summary>
        /// <param name="processId">进程id</param>
        /// <param name="dllName">要注入的dll名</param>
        /// <returns></returns>
        [DllImport("Core.dll")]
        public static extern bool CreateRemoteThreadWithDll(int processId, string dllName);

        /// <summary>
        /// 安装Window钩子
        /// </summary>
        /// <param name="idHook">钩子类型</param>
        /// <param name="hookDelegate">回调函数委托的指针</param>
        /// <returns></returns>
        [DllImport("Core.dll")]
        public static extern bool HookWindow(int idHook, IntPtr hookDelegate);

        /// <summary>
        /// 反安装Window钩子
        /// </summary>
        /// <param name="idHook">钩子类型</param>
        [DllImport("Core.dll")]
        public static extern void UnHookWindow(int idHook);
    }
}
