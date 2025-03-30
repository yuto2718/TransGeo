#include "coordinate/ECEF_coordinate.hpp"

#include <iomanip>
#include <sstream>
#include <stdexcept>

namespace trans_geo::coordinate {
ECEFCoordinate::ECEFCoordinate(double x, double y, double z) noexcept
    : x_(x), y_(y), z_(z) {}

std::vector<double> ECEFCoordinate::getValues() const { return {x_, y_, z_}; }

void ECEFCoordinate::setValues(const std::vector<double>& values) {
  if (values.size() != 3) {
    throw std::invalid_argument(
        "ECEFCoordinate::setValues requires a vector of size 3.");
  }
  x_ = values[0];
  y_ = values[1];
  z_ = values[2];
}

std::string ECEFCoordinate::toString() const {
  std::ostringstream oss;
  oss << std::fixed << std::setprecision(6);
  oss << "[" << x_ << ", " << y_ << ", " << z_ << "]";
  return oss.str();
}

double ECEFCoordinate::getX() const noexcept { return x_; }

double ECEFCoordinate::getY() const noexcept { return y_; }

double ECEFCoordinate::getZ() const noexcept { return z_; }

void ECEFCoordinate::setX(double x) noexcept { x_ = x; }

void ECEFCoordinate::setY(double y) noexcept { y_ = y; }

void ECEFCoordinate::setZ(double z) noexcept { z_ = z; }

std::unique_ptr<trans_geo::interface::ICoordinate> ECEFCoordinate::clone()
    const {
  return std::make_unique<ECEFCoordinate>(x_, y_, z_);
}
}  // namespace trans_geo::coordinate