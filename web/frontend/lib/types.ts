export interface RiskResult {
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

export interface BenchmarkResult {
  benchmarks: Array<{
    backend: string;
    total_ms: string;
    compute_ms: string;
    simulations: string;
  }>;
}

export interface StressResult {
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

export interface Portfolio {
  symbol: string;
  quantity: number;
  price: number;
}

export interface EngineStatus {
  status: 'online' | 'offline' | 'error';
  backend: 'CPU' | 'OpenMP' | 'CUDA' | 'Auto';
  gpu_available: boolean;
  cpu_utilization: number;
  memory_usage: number;
  current_simulation: {
    running: boolean;
    progress: number;
    paths_completed: number;
    total_paths: number;
    throughput: number;
  } | null;
}

export interface MetricCard {
  id: string;
  label: string;
  value: number | string;
  change?: number;
  format: 'currency' | 'percent' | 'number' | 'duration';
  trend?: 'up' | 'down' | 'neutral';
  clickable: boolean;
  route?: string;
}
