using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.IO.Ports;
using System.Windows.Forms;

namespace mtcLogger
{
    public partial class SerialSetting : Form
    {
        int[] baudRate = { 4800, 9600, 19200, 38400, 57600, 115200 };
        int[] byteSize = { 7,8};
        string ret = "NAN";
        public SerialSetting()
        {
            InitializeComponent();
            getSrialData();
        }

        private void getSrialData()
        {
            //Port name
            string[] str = SerialPort.GetPortNames();
            foreach (string subStr in str)
            {
                cbComPort.Items.Add(subStr);
            }
            try
            {
                cbComPort.SelectedIndex = 0;
            }
            catch { }
            //Baud
            foreach (object data in baudRate)
            {
                cbBaudRate.Items.Add(data);
            }
            cbBaudRate.Text = "19200";
            //Byte size
            foreach (object bj in byteSize)
            {
                cbByteSize.Items.Add(bj);
            }
            cbByteSize.Text = "8";
            //Stop bit
            cbStopBit.DataSource = Enum.GetValues(typeof(System.IO.Ports.StopBits));
            cbStopBit.Text = "One";
            //Parity
            cbParity.DataSource = Enum.GetValues(typeof(System.IO.Ports.Parity));
            cbParity.Text = "None";
        }

        private void buttOK_Click(object sender, EventArgs e)
        {
            //Check 
            if (cbComPort.Items.Count == 0)
            {
                //Do nothing ..
                //ret = "NAN"
            }
            else
            {
                ret = cbComPort.SelectedItem.ToString() + ","
               + cbBaudRate.SelectedItem.ToString() + ","
               + cbByteSize.SelectedItem.ToString() + ","
               + cbParity.SelectedItem.ToString() + ","
               + cbStopBit.SelectedItem.ToString();
                MessageBox.Show(ret);
            }
           
            this.Close();
        }

        public string MyString
        {     //  in 
            get { return ret; }  //  .NET
        }

        private void buttCancel_Click(object sender, EventArgs e)
        {
            this.Close();
        }

        private void cbComPort_MouseClick(object sender, MouseEventArgs e)
        {
            //Port name
            string[] str = SerialPort.GetPortNames();
            cbComPort.Items.Clear();
            cbComPort.Text = "";
            foreach (string subStr in str)
            {
                cbComPort.Items.Add(subStr);
            }
        }
    }
}
