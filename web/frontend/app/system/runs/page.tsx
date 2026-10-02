"use client";

export default function SystemRunsPage() {
  return (
    <div className="p-6 space-y-6">
      <div className="mb-6">
        <h1 className="text-2xl font-bold tracking-tight">Run History</h1>
        <p className="text-sm text-foreground-muted mt-1">Simulation run history and reproducibility</p>
      </div>

      <div className="bg-surface border border-border rounded-lg p-6">
        <h2 className="text-lg font-medium mb-4">Recent Runs</h2>
        <div className="text-sm text-foreground-muted">
          Run history table will be displayed here.
        </div>
      </div>
    </div>
  );
}
