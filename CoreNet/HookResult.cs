using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace CoreNet
{
    /// <summary>
    /// 表示Hook结果
    /// </summary>
    /// <typeparam name="TDelegate">委托类型</typeparam>
    public class HookResult<TDelegate>
    {
        internal IntPtr _targetApiPointer;
        internal IntPtr _proxyApiPoinpter;
        internal TDelegate _proxyApi;

        /// <summary>
        /// 获取Api所在Api所在模块名
        /// </summary>
        public string LibName { get; internal set; }
        /// <summary>
        /// 获取Api名
        /// </summary>
        public string ApiName { get; internal set; }
        /// <summary>
        /// 获取目标Api
        /// 此Api指向Hook之前的地址
        /// </summary>
        public TDelegate TargetApi { get; internal set; }

        /// <summary>
        /// Hook结果
        /// </summary>
        internal HookResult()
        {
        }

        /// <summary>
        /// 取消Hook
        /// </summary>
        /// <returns></returns>
        public bool UnHook()
        {
            if (this._targetApiPointer == IntPtr.Zero)
            {
                return true;
            }

            var state = Extern.UnHookFunction(ref this._targetApiPointer, this._proxyApiPoinpter);
            if (state == true)
            {
                this._proxyApiPoinpter = IntPtr.Zero;
                this._targetApiPointer = IntPtr.Zero;
                this._proxyApi = default(TDelegate);
                this.TargetApi = default(TDelegate);
                ReferenceTable.RemoveRef(this);
            }
            return state;
        }
    }
}
