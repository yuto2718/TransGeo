#include "coordinate/ENU_coordinate.hpp"

#include <iomanip>
#include <sstream>
#include <stdexcept>

namespace trans_geo::coordinate {
using std::make_unique;

ENUCoordinate::ENUCoordinate(double east, double north, double up,
                             const trans_geo::interface::ICoordinate& origin)
    : east_(east), north_(north), up_(up), origin_(origin.clone()) {
  auto values = origin_->getValues();
  if (values.size() != 2 && values.size() != 3) {
    throw std::invalid_argument(
        "ENUCoordinate::ENUCoordinate requires origin coordinate with 2 or 3 "
        "values.");
  }
}

std::vector<double> ENUCoordinate::getValues() const {
  return {east_, north_, up_};
}

void ENUCoordinate::setValues(const std::vector<double>& values) {
  if (values.size() != 3) {
    throw std::invalid_argument(
        "ENUCoordinate::setValues requires a vector of size 3.");
  }
  east_ = values[0];
  north_ = values[1];
  up_ = values[2];
}

std::string ENUCoordinate::toString() const {
  std::ostringstream oss;
  oss << std::fixed << std::setprecision(6);
  oss << "[" << east_ << ", " << north_ << ", " << up_ << "]";
  return oss.str();
}

std::unique_ptr<trans_geo::interface::ICoordinate> ENUCoordinate::getOrigin()
    const {
  return origin_->clone();
}

void ENUCoordinate::setOrigin(const trans_geo::interface::ICoordinate& origin) {
  auto values = origin.getValues();
  if (values.size() != 2 && values.size() != 3) {
    throw std::invalid_argument(
        "ENUCoordinate::setOrigin requires origin coordinate with 2 or 3 "
        "values.");
  }
  origin_ = origin.clone();
}

double ENUCoordinate::getEast() const noexcept { return east_; }

double ENUCoordinate::getNorth() const noexcept { return north_; }

double ENUCoordinate::getUp() const noexcept { return up_; }

void ENUCoordinate::setEast(double east) noexcept { east_ = east; }

void ENUCoordinate::setNorth(double north) noexcept { north_ = north; }

void ENUCoordinate::setUp(double up) noexcept { up_ = up; }

std::unique_ptr<trans_geo::interface::ICoordinate> ENUCoordinate::clone()
    const {
  return make_unique<ENUCoordinate>(east_, north_, up_, *origin_);
}
}  // namespace trans_geo::coordinate