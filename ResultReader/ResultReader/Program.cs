using System.Collections.Generic;
using System.Linq;
using System.IO;

namespace SplitImage {
    static class Program {

        static void Main(string[] args) {
            StreamWriter writer = new StreamWriter("../../out.csv");
            writer.AutoFlush = true;
            List<string> files = Directory.GetFiles("../../in", "*.txt").ToList();
            files.Sort();
            foreach (string filePath in files) {
                string[] lines = File.ReadAllLines(filePath);
                double duration, error;
                double.TryParse(lines[13].Split()[0].Replace('.', ','), out duration);
                double.TryParse(lines[15].Split()[0].Replace('.', ','), out error);
                writer.Write(duration);
                writer.Write(';');
                writer.WriteLine(error);
            }
        }
    }
}
