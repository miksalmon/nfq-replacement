namespace FileSystem;

public record ProjectionOptions
{
    public required string Folder { get; init; }

    public required SortOptions Sort { get; init; }

    public SortOptions FallbackSort { get; init; } = new SortOptions() { Field = SortField.Name, Order = SortOrder.Ascending };

}
