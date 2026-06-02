<section id="contact">
  <h2><i class="fas fa-envelope"></i> Contact</h2>
  <div class="card">
    <div class="social-grid">
      <a href="https://github.com/AcreetionOS-Code" class="btn-social" target="_blank">
        <i class="fab fa-github"></i> GitHub
      </a>
      <a href="https://acreetionos.org" class="btn-social" target="_blank">
        <i class="fas fa-globe"></i> AcreetionOS
      </a>
      <a href="https://discord.gg/acreetionos" class="btn-social" target="_blank">
        <i class="fab fa-discord"></i> Discord
      </a>
    </div>
  </div>
</section>

<style>
  h2 {
    font-size: 1.6rem;
    font-weight: 700;
    color: #2ecc71;
    margin-bottom: 16px;
    display: flex;
    align-items: center;
    gap: 10px;
  }
  h2 i { font-size: 1.2rem; color: #2ecc71; }
  .card {
    background: rgba(255, 255, 255, 0.03);
    border: 1px solid rgba(255, 255, 255, 0.06);
    border-radius: 12px;
    padding: 28px;
  }
  .social-grid {
    display: grid;
    grid-template-columns: repeat(auto-fit, minmax(140px, 1fr));
    gap: 10px;
  }
  .btn-social {
    display: flex;
    align-items: center;
    justify-content: center;
    gap: 8px;
    background: rgba(46, 204, 113, 0.08);
    color: #e1e1e6;
    text-decoration: none;
    padding: 12px 14px;
    border-radius: 8px;
    border: 1px solid rgba(46, 204, 113, 0.12);
    transition: all 0.15s;
    font-weight: 500;
    font-size: 0.85rem;
  }
  .btn-social:hover {
    background: rgba(46, 204, 113, 0.18);
    border-color: rgba(46, 204, 113, 0.3);
    color: #2ecc71;
    transform: translateY(-1px);
  }
</style>
