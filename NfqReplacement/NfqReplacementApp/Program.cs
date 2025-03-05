using NfqReplacementLib;
using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using Windows.Storage;
using Windows.Storage.Search;

namespace NfqReplacementApp;

internal class Program
{
    [STAThread]
    static void Main(string[] args)
    {
        var stopWatch = new Stopwatch();

        string folderPath = FileExplorer.GetFolder();
        Console.WriteLine($"Selected folder: {folderPath}.\n");

        stopWatch.Start();
        IList<FileExplorerSort> sortColumns = sortColumns = FileExplorer.GetSortColumns(folderPath);
        stopWatch.Stop();
        FileExplorerSort sort = sortColumns.First();
        Console.WriteLine($"Sorting by {sort.PropertyKey} in {(sort.Ascending ? "Ascending" : "Descending")} order.");
        Console.WriteLine($"Found sort order in: {stopWatch.ElapsedMilliseconds}ms.\n");

        stopWatch.Reset();

        stopWatch.Start();
        Console.WriteLine($"Using Windows Search Indexer.");
        // TODO: plug ascending or descending order here
        var files = WindowsSearch.GetFiles(folderPath, sort.PropertyKey, sort.Ascending);
        files = files.OrderBy(x => x.DateCreated).ToList();
        stopWatch.Stop();
        Console.WriteLine($"Found {files.Count} files.");
        Console.WriteLine($"Search time: {stopWatch.ElapsedMilliseconds}ms.\n");

        foreach (var file in files)
        {
            Console.WriteLine($"Name: {file.Name}");
            Console.WriteLine($"Type: {file.Type}");
            Console.WriteLine($"ItemDate: {file.ItemDate}");
            Console.WriteLine($"DateTaken: {file.DateTaken}");
            Console.WriteLine($"DateModified: {file.DateModified}");
            Console.WriteLine($"DateCreated: {file.DateCreated}");
            Console.WriteLine($"Size: {file.Size}");
            Console.WriteLine($"Dimensions: {file.Dimensions}");
            //Console.WriteLine($"Tags: {file.Tags}");
            Console.WriteLine($"Rating: {file.Rating}");

            Console.WriteLine();
        }
        Console.WriteLine();

        Console.WriteLine("Press any key to exit...");
        Console.ReadLine();
    }
}
