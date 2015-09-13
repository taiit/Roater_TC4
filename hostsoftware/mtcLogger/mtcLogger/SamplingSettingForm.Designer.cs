namespace mtcLogger
{
    partial class SamplingForm
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.tbSeconds = new System.Windows.Forms.TextBox();
            this.lblSampling1 = new System.Windows.Forms.Label();
            this.buttSamplingOK = new System.Windows.Forms.Button();
            this.buttSamplingCancel = new System.Windows.Forms.Button();
            this.lblSampling2 = new System.Windows.Forms.Label();
            this.SuspendLayout();
            // 
            // tbSeconds
            // 
            this.tbSeconds.Location = new System.Drawing.Point(98, 29);
            this.tbSeconds.Name = "tbSeconds";
            this.tbSeconds.Size = new System.Drawing.Size(53, 20);
            this.tbSeconds.TabIndex = 0;
            this.tbSeconds.Text = "3";
            this.tbSeconds.KeyDown += new System.Windows.Forms.KeyEventHandler(this.tbSeconds_KeyDown);
            // 
            // lblSampling1
            // 
            this.lblSampling1.AutoSize = true;
            this.lblSampling1.Location = new System.Drawing.Point(14, 32);
            this.lblSampling1.Name = "lblSampling1";
            this.lblSampling1.Size = new System.Drawing.Size(79, 13);
            this.lblSampling1.TabIndex = 1;
            this.lblSampling1.Text = "Sampling Time:";
            // 
            // buttSamplingOK
            // 
            this.buttSamplingOK.Location = new System.Drawing.Point(17, 71);
            this.buttSamplingOK.Name = "buttSamplingOK";
            this.buttSamplingOK.Size = new System.Drawing.Size(82, 32);
            this.buttSamplingOK.TabIndex = 2;
            this.buttSamplingOK.Text = "OK";
            this.buttSamplingOK.UseVisualStyleBackColor = true;
            this.buttSamplingOK.Click += new System.EventHandler(this.buttSamplingOK_Click);
            // 
            // buttSamplingCancel
            // 
            this.buttSamplingCancel.Location = new System.Drawing.Point(127, 71);
            this.buttSamplingCancel.Name = "buttSamplingCancel";
            this.buttSamplingCancel.Size = new System.Drawing.Size(82, 32);
            this.buttSamplingCancel.TabIndex = 2;
            this.buttSamplingCancel.Text = "CANCEL";
            this.buttSamplingCancel.UseVisualStyleBackColor = true;
            this.buttSamplingCancel.Click += new System.EventHandler(this.buttSamplingCancel_Click);
            // 
            // lblSampling2
            // 
            this.lblSampling2.AutoSize = true;
            this.lblSampling2.Location = new System.Drawing.Point(162, 32);
            this.lblSampling2.Name = "lblSampling2";
            this.lblSampling2.Size = new System.Drawing.Size(47, 13);
            this.lblSampling2.TabIndex = 1;
            this.lblSampling2.Text = "seconds";
            // 
            // SamplingForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(230, 125);
            this.Controls.Add(this.buttSamplingCancel);
            this.Controls.Add(this.buttSamplingOK);
            this.Controls.Add(this.lblSampling2);
            this.Controls.Add(this.lblSampling1);
            this.Controls.Add(this.tbSeconds);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedToolWindow;
            this.Name = "SamplingForm";
            this.ShowIcon = false;
            this.ShowInTaskbar = false;
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.Text = "         Setting Sampling Time";
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.TextBox tbSeconds;
        private System.Windows.Forms.Label lblSampling1;
        private System.Windows.Forms.Button buttSamplingOK;
        private System.Windows.Forms.Button buttSamplingCancel;
        private System.Windows.Forms.Label lblSampling2;
    }
}