namespace FileSystem;

public record SortOptions
{
    public required SortOrder Order { get; init; } = SortOrder.Ascending;

    public required SortField Field { get; init; } = SortField.Name;

    // TODO: add fallback sort
}