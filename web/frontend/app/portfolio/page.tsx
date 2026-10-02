"use client";

import MetricCard from '@/components/dashboard/MetricCard';

export default function PortfolioPage() {
  return (
    <div className="p-6 space-y-6">
      <div className="mb-6">
        <h1 className="text-2xl font-bold tracking-tight">Portfolio</h1>
        <p className="text-sm text-foreground-muted mt-1">Portfolio analysis and positions</p>
      </div>

      <div className="grid grid-cols-4 gap-4">
        <MetricCard label="Portfolio Value" value={1247500} format="currency" accent="primary" />
        <MetricCard label="Daily P&L" value={28500} format="currency" accent="success" />
        <MetricCard label="Return" value={0.1523} format="percent" accent="success" />
        <MetricCard label="Volatility" value={0.142} format="percent" accent="warning" />
      </div>

      <div className="bg-surface border border-border rounded-lg p-6">
        <h2 className="text-lg font-medium mb-4">Positions</h2>
        <div className="text-sm text-foreground-muted">
          Portfolio positions table will be displayed here.
        </div>
      </div>
    </div>
  );
}
