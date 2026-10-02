"use client";

import { useState } from "react";
import { Play, Loader2 } from "lucide-react";

interface RiskResult {
  metric: string;
  value: number;
  confidence: number;
  simulations_run: number;
  seed: number;
  backend: string;
  library_version: string;
  git_commit: string;
  total_ms: number;
  compute_ms: number;
}

export default function RiskCalculator() {
  const [portfolio, setPortfolio] = useState("symbol,quantity,price\nAAPL,1000,190.0\nGOOGL,500,140.0\nMSFT,800,370.0");
  const [metric, setMetric] = useState("var");
  const [confidence, setConfidence] = useState(0.99);
  const [simulations, setSimulations] = useState(1000000);
  const [loading, setLoading] = useState(false);
  const [result, setResult] = useState<RiskResult | null>(null);
  const [error, setError] = useState("");

  const calculateRisk = async () => {
    setLoading(true);
    setError("");
    setResult(null);

    try {
      const response = await fetch("http://localhost:8000/api/risk", {
        method: "POST",
        headers: { "Content-Type": "application/json" },
        body: JSON.stringify({
          portfolio_csv: portfolio,
          metric,
          confidence,
          simulations,
        }),
      });

      if (!response.ok) {
        const err = await response.json();
        throw new Error(err.detail || "Failed to calculate risk");
      }

      const data = await response.json();
      setResult(data);
    } catch (err: any) {
      setError(err.message);
    } finally {
      setLoading(false);
    }
  };

  const loadSample = async () => {
    try {
      const response = await fetch("http://localhost:8000/api/sample-portfolio");
      const data = await response.json();
      setPortfolio(data.csv);
    } catch (err) {
      setError("Failed to load sample portfolio");
    }
  };

  return (
    <div className="space-y-6">
      <h2 className="text-2xl font-bold mb-4">Risk Calculator</h2>

      <div className="space-y-4">
        <div>
          <label className="block text-sm font-medium mb-2">Portfolio (CSV)</label>
          <textarea
            value={portfolio}
            onChange={(e) => setPortfolio(e.target.value)}
            className="w-full h-32 bg-gray-700 border border-gray-600 rounded-lg p-3 text-sm font-mono"
            placeholder="symbol,quantity,price"
          />
          <button
            onClick={loadSample}
            className="mt-2 text-sm text-blue-400 hover:text-blue-300"
          >
            Load sample portfolio
          </button>
        </div>

        <div className="grid grid-cols-3 gap-4">
          <div>
            <label className="block text-sm font-medium mb-2">Metric</label>
            <select
              value={metric}
              onChange={(e) => setMetric(e.target.value)}
              className="w-full bg-gray-700 border border-gray-600 rounded-lg p-2"
            >
              <option value="var">VaR</option>
              <option value="cvar">CVaR</option>
            </select>
          </div>

          <div>
            <label className="block text-sm font-medium mb-2">Confidence</label>
            <select
              value={confidence}
              onChange={(e) => setConfidence(parseFloat(e.target.value))}
              className="w-full bg-gray-700 border border-gray-600 rounded-lg p-2"
            >
              <option value="0.95">95%</option>
              <option value="0.99">99%</option>
              <option value="0.999">99.9%</option>
            </select>
          </div>

          <div>
            <label className="block text-sm font-medium mb-2">Simulations</label>
            <input
              type="number"
              value={simulations}
              onChange={(e) => setSimulations(parseInt(e.target.value))}
              className="w-full bg-gray-700 border border-gray-600 rounded-lg p-2"
              min="1000"
              step="100000"
            />
          </div>
        </div>

        <button
          onClick={calculateRisk}
          disabled={loading}
          className="flex items-center gap-2 bg-blue-600 hover:bg-blue-700 disabled:bg-gray-600 px-6 py-2 rounded-lg font-medium transition-colors"
        >
          {loading ? (
            <Loader2 className="w-5 h-5 animate-spin" />
          ) : (
            <Play className="w-5 h-5" />
          )}
          Calculate
        </button>
      </div>

      {error && (
        <div className="bg-red-900/50 border border-red-700 rounded-lg p-4 text-red-300">
          {error}
        </div>
      )}

      {result && (
        <div className="bg-gray-700 rounded-lg p-6 space-y-4">
          <h3 className="text-xl font-bold text-green-400">
            {result.metric} Result: ${result.value.toLocaleString()}
          </h3>
          <div className="grid grid-cols-2 gap-4 text-sm">
            <div>
              <span className="text-gray-400">Confidence:</span> {(result.confidence * 100).toFixed(1)}%
            </div>
            <div>
              <span className="text-gray-400">Simulations:</span> {result.simulations_run.toLocaleString()}
            </div>
            <div>
              <span className="text-gray-400">Backend:</span> {result.backend}
            </div>
            <div>
              <span className="text-gray-400">Compute Time:</span> {result.compute_ms.toFixed(2)}ms
            </div>
            <div>
              <span className="text-gray-400">Total Time:</span> {result.total_ms.toFixed(2)}ms
            </div>
            <div>
              <span className="text-gray-400">Seed:</span> {result.seed}
            </div>
          </div>
        </div>
      )}
    </div>
  );
}
