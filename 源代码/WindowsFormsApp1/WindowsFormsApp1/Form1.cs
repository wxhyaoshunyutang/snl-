using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Diagnostics;
using System.Drawing;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace WindowsFormsApp1
{
    public partial class Form1 : Form
    {
        Process process = new Process();
        ProcessStartInfo startInfo = new ProcessStartInfo();
        string fileName = "";
        public Form1()
        {
            InitializeComponent();
        }

        private void showLineNo()

        {

            //获得当前坐标信息

            Point p = this.richTextBox1.Location;

            int crntFirstIndex = this.richTextBox1.GetCharIndexFromPosition(p);

            int crntFirstLine = this.richTextBox1.GetLineFromCharIndex(crntFirstIndex);

            Point crntFirstPos = this.richTextBox1.GetPositionFromCharIndex(crntFirstIndex);

            //

            p.Y += this.richTextBox1.Height;

            //

            int crntLastIndex = this.richTextBox1.GetCharIndexFromPosition(p);

            int crntLastLine = this.richTextBox1.GetLineFromCharIndex(crntLastIndex);

            Point crntLastPos = this.richTextBox1.GetPositionFromCharIndex(crntLastIndex);

            //

            //

            //准备画图

            Graphics g = this.panel1.CreateGraphics();

            Font font = new Font(this.richTextBox1.Font, this.richTextBox1.Font.Style);

            SolidBrush brush = new SolidBrush(Color.Green);

            //

            //

            //画图开始

            //刷新画布

            Rectangle rect = this.panel1.ClientRectangle;

            brush.Color = this.panel1.BackColor;

            g.FillRectangle(brush, 0, 0, this.panel1.ClientRectangle.Width, this.panel1.ClientRectangle.Height);

            brush.Color = Color.Green;//重置画笔颜色

            //

            //绘制行号

            int lineSpace = 0;

            if (crntFirstLine != crntLastLine)

            {

                lineSpace = (crntLastPos.Y - crntFirstPos.Y) / (crntLastLine - crntFirstLine);

            }

            else

            {

                lineSpace = Convert.ToInt32(this.richTextBox1.Font.Size);

            }

            int brushX = this.panel1.ClientRectangle.Width - Convert.ToInt32(font.Size * 3)+5;

            int brushY = crntLastPos.Y + Convert.ToInt32(font.Size * 0.21f);

            for (int i = crntLastLine; i >= crntFirstLine; i--)

            {

                g.DrawString((i + 1).ToString(), font, brush, brushX, brushY);

                brushY -= lineSpace;

            }

            g.Dispose();

            font.Dispose();

            brush.Dispose();

        }

        private void panel1_Paint(object sender, PaintEventArgs e)
        {
            showLineNo();
        }

        private void richTextBox1_TextChanged(object sender, EventArgs e)
        {
            panel1.Invalidate();
        }

        private void richTextBox1_VScroll(object sender, EventArgs e)
        {
            panel1.Invalidate();
        }

        private void 打开ToolStripMenuItem_Click(object sender, EventArgs e)
        {
            this.openFileDialog1.Title="选择文件";
            this.openFileDialog1.Filter = "text(*.txt)|*.txt";
            this.openFileDialog1.ShowDialog();
        }

        private void openFileDialog1_FileOk(object sender, CancelEventArgs e)
        {
            this.button3.Enabled = false;
            this.button4.Enabled = false;
            this.button5.Enabled = false;
            this.button6.Enabled = false;
            this.richTextBox2.Text = "";
            fileName = this.openFileDialog1.FileName;
            FileStream fs = new FileStream(fileName, FileMode.Open, FileAccess.Read);
            StreamReader sr = new StreamReader(fs, Encoding.UTF8);
            this.richTextBox1.Text = sr.ReadToEnd();
            sr.Close();
        }

        private async void 保存ToolStripMenuItem_Click(object sender, EventArgs e)
        {
            this.saveFileDialog1.Title = "保存至";
            if(fileName == ""||fileName == ".txt") 
            { 
                this.saveFileDialog1.ShowDialog();
                fileName = this.saveFileDialog1.FileName+".txt";
            }
            File.WriteAllText(fileName, this.richTextBox1.Text);
        }

        private void 新建ToolStripMenuItem_Click(object sender, EventArgs e)
        {
            this.richTextBox2.Text = "";
            fileName = "";
            this.richTextBox1.Text = "";
            this.button3.Enabled = false;
            this.button4.Enabled = false;
            this.button5.Enabled = false;
            this.button6.Enabled = false;
        }

        private void button1_Click(object sender, EventArgs e)      //LL1按钮
        {
            this.richTextBox2.Text = "";
            this.button3.Enabled = false;
            this.button4.Enabled = false;
            this.button5.Enabled = false;
            this.button6.Enabled = false;
            if (fileName == "" || fileName == ".txt")
            {
                this.saveFileDialog1.Title = "保存至";
                this.saveFileDialog1.ShowDialog();
                fileName = this.saveFileDialog1.FileName + ".txt";
                if(fileName == ".txt")
                {
                    this.richTextBox2.Text = "!!!error:文件未命名";
                    return;
                }
            }
            File.WriteAllText(fileName, this.richTextBox1.Text);
            startInfo.FileName = "C:\\Users\\10104\\Desktop\\M_编译原理课程设计(1)(1)\\x64\\Debug\\M_编译原理课程设计.exe";
            startInfo.Arguments = "ll1 " + fileName;
            startInfo.WindowStyle = ProcessWindowStyle.Normal;
            process.StartInfo = startInfo;
            process.Start();
            process.WaitForExit();
            
            string outFile1 = fileName.Substring(0,fileName.Length-4)+"_cifa.txt";
            if(File.Exists(outFile1)&&File.ReadAllText(outFile1) != "")
            {
                button3.Enabled = true;
            }
            string outFile2 = fileName.Substring(0, fileName.Length - 4) + "_yufa.txt";
            if (File.Exists(outFile2) && File.ReadAllText(outFile2) != "")
            {
                button4.Enabled = true;
            }
            string outFile3 = fileName.Substring(0, fileName.Length - 4) + "_tree.txt";
            if (File.Exists(outFile3) && File.ReadAllText(outFile3) != "")
            {
                button5.Enabled = true;
            }
            string outFile4 = fileName.Substring(0, fileName.Length - 4) + "_yuyi.txt";
            if (File.Exists(outFile4) && File.ReadAllText(outFile4) != "")
            {
                button6.Enabled = true;
                richTextBox2.Text = "编译完成";
            }
        }

        private void button2_Click(object sender, EventArgs e)      //递归下降按钮
        {
            this.richTextBox2.Text = "";
            this.button3.Enabled = false;
            this.button4.Enabled = false;
            this.button5.Enabled = false;
            this.button6.Enabled = false;
            if (fileName == "" || fileName == ".txt")
            {
                this.saveFileDialog1.Title = "保存至";
                this.saveFileDialog1.ShowDialog();
                fileName = this.saveFileDialog1.FileName + ".txt";
                if (fileName == ".txt")
                {
                    this.richTextBox2.Text = "!!!error:文件未命名";
                    return;
                }
            }
            File.WriteAllText(fileName, this.richTextBox1.Text);
            startInfo.FileName = "C:\\Users\\10104\\Desktop\\M_编译原理课程设计(1)(1)\\x64\\Debug\\M_编译原理课程设计.exe";
            startInfo.Arguments = "digui " + fileName;
            startInfo.WindowStyle = ProcessWindowStyle.Normal;
            process.StartInfo = startInfo;
            process.Start();
            process.WaitForExit();
            string outFile1 = fileName.Substring(0, fileName.Length - 4) + "_cifa.txt";
            if (File.Exists(outFile1) && File.ReadAllText(outFile1) != "")
            {
                button3.Enabled = true;
            }
            string outFile2 = fileName.Substring(0, fileName.Length - 4) + "_yufa.txt";
            if (File.Exists(outFile2) && File.ReadAllText(outFile2) != "")
            {
                button4.Enabled = true;
            }
            string outFile3 = fileName.Substring(0, fileName.Length - 4) + "_tree.txt";
            if (File.Exists(outFile3) && File.ReadAllText(outFile3) != "")
            {
                button5.Enabled = true;
            }
            string outFile4 = fileName.Substring(0, fileName.Length - 4) + "_yuyi.txt";
            if (File.Exists(outFile4) && File.ReadAllText(outFile4) != "")
            {
                button6.Enabled = true;
                richTextBox2.Text = "编译完成";
            }
        }

        private void button3_Click(object sender, EventArgs e)
        {
            string outFile = fileName.Substring(0, fileName.Length - 4) + "_cifa.txt";
            FileStream fs = new FileStream(outFile, FileMode.Open, FileAccess.Read);
            StreamReader sr = new StreamReader(fs, Encoding.Default);
            this.richTextBox2.Text = sr.ReadToEnd();
            sr.Close();
        }

        private void button4_Click(object sender, EventArgs e)
        {
            string outFile = fileName.Substring(0, fileName.Length - 4) + "_yufa.txt";
            FileStream fs = new FileStream(outFile, FileMode.Open, FileAccess.Read);
            StreamReader sr = new StreamReader(fs, Encoding.Default);
            this.richTextBox2.Text = sr.ReadToEnd();
            sr.Close();
        }

        private void button5_Click(object sender, EventArgs e)
        {
            string outFile = fileName.Substring(0, fileName.Length - 4) + "_tree.txt";
            FileStream fs = new FileStream(outFile, FileMode.Open, FileAccess.Read);
            StreamReader sr = new StreamReader(fs, Encoding.Default);
            this.richTextBox2.Text = sr.ReadToEnd();
            sr.Close();
        }

        private void button6_Click(object sender, EventArgs e)
        {
            string outFile = fileName.Substring(0, fileName.Length - 4) + "_yuyi.txt";
            FileStream fs = new FileStream(outFile, FileMode.Open, FileAccess.Read);
            StreamReader sr = new StreamReader(fs, Encoding.Default);
            this.richTextBox2.Text = sr.ReadToEnd();
            sr.Close();
        }
    }
}