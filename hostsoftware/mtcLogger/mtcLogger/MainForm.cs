using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using ZedGraph;
using System.IO.Ports;


namespace mtcLogger
{
    public partial class mainForm : Form
    {
//VARIABLE REGION
        #region VARIABLE
    //Graph
        UInt16 maxPoint = 60000; //60000 point
        int samplingTime = 3; //Default 3s
        UInt64 cntTeemp = 0;
        //Uart
   //SerialPortManager _spManager;
        private string s_port_name = "COM1"; //default com port name
        private int i_baudrate = 19200; //default baurate
        private SerialPort serial_port = null;
        delegate void SetTextCallback(string text);
#endregion //VARIABLE
        
//FORM FUNCTION REGION
        #region FORM FUNCTION
        //Function, Concontructor
        public mainForm()
        {
            InitializeComponent();
            graph_load();            
        }
        /// <summary>
        /// Update status about your application
        /// </summary>
        /// <param name="status">status your want to update</param>
        /// <param name="is_new">true mean overwrite old status</param>
        void update_status(string status, bool is_new)
        {
            if (is_new)
            {
                lblStatus.Text = status;
            }
            else lblStatus.Text += status;
        }
        /// <summary>
        /// Enable button, graph
        /// </summary>
        private void enable_()
        {
            button2.Enabled = true;
            button3.Enabled = true;
            buttStart.Enabled = true;
        }
        /// <summary>
        /// Disable button, graph
        /// </summary>
        private void disable_()
        {
            button2.Enabled = false;
            button3.Enabled = false;
            buttStart.Enabled = false;
        }          
        /// <summary>
        /// Out data from serial
        /// Log temperature
        /// </summary>
       
        private void rtb_out(string text, Color color, bool AddNewLine = false)
        {
    
            double data;
            var box = rtbMain;
            if (AddNewLine)
            {
                text += Environment.NewLine;
            }
            box.SelectionStart = box.TextLength;
            box.SelectionLength = 0;

            box.SelectionColor = color;
            box.AppendText(text);
            box.SelectionColor = box.ForeColor;

            string[] strArr = text.Split(',');
            try
            {
                data = double.Parse(strArr[1], System.Globalization.CultureInfo.InstalledUICulture.NumberFormat);
                draw((float)((UInt64) samplingTime * cntTeemp++), data);
                label1.Text = data.ToString();
            }
            catch { }           
        }
        /// <summary>
        /// Send serial to set temperature channel
        /// </summary>
        private void setChanel()
        {
            try
            {
                //Default channel
                serial_port.Write("chan 0001/r/n");
            }
            catch { }
        }
        /// <summary>
        /// Setting new interval
        /// </summary>
        private void upDateTimer()
        {
            tmrSample.Interval = samplingTime * 1000; //Seconds  
        }       
        #endregion //FORM FUNCTION

//FORM EVENT REGION
        #region FORM EVENT
    //Form
        private void mainForm_Load(object sender, EventArgs e)
        {
            //Get serial port name and baudrate
            vGetSerialPortAndBaudRate();
        }
        private void mainForm_FormClosing(object sender, FormClosingEventArgs e)
        {

        }

        private void mainForm_Resize(object sender, EventArgs e)
        {
            zed.MasterPane[0].IsFontsScaled = false;
            zed.Refresh();
        }
        private void exitToolStripMenuItem_Click(object sender, EventArgs e)
        {
            Application.Exit();
        }
   //Button
        private void buttConnect_Click(object sender, EventArgs e)
        {
            //enable_();
            //return;
            if (buttConnect.Text.Equals("Connect"))//Connect
            {
                try
                {
                    if (serial_port == null) open_com_port(s_port_name, i_baudrate);
                    else serial_port.Open();
                }
                catch (Exception er) { update_status(er.ToString(), true); }
                buttConnect.Text = "Disconnect";
                //
                if (serial_port.IsOpen)
                {
                    enable_();
                }
            }
            else//Disconnect
            {
                try { serial_port.Close(); serial_port = null; }
                catch (Exception er) { }

                update_status("Disconnect", true);
                buttConnect.Text = "Connect";
                if (serial_port == null)
                {
                    disable_();
                }
            }
        }

        private void buttStart_Click(object sender, EventArgs e)
        {
            upDateTimer();
            tmrSample.Start();
            setChanel();
        }
       
