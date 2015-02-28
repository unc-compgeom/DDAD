namespace Intersection {

class Ray_3rSceneObject {
public:
    Ray_3rSceneObject();
    Ray_3rSceneObject(bool empty, const rational& time);

    bool empty() const;
    const rational& time();

private:
    bool empty_;
    rational time_;
};

}