﻿using System;
using System.Collections.Generic;
using System.IO;
using System.Threading.Tasks;

namespace FileSystem;

public class ProjectionManager
{
    private Dictionary<Guid, ProjectionHandler> ProjectionHandlers { get; } = new();

    public async Task<Projection> CreateProjection(ProjectionOptions options)
    {
        var projectionHandler = new ProjectionHandler();
        var projection = await projectionHandler.CreateProjection(options);
        ProjectionHandlers.Add(projection.Id, projectionHandler);
        return projection;
    }

    public void RemoveProjection(Projection projection)
    {
        if (ProjectionHandlers.TryGetValue(projection.Id, out var projectionHandler))
        {
            projectionHandler.Dispose();
        }

        ProjectionHandlers.Remove(projection.Id);
    }
}
