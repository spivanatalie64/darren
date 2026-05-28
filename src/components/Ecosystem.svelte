<script>
  const githubRepos = [
    'acreetionos', 'acreetionos32', 'acreetionos-archive-uploader', 'acreetionos-blog',
    'AcreetionOS-Cinnamon-Wayland', 'AcreetionOS-Cinnamon-X11', 'AcreetionOS-Cinnamon-XLibre',
    'acreetionos-code.github.io', 'acreetionos-flasher', 'acreetionos-gnome',
    'acreetionos-hyprland', 'AcreetionOS-Mate', 'acreetionos-newsletter', 'AcreetionOS-Tracker',
    'aiden-desktop', 'arttulos', 'cinderfox', 'ISO', 'Linux-Mint-Ports', 'llehs-emong', 'mdg',
    'messengerpwat', 'oglo', 'OVSC', 'qt-calculator', 'ScrapeC', 'spivanatalie64.github.io',
    'Trans_Tracker_101', 'Ace', 'git'
  ];

  const gitlabRepos = [
    'yacfs-tools', 'yacfs-replicate', 'yacfs-metrics', 'acreetionos-memory-manager',
    'acreetion-appstore', 'acreetionos-archive-uploader', 'acreetionos-flasher',
    'acreetionos-newsletter', 'acreetionos-news-tracker', 'acreetionos-website',
    'arttulos-site', 'crosspost',
    'qt-calculator', 'TrumpOS', 'Trans_Tracker_101',
    'spivanatalie64.github.io', 'OVSC', 'oglo', 'Linux-Mint-Ports', 'ISO', 'arttulos',
    'AcreetionOS-Tracker', 'acreetionos-gnome', 'acreetionos-code.github.io', 'acreetionos',
    'cinderfox', 'mdg', 'llehs-emong', 'AcreetionOS-Mate', 'ovsc-core', 'ovsc-sidecar',
    'ovsc-watcher', 'ovsc-host', 'X11aw', 'scrapec', 'AcreetionOSCloud', 'messengerpwat',
    'RiseupVPN-OpenVPN', 'acreetionos-terminal', 'aiden-desktop', 'calamares-config',
    'cinnamon-themes', 'root-distribution', 'thunderos', 'openrouter-tui'
  ];

  const featured = [
    { name: 'YAcFS', desc: 'Yet Another common File System', url: '#docs', icon: '💾', color: '#55cdfc' },
    { name: 'AcreetionOS', desc: 'High-availability Arch Linux distribution', url: 'https://acreetionos.org', icon: '🖥️', color: '#a78bfa' },
    { name: 'ArttulOS', desc: 'Accessibility layer of the future', url: 'https://arttulos.acreetionos.org', icon: '♿', color: '#f9a8d4' },
    { name: 'Ion ROM', desc: 'Open-source Android ROM', url: 'https://spivanatalie64.github.io/ion-website/', icon: '📱', color: '#55cdfc' },
    { name: 'OVSC', desc: 'Orchestrated Virtual Server Cloud', url: 'https://gitlab.acreetionos.org/natalie/OVSC', icon: '☁️', color: '#2dd4bf' },
    { name: 'Mirror Network', desc: '72+ active nodes worldwide', url: '#', icon: '🌍', color: '#2dd4bf' },
    { name: 'MessengerPWA', desc: 'Messenger progressive web app', url: 'https://gitlab.acreetionos.org/natalie/messengerpwat', icon: '💬', color: '#fbbf24' },
    { name: 'Blog', desc: 'AcreetionOS updates & dev logs', url: 'https://blog.natalie.acreetionos.org', icon: '📝', color: '#fbbf24' },
  ];
</script>

<section class="glass-card">
  <h2><i class="fas fa-project-diagram"></i> Ecosystem Nodes 🐱🌐</h2>
  <p>Active development across GitHub and GitLab:</p>

  <div class="eco-grid">
    {#each featured as repo}
      <a href={repo.url} target={repo.url.startsWith('http') ? '_blank' : ''} class="eco-node">
        <div class="node-icon" style="background:rgba(0,0,0,0.3);color:{repo.color};">{repo.icon}</div>
        <div class="node-info">
          <strong>{repo.name}</strong>
          <span>{repo.desc}</span>
        </div>
      </a>
    {/each}
  </div>

  <div class="repo-section">
    <h3><i class="fab fa-github"></i> GitHub — spivanatalie64</h3>
    <div class="repo-grid">
      {#each githubRepos as repo}
        <a href="https://github.com/spivanatalie64/{repo}" target="_blank" class="repo-chip">
          <i class="fab fa-github"></i> {repo}
        </a>
      {/each}
    </div>
  </div>

  <div class="repo-section">
    <h3><i class="fab fa-gitlab"></i> GitLab — natalie@acreetionos.org</h3>
    <div class="repo-grid">
      {#each gitlabRepos as repo}
        <a href="https://gitlab.acreetionos.org/natalie/{repo}" target="_blank" class="repo-chip">
          <i class="fab fa-gitlab"></i> {repo}
        </a>
      {/each}
    </div>
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
    top: 0; left: 0; right: 0;
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
    width: 36px; height: 36px;
    border-radius: 10px;
    display: flex;
    align-items: center;
    justify-content: center;
    flex-shrink: 0;
  }

  .node-info strong { display: block; font-size: 0.9rem; }
  .node-info span { display: block; font-size: 0.75rem; opacity: 0.6; }

  .repo-section {
    margin-top: 28px;
    padding-top: 20px;
    border-top: 1px solid rgba(255, 255, 255, 0.08);
  }

  .repo-section h3 {
    font-size: 1rem;
    color: #55cdfc;
    margin-bottom: 12px;
    display: flex;
    align-items: center;
    gap: 8px;
  }

  .repo-grid {
    display: flex;
    flex-wrap: wrap;
    gap: 8px;
  }

  .repo-chip {
    display: inline-flex;
    align-items: center;
    gap: 5px;
    padding: 6px 14px;
    background: rgba(255, 255, 255, 0.04);
    border: 1px solid rgba(255, 255, 255, 0.08);
    border-radius: 20px;
    color: #d1d5db;
    text-decoration: none;
    font-size: 0.8rem;
    transition: all 0.2s;
  }

  .repo-chip:hover {
    background: rgba(85, 205, 252, 0.1);
    border-color: #55cdfc;
    color: #ffffff;
    transform: translateY(-2px);
  }

  @media (max-width: 768px) {
    h2 { font-size: 1.5rem; }
    .glass-card { padding: 24px; }
    .eco-grid { grid-template-columns: 1fr; }
  }
</style>
