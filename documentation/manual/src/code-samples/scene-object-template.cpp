class SceneMyGeometricType_2 : public ISceneObject, public Visual::Geometry {
public:
    SceneMyGeometricType_2() {
        model_my_geometric_type_.AddObserver(this);
    }

    ~SceneMyGeometricType_2() {}

    void Select() override {}
    void Deselect() override {}
    Intersection::Ray_3rSceneObject intersect(const Ray_3r &ray) override {}
    void UpdateColor(const QColor &color) override {}
    SceneObjectType scene_object_type() const override {
        return SceneObjectType::MY_GEOMETRIC_TYPE_2;
    }
    const QString& name() const override {
        return name_;
    }
    void set_name(const QString &name) override {
        name_ = name;
    }
    const MyGeometricType_2r& model_my_geometric_type() const {
        return model_my_geometric_type_;
    }

private:
    MyGeometricType_2r model_my_geometric_type_;
    QString name_;
};