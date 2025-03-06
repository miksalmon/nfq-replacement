namespace FileSystem;

public record ProjectionOptions
{
    public required string Folder { get; init; }

    public required SortOptions Sort { get; init; }
}
