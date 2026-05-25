<section class="glass-card">
  <h2><i class="fas fa-project-diagram"></i> Ecosystem Nodes 🐱🌐</h2>
  <p>Active development and management across decentralized networks:</p>
  <div class="eco-grid">
    <a href="https://acreetionos.org" target="_blank" class="eco-node">
      <div class="node-icon" style="background:rgba(124,58,237,0.2);color:#a78bfa;">🖥️</div>
      <div class="node-info">
        <strong>AcreetionOS</strong>
        <span>High-availability from kernel up</span>
      </div>
    </a>
    <a href="https://arttulos.acreetionos.org" target="_blank" class="eco-node">
      <div class="node-icon" style="background:rgba(236,72,153,0.2);color:#f9a8d4;">♿</div>
      <div class="node-info">
        <strong>ArttulOS</strong>
        <span>Accessibility layer of the future</span>
      </div>
    </a>
    <a href="https://spivanatalie64.github.io/ion-website/" target="_blank" class="eco-node">
      <div class="node-icon" style="background:rgba(85,205,252,0.2);color:#55cdfc;">📱</div>
      <div class="node-info">
        <strong>Ion ROM</strong>
        <span>Open-source Android ROM</span>
      </div>
    </a>
    <div class="eco-node" style="cursor:default;">
      <div class="node-icon" style="background:rgba(45,212,191,0.2);color:#2dd4bf;">🌍</div>
      <div class="node-info">
        <strong>Mirror Network</strong>
        <span>72+ active nodes worldwide</span>
      </div>
    </div>
    <a href="https://natalie.acreetionos.org" target="_blank" class="eco-node">
      <div class="node-icon" style="background:rgba(251,191,36,0.2);color:#fbbf24;">📝</div>
      <div class="node-info">
        <strong>Blog</strong>
        <span>AcreetionOS updates &amp; dev logs</span>
      </div>
    </a>
  </div>
</section>

<style>
  .glass-card {
    background: rgba(255, 255, 255, 0.06);
    backdrop-filter: blur(20px);
    -webkit-backdrop-filter: blur(20px);
    border: 1px solid rgba(255, 255, 255, 0.12);
    border-radius: 24px;
    padding: 32px;
    margin-bottom: 28px;
    box-shadow: 0 8px 40px rgba(0, 0, 0, 0.3);
    transition: all 0.4s cubic-bezier(0.34, 1.56, 0.64, 1);
    position: relative;
    overflow: hidden;
  }

  .glass-card::before {
    content: '';
    position: absolute;
    top: 0;
    left: 0;
    right: 0;
    height: 3px;
    background: linear-gradient(90deg, #55cdfc, #f7a8b8, #7c3aed, #f7a8b8, #55cdfc);
    background-size: 200% 100%;
    animation: shimmer 4s linear infinite;
  }

  @keyframes shimmer {
    0% { background-position: 0% 0%; }
    100% { background-position: 200% 0%; }
  }

  .glass-card:hover {
    border-color: rgba(255, 255, 255, 0.25);
    transform: translateY(-4px);
    box-shadow: 0 12px 56px rgba(0, 0, 0, 0.4);
  }

  h2 {
    font-family: 'Quicksand', sans-serif;
    font-size: 1.9rem;
    color: #f7a8b8;
    margin-bottom: 18px;
    display: flex;
    align-items: center;
    gap: 10px;
  }

  .eco-grid {
    display: grid;
    grid-template-columns: repeat(auto-fit, minmax(200px, 1fr));
    gap: 14px;
    margin-top: 16px;
  }

  .eco-node {
    background: rgba(0, 0, 0, 0.2);
    padding: 16px 18px;
    border-radius: 14px;
    border: 1px solid rgba(255, 255, 255, 0.06);
    transition: all 0.3s cubic-bezier(0.34, 1.56, 0.64, 1);
    text-decoration: none;
    color: #ffffff;
    display: flex;
    align-items: center;
    gap: 10px;
  }

  .eco-node:hover {
    background: rgba(124, 58, 237, 0.15);
    border-color: #f7a8b8;
    transform: translateY(-3px) scale(1.02);
  }

  .node-icon {
    font-size: 20px;
    width: 36px;
    height: 36px;
    border-radius: 10px;
    display: flex;
    align-items: center;
    justify-content: center;
    flex-shrink: 0;
  }

  .node-info strong {
    display: block;
    font-size: 0.9rem;
  }

  .node-info span {
    display: block;
    font-size: 0.75rem;
    opacity: 0.6;
  }

  @media (max-width: 768px) {
    h2 { font-size: 1.5rem; }
    .glass-card { padding: 24px; }
  }
</style>
