#pragma once

class Vector2D {
public:
    double x = 0;
    double y = 0;

    Vector2D();
    Vector2D(double x, double y);
    ~Vector2D() = default;

    Vector2D operator+(const Vector2D& a) const {
        return {this->x + a.x, this->y + a.y};
    }

    Vector2D operator-(const Vector2D& a) const {
        return {this->x - a.x, this->y - a.y};
    }

    Vector2D operator*(const float a) const {
        return {this->x * a, this->y * a};
    }

    Vector2D operator/(const float a) const {
        return {this->x / a, this->y / a};
    }
};
