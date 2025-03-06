using NfqReplacementLib;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace NfqReplacementApp.Extensions
{
    public static class WindowsSearchResultItemExtensions
    {
        public static readonly HashSet<string> SupportedFileTypes = new HashSet<string>
        {
            // Regular image formats
            ".jpg", ".jpeg", ".thumb", ".png", ".tif", ".tiff", ".dng", ".bmp", ".dib", ".gif", ".jfif",
            ".jpe", ".jxr", ".wdp", ".ico", ".heic", ".heif", ".hif", ".avif", ".webp", ".jxl",

            // Raw formats
            ".arw", ".cr2", ".crw", ".erf", ".kdc", ".mrw", ".nef", ".nrw", ".orf", ".pef", ".raf", ".raw", ".rw2", ".rwl", ".sr2", ".srw", ".avci",
            ".ori", ".cr3", ".srf", ".dcs", ".dcr", ".drf", ".k25", ".3fr", ".ari", ".bay", ".cap", ".iiq", ".eip", ".fff", ".mef", ".mdc", ".mos", ".ptx", ".pxn", ".r3d", ".rwz",

            // Video formats
            ".3g2", ".3gp", ".3gp2", ".3gpp", ".asf", ".avi", ".m2t", ".m2ts", ".m4v", ".mkv", ".mov", ".mp4", ".mp4v", ".mts", ".wm", ".wmv"
        };

        public static bool IsSupportedFileType(this WindowsSearchResultItem item)
        {
            return SupportedFileTypes.Contains(item.Type);
        }

        public static IEnumerable<WindowsSearchResultItem> Sort(this IEnumerable<WindowsSearchResultItem> results, FileExplorerSort sort)
        {
            IEnumerable<WindowsSearchResultItem> sortedFiles;

            switch (sort.PropertyKey)
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

            return sortedFiles;
        }
    }
}
