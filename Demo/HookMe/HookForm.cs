using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace HookMe
{
    public partial class HookForm : Form
    {
        public HookForm()
        {
            InitializeComponent();
        }

        private void m_Btn_Click(object sender, EventArgs e)
        {
            MessageBox.Show("原始内容", "原始标题", MessageBoxButtons.OK);
        }
    }
}