    //Combobox
        private void cbComPort_SelectedIndexChanged(object sender, EventArgs e)
        {
            s_port_name = cbComPort.SelectedItem.ToString();
            update_status("Seleted port name: " + s_port_name, true);
        }
        private void cbComPort_MouseClick(object sender, MouseEventArgs e)
        {
            //Get port name
            cbComPort.Items.Clear();
            string[] str = SerialPort.GetPortNames();
            foreach (string subStr in str)
            {
                cbComPort.Items.Add(subStr);
            }
        }
        private void cbBaudRate_SelectedIndexChanged(object sender, EventArgs e)
        {
            i_baudrate = (int)cbBaudRate.SelectedItem;
            update_status("Seleted baudrate: " + i_baudrate, true);
        }
    //Menu toolStrip
        //Sampling time setting
        private void intervalSampToolStripMenuItem_Click(object sender, EventArgs e)
        {
            string strRet;
            using (SamplingForm formSetting = new SamplingForm())
            {
                formSetting.ShowDialog();
                strRet = formSetting.MyString;               
                if (strRet != "NAN")
                {
                    samplingTime = int.Parse(strRet);                   
                }
                update_status("Set sampling time is: " + samplingTime.ToString(), true);
                upDateTimer();
            }
        }
        //Serial port setting
        private void serialToolStripMenuItem_Click(object sender, EventArgs e)
        {
            string strRet;
            using(SerialSetting formSerialSetting = new SerialSetting())
            {
                formSerialSetting.ShowDialog();
                strRet = formSerialSetting.MyString;
                MessageBox.Show(strRet);
                if (strRet != "NAN")
                {
                   // samplingTime = int.Parse(strRet);
                }
               
               // upDateTimer();
            }
        }
     //Timer here
        //Timer
        private void tmrSample_Tick(object sender, EventArgs e)
        {
            try
            {
                serial_port.Write("read\r\n");
            }
            catch { }
        }    
        #endregion//FORM EVENT

//GRAPH REGION
        #region 
        /// <summary>
        /// Init pid graph
        /// </summary>
        private void graph_load()
        {
            GraphPane myPane = zed.GraphPane;

            myPane.Title.IsVisible = false;
           // myPane.Title.Text = "MTC Logger";
            
            myPane.XAxis.Title.Text = "Thời Gian";
            myPane.XAxis.Title.FontSpec.Size = 12.0f;
            myPane.YAxis.Title.Text = "Nhiệt Độ";
            myPane.YAxis.Title.FontSpec.Size = 12.0f;
            RollingPointPairList list = new RollingPointPairList(maxPoint);         

            LineItem curve = myPane.AddCurve("Nhiệt Độ", list, Color.Red, SymbolType.None);          

            curve.Line.Width = 2;

            myPane.XAxis.Scale.Min = 0;
            myPane.XAxis.Scale.Max = 90;
            myPane.XAxis.Scale.MinorStep = 10;
            myPane.XAxis.Scale.MajorStep = 30;

            myPane.YAxis.Scale.Min = 0;
            myPane.YAxis.Scale.Max = 300;
            myPane.YAxis.Scale.MinorStep = 10;
            myPane.YAxis.Scale.MajorStep = 100;

        }
        /// <summary>
        /// 
        /// </summary>
        /// <param name="_x1"></param>
        /// <param name="_y1"></param>
        /// <param name="_x2"></param>
        /// <param name="_y2"></param>
        private void draw(double _x1, double _y1)
        {
            if (zed.GraphPane.CurveList.Count <= 0) return;
            LineItem curve = zed.GraphPane.CurveList[0] as LineItem;           

            if (curve == null ) return;

            IPointListEdit list = curve.Points as IPointListEdit;          

            if (list == null) return;

            list.Add(_x1, _y1);
          

            if (_x1 > (zed.GraphPane.XAxis.Scale.Max - zed.GraphPane.XAxis.Scale.MajorStep + 20))
            {
                zed.GraphPane.XAxis.Scale.Max = _x1 + 120;
               // zed.GraphPane.XAxis.Scale.Min += 60;
            }
            zed.AxisChange();
            zed.Invalidate();
            zed.Refresh();
        }
        #endregion //GRAPH

//SERIAL REGION
        #region SERIAL
        private void vGetSerialPortAndBaudRate()
        {
            //Get port name
            cbComPort.Items.Clear();
            string[] str = SerialPort.GetPortNames();
            foreach (string subStr in str)
            {
                cbComPort.Items.Add(subStr);
            }
            //Get serial baud rate
            cbBaudRate.Items.Clear();
            int[] baudRate = { 4800, 9600, 19200, 38400, 57600, 115200 };
            foreach (object data in baudRate)
            {
                cbBaudRate.Items.Add(data);
            }
            //Default port name and baud rate
            cbBaudRate.SelectedText = "19200";
            try
            {
                cbComPort.SelectedIndex = 0;
            }
            catch { };
            
        }
       
        /// <summary>
        /// Open your com port
        /// </summary>
        /// <param name="port_name">name of com</param>
        /// <param name="baudrate">baud rate of com</param>
        private void open_com_port(string port_name, int baudrate)
        {
            serial_port = new SerialPort(port_name, baudrate, Parity.None, 8, StopBits.One);
            try
            {
                serial_port.Open();
                update_status("Open: " + port_name + ". Baud: " + baudrate, true);
                serial_port.DataReceived += new SerialDataReceivedEventHandler(serial_port_DataReceived);
            }
            catch (Exception e)
            {
                MessageBox.Show(e.ToString());
                update_status("No Connect", true);
            }
        }

        private void serial_port_DataReceived(object sender, SerialDataReceivedEventArgs e)
        {
            //SerialPort sp = (SerialPort)sender;
            //string indata = sp.ReadExisting();
            //rtb_out(indata, Color.Black, false); 

            System.Threading.Thread.Sleep(10);
            SetText(serial_port.ReadLine());       
        }
        private void SetText(string text)
        {
            if (this.rtbMain.InvokeRequired)
            {
                SetTextCallback d = new SetTextCallback(SetText);
                this.Invoke(d, new object[] { text });
            }
            else
            {
                rtb_out(text, Color.Black, false);                
            }
        }
        #endregion//SERIAL      
        
//End class
    }
}
