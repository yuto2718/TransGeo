#pragma once

namespace trans_geo::ellipsoid {
struct Ellipsoid {
  double a;   // 長半径
  double f;   // 扁平率
  double e2;  // 離心率²

  constexpr Ellipsoid(double a_, double f_)
      : a(a_), f(f_), e2(eccentricitySquared()) {}

  constexpr double eccentricitySquared() const { return 2 * f - f * f; }
};

// WGS84
constinit const Ellipsoid WGS84{6378137.0, 1.0 / 298.257223563};

// GRS80
constinit const Ellipsoid GRS80{6378137.0, 1.0 / 298.257222101};

// 以下おまけ
// IERS2003
constinit const Ellipsoid IERS2003{6378136.6, 1.0 / 298.25642};

// GRS67
constinit const Ellipsoid GRS67{6378160.0, 1.0 / 298.247167427};

// Airy 1830
constinit const Ellipsoid Airy1830{6377563.396, 1.0 / 299.3249646};

// Bessel 1841
constinit const Ellipsoid Bessel1841{6377397.155, 1.0 / 299.1528128};

// Clarke 1866
constinit const Ellipsoid Clarke1866{6378206.4, 1.0 / 294.9786982};

// International 1924 (Hayford 1909)
constinit const Ellipsoid International1924{6378388.0, 1.0 / 297.0};

}  // namespace trans_geo::ellipsoid
