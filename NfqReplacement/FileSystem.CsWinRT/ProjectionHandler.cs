using System.IO;
using System;
using System.Collections.Generic;
using System.Threading.Tasks;

namespace FileSystem;

internal class ProjectionHandler : IDisposable
{
    private FileSystemWatcher? FileSystemWatcher { get; set; }

    private Projection? Projection { get; set; }

    private ProjectionOptions? Options { get; set; }

    public Projection CreateProjection(ProjectionOptions options, bool handleFileSystemChanges)
    {
        Options = options;
        if (handleFileSystemChanges)
        {
            FileSystemWatcher = CreateFileSystemWatcher(options.Folder);
        }

        (IEnumerable<FileSystemItem> items, bool useFallback) = FileSystemItemFetcher.FetchItems(options);

        var sort = useFallback ? options.FallbackSort : options.Sort;

        var comparer = FileSystemItemComparerDecider.GetComparer(sort);

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
                }
                else
                {
                    Projection.Items.UpdateAt(i, newFileSystemItem);
                    Console.WriteLine($"Property changed: {newFileSystemItem.Path}");
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
                break;
            }
        }
    }
}