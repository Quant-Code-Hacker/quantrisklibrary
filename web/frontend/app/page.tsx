"use client";

import MetricCard from '@/components/dashboard/MetricCard';

export default function CommandCenter() {
  return (
    <div className="p-6 space-y-6">
      {/* Header */}
      <div className="mb-6">
        <h1 className="text-2xl font-bold tracking-tight">Command Center</h1>
        <p className="text-sm text-foreground-muted mt-1">Real-time portfolio and risk overview</p>
      </div>

      {/* KPI Grid */}
      <div className="grid grid-cols-4 gap-4">
        <MetricCard
          label="Portfolio Value"
          value={1247500}
          format="currency"
          change={0.0234}
          trend="up"
          clickable
          accent="primary"
        />
        <MetricCard
          label="Daily P&L"
          value={28500}
          format="currency"
          change={0.0234}
          trend="up"
          accent="success"
        />
        <MetricCard
          label="Return"
          value={0.1523}
          format="percent"
          change={0.0045}
          trend="up"
          accent="success"
        />
        <MetricCard
          label="Volatility"
          value={0.142}
          format="percent"
          change={-0.0023}
          trend="down"
          accent="warning"
        />
        <MetricCard
          label="VaR 99%"
          value={184083}
          format="currency"
          clickable
          accent="danger"
        />
        <MetricCard
          label="CVaR 99%"
          value={221500}
          format="currency"
          clickable
          accent="danger"
        />
        <MetricCard
          label="Max Drawdown"
          value={-0.0842}
          format="percent"
          trend="down"
          accent="warning"
        />
        <MetricCard
          label="Sharpe Ratio"
          value={1.42}
          format="number"
          change={0.05}
          trend="up"
          accent="success"
        />
      </div>

      {/* Secondary Metrics */}
      <div className="grid grid-cols-6 gap-4">
        <MetricCard
          label="Beta"
          value={1.15}
          format="number"
          accent="primary"
        />
        <MetricCard
          label="Tracking Error"
          value={0.032}
          format="percent"
          accent="primary"
        />
        <MetricCard
          label="Skewness"
          value={-0.42}
          format="number"
          accent="primary"
        />
        <MetricCard
          label="Kurtosis"
          value={3.85}
          format="number"
          accent="primary"
        />
        <MetricCard
          label="Sortino Ratio"
          value={1.89}
          format="number"
          accent="success"
        />
        <MetricCard
          label="Calmar Ratio"
          value={1.81}
          format="number"
          accent="success"
        />
      </div>

      {/* Quick Actions */}
      <div className="bg-surface border border-border rounded-lg p-4">
        <h2 className="text-sm font-medium text-foreground-muted uppercase tracking-wider mb-3">
          Quick Actions
        </h2>
        <div className="grid grid-cols-4 gap-3">
          <button className="px-4 py-2 bg-surface-elevated hover:bg-surface-hover border border-border rounded-lg text-sm transition-colors">
            Run VaR Calculation
          </button>
          <button className="px-4 py-2 bg-surface-elevated hover:bg-surface-hover border border-border rounded-lg text-sm transition-colors">
            Run Benchmark
          </button>
          <button className="px-4 py-2 bg-surface-elevated hover:bg-surface-hover border border-border rounded-lg text-sm transition-colors">
            Stress Test
          </button>
          <button className="px-4 py-2 bg-surface-elevated hover:bg-surface-hover border border-border rounded-lg text-sm transition-colors">
            Monte Carlo Sim
          </button>
        </div>
      </div>
    </div>
  );
}
