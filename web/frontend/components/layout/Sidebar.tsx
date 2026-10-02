"use client";

import { useState } from 'react';
import { motion } from 'framer-motion';
import {
  LayoutDashboard,
  TrendingUp,
  Shield,
  Activity,
  Cpu,
  Settings,
  ChevronRight,
  FolderKanban,
  BarChart3,
  Zap,
  Database,
  Clock,
  FileText,
} from 'lucide-react';
import { cn } from '@/lib/utils';

interface NavItem {
  id: string;
  label: string;
  icon: any;
  children?: NavItem[];
  route?: string;
}

const navItems: NavItem[] = [
  {
    id: 'overview',
    label: 'Overview',
    icon: LayoutDashboard,
    children: [
      { id: 'command-center', label: 'Command Center', icon: Activity, route: '/' },
      { id: 'portfolio', label: 'Portfolio', icon: FolderKanban, route: '/portfolio' },
      { id: 'market', label: 'Market', icon: TrendingUp, route: '/market' },
    ],
  },
  {
    id: 'risk',
    label: 'Risk',
    icon: Shield,
    children: [
      { id: 'risk-lab', label: 'Risk Lab', icon: BarChart3, route: '/risk-lab' },
      { id: 'var', label: 'VaR', icon: Zap, route: '/risk/var' },
      { id: 'es', label: 'Expected Shortfall', icon: Activity, route: '/risk/es' },
      { id: 'drawdown', label: 'Drawdown', icon: TrendingUp, route: '/risk/drawdown' },
    ],
  },
  {
    id: 'compute',
    label: 'Compute',
    icon: Cpu,
    children: [
      { id: 'cpu', label: 'CPU', icon: Cpu, route: '/compute/cpu' },
      { id: 'openmp', label: 'OpenMP', icon: Zap, route: '/compute/openmp' },
      { id: 'cuda', label: 'CUDA', icon: Activity, route: '/compute/cuda' },
      { id: 'benchmark', label: 'Benchmark', icon: BarChart3, route: '/compute/benchmark' },
    ],
  },
  {
    id: 'simulation',
    label: 'Simulation',
    icon: Database,
    children: [
      { id: 'monte-carlo', label: 'Monte Carlo', icon: Activity, route: '/simulation/monte-carlo' },
      { id: 'stress', label: 'Stress Testing', icon: Shield, route: '/simulation/stress' },
    ],
  },
  {
    id: 'system',
    label: 'System',
    icon: Settings,
    children: [
      { id: 'runs', label: 'Runs', icon: Clock, route: '/system/runs' },
      { id: 'logs', label: 'Logs', icon: FileText, route: '/system/logs' },
      { id: 'config', label: 'Configuration', icon: Settings, route: '/system/config' },
    ],
  },
];

export default function Sidebar() {
  const [expandedItems, setExpandedItems] = useState<Set<string>>(new Set(['overview']));

  const toggleExpanded = (id: string) => {
    const newExpanded = new Set(expandedItems);
    if (newExpanded.has(id)) {
      newExpanded.delete(id);
    } else {
      newExpanded.add(id);
    }
    setExpandedItems(newExpanded);
  };

  return (
    <div className="w-64 h-screen bg-surface border-r border-border flex flex-col">
      {/* Logo */}
      <div className="p-4 border-b border-border">
        <div className="flex items-center gap-3">
          <div className="w-8 h-8 bg-accent-primary rounded flex items-center justify-center">
            <Activity className="w-5 h-5 text-white" />
          </div>
          <div>
            <h1 className="text-lg font-bold tracking-tight">QUANTFORGE</h1>
            <p className="text-xs text-foreground-subtle">Risk Computing Engine</p>
          </div>
        </div>
      </div>

      {/* Navigation */}
      <nav className="flex-1 overflow-y-auto p-2 space-y-1">
        {navItems.map((item) => (
          <div key={item.id}>
            <button
              onClick={() => toggleExpanded(item.id)}
              className={cn(
                "w-full flex items-center gap-3 px-3 py-2 rounded-lg text-sm transition-colors",
                "hover:bg-surface-hover text-foreground-muted hover:text-foreground"
              )}
            >
              <item.icon className="w-4 h-4" />
              <span className="flex-1 text-left">{item.label}</span>
              <ChevronRight
                className={cn(
                  "w-4 h-4 transition-transform",
                  expandedItems.has(item.id) ? "rotate-90" : ""
                )}
              />
            </button>

            {expandedItems.has(item.id) && item.children && (
              <motion.div
                initial={{ opacity: 0, height: 0 }}
                animate={{ opacity: 1, height: 'auto' }}
                exit={{ opacity: 0, height: 0 }}
                className="ml-4 mt-1 space-y-1"
              >
                {item.children.map((child) => (
                  <a
                    key={child.id}
                    href={child.route}
                    className={cn(
                      "flex items-center gap-2 px-3 py-1.5 rounded-lg text-xs transition-colors",
                      "hover:bg-surface-hover text-foreground-subtle hover:text-foreground"
                    )}
                  >
                    <child.icon className="w-3.5 h-3.5" />
                    {child.label}
                  </a>
                ))}
              </motion.div>
            )}
          </div>
        ))}
      </nav>

      {/* Status */}
      <div className="p-4 border-t border-border">
        <div className="flex items-center gap-2 text-xs">
          <div className="w-2 h-2 bg-accent-success rounded-full animate-pulse-subtle" />
          <span className="text-foreground-muted">Engine Online</span>
        </div>
        <div className="mt-2 text-xs text-foreground-subtle font-mono">
          v0.1.0 · 8617a49
        </div>
      </div>
    </div>
  );
}
