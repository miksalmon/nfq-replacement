﻿using System;

namespace FileSystem;

public class Projection
{
    public Guid Id { get; } = Guid.NewGuid();

    required public SortedObservableCollection<FileSystemItem> Items { get; init; }
}
