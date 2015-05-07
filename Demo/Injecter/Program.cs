using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using CoreNet;
using System.Threading;
using System.Runtime.InteropServices;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Diagnostics;

namespace Injecter
{
    class Program
    {
        static void Main(string[] args)
        {
            Application.EnableVisualStyles();
           
            var hookMe = Process.GetProcessesByName("HookMe").FirstOrDefault();
            if (hookMe == null)
            {
                MessageBox.Show("请先运行HookMe.exe", "提示");
                return;
            }

            var assembly = "HookMessageBoxW.dll";
            var state = Inject.InjectToProcess(hookMe.Id, assembly);
            MessageBox.Show("注入HookMe.exe" + (state ? "成功" : "失败"), "提示");
        }
    }
}
