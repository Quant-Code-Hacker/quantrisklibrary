import { RiskResult, BenchmarkResult, StressResult } from './types';

const API_BASE = 'http://localhost:8000';

export async function calculateRisk(
  portfolioCsv: string,
  metric: string = 'var',
  confidence: number = 0.99,
  simulations: number = 1000000
): Promise<RiskResult> {
  const response = await fetch(`${API_BASE}/api/risk`, {
    method: 'POST',
    headers: { 'Content-Type': 'application/json' },
    body: JSON.stringify({
      portfolio_csv: portfolioCsv,
      metric,
      confidence,
      simulations,
    }),
  });

  if (!response.ok) {
    throw new Error('Failed to calculate risk');
  }

  return response.json();
}

export async function runBenchmark(
  portfolioCsv: string,
  simulations: number = 1000000
): Promise<BenchmarkResult> {
  const response = await fetch(`${API_BASE}/api/benchmark`, {
    method: 'POST',
    headers: { 'Content-Type': 'application/json' },
    body: JSON.stringify({
      portfolio_csv: portfolioCsv,
      simulations,
    }),
  });

  if (!response.ok) {
    throw new Error('Failed to run benchmark');
  }

  return response.json();
}

export async function runStressTest(
  portfolioCsv: string,
  scenario: string = 'market-crash'
): Promise<StressResult> {
  const response = await fetch(`${API_BASE}/api/stress`, {
    method: 'POST',
    headers: { 'Content-Type': 'application/json' },
    body: JSON.stringify({
      portfolio_csv: portfolioCsv,
      scenario,
    }),
  });

  if (!response.ok) {
    throw new Error('Failed to run stress test');
  }

  return response.json();
}

export async function getSamplePortfolio(): Promise<string> {
  const response = await fetch(`${API_BASE}/api/sample-portfolio`);
  if (!response.ok) {
    throw new Error('Failed to get sample portfolio');
  }
  const data = await response.json();
  return data.csv;
}

export async function getHealth(): Promise<{ status: string; cli_exists: boolean }> {
  const response = await fetch(`${API_BASE}/health`);
  if (!response.ok) {
    throw new Error('Failed to get health status');
  }
  return response.json();
}
