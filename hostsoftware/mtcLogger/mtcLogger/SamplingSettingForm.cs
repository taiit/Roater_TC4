using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace mtcLogger
{
    public partial class SamplingForm : Form
    {
        string ret = "3"; //default 3s
        public SamplingForm()
        {
            InitializeComponent();
        }
        public string MyString
        {     //  in 
            get { return ret; }  //  .NET
        }
        private void upDateData()
        {
            int data = 0;
            try
            {
                data = int.Parse(tbSeconds.Text);
            }
            catch { }
            if (data < 1 || data > 30)
            {
                MessageBox.Show("Incorrect Sampling Time!\nTime from 1s to 30s", "Warring", MessageBoxButtons.OK, MessageBoxIcon.Warning);
            }
            else
            {
                ret = tbSeconds.Text;
                this.Close();
            }
        }

        private void buttSamplingOK_Click(object sender, EventArgs e)
        {
            upDateData();
        }
        private void buttSamplingCancel_Click(object sender, EventArgs e)
        {
            ret = "NAN";
            this.Close();
        }
        private void tbSeconds_KeyDown(object sender, KeyEventArgs e)
        {
            if (e.KeyCode == Keys.Enter)
            {
                upDateData();
            }
        }

       
    }
}
