using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.IO.Ports;
using System.Threading;

namespace MyForm
{
   
    public partial class Form1 : Form
    {
        private SerialPort ComDevice = new SerialPort();
        private int i = 1;
        private string str = "";
        
        public Form1()
        {
            
            Image image1 = Image.FromFile(@"image.jpg");
            this.BackgroundImage = image1;
            this.BackgroundImageLayout = ImageLayout.Stretch;
            AutoScale(this);
            InitializeComponent();
            ComDevice.PortName = "COM3";
            ComDevice.BaudRate = 9600;
            
        }

        #region 窗体缩放
        public void AutoScale(Form frm)
        {
            frm.Tag = frm.Width.ToString() + "," + frm.Height.ToString();
            frm.SizeChanged += new EventHandler(Form1_Resize);
        }

        private void Form1_Resize(object sender, EventArgs e)
        {
            string[] tmp = ((Form)sender).Tag.ToString().Split(',');
            float width = (float)((Form)sender).Width / (float)Convert.ToInt16(tmp[0]);
            float heigth = (float)((Form)sender).Height / (float)Convert.ToInt16(tmp[1]);
            ((Form)sender).Tag = ((Form)sender).Width.ToString() + "," + ((Form)sender).Height;
            foreach (Control control in ((Form)sender).Controls)
            {
                control.Scale(new SizeF(width, heigth));
            }
        }
        #endregion


        private void button1_Click(object sender, EventArgs e)
        {
            
            string str = textBox2.Text;
            ComDevice.Open();
            ComDevice.Write(str);  //发送数据
            Thread.Sleep(100);
            //MessageBox.Show("发送的数据：" + str);
            int n = ComDevice.BytesToRead; //获取字节数
            byte[] buf = new byte[n];//声明一个临时数组存储当前来的串口数据 
            Thread.Sleep(100);
            ComDevice.Read(buf, 0, n);
            this.Invoke((EventHandler)(delegate
            {
                //直接按ASCII规则转换成字符串   
                str = "第";
                str += i++;
                str += "次接收到的数据为：";
                str += Encoding.ASCII.GetString(buf);
                str += "\r\n";
                textBox1.AppendText(str);
                //MessageBox.Show("接收到数据长度：" +buf.Length);
            }));
            ComDevice.Close();

        }

        private void Form1_Load(object sender, EventArgs e)
        {
            Image image2 = Image.FromFile(@"button_logo1.jpg");
            this.button1.BackgroundImage = image2;
            this.button1.BackgroundImageLayout = ImageLayout.Stretch;
            
        }
        private void button1_MouseLeave(object sender, EventArgs e)
        {
            Image image2 = Image.FromFile(@"button_logo1.jpg");
            this.button1.BackgroundImage = image2;
            this.button1.BackgroundImageLayout = ImageLayout.Stretch;
        }

        private void button1_MouseEnter(object sender, EventArgs e)
        {
            Image image2 = Image.FromFile(@"button_logo2.jpg");
            this.button1.BackgroundImage = image2;
            this.button1.BackgroundImageLayout = ImageLayout.Stretch;
        }

        //重新绘制TabControl的格式
        private void tabControl1_DrawItem(object sender, DrawItemEventArgs e)
        {
            
            string text = ((TabControl)sender).TabPages[e.Index].Text;

            SolidBrush brush = new SolidBrush(Color.Black);

            StringFormat sf = new StringFormat(StringFormatFlags.DirectionRightToLeft);

            sf.LineAlignment = StringAlignment.Center;

            sf.Alignment = StringAlignment.Center;

            e.Graphics.DrawString(text, SystemInformation.MenuFont, brush, e.Bounds, sf);
        }
        /*
         *  private void tabControl1_DrawItem(object sender, DrawItemEventArgs e)
        {
            
            string text = ((TabControl)sender).TabPages[e.Index].Text;

            SolidBrush brush = new SolidBrush(Color.Black);

            StringFormat sf = new StringFormat(StringFormatFlags.DirectionRightToLeft);

            sf.LineAlignment = StringAlignment.Center;

            sf.Alignment = StringAlignment.Center;

            e.Graphics.DrawString(text, SystemInformation.MenuFont, brush, e.Bounds, sf);
        }
         */
        private void textBox3_TextChanged(object sender, EventArgs e)
        {

        }

        private void button2_Click(object sender, EventArgs e)
        {
            MessageBox.Show("click");
        }
    }
}
