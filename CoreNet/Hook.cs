using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Collections.Concurrent;
using System.Runtime.InteropServices;
using System.Text;

namespace CoreNet
{
    /// <summary>
    /// 提供Hook相关操作
    /// </summary>
    public static class Hook
    {
        /// <summary>
        /// Hook一个Api     
        /// </summary>
        /// <typeparam name="TDelegate">委托</typeparam>
        /// <param name="libName">Api所在模块名</param>
        /// <param name="apiName">Api名</param>
        /// <param name="proxyApi">代理api</param>
        /// <exception cref="ArgumentNullException"></exception>
        /// <exception cref="ArgumentException"></exception>        
        /// <returns></returns>
        public static HookResult<TDelegate> HookApi<TDelegate>(string libName, string apiName, TDelegate proxyApi) where TDelegate : class
        {
            if (libName == null)
            {
                throw new ArgumentNullException("libName");
            }

            if (apiName == null)
            {
                throw new ArgumentNullException("apiName");
            }

            if (proxyApi == null)
            {
                throw new ArgumentNullException("proxyApi");
            }

            var delegateType = typeof(TDelegate);
            if (typeof(Delegate).IsAssignableFrom(delegateType) == false || delegateType.IsGenericType == true)
            {
                throw new ArgumentException("TDelegate必须为非泛型委托");
            }

            var result = new HookResult<TDelegate>
            {
                LibName = libName,
                ApiName = apiName
            };

            var targetApiPointer = Extern.FindFunction(libName, apiName);
            if (targetApiPointer == IntPtr.Zero)
            {
                return result;
            }

            var proxyApiPointer = Marshal.GetFunctionPointerForDelegate(proxyApi as Delegate);
            if (Extern.HookFunction(ref targetApiPointer, proxyApiPointer) == false)
            {
                return result;
            }

            result._proxyApi = proxyApi;
            result._proxyApiPoinpter = proxyApiPointer;
            result._targetApiPointer = targetApiPointer;
            result.TargetApi = Marshal.GetDelegateForFunctionPointer(targetApiPointer, typeof(TDelegate)) as TDelegate;

            // 添加引用
            ReferenceTable.AddRef(result);
            return result;
        }

        /// <summary>
        /// 安装Window钩子到主线程
        /// </summary>
        /// <param name="hookType">钩子类型</param>
        /// <param name="callBack">回调函数</param>
        /// <exception cref="ArgumentNullException"></exception>
        /// <exception cref="ArgumentException"></exception>
        /// <returns></returns>
        public static bool HookWindow(WinHookType hookType, WinHookCallback callBack)
        {
            if (callBack == null)
            {
                throw new ArgumentNullException("callBack");
            }

            if (Enum.IsDefined(typeof(WinHookType), hookType) == false)
            {
                throw new ArgumentException("hookType的值无效");
            }

            var callBackPointer = Marshal.GetFunctionPointerForDelegate(callBack);
            var state = Extern.HookWindow((int)hookType, callBackPointer);

            if (state == true)
            {
                ReferenceTable.AddRef(callBack);
            }
            return state;
        }

        /// <summary>
        /// 反安装Window钩子
        /// </summary>
        /// <param name="hookType">钩子类型</param>
        public static void UnHookWindow(WinHookType hookType)
        {
            Extern.UnHookWindow((int)hookType);
        }
    }
}
