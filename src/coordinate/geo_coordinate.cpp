#include "coordinate/geo_coordinate.hpp"

#include <iomanip>
#include <sstream>
#include <stdexcept>

namespace trans_geo::coordinate {

GeoCoordinate::GeoCoordinate(double latitude, double longitude) noexcept
    : latitude_(latitude), longitude_(longitude), altitude_(std::nullopt) {}

GeoCoordinate::GeoCoordinate(double latitude, double longitude,
                             double altitude) noexcept
    : latitude_(latitude), longitude_(longitude), altitude_(altitude) {}

std::vector<double> GeoCoordinate::getValues() const {
  std::vector<double> values{latitude_, longitude_};
  if (altitude_.has_value()) {
    values.push_back(altitude_.value());
  }
  return values;
}

void GeoCoordinate::setValues(const std::vector<double>& values) {
  if (values.size() == 2) {
    latitude_ = values[0];
    longitude_ = values[1];
    altitude_ = std::nullopt;
  } else if (values.size() == 3) {
    latitude_ = values[0];
    longitude_ = values[1];
    altitude_ = values[2];
  } else {
    throw std::invalid_argument(
        "GeoCoordinate::setValues requires a vector of size 2 or 3.");
  }
}

std::string GeoCoordinate::toString() const {
  std::vector<double> values = getValues();
  std::ostringstream oss;
  oss << std::fixed << std::setprecision(6);
  oss << "[";
  for (size_t i = 0; i < values.size(); ++i) {
    oss << values[i];
    if (i != values.size() - 1) {
      oss << ", ";
    }
  }
  oss << "]";
  return oss.str();
}

double GeoCoordinate::getLatitude() const noexcept { return latitude_; }

double GeoCoordinate::getLongitude() const noexcept { return longitude_; }

std::optional<double> GeoCoordinate::getAltitude() const noexcept {
  return altitude_;
}

void GeoCoordinate::setLatitude(double latitude) noexcept {
  latitude_ = latitude;
}

void GeoCoordinate::setLongitude(double longitude) noexcept {
  longitude_ = longitude;
}

void GeoCoordinate::setAltitude(double altitude) noexcept {
  altitude_ = altitude;
}

std::unique_ptr<trans_geo::interface::ICoordinate> GeoCoordinate::clone()
    const {
  if (altitude_.has_value()) {
    return std::make_unique<GeoCoordinate>(latitude_, longitude_,
                                           altitude_.value());
  } else {
    return std::make_unique<GeoCoordinate>(latitude_, longitude_);
  }
}

}  // namespace trans_geo::coordinate
