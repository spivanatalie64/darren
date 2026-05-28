<script>
  let { compact = false } = $props();
</script>

<section id="contact" class="glass-card">
  <h2><i class="fas fa-link"></i> Let's Connect 🌈</h2>
  <div class="social-grid">
    <a href="https://github.com/spivanatalie64" class="btn-social" target="_blank">
      <i class="fab fa-github"></i> GitHub
    </a>
    <a href="https://gitlab.acreetionos.org/natalie" class="btn-social" target="_blank">
      <i class="fab fa-gitlab"></i> GitLab
    </a>
    <a href="https://x.com/spiva4spiva" class="btn-social" target="_blank">
      <i class="fab fa-twitter"></i> X / Twitter
    </a>
    <a href="https://blog.natalie.acreetionos.org" class="btn-social" target="_blank">
      <i class="fas fa-pen"></i> Blog
    </a>
    <a href="https://acreetionos.org" class="btn-social" target="_blank">
      <i class="fas fa-globe"></i> AcreetionOS
    </a>
    <a href="https://discord.gg/acreetionos" class="btn-social" target="_blank">
      <i class="fab fa-discord"></i> Discord
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

  .social-grid {
    display: grid;
    grid-template-columns: repeat(auto-fit, minmax(140px, 1fr));
    gap: 12px;
    margin-top: 8px;
  }

  .social-grid.compact {
    grid-template-columns: repeat(auto-fit, minmax(100px, 1fr));
    gap: 8px;
  }

  .btn-social {
    display: flex;
    align-items: center;
    justify-content: center;
    gap: 8px;
    background: rgba(124, 58, 237, 0.15);
    color: #ffffff;
    text-decoration: none;
    padding: 12px 14px;
    border-radius: 14px;
    border: 1px solid rgba(255, 255, 255, 0.08);
    transition: all 0.3s cubic-bezier(0.34, 1.56, 0.64, 1);
    font-weight: 600;
    font-size: 0.85rem;
  }

  .btn-social:hover {
    background: #ec4899;
    border-color: #ffffff;
    transform: translateY(-3px) scale(1.04);
    box-shadow: 0 8px 24px rgba(236, 72, 153, 0.3);
  }

  @media (max-width: 768px) {
    h2 { font-size: 1.5rem; }
    .glass-card { padding: 24px; }
  }
</style>
