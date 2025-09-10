class XY {
protected:
    int x, y;
public:
    XY() : x(0), y(0) {}
    XY(int x, int y) : x(x), y(y) {}
};

class Point : public XY {
public:
    Point() : XY(0, 0) {}
    Point(int x, int y) : XY(x, y) {}
};

class Polyline : public XY {
protected:
    Point* points;
    int count;
public:
    Polyline(Point* pts, int count) : XY(0, 0), count(count) {
        points = new Point[count];
        for (int i = 0; i < count; ++i)
            points[i] = pts[i];
    }
    ~Polyline() {
        delete[] points;
    }
};

class Rectangle : public XY {
protected:
    int width, height;
public:
    Rectangle(int x, int y, int w, int h) : XY(x, y), width(w), height(h) {}
};

class Tree : public Point {
public:
    Tree(int x, int y) : Point(x, y) {}
};

class Car : public Point {
public:
    Car(int x, int y) : Point(x, y) {}
};

class Road : public Polyline {
public:
    Road(Point* pts, int count) : Polyline(pts, count) {}
};

class Route : public Polyline {
public:
    Route(Point* pts, int count) : Polyline(pts, count) {}
};

class ObjectArray {
private:
    XY** arr;
    int capacity;
    int size;
public:
    ObjectArray() : capacity(10), size(0) {
        arr = new XY * [capacity];
    }
    ~ObjectArray() {
        for (int i = 0; i < size; ++i)
            delete arr[i];
        delete[] arr;
    }
    void push_back(XY* obj) {
        if (size == capacity) {
            int newCapacity = capacity * 2;
            XY** newArr = new XY * [newCapacity];
            for (int i = 0; i < size; ++i)
                newArr[i] = arr[i];
            delete[] arr;
            arr = newArr;
            capacity = newCapacity;
        }
        arr[size++] = obj;
    }
};

class Map : public Rectangle {
    ObjectArray objects;
public:
    Map(int x, int y, int w, int h) : Rectangle(x, y, w, h) {}
    void addObject(XY* obj) {
        objects.push_back(obj);
    }
};

int main() {
    Map cityMap(0, 0, 100, 100);
    cityMap.addObject(new Tree(10, 20));
    cityMap.addObject(new Car(30, 40));
    return 0;
}