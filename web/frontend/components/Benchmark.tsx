"use client";

import { useState } from "react";
import { Play, Loader2, Cpu } from "lucide-react";
import { BarChart, Bar, XAxis, YAxis, CartesianGrid, Tooltip, ResponsiveContainer } from "recharts";

interface BenchmarkResult {
  benchmarks: Array<{
    backend: string;
    total_ms: string;
    compute_ms: string;
    simulations: string;
  }>;
}

export default function Benchmark() {
  const [portfolio, setPortfolio] = useState("symbol,quantity,price\nAAPL,1000,190.0\nGOOGL,500,140.0\nMSFT,800,370.0");
  const [simulations, setSimulations] = useState(1000000);
  const [loading, setLoading] = useState(false);
  const [result, setResult] = useState<BenchmarkResult | null>(null);
  const [error, setError] = useState("");

  const runBenchmark = async () => {
    setLoading(true);
    setError("");
    setResult(null);

    try {
      const response = await fetch("http://localhost:8000/api/benchmark", {
        method: "POST",
        headers: { "Content-Type": "application/json" },
        body: JSON.stringify({
          portfolio_csv: portfolio,
          simulations,
        }),
      });

      if (!response.ok) {
        const err = await response.json();
        throw new Error(err.detail || "Failed to run benchmark");
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

  const chartData = result?.benchmarks.map((b) => ({
    backend: b.backend.toUpperCase(),
    compute: parseFloat(b.compute_ms),
    total: parseFloat(b.total_ms),
  })) || [];

  return (
    <div className="space-y-6">
      <h2 className="text-2xl font-bold mb-4">Backend Benchmark</h2>

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

        <button
          onClick={runBenchmark}
          disabled={loading}
          className="flex items-center gap-2 bg-blue-600 hover:bg-blue-700 disabled:bg-gray-600 px-6 py-2 rounded-lg font-medium transition-colors"
        >
          {loading ? (
            <Loader2 className="w-5 h-5 animate-spin" />
          ) : (
            <Play className="w-5 h-5" />
          )}
          Run Benchmark
        </button>
      </div>

      {error && (
        <div className="bg-red-900/50 border border-red-700 rounded-lg p-4 text-red-300">
          {error}
        </div>
      )}

      {result && (
        <div className="space-y-6">
          <div className="bg-gray-700 rounded-lg p-6">
            <h3 className="text-xl font-bold mb-4 flex items-center gap-2">
              <Cpu className="w-6 h-6" />
              Performance Comparison
            </h3>
            <div className="h-64">
              <ResponsiveContainer width="100%" height="100%">
                <BarChart data={chartData}>
                  <CartesianGrid strokeDasharray="3 3" stroke="#4B5563" />
                  <XAxis dataKey="backend" stroke="#9CA3AF" />
                  <YAxis stroke="#9CA3AF" />
                  <Tooltip
                    contentStyle={{ backgroundColor: "#1F2937", border: "1px solid #4B5563" }}
                    itemStyle={{ color: "#E5E7EB" }}
                  />
                  <Bar dataKey="compute" fill="#3B82F6" name="Compute (ms)" />
                  <Bar dataKey="total" fill="#10B981" name="Total (ms)" />
                </BarChart>
              </ResponsiveContainer>
            </div>
          </div>

          <div className="bg-gray-700 rounded-lg p-6">
            <h3 className="text-xl font-bold mb-4">Detailed Results</h3>
            <div className="overflow-x-auto">
              <table className="w-full text-sm">
                <thead>
                  <tr className="border-b border-gray-600">
                    <th className="text-left py-2">Backend</th>
                    <th className="text-right py-2">Compute (ms)</th>
                    <th className="text-right py-2">Total (ms)</th>
                    <th className="text-right py-2">Simulations</th>
                  </tr>
                </thead>
                <tbody>
                  {result.benchmarks.map((b, i) => (
                    <tr key={i} className="border-b border-gray-600">
                      <td className="py-2 font-medium">{b.backend.toUpperCase()}</td>
                      <td className="text-right py-2">{parseFloat(b.compute_ms).toFixed(2)}</td>
                      <td className="text-right py-2">{parseFloat(b.total_ms).toFixed(2)}</td>
                      <td className="text-right py-2">{parseInt(b.simulations).toLocaleString()}</td>
                    </tr>
                  ))}
                </tbody>
              </table>
            </div>
          </div>
        </div>
      )}
    </div>
  );
}
