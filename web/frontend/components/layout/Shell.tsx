"use client";

import Sidebar from './Sidebar';
import StatusBar from './StatusBar';

export default function Shell({ children }: { children: React.ReactNode }) {
  return (
    <div className="flex h-screen bg-background bg-grid">
      <Sidebar />
      <div className="flex-1 flex flex-col overflow-hidden">
        <StatusBar />
        <main className="flex-1 overflow-auto">
          {children}
        </main>
      </div>
    </div>
  );
}
