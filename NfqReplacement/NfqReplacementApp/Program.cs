using NfqReplacementLib;
using System;

namespace NfqReplacementApp;

internal class Program
{
    static void Main(string[] args)
    {
        string path = "E:\\TestPhotos\\smallFolders\\20";
        string sortProperty = "System.ItemNameDisplay";
        var files = WindowsSearch.GetFiles(path, sortProperty);
        Console.WriteLine($"Files in {path}");

        foreach (var file in files)
        {
            Console.WriteLine(file);
        }
    }
}
