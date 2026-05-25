<script>
  import { onMount } from 'svelte';

  let navLinks;

  onMount(() => {
    navLinks = document.querySelectorAll('.nav-link');
    navLinks.forEach(a => {
      a.addEventListener('click', e => {
        const href = a.getAttribute('href');
        if (href && href.startsWith('#')) {
          e.preventDefault();
          document.querySelector(href).scrollIntoView({ behavior: 'smooth' });
        }
      });
    });
  });
</script>

<header>
  <div class="profile-container">
    <img src="/Business.png" alt="Natalie Spiva" class="profile-img" />
    <div class="derp-badge">😜</div>
  </div>
  <h1>Natalie Spiva <span class="wave">👋</span></h1>
  <p class="tagline">🏳️‍⚧️ Lead Infrastructure Architect &bull; Master of 99.9% Uptime</p>
  <p class="tagline-derp"><span>😂</span> full-time sysadmin &bull; part-time derp &bull; kibby girl 24/7 <span>🐱</span></p>
  <nav>
    <a href="#about" class="nav-link"><i class="fas fa-microchip"></i> System Logs</a>
    <a href="#infrastructure" class="nav-link"><i class="fas fa-server"></i> Infrastructure</a>
    <a href="#contact" class="nav-link"><i class="fas fa-link"></i> Connect</a>
    <a href="https://blog.natalie.acreetionos.org" class="nav-link" target="_blank"><i class="fas fa-pen"></i> Blog</a>
    <a href="/resume.pdf" class="nav-link" target="_blank"><i class="fas fa-file-pdf"></i> Resume.pdf</a>
  </nav>
</header>

<style>
  header {
    position: relative;
    z-index: 10;
    background: linear-gradient(135deg, #55cdfc, #f7a8b8, #7c3aed);
    background-size: 300% 300%;
    animation: gradient-shift 12s ease infinite;
    padding: 60px 20px 80px;
    text-align: center;
    clip-path: polygon(0 0, 100% 0, 100% 88%, 0 100%);
    overflow: hidden;
  }

  @keyframes gradient-shift {
    0% { background-position: 0% 50%; }
    50% { background-position: 100% 50%; }
    100% { background-position: 0% 50%; }
  }

  header::after {
    content: '🐱 🌈 🐱 🌈 🐱';
    position: absolute;
    bottom: 10px;
    left: 50%;
    transform: translateX(-50%);
    font-size: 16px;
    letter-spacing: 12px;
    opacity: 0.2;
    pointer-events: none;
  }

  .profile-container {
    margin-bottom: 20px;
    position: relative;
    display: inline-block;
  }

  .profile-img {
    width: 170px;
    height: 170px;
    border-radius: 50%;
    border: 4px solid #ffffff;
    box-shadow:
      0 0 0 6px rgba(85, 205, 252, 0.2),
      0 0 0 12px rgba(247, 168, 184, 0.15),
      0 0 0 18px rgba(124, 58, 237, 0.1),
      0 0 40px rgba(236, 72, 153, 0.3);
    object-fit: cover;
    transition: transform 0.5s cubic-bezier(0.34, 1.56, 0.64, 1);
  }

  .profile-img:hover {
    transform: scale(1.08) rotate(4deg);
  }

  .derp-badge {
    position: absolute;
    bottom: -4px;
    right: -4px;
    background: #0f001a;
    border: 2px solid #f7a8b8;
    border-radius: 50%;
    width: 44px;
    height: 44px;
    display: flex;
    align-items: center;
    justify-content: center;
    font-size: 22px;
    animation: derp-float 3s ease-in-out infinite;
    box-shadow: 0 0 16px rgba(247, 168, 184, 0.4);
  }

  @keyframes derp-float {
    0%, 100% { transform: translateY(0) scale(1); }
    25% { transform: translateY(-8px) scale(1.05) rotate(-3deg); }
    75% { transform: translateY(-4px) scale(0.95) rotate(3deg); }
  }

  h1 {
    font-family: 'Quicksand', sans-serif;
    font-size: 3.8rem;
    font-weight: 700;
    margin-bottom: 8px;
    text-shadow: 0 2px 20px rgba(0, 0, 0, 0.3);
  }

  .wave {
    display: inline-block;
    animation: wave 2.5s ease-in-out infinite;
    transform-origin: 70% 70%;
  }

  @keyframes wave {
    0%, 100% { transform: rotate(0deg); }
    10% { transform: rotate(14deg); }
    20% { transform: rotate(-8deg); }
    30% { transform: rotate(14deg); }
    40% { transform: rotate(-4deg); }
    50% { transform: rotate(10deg); }
    60%, 100% { transform: rotate(0deg); }
  }

  .tagline {
    font-size: 1.3rem;
    font-weight: 300;
    opacity: 0.92;
    margin-bottom: 8px;
  }

  .tagline-derp {
    font-size: 1rem;
    opacity: 0.7;
    font-weight: 300;
    margin-bottom: 24px;
  }

  .tagline-derp span {
    display: inline-block;
    animation: derp-float 4s ease-in-out infinite;
  }

  nav {
    display: flex;
    justify-content: center;
    gap: 12px;
    margin-top: 28px;
    flex-wrap: wrap;
  }

  .nav-link {
    color: #ffffff;
    text-decoration: none;
    font-weight: 600;
    font-size: 0.9rem;
    padding: 10px 20px;
    border-radius: 50px;
    background: rgba(255, 255, 255, 0.10);
    border: 1px solid rgba(255, 255, 255, 0.18);
    transition: all 0.3s cubic-bezier(0.34, 1.56, 0.64, 1);
    backdrop-filter: blur(8px);
  }

  .nav-link:hover {
    background: #ec4899;
    border-color: #ffffff;
    transform: translateY(-4px) scale(1.04);
    box-shadow: 0 8px 24px rgba(236, 72, 153, 0.4);
  }

  @media (max-width: 768px) {
    h1 { font-size: 2.5rem; }
    header { padding: 40px 16px 70px; }
    .profile-img { width: 140px; height: 140px; }
    nav { gap: 8px; }
    .nav-link { font-size: 0.8rem; padding: 8px 14px; }
  }
</style>
