using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;

namespace FileSystem;

public class SortedObservableCollection<T> : ObservableCollection<T>
{
    public SortedObservableCollection(IComparer<T> comparer, IEnumerable<T> items)
        :base(items)
    {
        Comparer = comparer;
    }

    private IComparer<T> Comparer { get; }

    public void AddSorted(T item)
    {
        int index = BinarySearch(item);
        InsertItem(index, item);
    }

    public void UpdateSorted(T item)
    {
        int index = BinarySearch(item);
        if (index < Count && (Items[index]?.Equals(item) ?? false))
        {
            SetItem(index, item);
        }
    }

    public void UpdateAt(int index, T item)
    {
        if (index < Count)
        {
            SetItem(index, item);
        }
    }

    private int BinarySearch(T item)
    {
        int low = 0;
        int high = Count - 1;

        while (low <= high)
        {
            int mid = (low + high) / 2;
            int comparison = Comparer.Compare(Items[mid], (item));

            if (comparison == 0)
            {
                return mid;
            }
            else if (comparison < 0)
            {
                low = mid + 1;
            }
            else
            {
                high = mid - 1;
            }
        }

        return low;
    }
}