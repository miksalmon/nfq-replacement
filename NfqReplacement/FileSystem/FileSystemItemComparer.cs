using System;
using System.Collections.Generic;
using WindowsSearch;

namespace FileSystem
{
    public static class FileSystemItemComparerDecider
    {
        public static Comparer<FileSystemItem> GetComparer(SortField sortField)
        {
            switch (sortField)
            {
                case SortField.Name:
                    return new NameComparer();
                case SortField.Size:
                    return new SizeComparer();
                case SortField.Tags:
                    return new TagsComparer();
                case SortField.Rating:
                    return new RatingComparer();
                case SortField.Dimensions:
                    return new DimensionsComparer();
                case SortField.Date:
                    return new DateComparer();
                case SortField.DateCreated:
                    return new DateCreatedComparer();
                case SortField.DateModified:
                    return new DateModifiedComparer();
                case SortField.DateTaken:
                    return new DateTakenComparer();
                default:
                    throw new ArgumentException("Invalid sort field", nameof(sortField));
            }
        }
    }

    public class NameComparer : Comparer<FileSystemItem>
    {
        public override int Compare(FileSystemItem? x, FileSystemItem? y)
        {
            return string.Compare(x?.Name, y?.Name, StringComparison.Ordinal);
        }
    }

    public class SizeComparer : Comparer<FileSystemItem>
    {
        public override int Compare(FileSystemItem? x, FileSystemItem? y)
        {
            return x?.Size.CompareTo(y?.Size) ?? 0;
        }
    }

    public class TagsComparer : Comparer<FileSystemItem>
    {
        public override int Compare(FileSystemItem? x, FileSystemItem? y)
        {
            var xTags = string.Join("; ", x?.Tags ?? []);
            var yTags = string.Join("; ", x?.Tags ?? []);
            return string.Compare(xTags, yTags, StringComparison.Ordinal);
        }
    }

    public class RatingComparer : Comparer<FileSystemItem>
    {
        public override int Compare(FileSystemItem? x, FileSystemItem? y)
        {
            if (x?.Rating < y?.Rating) return -1;
            if (x?.Rating > y?.Rating) return 1;
            return 0;

        }
    }

    public class DimensionsComparer : Comparer<FileSystemItem>
    {
        public override int Compare(FileSystemItem? x, FileSystemItem? y)
        {
            return string.Compare(x?.Dimensions, y?.Dimensions, StringComparison.Ordinal);
        }
    }

    public class DateComparer : Comparer<FileSystemItem>
    {
        public override int Compare(FileSystemItem? x, FileSystemItem? y)
        {
            if (x?.ItemDate < y?.ItemDate) return -1;
            if (x?.ItemDate > y?.ItemDate) return 1;
            return 0;
        }
    }

    public class DateCreatedComparer : Comparer<FileSystemItem>
    {
        public override int Compare(FileSystemItem? x, FileSystemItem? y)
        {
            if (x?.DateCreated < y?.DateCreated) return -1;
            if (x?.DateCreated > y?.DateCreated) return 1;
            return 0;
        }
    }

    public class DateModifiedComparer : Comparer<FileSystemItem>
    {
        public override int Compare(FileSystemItem? x, FileSystemItem? y)
        {
            if (x?.DateModified < y?.DateModified) return -1;
            if (x?.DateModified > y?.DateModified) return 1;
            return 0;
        }
    }

    public class DateTakenComparer : Comparer<FileSystemItem>
    {
        public override int Compare(FileSystemItem? x, FileSystemItem? y)
        {
            if (x?.DateTaken < y?.DateTaken) return -1;
            if (x?.DateTaken > y?.DateTaken) return 1;
            return 0;
        }
    }
}
