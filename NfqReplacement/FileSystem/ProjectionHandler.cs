using System.IO;
using System;
using System.Collections.Generic;
using NfqReplacementLib;
using System.Threading.Tasks;

namespace FileSystem;

internal class ProjectionHandler : IDisposable
{
    private FileSystemWatcher? FileSystemWatcher { get; set; }

    private Projection? Projection { get; set; }

    private ProjectionOptions? Options { get; set; }

    public Projection CreateProjection(ProjectionOptions options)
    {
        Options = options;
        FileSystemWatcher = CreateFileSystemWatcher(options.Folder);

        IEnumerable<FileSystemItem> items = FileSystemItemFetcher.FetchItems(options);

        // todo: figure out if we fell back to enumerate files to use options.FallbackSort
        var comparer = FileSystemItemComparerDecider.GetComparer(options.Sort);

        var fileSystemItems = new SortedObservableCollection<FileSystemItem>(comparer, items);
        Projection = new Projection { Items = fileSystemItems };
        return Projection;
    }

    public void Dispose()
    {
        FileSystemWatcher?.Dispose();
        Projection = null;
    }

    private FileSystemWatcher CreateFileSystemWatcher(string folder)
    {
        var fileSystemWatcher = new FileSystemWatcher(folder)
        {
            EnableRaisingEvents = true,
            IncludeSubdirectories = true,
            NotifyFilter = NotifyFilters.Attributes
                         | NotifyFilters.CreationTime
                         | NotifyFilters.DirectoryName
                         | NotifyFilters.FileName
                         | NotifyFilters.LastAccess
                         | NotifyFilters.LastWrite
                         | NotifyFilters.Security
                         | NotifyFilters.Size
        };

        fileSystemWatcher.Changed += FileSystemWatcher_Changed;
        fileSystemWatcher.Created += FileSystemWatcher_Created;
        fileSystemWatcher.Deleted += FileSystemWatcher_Deleted;
        fileSystemWatcher.Renamed += FileSystemWatcher_Renamed;

        fileSystemWatcher.EnableRaisingEvents = true;

        return fileSystemWatcher;
    }

    private /*async*/ void FileSystemWatcher_Changed(object sender, FileSystemEventArgs e)
    {
        var newFileSystemItem = FileSystemItemFetcher.CreateFileSystemItem(e.FullPath);
        for (int i = 0; i < Projection?.Items.Count; i++)
        {
            var item = Projection.Items[i];
            if (item.Path == e.FullPath)
            {
                var comparer = FileSystemItemComparerDecider.GetComparer(Options?.Sort ?? new SortOptions() { Field = SortField.Name, Order = SortOrder.Ascending });
                if (comparer.Compare(item, newFileSystemItem) == 0)
                {
                    Projection.Items.RemoveAt(i);
                    Projection.Items.AddSorted(newFileSystemItem);
                    Console.WriteLine($"Sort property changed: {newFileSystemItem.Path}");
                    PrintResults(Projection.Items);
                }
                else
                {
                    Projection.Items.UpdateAt(i, newFileSystemItem);
                    Console.WriteLine($"Property changed: {newFileSystemItem.Path}");
                    PrintResults(Projection.Items);
                }

                break;
            }
        }
    }

    private void FileSystemWatcher_Created(object sender, FileSystemEventArgs e)
    {
        var newFileSystemItem = FileSystemItemFetcher.CreateFileSystemItem(e.FullPath);

        Projection?.Items.AddSorted(newFileSystemItem);
        Console.WriteLine($"Added: {e.FullPath}");
        PrintResults(Projection!.Items);
    }

    private void FileSystemWatcher_Deleted(object sender, FileSystemEventArgs e)
    {
        for (int i = 0; i < Projection?.Items.Count; i++)
        {
            var item = Projection.Items[i];
            if (item.Path == e.FullPath)
            {
                Projection.Items.RemoveAt(i);
                Console.WriteLine($"Removed: {item.Path}");
                PrintResults(Projection.Items);
                break;
            }
        }
    }

    private void FileSystemWatcher_Renamed(object sender, RenamedEventArgs e)
    {
        for (int i = 0; i < Projection?.Items.Count; i++)
        {
            var item = Projection.Items[i];
            if (item.Path == e.OldFullPath)
            {
                item.Path = e.FullPath;
                Projection.Items.UpdateSorted(item);
                Console.WriteLine($"Renamed: {e.OldFullPath} -> {item.Path}");
                PrintResults(Projection.Items);
                break;
            }
        }
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