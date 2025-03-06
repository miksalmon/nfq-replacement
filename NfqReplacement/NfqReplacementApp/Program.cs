using NfqReplacementApp.Extensions;
using WindowsSearch;
using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;

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

        Console.WriteLine($"Querying Windows Search Indexer.");
        stopWatch.Start();
        Console.WriteLine($"Using Windows Search Indexer.");

        WindowsSearchResult result = WindowsSearcher.GetFiles(folderPath);
        if (result.Status != WindowsSearchResultStatus.Success)
        {
            Console.WriteLine($"Error querying Windows Search Indexer.: {result.Status}.");
            Console.WriteLine("Press any key to exit...");
            Console.ReadLine();
            return;
        }

        IList<FileSystemItem> files = result.Items;

        stopWatch.Stop();
        var queryTime = stopWatch.ElapsedMilliseconds;
        Console.WriteLine($"Found {files.Count} files.");
        Console.WriteLine($"Search time: {queryTime}ms.\n");

        stopWatch.Reset();

        Console.WriteLine($"Filtering and Sorting with LINQ.");
        stopWatch.Start();

        files = files.Where(f => f.IsSupportedFileType()).Sort(sort).ToList();

        stopWatch.Stop();
        var filterSortTime = stopWatch.ElapsedMilliseconds;
        Console.WriteLine($"Filtering and Sorting time: {filterSortTime}ms.\n");

        var totalTime = queryTime + filterSortTime;
        Console.WriteLine($"Total time: {totalTime}ms\n");

        Console.WriteLine("Results:");
        PrintResults(files);
        Console.WriteLine();

        Console.WriteLine($"Final item count: {files.Count} files.");

        Console.WriteLine("Press any key to exit...");
        Console.ReadLine();
    }

    private static void PrintResults(IList<FileSystemItem> files)
    {
        foreach (var file in files)
        {
            PrintFileProperties(file);

            Console.WriteLine();
        }
    }

    private static void PrintFileProperties(FileSystemItem file)
    {
        Console.WriteLine($"Name: {file.Name}");
        Console.WriteLine($"Path: {file.Path}");
        Console.WriteLine($"Type: {file.Type}");
        Console.WriteLine($"ItemDate: {file.ItemDate}");
        Console.WriteLine($"DateTaken: {file.DateTaken}");
        Console.WriteLine($"DateModified: {file.DateModified}");
        Console.WriteLine($"DateCreated: {file.DateCreated}");
        Console.WriteLine($"Size: {file.Size}");
        Console.WriteLine($"Dimensions: {file.Dimensions}");
        Console.WriteLine($"Tags: {string.Join("; ", file.Tags)}");
        Console.WriteLine($"Rating: {file.Rating}");
    }
}
