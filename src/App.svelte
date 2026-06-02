<script>
  import Header from './components/Header.svelte';
  import About from './components/About.svelte';
  import Projects from './components/Projects.svelte';
  import Contact from './components/Contact.svelte';
  import Footer from './components/Footer.svelte';
</script>

<div class="app">
  <Header />
  <main>
    <About />
    <Projects />
    <Contact />
  </main>
  <Footer />
</div>

<style>
  :global(*) {
    box-sizing: border-box;
    margin: 0;
    padding: 0;
  }

  :global(body) {
    font-family: -apple-system, BlinkMacSystemFont, 'Segoe UI', Roboto, Oxygen, Ubuntu, sans-serif;
    background: #0a0a0f;
    color: #e1e1e6;
    line-height: 1.7;
    min-height: 100vh;
  }

  .app {
    max-width: 960px;
    margin: 0 auto;
    padding: 0 24px;
  }

  main {
    padding: 40px 0 60px;
  }

  h2 {
    font-size: 1.6rem;
    font-weight: 700;
    color: #c084fc;
    margin-bottom: 16px;
    display: flex;
    align-items: center;
    gap: 10px;
  }

  h2 i {
    font-size: 1.2rem;
    color: #7c3aed;
  }
</style>
