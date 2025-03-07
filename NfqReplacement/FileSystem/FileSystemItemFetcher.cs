using System;
using System.Collections.Generic;
using System.IO;
using System.Threading.Tasks;
using Windows.Storage;
using NfqReplacementLib;
using System.Xml.Linq;
using System.Linq;


namespace FileSystem;

static class FileSystemItemFetcher
{
    public static async Task<FileSystemItem> CreateFileSystemItemWithAllMetadataAsync(string filePath)
    {
        var fileInfo = new FileInfo(filePath);

        var name = fileInfo.Name;
        var type = fileInfo.Extension;
        var size = fileInfo.Length;
        var dateCreated = fileInfo.CreationTime;
        var dateModified = fileInfo.LastWriteTime;

        uint rating = 0;
        DateTimeOffset dateTaken = DateTimeOffset.FromFileTime(0);
        DateTimeOffset itemDate = DateTimeOffset.FromFileTime(0);
        string dimensions = string.Empty;
        IList<string> tags = new List<string>();

        var storageFile = await StorageFile.GetFileFromPathAsync(filePath);

        var basicProperties = await storageFile.GetBasicPropertiesAsync();
        if (basicProperties != null)
        {
            itemDate = basicProperties.ItemDate;
        }

        var properties = await storageFile.Properties.GetImagePropertiesAsync();
        if (properties != null)
        {
            dimensions = $"{properties.Width}x{properties.Height}";
            rating = properties.Rating;
            dateTaken = properties.DateTaken;
            tags = properties.Keywords;
        }

        return new FileSystemItem
        {
            Name = name,
            Type = type,
            Size = (ulong)size,
            DateCreated = dateCreated,
            DateModified = dateModified,

            // Extra Metadata
            ItemDate = itemDate,
            Rating = rating,
            DateTaken = dateTaken,
            Dimensions = dimensions,
            Tags = tags
        };
    }

    public static FileSystemItem CreateFileSystemItem(string filePath)
    {
        var fileInfo = new FileInfo(filePath);

        var name = fileInfo.Name;
        var path = fileInfo.FullName;
        var type = fileInfo.Extension;
        var size = fileInfo.Length;
        var dateCreated = fileInfo.CreationTime;
        var dateModified = fileInfo.LastWriteTime;

        uint rating = 0;
        DateTimeOffset dateTaken = DateTimeOffset.FromFileTime(0);
        DateTimeOffset itemDate = DateTimeOffset.FromFileTime(0);
        string dimensions = string.Empty;
        IList<string> tags = new List<string>();

        return new FileSystemItem
        {
            Name = name,
            Path = path,
            Type = type,
            Size = (ulong)size,
            DateCreated = dateCreated,
            DateModified = dateModified,

            // Extra Metadata
            ItemDate = itemDate,
            Rating = rating,
            DateTaken = dateTaken,
            Dimensions = dimensions,
            Tags = tags
        };
    }

    public static IEnumerable<FileSystemItem> FetchItems(ProjectionOptions options)
    {
        var loadExtraMetadata = options.Sort.Field == SortField.Date ||
            options.Sort.Field == SortField.DateTaken ||
            options.Sort.Field == SortField.Tags ||
            options.Sort.Field == SortField.Dimensions ||
            options.Sort.Field == SortField.Rating;

        IEnumerable<FileSystemItem> items;
        if (loadExtraMetadata)
        {
            Console.WriteLine($"Using Windows Search Indexer...");
            var result = WindowsSearch.GetFiles(options.Folder);
            if (result.Status != WindowsSearchResultStatus.Success)
            {
                Console.WriteLine($"Error querying Windows Search Indexer: {result.Status}.");
                items = DotNetEnumerateFiles(options);
            }

            items = result.Items;
        }
        else
        {
            items = DotNetEnumerateFiles(options);
        }

        items = items.Where(item => item.IsSupportedFileType());

        return items;
    }

    private static IList<FileSystemItem> DotNetEnumerateFiles(ProjectionOptions options)
    {
        IList<FileSystemItem> items;

        Console.WriteLine($"Using Directory.EnumerateFiles...");
        var files = Directory.EnumerateFiles(options.Folder, "*.*", SearchOption.TopDirectoryOnly);
        items = new List<FileSystemItem>();
        foreach (var file in files)
        {
            var item = CreateFileSystemItem(file);

            items.Add(item);
        }

        return items;
    }
}
