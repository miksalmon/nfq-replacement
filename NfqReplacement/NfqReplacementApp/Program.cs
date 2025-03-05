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

        Console.WriteLine($"Querying Windows Search Indexer.");
        stopWatch.Start();
        Console.WriteLine($"Using Windows Search Indexer.");

        var files = WindowsSearch.GetFiles(folderPath);
        stopWatch.Stop();
        var queryTime = stopWatch.ElapsedMilliseconds;
        Console.WriteLine($"Found {files.Count} files.");
        Console.WriteLine($"Search time: {queryTime}ms.\n");

        stopWatch.Reset();

        Console.WriteLine($"Sorting with LINQ.");
        stopWatch.Start();

        files = SortSearchResults(files, sort);

        stopWatch.Stop();
        var sortTime = stopWatch.ElapsedMilliseconds;
        Console.WriteLine($"Sort time: {sortTime}ms.\n");

        var totalTime = queryTime + sortTime;
        Console.WriteLine($"Total time: {totalTime}ms\n");

        Console.WriteLine("Results:");
        //PrintResults(files);
        Console.WriteLine();

        Console.WriteLine("Press any key to exit...");
        Console.ReadLine();
    }

    private static IList<WindowsSearchResultItem> SortSearchResults(IList<WindowsSearchResultItem> results, FileExplorerSort sort)
    {
        var getSortParameter = IComparable (WindowsSearchResultItem result, string propertyKey) => propertyKey switch
        {
            "System.ItemNameDisplay" => result.Name,
            "System.ItemDate" => result.ItemDate,
            "System.DateModified" => result.DateModified,
            "System.DateCreated" => result.DateCreated,
            _ => result.Name
        };

        IEnumerable<WindowsSearchResultItem> sortedFiles;
        
        switch(sort.PropertyKey)
        {
            case "System.ItemDate":
                sortedFiles = sort.Ascending ? results.OrderBy(f => f.ItemDate) : results.OrderByDescending(f => f.ItemDate);
                break;
            case "System.Photo.DateTaken":
                sortedFiles = sort.Ascending ? results.OrderBy(f => f.DateTaken) : results.OrderByDescending(f => f.DateTaken);
                break;
            case "System.DateModified":
                sortedFiles = sort.Ascending ? results.OrderBy(f => f.DateModified) : results.OrderByDescending(f => f.DateModified);
                break;
            case "System.DateCreated":
                sortedFiles = sort.Ascending ? results.OrderBy(f => f.DateCreated) : results.OrderByDescending(f => f.DateCreated);
                break;
            case "System.Size":
                sortedFiles = sort.Ascending ? results.OrderBy(f => f.Size) : results.OrderByDescending(f => f.Size);
                break;
            case "System.Image.Dimensions":
                sortedFiles = sort.Ascending ? results.OrderBy(f => f.Dimensions) : results.OrderByDescending(f => f.Dimensions);
                break;
            case "System.Rating":
                sortedFiles = sort.Ascending ? results.OrderBy(f => f.Rating) : results.OrderByDescending(f => f.Rating);
                break;
            case "System.ItemNameDisplay":
            default:
                sortedFiles = sort.Ascending ? results.OrderBy(f => f.Name) : results.OrderByDescending(f => f.Name);
                break;
        }

        return sortedFiles.ToList();
    }

    private static void PrintResults(IList<WindowsSearchResultItem> files)
    {
        foreach (var file in files)
        {
            PrintFileProperties(file);

            Console.WriteLine();
        }
    }

    private static void PrintFileProperties(WindowsSearchResultItem file)
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
    }
}
