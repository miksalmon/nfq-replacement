# NFQ Replacement
 
This project is an experimental library designed to replace the Neighboring Files Query (NFQ) on Windows. It is built using Native AOT, C#/WinRT, and C++/WinRT. 

The primary goal is to match the features of the existing NFQ while significantly improving performance and reliability.

This implementation was quickly hacked together to explore a feasible path for future development. The gist of this implementation is to query all supported files in a folder following File Explorer sorting and ordering.
 
## Features
 
- **Feature Parity with NFQ**:
  - Gives access to neighboring files from File Explorer.
  - Maintains the sort order of File Explorer.
  - Filters desired file types.
  - Updates with file system changes.
- **Improved Performance and Reliability**:
  - Uses `System.IO.Directory.EnumerateFiles` for fast file enumeration.
    - Used when sorting by `Date Created`, `Date Modified`, `Name`, `Size`, `Type`.
  - Leverages the Windows Search Indexer for querying folders sorted with extra metadata.
    - Used when sorting by `Date`, `Date Taken`, `Tags`, `Dimensions`, `Rating`.
  - Implements `System.IO.FileSystemWatcher` to easily watch file system changes.
- **High Productivity and Easy Maintenance**:
  - Leverages C# for rapid development and utilization of simple APIs.
  - Uses C++/WinRT sparingly to minimize P/Invoke and COM interop overhead.

## Technologies Used 
- **Native AOT**: For ahead-of-time compilation to improve performance and eliminate dependency on the .NET runtime.
- **C#/WinRT**: For leveraging the productivity of C#.
- **C++/WinRT**: For critical performance paths and minimizing interop.
 
## Performance Metrics
 
### Querying a folder of 1k files
| Sorting Criteria           | Sort Order Found Time (ms) | Query Time (ms) | Total Time (ms) |
|----------------------------|----------------------------|-----------------|-----------------|
| Name (Ascending)           | 19.67                      | 45.00           | 64.67           |
| DateModified (Descending)  | 18.33                      | 50.67           | 69.00           |
| Date (Descending)          | 19.33                      | 99.67           | 119.00          |
| Dimensions (Ascending)     | 19.67                      | 87.00           | 106.67          |

### Querying a folder of 10k files
| Sorting Criteria           | Sort Order Found Time (ms) | Query Time (ms) | Total Time (ms) |
|----------------------------|----------------------------|-----------------|-----------------|
| Name (Ascending)           | 19.00                      | 205.67          | 224.00          |
| DateModified (Descending)  | 19.33                      | 285.33          | 304.67          |
| Date (Descending)          | 19.67                      | 612.67          | 632.33          |
| Dimensions (Ascending)     | 19.00                      | 524.00          | 543.00          |

### Querying a folder of 100k files
| Sorting Criteria           | Sort Order Found Time (ms) | Query Time (ms) | Total Time (ms) |
|----------------------------|----------------------------|-----------------|-----------------|
| Name (Ascending)           | 18.67                      | 1846.67         | 1865.67         |
| DateModified (Descending)  | 19.00                      | 2382.67         | 2401.67         |
| Date (Descending)          | 19.33                      | 5696.33         | 5715.67         |
| Dimensions (Ascending)     | 19.67                      | 5197.67         | 5217.33         |
 
- **Extended Feature Set**:
  - 1:1 File Explorer sorting order: This would require understanding all the ordering heuristics used by File Explorer.
  - Use better fallbacks if the Windows Search Indexer is not ready.
      - For smaller collection, use `System.IO.Directory.EnumerateFiles`, read the extra metadata needed on disk and then sort with it.
      - For larger collection, we might have to fallback to another sort order, which would provide different sort options than File Explorer gives.
  - Handle efficiently all cases of file system changes.
- **Performance Optimization**:
  - Improve sort and filtering, potentially using either C# `Span` or move sorting and filtering to C++ using `std::vector` and `std::sort`.
