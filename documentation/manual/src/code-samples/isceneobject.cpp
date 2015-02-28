struct ISceneObject {
    virtual ~ISceneObject() {}
    virtual void Select() = 0;
    virtual void Deselect() = 0;
    virtual void UpdateColor(const QColor& color) = 0;
    virtual const QString& name() const = 0;
    virtual void set_name(const QString& name) = 0;
    virtual Intersection::Ray_3rSceneObject intersect(const Ray_3r& ray) = 0;
};