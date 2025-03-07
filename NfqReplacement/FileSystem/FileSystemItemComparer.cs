using System;
using System.Collections.Generic;
using NfqReplacementLib;

namespace FileSystem
{
    public static class FileSystemItemComparerDecider
    {
        public static Comparer<FileSystemItem> GetComparer(SortOptions sortOptions)
        {
            Comparer<FileSystemItem> comparer = sortOptions.Field switch
            {
                SortField.Name => new NameComparer(),
                SortField.Size => new SizeComparer(),
                SortField.Tags => new TagsComparer(),
                SortField.Rating => new RatingComparer(),
                SortField.Dimensions => new DimensionsComparer(),
                SortField.Date => new DateComparer(),
                SortField.DateCreated => new DateCreatedComparer(),
                SortField.DateModified => new DateModifiedComparer(),
                SortField.DateTaken => new DateTakenComparer(),
                _ => throw new ArgumentException("Invalid sort field", nameof(sortOptions.Field))
            };

            return sortOptions.Order == SortOrder.Ascending ? comparer : comparer.Inverse();
        }

        private static Comparer<FileSystemItem> Inverse(this Comparer<FileSystemItem> comparer)
        {
            var inverseComparer = Comparer<FileSystemItem>.Create((x, y) => comparer.Compare(y, x));
            return inverseComparer;
        }
    }

    public class NameComparer : Comparer<FileSystemItem>
    {
        public override int Compare(FileSystemItem? x, FileSystemItem? y)
        {
            return string.Compare(x?.Name, y?.Name, StringComparison.OrdinalIgnoreCase);
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
