"use client";

import { useState } from "react";
import { Calculator, BarChart3, AlertTriangle, Activity } from "lucide-react";
import RiskCalculator from "@/components/RiskCalculator";
import Benchmark from "@/components/Benchmark";
import StressTest from "@/components/StressTest";

type Tab = "risk" | "benchmark" | "stress";

export default function Home() {
  const [activeTab, setActiveTab] = useState<Tab>("risk");

  return (
    <main className="min-h-screen bg-gradient-to-br from-gray-900 to-gray-800 text-white">
      <div className="container mx-auto px-4 py-8">
        <header className="mb-8">
          <h1 className="text-4xl font-bold mb-2 flex items-center gap-3">
            <Activity className="w-10 h-10 text-blue-400" />
            QuantForge
          </h1>
          <p className="text-gray-400">Financial Risk Computing Engine</p>
        </header>

        <div className="flex gap-2 mb-6">
          <button
            onClick={() => setActiveTab("risk")}
            className={`flex items-center gap-2 px-4 py-2 rounded-lg transition-colors ${
              activeTab === "risk"
                ? "bg-blue-600 text-white"
                : "bg-gray-700 text-gray-300 hover:bg-gray-600"
            }`}
          >
            <Calculator className="w-5 h-5" />
            Risk Calculator
          </button>
          <button
            onClick={() => setActiveTab("benchmark")}
            className={`flex items-center gap-2 px-4 py-2 rounded-lg transition-colors ${
              activeTab === "benchmark"
                ? "bg-blue-600 text-white"
                : "bg-gray-700 text-gray-300 hover:bg-gray-600"
            }`}
          >
            <BarChart3 className="w-5 h-5" />
            Benchmark
          </button>
          <button
            onClick={() => setActiveTab("stress")}
            className={`flex items-center gap-2 px-4 py-2 rounded-lg transition-colors ${
              activeTab === "stress"
                ? "bg-blue-600 text-white"
                : "bg-gray-700 text-gray-300 hover:bg-gray-600"
            }`}
          >
            <AlertTriangle className="w-5 h-5" />
            Stress Test
          </button>
        </div>

        <div className="bg-gray-800 rounded-xl p-6 shadow-xl">
          {activeTab === "risk" && <RiskCalculator />}
          {activeTab === "benchmark" && <Benchmark />}
          {activeTab === "stress" && <StressTest />}
        </div>
      </div>
    </main>
  );
}
