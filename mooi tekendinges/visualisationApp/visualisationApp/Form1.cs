using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;


namespace visualisationApp
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
        }

        // method for drawing a line on the screen
        public void drawLine(Color col, int thickness, int x1, int y1, int x2, int y2)
        {
            Graphics gr = CreateGraphics();
            gr.DrawLine((new Pen(new SolidBrush(col), thickness)), x1, y1, x2, y2);
            gr.Dispose();
            //this.Update();
        }

        // method for drawing a planet on the screen
        public void drawPlanet(Color col, int thickness, int x, int y, int radius)
        {
            Graphics gr = CreateGraphics();
            x++;
            gr.DrawEllipse((new Pen(new SolidBrush(col), thickness)), x, y, radius, radius);
            gr.Dispose();
        }

        private void panel1_Paint(object sender, PaintEventArgs e)
        {
            Graphics gr = CreateGraphics();
            Brush brush = new SolidBrush(Color.Red);
            Pen pen = new Pen(brush, 80);
            gr.DrawLine(pen, 10, 10, 500, 500);
            gr.Dispose();

            //drawLine(Color.Red, 2, 0, 0, 500, 500);
            //drawPlanet(Color.Blue, 3, 50, 50, 30);
        }
        /*
        public void canvas_Paint(object sender, PaintEventArgs e)
        {
            Graphics gr = CreateGraphics();
            Brush brush = new SolidBrush(Color.Red);
            Pen pen = new Pen(brush, 80);
            gr.DrawLine(pen, 10, 10, 500, 500);
        }
        */

        private void Form1_Load(object sender, EventArgs e)
        {

        }
    
    }
}
