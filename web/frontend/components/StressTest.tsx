"use client";

import { useState } from "react";
import { Play, Loader2, AlertTriangle } from "lucide-react";

interface StressResult {
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

export default function StressTest() {
  const [portfolio, setPortfolio] = useState("symbol,quantity,price\nAAPL,1000,190.0\nGOOGL,500,140.0\nMSFT,800,370.0");
  const [scenario, setScenario] = useState("market-crash");
  const [loading, setLoading] = useState(false);
  const [result, setResult] = useState<StressResult | null>(null);
  const [error, setError] = useState("");

  const scenarios = [
    { value: "market-crash", label: "Market Crash (70% drop)" },
    { value: "correction", label: "Market Correction (20% drop)" },
    { value: "volatility-spike", label: "Volatility Spike" },
  ];

  const runStressTest = async () => {
    setLoading(true);
    setError("");
    setResult(null);

    try {
      const response = await fetch("http://localhost:8000/api/stress", {
        method: "POST",
        headers: { "Content-Type": "application/json" },
        body: JSON.stringify({
          portfolio_csv: portfolio,
          scenario,
        }),
      });

      if (!response.ok) {
        const err = await response.json();
        throw new Error(err.detail || "Failed to run stress test");
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
      <h2 className="text-2xl font-bold mb-4">Stress Testing</h2>

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

        <div>
          <label className="block text-sm font-medium mb-2">Scenario</label>
          <select
            value={scenario}
            onChange={(e) => setScenario(e.target.value)}
            className="w-full bg-gray-700 border border-gray-600 rounded-lg p-2"
          >
            {scenarios.map((s) => (
              <option key={s.value} value={s.value}>
                {s.label}
              </option>
            ))}
          </select>
        </div>

        <button
          onClick={runStressTest}
          disabled={loading}
          className="flex items-center gap-2 bg-orange-600 hover:bg-orange-700 disabled:bg-gray-600 px-6 py-2 rounded-lg font-medium transition-colors"
        >
          {loading ? (
            <Loader2 className="w-5 h-5 animate-spin" />
          ) : (
            <Play className="w-5 h-5" />
          )}
          Run Stress Test
        </button>
      </div>

      {error && (
        <div className="bg-red-900/50 border border-red-700 rounded-lg p-4 text-red-300">
          {error}
        </div>
      )}

      {result && (
        <div className="bg-gray-700 rounded-lg p-6 space-y-4">
          <h3 className="text-xl font-bold text-orange-400 flex items-center gap-2">
            <AlertTriangle className="w-6 h-6" />
            Stress Test Results
          </h3>
          <div className="bg-orange-900/30 border border-orange-700 rounded-lg p-4">
            <p className="text-sm text-gray-400 mb-1">Scenario: {scenario}</p>
            <p className="text-3xl font-bold">
              {result.metric}: ${result.value.toLocaleString()}
            </p>
          </div>
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
          </div>
        </div>
      )}
    </div>
  );
}
