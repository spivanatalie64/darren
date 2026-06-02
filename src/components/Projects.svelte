<section id="projects">
  <h2><i class="fas fa-project-diagram"></i> Projects</h2>
  <div class="card">
    <div class="project-list">
      <div class="project-item">
        <i class="fas fa-linux"></i>
        <div>
          <strong>AcreetionOS</strong>
          <span>Arch Linux distribution — co-creator and developer</span>
        </div>
      </div>
      <div class="project-item">
        <i class="fas fa-cube"></i>
        <div>
          <strong>FlatFree</strong>
          <span>Community Flatpak repository</span>
        </div>
      </div>
      <div class="project-item">
        <i class="fas fa-shield-alt"></i>
        <div>
          <strong>Repository Sovereignty</strong>
          <span>Advocating for independent software distribution</span>
        </div>
      </div>
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
  .project-list { display: flex; flex-direction: column; gap: 10px; }
  .project-item {
    display: flex;
    align-items: flex-start;
    gap: 14px;
    background: rgba(255, 255, 255, 0.03);
    border: 1px solid rgba(255, 255, 255, 0.06);
    border-radius: 10px;
    padding: 18px;
    transition: all 0.15s;
  }
  .project-item:hover {
    border-color: rgba(46, 204, 113, 0.2);
    background: rgba(46, 204, 113, 0.04);
  }
  .project-item i {
    font-size: 1.3rem;
    color: #2ecc71;
    margin-top: 2px;
    flex-shrink: 0;
  }
  .project-item strong { display: block; font-size: 0.9rem; color: #e1e1e6; margin-bottom: 2px; }
  .project-item span { display: block; font-size: 0.8rem; color: #71717a; }
</style>
