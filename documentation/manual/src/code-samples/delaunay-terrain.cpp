Terrain_3r DelaunayTerrain(const PointSet_3r& samples, IGeometryObserver* obs) {
    Terrain_3r terrain;
    terrain.AddObserver(obs);

    terrain.Initialize(samples);

    for (auto sample : samples.points()) {
        terrain.AddSample(*sample);
    }

    return terrain;
}