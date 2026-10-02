#pragma once

#include <string>
#include <vector>

namespace quantforge {

// Tag types. Each has a matching compute() free function found via ADL in
// src/risk/metrics/*.cpp. Adding a new metric means adding a new tag +
// implementation file — nothing else in the engine changes.

// Market Risk Metrics
struct VaR                    { static constexpr const char* name = "VaR"; };
struct CVaR                   { static constexpr const char* name = "CVaR"; };
struct Volatility             { static constexpr const char* name = "Volatility"; };
struct DownsideDeviation      { static constexpr const char* name = "DownsideDeviation"; };
struct SemiDeviation          { static constexpr const char* name = "SemiDeviation"; };
struct Skewness               { static constexpr const char* name = "Skewness"; };
struct Kurtosis               { static constexpr const char* name = "Kurtosis"; };
struct MarginalVaR            { static constexpr const char* name = "MarginalVaR"; };
struct ComponentVaR           { static constexpr const char* name = "ComponentVaR"; };
struct IncrementalVaR         { static constexpr const char* name = "IncrementalVaR"; };
struct Beta                  { static constexpr const char* name = "Beta"; };
struct TrackingError          { static constexpr const char* name = "TrackingError"; };

// Drawdown Metrics
struct MaxDrawdown            { static constexpr const char* name = "MaxDrawdown"; };
struct AverageDrawdown        { static constexpr const char* name = "AverageDrawdown"; };
struct DrawdownDuration       { static constexpr const char* name = "DrawdownDuration"; };
struct UlcerIndex             { static constexpr const char* name = "UlcerIndex"; };

// Risk-Adjusted Performance Ratios
struct SharpeRatio            { static constexpr const char* name = "SharpeRatio"; };
struct SortinoRatio           { static constexpr const char* name = "SortinoRatio"; };
struct CalmarRatio            { static constexpr const char* name = "CalmarRatio"; };
struct InformationRatio       { static constexpr const char* name = "InformationRatio"; };
struct OmegaRatio             { static constexpr const char* name = "OmegaRatio"; };
struct TreynorRatio           { static constexpr const char* name = "TreynorRatio"; };
struct SterlingRatio          { static constexpr const char* name = "SterlingRatio"; };
struct BurkeRatio             { static constexpr const char* name = "BurkeRatio"; };

// Greeks (placeholder - requires options pricing models)
struct Delta                  { static constexpr const char* name = "Delta"; };
struct Gamma                  { static constexpr const char* name = "Gamma"; };
struct Vega                   { static constexpr const char* name = "Vega"; };
struct Theta                  { static constexpr const char* name = "Theta"; };
struct Rho                    { static constexpr const char* name = "Rho"; };

// Credit Risk Metrics (placeholder - requires credit models)
struct ProbabilityOfDefault   { static constexpr const char* name = "ProbabilityOfDefault"; };
struct LossGivenDefault       { static constexpr const char* name = "LossGivenDefault"; };
struct ExpectedCreditLoss     { static constexpr const char* name = "ExpectedCreditLoss"; };
struct CreditVaR              { static constexpr const char* name = "CreditVaR"; };

// Duration Risk Metrics (placeholder - requires bond pricing models)
struct Duration               { static constexpr const char* name = "Duration"; };
struct Convexity              { static constexpr const char* name = "Convexity"; };
struct DV01                   { static constexpr const char* name = "DV01"; };

// Every metric implementation has this shape:
//   double compute(const std::vector<double>& losses, double confidence);
double compute(VaR, const std::vector<double>& losses, double confidence);
double compute(CVaR, const std::vector<double>& losses, double confidence);
double compute(Volatility, const std::vector<double>& losses, double confidence);
double compute(DownsideDeviation, const std::vector<double>& losses, double confidence);
double compute(SemiDeviation, const std::vector<double>& losses, double confidence);
double compute(Skewness, const std::vector<double>& losses, double confidence);
double compute(Kurtosis, const std::vector<double>& losses, double confidence);
double compute(MaxDrawdown, const std::vector<double>& losses, double confidence);
double compute(AverageDrawdown, const std::vector<double>& losses, double confidence);
double compute(UlcerIndex, const std::vector<double>& losses, double confidence);
double compute(SharpeRatio, const std::vector<double>& losses, double confidence);
double compute(SortinoRatio, const std::vector<double>& losses, double confidence);
double compute(CalmarRatio, const std::vector<double>& losses, double confidence);
double compute(InformationRatio, const std::vector<double>& losses, double confidence);
double compute(OmegaRatio, const std::vector<double>& losses, double confidence);
double compute(MarginalVaR, const std::vector<double>& losses, double confidence);
double compute(ComponentVaR, const std::vector<double>& losses, double confidence);
double compute(IncrementalVaR, const std::vector<double>& losses, double confidence);
double compute(Beta, const std::vector<double>& losses, double confidence);
double compute(TrackingError, const std::vector<double>& losses, double confidence);
double compute(SterlingRatio, const std::vector<double>& losses, double confidence);
double compute(BurkeRatio, const std::vector<double>& losses, double confidence);
double compute(DiversificationRatio, const std::vector<double>& losses, double confidence);
double compute(HHI, const std::vector<double>& losses, double confidence);

} // namespace quantforge
