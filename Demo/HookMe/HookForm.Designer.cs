namespace HookMe
{
    partial class HookForm
    {
        /// <summary>
        /// 必需的设计器变量。
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// 清理所有正在使用的资源。
        /// </summary>
        /// <param name="disposing">如果应释放托管资源，为 true；否则为 false。</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows 窗体设计器生成的代码

        /// <summary>
        /// 设计器支持所需的方法 - 不要
        /// 使用代码编辑器修改此方法的内容。
        /// </summary>
        private void InitializeComponent()
        {
            this.m_Btn = new System.Windows.Forms.Button();
            this.SuspendLayout();
            // 
            // m_Btn
            // 
            this.m_Btn.Location = new System.Drawing.Point(108, 76);
            this.m_Btn.Name = "m_Btn";
            this.m_Btn.Size = new System.Drawing.Size(88, 31);
            this.m_Btn.TabIndex = 0;
            this.m_Btn.Text = "MessageBox";
            this.m_Btn.UseVisualStyleBackColor = true;
            this.m_Btn.Click += new System.EventHandler(this.m_Btn_Click);
            // 
            // HookForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(298, 203);
            this.Controls.Add(this.m_Btn);
            this.Name = "HookForm";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.Text = "目标进程";
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.Button m_Btn;
    }
}

