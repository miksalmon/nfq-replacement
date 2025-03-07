using System;
using System.Collections.Generic;
using System.IO;
using System.Threading.Tasks;
using Windows.Storage;
using NfqReplacementLib;
using Microsoft.UI.Xaml.Shapes;
using System.Linq;


namespace FileSystem;

static class FileSystemItemFetcher
{
    public static async Task<FileSystemItem> FetchItemAsync(string filePath, bool loadExtraMetadata = false)
    {
        var fileInfo = new FileInfo(filePath);

        var name = fileInfo.Name;
        var type = fileInfo.Extension;
        var size = fileInfo.Length;
        var dateCreated = fileInfo.CreationTime;
        var dateModified = fileInfo.LastWriteTime;

        uint rating = 0;
        DateTimeOffset dateTaken = DateTimeOffset.MinValue;
        DateTimeOffset itemDate = DateTimeOffset.MinValue;
        string dimensions = string.Empty;
        IList<string> tags = new List<string>();

        if (loadExtraMetadata)
        {
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
        }

        return new FileSystemItem
        {
            Name = name,
            Type = type,
            Size = (ulong)size,
            DateCreated = dateCreated,
            DateModified = dateModified,
            Rating = rating,
            DateTaken = dateTaken,
            Dimensions = dimensions,
            Tags = tags
        };
    }

    public static async Task<IList<FileSystemItem>> FetchItemsAsync(string folderPath, bool loadExtraMetadata = false)
    {
        IList<FileSystemItem> items;
        if (loadExtraMetadata)
        {
            var result = WindowsSearch.GetFiles(folderPath);
            if (result.Status != WindowsSearchResultStatus.Success)
            {
                throw new InvalidOperationException($"Error querying Windows Search Indexer: {result.Status}.");
            }

            items = result.Items;
        }
        else
        {
            // filter?
            // SearchOption.AllDirectories?
            var files = Directory.EnumerateFiles(folderPath, "*.*", SearchOption.TopDirectoryOnly);
            items = new List<FileSystemItem>();
            foreach (var file in files)
            {
                var item = await FetchItemAsync(file, loadExtraMetadata);
                items.Add(item);
            }
        }


        return items;
    }
}
