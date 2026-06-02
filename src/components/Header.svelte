<script>
</script>

<header>
  <div class="header-inner">
    <div class="brand">
      <div class="avatar">
        <img src="/darren_avatar.webp" alt="Darren Clift" />
      </div>
      <div>
        <h1>Darren Clift</h1>
        <p class="title">Codeveloper • AcreetionOS</p>
      </div>
    </div>
    <nav>
      <a href="#about">About</a>
      <a href="#projects">Projects</a>
      <a href="#contact">Contact</a>
      <a href="https://acreetionos.org" target="_blank" class="external">AcreetionOS <i class="fas fa-external-link-alt"></i></a>
    </nav>
  </div>
</header>

<style>
  header {
    padding: 20px 0;
    border-bottom: 1px solid rgba(255, 255, 255, 0.06);
    margin-bottom: 8px;
  }

  .header-inner {
    display: flex;
    align-items: center;
    justify-content: space-between;
    gap: 24px;
  }

  .brand {
    display: flex;
    align-items: center;
    gap: 16px;
    flex-shrink: 0;
  }

  .avatar {
    width: 52px;
    height: 52px;
    border-radius: 50%;
    overflow: hidden;
    border: 2px solid rgba(46, 204, 113, 0.3);
    flex-shrink: 0;
  }

  .avatar img {
    width: 100%;
    height: 100%;
    object-fit: cover;
  }

  h1 {
    font-size: 1.3rem;
    font-weight: 700;
    color: #e1e1e6;
    line-height: 1.2;
  }

  .title {
    font-size: 0.8rem;
    color: #2ecc71;
    font-weight: 500;
  }

  nav {
    display: flex;
    align-items: center;
    gap: 4px;
    flex-wrap: wrap;
  }

  nav a {
    color: #a1a1aa;
    text-decoration: none;
    font-size: 0.85rem;
    font-weight: 500;
    padding: 8px 14px;
    border-radius: 8px;
    transition: all 0.15s;
  }

  nav a:hover {
    color: #e1e1e6;
    background: rgba(255, 255, 255, 0.04);
  }

  nav a.external {
    color: #2ecc71;
  }

  nav a.external:hover {
    background: rgba(46, 204, 113, 0.1);
  }

  nav a i {
    font-size: 0.75rem;
    margin-left: 4px;
  }

  @media (max-width: 700px) {
    .header-inner {
      flex-direction: column;
      align-items: flex-start;
      gap: 16px;
    }
    nav { width: 100%; gap: 2px; }
    nav a { font-size: 0.8rem; padding: 6px 10px; }
  }
</style>
