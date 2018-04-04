using System.Collections.Generic;
using System.Linq;
using System.IO;
using System;

namespace SplitImage {
    static class Program {

        static void Main(string[] args) {
            StreamWriter writer = new StreamWriter("../../out.csv");
            writer.AutoFlush = true;
            List<string> files = Directory.GetFiles("../../in", "*.txt").ToList();
            files.Sort();
            List<double[]>[] Data = new List<double[]>[100];
            foreach (string filePath in files) {
                string[] lines = File.ReadAllLines(filePath);
                double stepSize, error;
                int steps;

                int.TryParse(lines[6].Split()[0].Replace('.', ','), out steps);
                //double.TryParse(lines[6].Split()[0].Replace('.', ','), out stepSize);
                double.TryParse(lines[15].Split()[0].Replace('.', ','), out error);
                if(Data[(steps / 100) - 1] == null)
                {
                    Data[(steps / 100) - 1] = new List<double[]>();
                }
                Data[(steps/100)-1].Add(new double[2]{steps, error});



            }
            foreach(List<double[]> l in Data)
            {
                writer.Write(l[0][0]);
                writer.Write(';');
                double errorsum = 0;
                double errorvarianceSum = 0;
                foreach (double[] d in l)
                {
                    errorsum += d[1];
                }
                double mean = errorsum / 100;
                foreach (double[] d in l)
                {
                    errorvarianceSum += (d[1]-mean)* (d[1] - mean);
                }
                double var = errorvarianceSum / 99;
                double stDev = Math.Sqrt(var);
                double standardError = Math.Sqrt(var / 100);
                writer.Write(mean);
                writer.Write(';');
                writer.WriteLine(standardError);
            }
        }
    }
}
