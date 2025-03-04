using NfqReplacementLib;
using System;

namespace NfqReplacementApp;

internal class Program
{
    static void Main(string[] args)
    {
        var path = "C:\\Users\\Public\\Documents";
        var files = WindowsSearch.GetFiles(path, "*.txt");
        Console.WriteLine($"Files in {path}");

        foreach (var file in files)
        {
            Console.WriteLine(file);
        }
    }
}
