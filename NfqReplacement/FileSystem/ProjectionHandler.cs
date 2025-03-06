using System.IO;
using System;
using System.Collections.Generic;
using WindowsSearch;

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

        var items = FileSystemItemFetcher.FetchItems(options.Folder);
        var comparer = FileSystemItemComparerDecider.GetComparer(options.Sort.Field);
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

    private async void FileSystemWatcher_Changed(object sender, FileSystemEventArgs e)
    {
        var newFileSystemItem = await FileSystemItemFetcher.FetchItemAsync(e.FullPath, loadExtraMetadata: true);
        for (int i = 0; i < Projection?.Items.Count; i++)
        {
            var item = Projection.Items[i];
            if (item.Path == e.FullPath)
            {
                var comparer = FileSystemItemComparerDecider.GetComparer(Options?.Sort.Field ?? SortField.Name);
                if (comparer.Compare(item, newFileSystemItem) == 0)
                {
                    Projection.Items.RemoveAt(i);
                    Projection.Items.AddSorted(newFileSystemItem);
                }
                else
                {
                    Projection?.Items.UpdateAt(i, newFileSystemItem);
                }

                break;
            }
        }
    }

    private async void FileSystemWatcher_Created(object sender, FileSystemEventArgs e)
    {
        var newFileSystemItem = await FileSystemItemFetcher.FetchItemAsync(e.FullPath, loadExtraMetadata: true);
        Projection?.Items.AddSorted(newFileSystemItem);
    }

    private void FileSystemWatcher_Deleted(object sender, FileSystemEventArgs e)
    {
        for (int i = 0; i < Projection?.Items.Count; i++)
        {
            var item = Projection.Items[i];
            if (item.Path == e.FullPath)
            {
                Projection.Items.RemoveAt(i);
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
                break;
            }
        }
    }
}