// -----------------------------------------------------------------------------
// Copyright  : (C) 2014 Andreas-C. Bernstein
// License    : MIT (see the file LICENSE)
// Maintainer : Andreas-C. Bernstein <andreas.bernstein@uni-weimar.de>
// Stability  : experimental
//
// Fensterchen
// -----------------------------------------------------------------------------

#ifndef BUW_FENSTERCHEN_HPP
#define BUW_FENSTERCHEN_HPP

#define RAY_EPSILON 0.00001;
#define MAX_RAY_RECURSION_DEPTH 5
#define MAX_RAY_LENGTH 1000.0f

#include <iostream>

#include "color.hpp"

#include "ray.hpp"
#include "intersection.hpp"

#include "material.hpp"

#include "shape.hpp"
#include "composite.hpp"
#include "box.hpp"
#include "sphere.hpp"
#include "cylinder.hpp"
#include "triangle.hpp"

#include "sample.hpp"

#include "sampler.hpp"
#include "standard_grid_sampler.hpp"
#include "rotated_grid_sampler.hpp"

#include "renderer.hpp"

#include "window.hpp"

#endif // BUW_FENSTERCHEN_HPP
