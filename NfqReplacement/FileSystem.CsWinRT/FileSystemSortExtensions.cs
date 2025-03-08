namespace FileSystem
{
    public static class FileSystemSortExtensions
    {
        public static SortOptions ToSortOptions(this FileExplorerSort feSort)
        {
            return new SortOptions
            {
                Field = feSort.PropertyKey switch
                {
                    "System.ItemNameDisplay" => SortField.Name,
                    "System.Size" => SortField.Size,
                    "System.ItemDate" => SortField.Date,
                    "System.DateModified" => SortField.DateModified,
                    "System.DateCreated" => SortField.DateCreated,
                    "System.Type" => SortField.Type,
                    "System.Rating" => SortField.Rating,
                    "System.Keywords" => SortField.Tags,
                    "System.Image.Dimensions" => SortField.Dimensions,
                    "System.Photo.DateTaken" => SortField.DateTaken,
                    _ => SortField.Name // Default case
                },
                Order = feSort.Ascending ? SortOrder.Ascending : SortOrder.Descending
            };
        }
    }
}
