#pragma once

namespace quantforge::simulation::models {

// Geometric Brownian Motion single-step terminal value model.
// S_T = S_0 * exp((drift - 0.5*vol^2) + vol * Z), Z ~ N(0,1)
//
// This header exists as the extension point for model-specific parameters
// (e.g. jump-diffusion params, GARCH state) once more than one model is
// implemented. See src/simulation/models/model_registry.cpp for how a new
// model plugs in.
struct GBMParams {
    double drift      = 0.0;
    double volatility = 0.0;
};

} // namespace quantforge::simulation::models
