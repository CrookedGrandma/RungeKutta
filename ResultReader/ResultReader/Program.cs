using System.Collections.Generic;
using System.Linq;
using System.IO;
using System;

namespace SplitImage {
    static class Program {

        static int n = 50;

        static void Main(string[] args) {
            StreamWriter writer = new StreamWriter("../../out.csv");
            writer.AutoFlush = true;
            List<string> files = Directory.GetFiles("../../in", "*.txt").ToList();
            files.Sort();
            List<double[]>[] Data = new List<double[]>[50];
            foreach (string filePath in files) {
                string[] lines = File.ReadAllLines(filePath);
                double stepSize, error;
                int numOfDigits, numOfSteps;

                //int.TryParse(lines[7].Split()[0].Replace('.', ','), out numOfDigits);
                int.TryParse(lines[6].Split()[0].Replace('.', ','), out numOfSteps);
                //double.TryParse(lines[6].Split()[0].Replace('.', ','), out stepSize);
                double.TryParse(lines[15].Split()[0].Replace('.', ','), out error);
                int index = numOfSteps / 10000 - 1;
                if (Data[index] == null) {
                    Data[index] = new List<double[]>();
                }
                Data[index].Add(new double[2] { numOfSteps, error });
            }
            foreach (List<double[]> l in Data) {
                writer.Write(l[0][0]);
                writer.Write(';');
                double errorsum = 0;
                double errorvarianceSum = 0;
                foreach (double[] d in l) {
                    errorsum += d[1];
                }
                double mean = errorsum / n;
                foreach (double[] d in l) {
                    errorvarianceSum += (d[1] - mean) * (d[1] - mean);
                }
                double var = errorvarianceSum / (n - 1);
                double stDev = Math.Sqrt(var);
                double standardError = Math.Sqrt(var / n);
                writer.Write(mean);
                writer.Write(';');
                writer.WriteLine(standardError);
            }
        }
    }
}