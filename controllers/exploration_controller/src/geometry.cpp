#include <cmath>

class SE2 {
public:
    SE2(double x, double y, double theta) : x(x), y(y), theta(theta) {}

    SE2 transform_point(double x_p, double y_p) const {
        // Transformation of a point using SE2 pose
    }

    static SE2 compose(const SE2& a, const SE2& b) {
        // Compose two SE2 transformations
    }

    static SE2 inverse(const SE2& pose) {
        // Inverse of an SE2 transformation
    }

    double x, y, theta;
};
