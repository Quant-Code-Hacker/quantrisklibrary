"use client";

import MetricCard from '@/components/dashboard/MetricCard';

export default function RiskLabPage() {
  return (
    <div className="p-6 space-y-6">
      <div className="mb-6">
        <h1 className="text-2xl font-bold tracking-tight">Risk Lab</h1>
        <p className="text-sm text-foreground-muted mt-1">Comprehensive risk metrics analysis</p>
      </div>

      <div className="grid grid-cols-4 gap-4">
        <MetricCard label="Marginal VaR" value={184083} format="currency" accent="danger" />
        <MetricCard label="Component VaR" value={184083} format="currency" accent="danger" />
        <MetricCard label="Incremental VaR" value={184083} format="currency" accent="danger" />
        <MetricCard label="Tracking Error" value={0.032} format="percent" accent="primary" />
      </div>

      <div className="bg-surface border border-border rounded-lg p-6">
        <h2 className="text-lg font-medium mb-4">Risk Analysis</h2>
        <div className="text-sm text-foreground-muted">
          Risk visualization and analysis tools will be displayed here.
        </div>
      </div>
    </div>
  );
}
