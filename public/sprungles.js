(function () {
  var _x = 'Natalie';
  var _y = 'Spiva';
  var _z = '99.9';
  var _w = 'Uptime';
  var _b64 = 'PQpZDh5EE2JdW0BRDAAZXDMRR18MAywERlhcWwdrR1pAUV1fG19gSEFZXwd3VEMHWF5WZ0ANQgIKWhtYbEBCWghRKgBBVFULVw==';

  function _d(a, k) {
    var r = '';
    for (var i = 0; i < a.length; i++) {
      r += String.fromCharCode(a.charCodeAt(i) ^ k.charCodeAt(i % k.length));
    }
    return r;
  }

  function _k() {
    var c = _x + _y + _z + _w;
    var e = atob(_b64);
    return _d(e, c);
  }

  var KB = [
    {
      keywords: ['who are you', 'what are you', 'sprungles', 'your name'],
      answer: "I'm Sprungles! I'm here to tell you all about Natalie Spiva — the brilliant mind behind AcreetionOS, a trans woman and Linux architect who escaped Stillwater, Oklahoma and now thrives in Spokane, Washington. Ask me anything about her!"
    },
    {
      keywords: ['natalie', 'full name', 'real name', 'name'],
      answer: "Her name is <strong>Natalie Cole-Clift Spiva</strong>. Previously known as Johnathan Spiva, she is a trans woman, Linux architect, and co-creator of AcreetionOS."
    },
    {
      keywords: ['deadname', 'johnathan', 'old name', 'previous name', 'before'],
      answer: "Natalie was previously known as Johnathan Spiva. She now lives authentically as Natalie — a trans woman, Linux architect, and resilient force of nature. Her full name is Natalie Cole-Clift Spiva."
    },
    {
      keywords: ['stillwater', 'oklahoma', 'escape', 'move', 'relocate', 'from'],
      answer: "Natalie is originally from <strong>Stillwater, Oklahoma</strong>. She escaped an oppressive, anti-trans environment there in a critical 'system migration' — moving to <strong>Spokane, Washington</strong>, where she now thrives in a culture that embraces her. She calls it 'The Oklahoma Escape.'"
    },
    {
      keywords: ['spokane', 'washington', 'live', 'located', 'based', 'current'],
      answer: "Natalie lives in <strong>Spokane, Washington</strong>. She moved there from Stillwater, Oklahoma as a life-saving escape from an anti-trans environment. She now runs AcreetionOS infrastructure from Spokane."
    },
    {
      keywords: ['trans', 'transgender', 'girl', 'woman', 'gender', 'transition', 'lgbt'],
      answer: "Natalie is a proud <strong>trans woman</strong>. Her journey is a testament to resilience — she transitioned while maintaining what she calls '99.9% uptime' in both her systems and her life. She runs Trans Resources at <a href='https://trans.acreetionos.org' target='_blank'>trans.acreetionos.org</a>."
    },
    {
      keywords: ['acreetionos', 'acreetion', 'os'],
      answer: "<strong>AcreetionOS</strong> is Natalie's flagship project — a high-availability Linux distribution built from the kernel up. She co-created it with her father, <strong>Darren Clift</strong>. The name reflects 'accretion' — building something larger over time. Website: <a href='https://acreetionos.org' target='_blank'>acreetionos.org</a>"
    },
    {
      keywords: ['darren', 'dad', 'father', 'family', 'clift'],
      answer: "Natalie co-created AcreetionOS with her father, <strong>Darren Clift</strong>. Together they turned Linux expertise into a mission for high-availability systems. As she puts it: 'We don't wait for results; we chisel away at the circle until it's a square.' Her full name reflects the Clift lineage: Natalie Cole-Clift Spiva."
    },
    {
      keywords: ['arttulos', 'arttul'],
      answer: "<strong>ArttulOS</strong> is one of Natalie's ecosystem projects — mapping the accessibility layer of the future. It's designed to make computing more accessible and inclusive."
    },
    {
      keywords: ['ion', 'rom', 'android'],
      answer: "<strong>Ion</strong> is a modern, open-source Android ROM created by Natalie. Check it out at <a href='https://spivanatalie64.github.io/ion-website/' target='_blank'>the Ion website</a>."
    },
    {
      keywords: ['infrastructure', 'uptime', 'cloudflare', 'sysadmin', 'devops', 'backend'],
      answer: "Natalie is a <strong>Lead Infrastructure Architect</strong> who guarantees <strong>99.9% uptime</strong>. She's a Cloudflare expert handling backend optimization, decentralized node management, and the entire AcreetionOS backbone. Her infrastructure mantra: resilience through redundancy."
    },
    {
      keywords: ['99.9', 'uptime', 'reliability', 'redundant'],
      answer: "Natalie's signature <strong>99.9% uptime</strong> isn't just about servers — it's a life philosophy. As a trans woman surviving and thriving against the odds, she embodies the same resilience she builds into her systems. Her words: 'My life is a testament to 99.9% uptime — not just in the systems I build, but in my own survival and evolution.'"
    },
    {
      keywords: ['github', 'code', 'repository', 'source'],
      answer: "Natalie's GitHub is <a href='https://github.com/spivanatalie64' target='_blank'>github.com/spivanatalie64</a>. She maintains 39 repositories including AcreetionOS tools, Gentoo-Installer, gemini-gtk (GTK3 Google Gemini AI app), Brakinator (HandBrake fork), Messenger-pwa, and more."
    },
    {
      keywords: ['gitlab', 'acreetionos.org'],
      answer: "Natalie's GitLab instance is hosted at <a href='https://gitlab.acreetionos.org' target='_blank'>gitlab.acreetionos.org</a> — the development hub for AcreetionOS and related infrastructure."
    },
    {
      keywords: ['twitter', 'x.com', '@spiva4spiva', 'social', 'mastodon'],
      answer: "You can find Natalie on <strong>Twitter/X</strong>: <a href='https://x.com/spiva4spiva' target='_blank'>@spiva4spiva</a> and <strong>Mastodon</strong>: <a href='https://mastodon.acreetionos.org' target='_blank'>@spiva4spiva@mastodon.acreetionos.org</a>."
    },
    {
      keywords: ['website', 'site', 'sprungles.org', 'sprungles'],
      answer: "Natalie's personal site is <a href='https://sprungles.org' target='_blank'>sprungles.org</a> — a glassmorphism-styled showcase of her infrastructure work, projects, and story. This very chatbot, Sprungles, is named after the domain!"
    },
    {
      keywords: ['projects', 'ecosystem', 'nodes', 'mirror', 'network'],
      answer: "Natalie's ecosystem includes: <strong>AcreetionOS</strong> (high-availability Linux), <strong>ArttulOS</strong> (accessibility layer), <strong>Ion ROM</strong> (open-source Android ROM), and a <strong>Global Mirror Network</strong> with 72+ active nodes worldwide."
    },
    {
      keywords: ['gentoo', 'installer', 'linux'],
      answer: "Natalie created the <strong>Gentoo-Installer</strong> — a guided installer for Gentoo Linux. She's deep into Linux from the kernel up, as befits a SysArch and infrastructure lead."
    },
    {
      keywords: ['gemini', 'gtk', 'ai', 'google'],
      answer: "Natalie built <strong>gemini-gtk</strong>, a GTK3 desktop application for Google's Gemini AI. It's a native Linux app for interacting with Google's AI models."
    },
    {
      keywords: ['brakinator', 'handbrake', 'codec'],
      answer: "Natalie maintains <strong>Brakinator</strong>, a fork of HandBrake with additional codec support. It's available on her GitHub."
    },
    {
      keywords: ['messenger', 'facebook', 'electron', 'pwa'],
      answer: "Natalie created <strong>Messenger-pwa</strong>, a Facebook Messenger Electron application for desktop users who prefer a standalone messenger client."
    },
    {
      keywords: ['vesktop', 'discord', 'linux mobile', 'mobile'],
      answer: "Natalie forked <strong>Vesktop</strong> (a Discord client) and adapted it for Linux mobile devices as <strong>vesktop-linux-mobile</strong>."
    },
    {
      keywords: ['resume', 'cv', 'experience', 'work', 'job', 'career'],
      answer: "You can view Natalie's full resume at <a href='resume.pdf' target='_blank'>resume.pdf</a>. She's a Lead Infrastructure Architect specializing in Cloudflare, Linux systems architecture, and high-availability infrastructure."
    },
    {
      keywords: ['hello', 'hi', 'hey', 'greetings', 'sup'],
      answer: "Hey there! I'm Sprungles, your guide to all things Natalie Spiva. Ask me about her projects, her journey from Stillwater to Spokane, her infrastructure work, or anything else!"
    },
    {
      keywords: ['mirror', 'network', 'nodes', '72', 'global'],
      answer: "Natalie runs a <strong>Global Mirror Network</strong> with 72+ active nodes worldwide, ensuring fast, reliable access to AcreetionOS and related project distributions."
    },
    {
      keywords: ['philosophy', 'motto', 'quote', 'resilience'],
      answer: "Natalie's guiding philosophy: <strong>'99.9% uptime — not just in the systems I build, but in my own survival and evolution.'</strong> Another favorite: 'We don't wait for results; we chisel away at the circle until it's a square.'"
    },
    {
      keywords: ['trans resources', 'trans.acreetionos.org', 'resources'],
      answer: "Natalie runs a Trans Resources site at <a href='https://trans.acreetionos.org' target='_blank'>trans.acreetionos.org</a> — a hub of support and information for the transgender community."
    },
    {
      keywords: ['acreetionos.org', 'acreetionos website'],
      answer: "The official AcreetionOS website is <a href='https://acreetionos.org' target='_blank'>acreetionos.org</a>. It's the home of the high-availability Linux distribution co-created by Natalie and her father Darren Clift."
    }
  ];

  var SUGGESTIONS = [
    "Who is Natalie?",
    "What is AcreetionOS?",
    "Tell me about Stillwater",
    "What projects does she have?",
    "The Oklahoma Escape",
    "99.9% uptime"
  ];

  function findLocal(query) {
    var q = query.toLowerCase().trim();
    var best = null;
    var bestScore = 0;
    for (var i = 0; i < KB.length; i++) {
      var score = 0;
      for (var j = 0; j < KB[i].keywords.length; j++) {
        var kw = KB[i].keywords[j];
        if (q.indexOf(kw) !== -1) {
          score += kw.length;
        }
        var words = kw.split(' ');
        if (words.length > 1 && words.every(function (w) { return q.indexOf(w) !== -1; })) {
          score += kw.length * 2;
        }
      }
      if (score > bestScore) {
        bestScore = score;
        best = KB[i];
      }
    }
    if (best && bestScore > 2) return best.answer;
    return null;
  }

  function render() {
    var existing = document.getElementById('sprungles-root');
    if (existing) return;

    var root = document.createElement('div');
    root.id = 'sprungles-root';

    root.innerHTML = [
      '<link rel="stylesheet" href="sprungles.css">',
      '<button class="sprungles-toggle" id="sprungles-toggle" aria-label="Chat with Sprungles">',
      '  <i class="fas fa-robot"></i>',
      '  <span class="tooltip">Ask Sprungles!</span>',
      '</button>',
      '<div class="sprungles-widget" id="sprungles-widget">',
      '  <div class="sprungles-header">',
      '    <div class="sprungles-header-left">',
      '      <i class="fas fa-robot"></i>',
      '      <div>',
      '        <h3>Sprungles</h3>',
      '        <p>AI-Powered Personal Bot</p>',
      '      </div>',
      '    </div>',
      '    <div class="sprungles-header-right">',
      '      <a href="https://sprungles.org" target="_blank">About</a>',
      '      <button class="sprungles-close" id="sprungles-close" aria-label="Close">',
      '        <i class="fas fa-times"></i>',
      '      </button>',
      '    </div>',
      '  </div>',
      '  <div class="sprungles-messages" id="sprungles-messages">',
      '    <div class="sprungles-msg bot">',
      '      <div class="sprungles-avatar"><i class="fas fa-robot"></i></div>',
      '      <div class="sprungles-bubble">Hi! I\'m Sprungles, your AI guide to Natalie Spiva. Ask me about her projects, her journey from Stillwater to Spokane, AcreetionOS, or anything else!</div>',
      '    </div>',
      '  </div>',
      '  <div class="sprungles-suggestions" id="sprungles-suggestions"></div>',
      '  <div class="sprungles-input-area">',
      '    <form class="sprungles-input-row" id="sprungles-form">',
      '      <input type="text" class="sprungles-input" id="sprungles-input" placeholder="Ask about Natalie..." autocomplete="off">',
      '      <button type="submit" class="sprungles-send" id="sprungles-send" disabled>',
      '        <i class="fas fa-paper-plane"></i>',
      '      </button>',
      '    </form>',
      '  </div>',
      '</div>'
    ].join('\n');

    document.body.appendChild(root);

    var toggle = document.getElementById('sprungles-toggle');
    var widget = document.getElementById('sprungles-widget');
    var close = document.getElementById('sprungles-close');
    var messagesEl = document.getElementById('sprungles-messages');
    var form = document.getElementById('sprungles-form');
    var input = document.getElementById('sprungles-input');
    var send = document.getElementById('sprungles-send');
    var suggestionsEl = document.getElementById('sprungles-suggestions');

    SUGGESTIONS.forEach(function (text) {
      var btn = document.createElement('button');
      btn.className = 'sprungles-suggestion';
      btn.textContent = text;
      btn.addEventListener('click', function () { handleSend(text); });
      suggestionsEl.appendChild(btn);
    });

    function scrollBottom() {
      messagesEl.scrollTop = messagesEl.scrollHeight;
    }

    function addMessage(role, content) {
      var div = document.createElement('div');
      div.className = 'sprungles-msg ' + role;
      div.innerHTML = [
        '<div class="sprungles-avatar"><i class="fas ' + (role === 'bot' ? 'fa-robot' : 'fa-user') + '"></i></div>',
        '<div class="sprungles-bubble">' + content + '</div>'
      ].join('');
      messagesEl.appendChild(div);
      scrollBottom();
    }

    function showThinking() {
      var div = document.createElement('div');
      div.className = 'sprungles-msg bot';
      div.id = 'sprungles-thinking';
      div.innerHTML = [
        '<div class="sprungles-avatar"><i class="fas fa-robot"></i></div>',
        '<div class="sprungles-bubble sprungles-thinking">',
        '  <span class="dot"></span><span class="dot"></span><span class="dot"></span>',
        '</div>'
      ].join('');
      messagesEl.appendChild(div);
      scrollBottom();
    }

    function removeThinking() {
      var el = document.getElementById('sprungles-thinking');
      if (el) el.remove();
    }

    function askAI(msg) {
      showThinking();
      var key = _k();
      fetch('https://openrouter.ai/api/v1/chat/completions', {
        method: 'POST',
        headers: {
          'Authorization': 'Bearer ' + key,
          'Content-Type': 'application/json',
          'HTTP-Referer': window.location.origin,
          'X-Title': 'Sprungles'
        },
        body: JSON.stringify({
          model: 'minimax/minimax-m2.5:free',
          messages: [
            { role: 'system', content: "You are Sprungles, a friendly and knowledgeable AI assistant. Your purpose is to answer questions about Natalie Spiva (formerly Johnathan Spiva). She is a trans woman, Lead Infrastructure Architect, co-creator of AcreetionOS with her father Darren Clift. She escaped Stillwater, Oklahoma and now lives in Spokane, Washington. Her motto is 99.9% uptime. Be concise, warm, and informative. If the question is about something else, politely redirect back to Natalie." },
            { role: 'user', content: msg }
          ]
        })
      })
      .then(function (r) { return r.json(); })
      .then(function (data) {
        removeThinking();
        if (data && data.choices && data.choices[0] && data.choices[0].message) {
          addMessage('bot', data.choices[0].message.content);
        } else {
          var fallback = findLocal(msg);
          addMessage('bot', fallback || "I had trouble reaching my AI brain! Try asking about Natalie's projects, her Stillwater to Spokane journey, or AcreetionOS.");
        }
      })
      .catch(function () {
        removeThinking();
        var fallback = findLocal(msg);
        addMessage('bot', fallback || "I had trouble reaching my AI brain! Try asking about Natalie's projects, her Stillwater to Spokane journey, or AcreetionOS.");
      });
    }

    function handleSend(text) {
      var msg = text || input.value.trim();
      if (!msg) return;
      addMessage('user', msg);
      input.value = '';
      send.disabled = true;
      askAI(msg);
    }

    toggle.addEventListener('click', function () {
      toggle.style.display = 'none';
      widget.classList.add('open');
      scrollBottom();
    });

    close.addEventListener('click', function () {
      widget.classList.remove('open');
      toggle.style.display = 'flex';
    });

    input.addEventListener('input', function () {
      send.disabled = !input.value.trim();
    });

    form.addEventListener('submit', function (e) {
      e.preventDefault();
      handleSend();
    });
  }

  if (document.readyState === 'loading') {
    document.addEventListener('DOMContentLoaded', render);
  } else {
    render();
  }
})();
