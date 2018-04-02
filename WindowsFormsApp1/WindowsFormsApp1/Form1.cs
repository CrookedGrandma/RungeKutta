using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Windows.Forms.DataVisualization.Charting;

namespace WindowsFormsApp1
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
        }

        private void Form1_Load(object sender, EventArgs e)
        {

        }

        private void chart1_Click(object sender, EventArgs e)
        {
            string[] input = System.IO.File.ReadAllLines(@"d:\Users\Renzo\Documents\GitHub\RungeKutta\RungeKutta\02-04-2018 02.42.23.txt");
            string precisionType = input[0];
            string startingPosSun = input[1];
            string massSun = input[2];
            string startingPosSatellite = input[3];
            string startingVelocitySatellite = input[4];
            string timeStepSize = input[5];
            int numberOfSteps = int.Parse(input[6].Split()[0]);
            int reportFrequency = int.Parse(input[6].Split()[1]);

            List<double> timeStep = new List<double>();
            List<double> xPos = new List<double>();
            List<double> yPos = new List<double>();

            int numberOfBlocks = numberOfSteps / reportFrequency;
            for (int i = 0; i < numberOfBlocks; i++)
            {
                timeStep.Add(double.Parse(input[8 + i * 5].Replace('.', ',')));
                xPos.Add(double.Parse(input[8 + i * 5 + 1].Split()[0].Replace('.', ',')));
                yPos.Add(double.Parse(input[8 + i * 5 + 1].Split()[1].Replace('.', ',')));
            }

            for (int i = 0; i < numberOfBlocks; i++)
            {
                chart1.Series[0].Points.AddXY(xPos[i], yPos[i]);
            }

            chart1.SaveImage(@"../../myChart.png", ChartImageFormat.Png);
        }

    }
}
