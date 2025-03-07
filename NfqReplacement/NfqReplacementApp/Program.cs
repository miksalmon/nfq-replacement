using FileSystem;
using NfqReplacementLib;
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

        Console.WriteLine($"Querying File System...");
        stopWatch.Start();

        var projectionManager = new ProjectionManager();
        var sortOptions = sort.ToSortOptions();
        var projectionOptions = new ProjectionOptions() { Folder = folderPath, Sort = sortOptions };
        var projection = projectionManager.CreateProjectionAsync(projectionOptions);
        IList<FileSystemItem> files = projection.Items;

        stopWatch.Stop();
        var queryTime = stopWatch.ElapsedMilliseconds;
        Console.WriteLine($"Found {files.Count} files.");
        Console.WriteLine($"Search time: {queryTime}ms.\n");

        stopWatch.Reset();

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
