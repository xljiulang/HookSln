using System;
using System.Collections.Concurrent;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace CoreNet
{
    /// <summary>
    /// 用于引用已实例化的对象
    /// 防止对象被GC回收
    /// </summary>
    internal static class ReferenceTable
    {
        private static readonly object syncRoot = new object();
        private static List<object> table = new List<object>();

        /// <summary>
        /// 添加引用      
        /// </summary>      
        /// <param name="obj"></param>
        public static void AddRef(object obj)
        {
            if (obj != null)
            {
                lock (syncRoot)
                {
                    table.Add(obj);
                }
            }
        }

        /// <summary>
        /// 删除结果的引用
        /// </summary>    
        /// <param name="obj"></param>
        public static void RemoveRef(object obj)
        {
            if (obj != null)
            {
                lock (syncRoot)
                {
                    table.Remove(obj);
                }
            }
        }
    }
}
