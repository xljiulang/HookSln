using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;

namespace CoreNet
{
    /// <summary>
    /// 提供程序集远程进程注入
    /// </summary>
    public static class Inject
    {
        /// <summary>
        /// 排它锁
        /// </summary>
        private static readonly object SyncRoot = new object();
        /// <summary>
        /// 最大等待时间
        /// </summary>
        private static readonly int MaxWaitTime = 5 * 1000;

        /// <summary>
        /// 创建应用进程并注入程序集
        /// </summary>
        /// <param name="fileName">进程文件名</param>
        /// <param name="argument">启动参数</param>
        /// <param name="workDir">工作路径</param>
        /// <param name="assembly">程序集路径</param>
        /// <exception cref="ArgumentNullException"></exception>
        /// <exception cref="FileNotFoundException"></exception>
        /// <returns></returns>
        public static bool InjectToProcess(string fileName, string argument, string workDir, string assembly)
        {
            if (fileName == null)
            {
                throw new ArgumentNullException("fileName");
            }

            if (assembly == null)
            {
                throw new ArgumentNullException("assembly");
            }

            if (File.Exists(fileName) == false)
            {
                throw new FileNotFoundException("找不到文件" + fileName);
            }

            if (File.Exists(assembly) == false)
            {
                throw new FileNotFoundException("找不到文件" + assembly);
            }

            var mainDll = "Main.dll";
            if (File.Exists(mainDll) == false)
            {
                throw new FileNotFoundException("找不到文件Main.dll");
            }

            lock (Inject.SyncRoot)
            {
                Extern.SetAssembly(Path.GetFullPath(assembly));
                if (Extern.CreateProcessWithDll(fileName, argument, workDir, Path.GetFullPath(mainDll)))
                {
                    Extern.WaitForLoadAssembly(MaxWaitTime);
                    return true;
                }
            }
            return false;
        }

        /// <summary>
        /// 注入程序集到指定进程
        /// </summary>
        /// <param name="processId">进程id</param>
        /// <param name="assembly">程序集路径</param>
        /// <exception cref="ArgumentNullException"></exception>
        /// <exception cref="FileNotFoundException"></exception>
        /// <returns></returns>
        public static bool InjectToProcess(int processId, string assembly)
        {
            if (assembly == null)
            {
                throw new ArgumentNullException("assembly");
            }

            if (File.Exists(assembly) == false)
            {
                throw new FileNotFoundException("找不到文件" + assembly);
            }

            var mainDll = Path.GetFullPath("Main.dll");
            if (File.Exists(mainDll) == false)
            {
                throw new FileNotFoundException("找不到文件Main.dll");
            }

            lock (Inject.SyncRoot)
            {
                Extern.SetAssembly(Path.GetFullPath(assembly));
                if (Extern.CreateRemoteThreadWithDll(processId, Path.GetFullPath(mainDll)))
                {
                    Extern.WaitForLoadAssembly(MaxWaitTime);
                    return true;
                }
            }
            return false;
        }
    }
}
