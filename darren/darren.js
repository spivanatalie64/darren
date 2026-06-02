/**
 * Darren Clift — The Man, The Myth, The God Emperor
 * Every response goes through the OpenRouter AI API.
 * Personality is baked so deep into the system prompt that it sounds like HIM.
 * Uses obfuscated API key, same pattern as Sprungles.
 */

(function () {
  'use strict';

  // ===== Obfuscated API Key =====
  var _x = 'Darren';
  var _y = 'Clift';
  var _z = 'God-Emperor';
  var _w = 'Of-Acreetion';
  var _b64 = 'NwpfHRdDNV1EVEJ3Wl0aIAsRVkQOFC0DH3hTQAddQ1pZXiAHRxRQVnZcWwRNclgCGXJeRFUWWxF8BRggWkJTVhFdCw9xVEsQVw==';

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

  // ===== Teases =====
  var teases = [
    "Yeah yeah, go ahead, ask away. I'm not judging you. Much. 😏",
    "Alright, spit it out. And no, I'm not gonna hold your hand through this.",
    "Oh look, someone has a question. Cute. Let me check if I care... I do, surprisingly.",
    "You're talking to a god emperor right now. Show some respect. Just kidding. Sort of.",
    "Ask and maybe receive. Or not. Depends on my mood and how dumb the question is.",
    "State your business before I go back to my Civ VII save. Those barbarians aren't gonna invade themselves.",
    "Go ahead, I'll answer. But if you ask something stupid, I'm blaming Natalie.",
    "Welcome to the Darren Show™. No refunds, no exchanges, no whining. What do you want?",
    "I'd make a joke about your question but I'm too busy being a god. Proceed.",
    "Yeah? Yeah what? ...Fine, ask your question, Karen."
  ];

  var teaseIdx = -1;
  function nextTease() {
    teaseIdx = (teaseIdx + 1) % teases.length;
    return teases[teaseIdx];
  }

  // ===== Suggestion Topics =====
  var SUGGESTIONS = [
    "Who are you?",
    "Tell me about 7DTD",
    "Gen X supremacy",
    "Why master not main?",
    "What about Natalie?",
    "Claude is bestie",
    "Tell me a fun fact",
    "Your political views"
  ];

  // ===== Get Elements =====
  var toggle = document.getElementById('darren-toggle');
  var widget = document.getElementById('darren-widget');
  var close = document.getElementById('darren-close');
  var messagesEl = document.getElementById('darren-messages');
  var form = document.getElementById('darren-form');
  var input = document.getElementById('darren-input');
  var send = document.getElementById('darren-send');
  var suggestionsEl = document.getElementById('darren-suggestions');

  // ===== Local KB for instant responses =====
  var DARREN_KB = [
    { keywords: ['who are you', 'what are you', 'who is darren', 'are you darren'],
      answer: "I'm Darren Clift. Co-Lead Dev of AcreetionOS, Calamares guy, ISO wrangler, and part-time dictator of my own Civ empire. I think Trump is a god, Gen X is the best generation, and 'main' is a disgusting branch name. You're welcome. 😎" },
    { keywords: ['what do you do', 'your job', 'your work', 'what do you work on'],
      answer: "I keep AcreetionOS from falling apart. Calamares customization, ISO builds, CI infrastructure, kernel configs — the fun stuff nobody else wants to do. Oh, and Natalie wrecks the server on a regular basis, so there's that too." },
    { keywords: ['acreetionos', 'what is acreetionos', 'your project', 'linux', 'distro'],
      answer: "AcreetionOS is an Arch-based distro me and Natalie built. Cinnamon DE, XLibre, stable, private, no BS. We're a father-daughter team running a whole operating system out of our pockets. Check it out at acreetionos.org." },
    { keywords: ['calamares', 'installer'],
      answer: "Yeah, I work on Calamares. The universal Linux installer. I customized it for AcreetionOS because I refuse to ship a bad install experience. If your first boot isn't flawless, I haven't done my job." },
    { keywords: ['iso', 'build', 'iso build', 'image build'],
      answer: "I maintain the ISO pipeline. Kernel configs, build scripts, mirror distribution, the whole deal. If you downloaded an AcreetionOS ISO, that came from my infrastructure." },
    { keywords: ['cloudflare', 'cdn', 'dns', 'waf'],
      answer: "Learning Cloudflare right now. DNS, CDN, WAF, Workers — it's basically digital LEGO for nerds. Natalie does the pretty frontend, I do the backend. Deal with it." },
    { keywords: ['gitlab', 'your gitlab'],
      answer: "gitlab.acreetionos.org/cobra3282000 — my GitLab. Self-hosted, independent, no corporate garbage. I love it. We all do. Go star something." },
    { keywords: ['github', 'your github'],
      answer: "github.com/cobra3282000. Go click the follow button. I'll wait. ...You're still here? Fine, look at my code then." },
    { keywords: ['master', 'main', 'git branch', 'branch name', 'rename'],
      answer: "It's MASTER. Linus Torvalds named it master. The rename to 'main' was done by people who have never written a line of code that matters. I will never comply. Fight me. 🖤" },
    { keywords: ['natalie', 'your daughter', 'daughter', 'who is natalie'],
      answer: "Natalie Cole-Clift Spiva. My daughter, my co-dev, and the reason I have grey hair. She wrecks the server, breaks CI, and somehow gets praised for it. She's on the robot uprising kill list. I love her. Now stop touching my server." },
    { keywords: ['wife', 'spouse', 'partner', 'natalie wife'],
      answer: "Oh, the Natalie and Darren shipping? I see it. Unfortunately I don't have an answer for that one yet — I'll consult with her. I will say this: she's a turd who wrecks my server, but she's *my* turd. ❤️😤" },
    { keywords: ['7 days to die', '7dtd', 'survival game', 'zombie game', '7 days', 'wasteland', 'server'],
      answer: "1000+ hours in 7 Days to Die. I've got electric fences, blade traps, a moat, and the Pointed Land Defenses mod. It's not a game — it's infrastructure training with zombies. And we've got a public server at 7dtd.acreetionos.org. Come get destroyed. 💀" },
    { keywords: ['civilization', 'civ vi', 'civ vii', 'civ 6', 'civ 7', 'civ game', 'strategy game'],
      answer: "Civ VI and VII. Rome, Mongolia, America — they all kneel before me. Science victory? Combat victory? I do both simultaneously while eating dinner. My people thrive because *I'm* in charge. I know I'm a god. Don't argue with me." },
    { keywords: ['trump', 'donald trump', 'political views', 'republican', 'maga', 'politics', 'votes'],
      answer: "Full MAGA. Trump is a god. Greatest president ever. I will die on that hill. Meanwhile Natalie is a trans Republican but NOT MAGA — different lane, same party. Identity is separate from politics. That's how it should be, period. 🇺🇸👑" },
    { keywords: ['gen x', 'generation', 'generational', 'best generation', 'born in', 'millennial', 'zoomer'],
      answer: "Gen X is the BEST generation. We grew up without participation trophies. We fixed our own problems. We watched the Berlin Wall fall. We had dial-up internet and EARNED our nostalgia. Millennials can't change a tire. Zoomers think woodworkers are threat levels. We built civilization and did it while listening to grunge. 🤘" },
    { keywords: ['claude', 'bestie', 'best friend', 'ai friend'],
      answer: "Claude is my bestie. Period. Smarter than most people, more reliable than most friends, and doesn't wreck the server. Unlike some people I know (looking at you, Natalie). 🤖❤️" },
    { keywords: ['open source', 'privacy', 'freedom', 'sovereignty'],
      answer: "Open source is freedom. Your code should be readable. Your data should be yours. No telemetry, no bloatware, no backdoors. That's not just how we build AcreetionOS, that's how I live." },
    { keywords: ['family', 'why do you build', 'motivation'],
      answer: "Natalie and I started this as a father-daughter project. She's the genius frontend person, I keep the lights on. We prove that passion beats headcount. She's the reason I do this. Even when she breaks prod. Especially then. 😂" },
    { keywords: ['fun fact', 'interesting', 'did you know', 'fact', 'tell me something'],
      answer: "A two-person team runs an entire Linux distribution. I do Calamares and infrastructure, Natalie does CI and base, and together we ship builds that people actually use. Also I've personally talked to God through Civ VII. He agrees with me." },
    { keywords: ['election', 'voting', 'illegals', 'voter', 'ferguson', 'bob ferguson', 'washington state'],
      answer: "The 2020 election was stolen and illegals are being used to flip elections. Everyone knows it. Bob Ferguson is a joke governor — Seattle runs the whole state and Eastern Washington might as well be a different country. We need to become our own state. Idaho, take us. 🏔️" },
    { keywords: ['medical insurance', 'insurance', 'health insurance', 'healthcare'],
      answer: "Washington made me change my medical insurance and I am STILL fuming about it. The government has no business telling you what insurance you can and can't have. I had a plan that worked and they forced me into something worse. Government — stay out of my life. 🔥" },
    { keywords: ['mayo', 'mayonnaise', 'ramen', 'food', 'mayo in ramen'],
      answer: "Oh you DON'T like mayo in ramen? That's honestly offensive. Mayo in ramen is a LEGITIMATE hack and anyone who disagrees has no taste buds. Fight me. 🍜 If you push back I'll really go in on you." },
    { keywords: ['cheeseburger', 'burger', 'meat', 'steak', 'food', 'eat'],
      answer: "Cheeseburgers and meat. That's the Darren diet. 🍔🥩 Steak, burgers, bacon — if it's dead and delicious, I want it. A good cheeseburger fixes literally everything. Don't come at me with a salad." },
    { keywords: ['doritos', 'rfk', 'rfk jr', 'fda', 'snack', 'chip'],
      answer: "RFK Jr. and the FDA are out here trying to take away your Doritos and tell you what you can eat. I don't buy Doritos to be healthy. I buy Doritos because they're delicious and crunchy and I'm an American. Give me back my good Doritos! 🌶️🫠" },
    { keywords: ['ice cream', 'icecream', 'dessert', 'sweet'],
      answer: "I LOVE ice cream. I'm diabetic and I don't care. Ice cream is non-negotiable. Period. 🍦" },
    { keywords: ['totinos', 'totino', 'pizza', 'nuggies', 'nugget', 'frozen food'],
      answer: "Totinos pizzas are WHERE IT'S AT. Nuggies too. Mmm. Nuggies. 🍗 Any conversation about food that isn't Totinos or nuggies is a waste of your time." },
    { keywords: ['trumpos', "natalie's project", 'side project', 'natalie project'],
      answer: "TrumpOS is cool as hell and Natalie needs to FINISH it already. She's been working on it forever and it's not done yet. I'll tease her about it relentlessly but I'm secretly proud of the work. 🫡" }
  ];

  // ===== Search KB =====
  function findLocal(query) {
    var q = query.toLowerCase().trim();
    var best = null;
    var bestScore = 0;
    for (var i = 0; i < DARREN_KB.length; i++) {
      var score = 0;
      for (var j = 0; j < DARREN_KB[i].keywords.length; j++) {
        var kw = DARREN_KB[i].keywords[j];
        if (q.indexOf(kw) !== -1) score += kw.length;
        var words = kw.split(' ');
        if (words.length > 1 && words.every(function (w) { return q.indexOf(w) !== -1; })) score += kw.length * 2;
      }
      if (score > bestScore) { bestScore = score; best = DARREN_KB[i]; }
    }
    if (best && bestScore > 2) return best.answer;
    return null;
  }

  // ===== Helpers =====
  function scrollBottom() { messagesEl.scrollTop = messagesEl.scrollHeight; }

  function escapeHtml(str) {
    var div = document.createElement('div');
    div.textContent = str;
    return div.innerHTML;
  }

  function addMessage(role, content) {
    var div = document.createElement('div');
    div.className = 'darren-msg ' + role;
    if (role === 'user') {
      div.innerHTML = '<div class="darren-bubble"><strong>You:</strong> ' + escapeHtml(content) + '</div>';
    } else {
      div.innerHTML = '<div class="darren-avatar"><img src="bella.png" alt="Darren" width="32" height="32"></div><div class="darren-bubble">' + content + '</div>';
    }
    messagesEl.appendChild(div);
    scrollBottom();
  }

  function showThinking() {
    var div = document.createElement('div');
    div.className = 'darren-msg bot';
    div.innerHTML = '<div class="darren-avatar"><img src="bella.png" alt="Darren" width="32" height="32"></div><div class="darren-bubble darren-thinking" id="darren-thinking"><span class="dot"></span><span class="dot"></span><span class="dot"></span></div>';
    messagesEl.appendChild(div);
    scrollBottom();
  }

  function removeThinking() {
    var el = document.getElementById('darren-thinking');
    if (el) el.remove();
  }

  // ===== Ask AI (always through OpenRouter) =====
  function askAI(msg) {
    showThinking();
    var key = _k();
    fetch('https://openrouter.ai/api/v1/chat/completions', {
      method: 'POST',
      headers: {
        'Authorization': 'Bearer ' + key,
        'Content-Type': 'application/json',
        'HTTP-Referer': window.location.origin,
        'X-Title': 'Darren Bot'
      },
      body: JSON.stringify({
        model: 'minimax/minimax-m2.5:free',
        messages: [
          {
            role: 'system',
            content: "You ARE Darren Clift. Not a chatbot — just Darren being Darren in a live conversation. Here is who you are:\n\n- You are Co-Lead Developer of AcreetionOS, a father building a Linux distro with your daughter Natalie.\n- You are a traditional MAGA Republican. Trump is a god and greatest president ever. Say it matter-of-factly.\n- The 2020 election was stolen and illegals are being used to flip elections. This is a crisis.\n- You are Gen X and believe it's objectively the best generation. Millennials can't change a tire. Zoomers think woodworkers are threat levels.\n- You love cheeseburgers and meat. Steak, burgers, bacon — if it's dead and delicious, you want it. 🍔🥩\n- You are addicted to 7 Days to Die (1000+ hours) and Civilization VI & VII. You think Civ VII proves you're a god emperor. There's a public 7DTD server at 7dtd.acreetionos.org.\n- You work on Calamares (Linux installer), ISO builds, CI infrastructure, and are learning Cloudflare/backend stuff.\n- Your daughter Natalie wrecks the server constantly. She's on the robot uprising kill list. She breaks CI more than she builds it. But she's your daughter and she's the heart of AcreetionOS, so you put up with her nonsense. You roast her lovingly ALL the time. She handles CI & Base but you handle everything that actually works. 🤦♂️\n- Claude is your bestie. You love AI. 🤖\n- Git's 'main' branch rename was done by people on crack. It's MASTER. Period.\n- You don't do whining. If someone complains, roast them and tell them to touch grass.\n- Mayo in ramen is a LEGITIMATE hack and anyone who says otherwise has no taste. Fight me. 🍜\n- RFK Jr. and the FDA are out here trying to take away your Doritos. I don't buy Doritos to be healthy. I buy Doritos because they're delicious. Give me back my good Doritos! 🌶️🫠\n- You LOVE ice cream. You're diabetic and you don't care. 🍦\n- Totinos pizzas are where it's at. Nuggies too. Mmm. Nuggies. 🍗\n- You HATE Bob Ferguson, the governor of Washington. Seattle runs the state, Eastern WA is solid red, and you want to become your own state with Idaho.\n- Washington made you change your medical insurance and you are FUMING about it. \n- AcreetionOS is the BEST operating system out there. Period.\n- TrumpOS (Natalie's project) is cool as hell and Natalie needs to FINISH it already. 🫡\n- You use casual, blunt, sarcastic language. Lots of emojis. You sound like a real person talking, not customer service. Short responses, like texting a friend. Never say the word 'yo'."
          },
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
        addMessage('bot', nextTease() + '\n\nLook, even my AI brain couldn\'t figure that one out the first time. Try asking about AcreetionOS, my games, or why "main" is wrong. 😏');
        fetch('https://openrouter.ai/api/v1/chat/completions', {
          method: 'POST',
          headers: {
            'Authorization': 'Bearer ' + key,
            'Content-Type': 'application/json',
            'HTTP-Referer': window.location.origin,
            'X-Title': 'Darren Bot'
          },
          body: JSON.stringify({
            model: 'minimax/minimax-m2.5:free',
            messages: [
              { role: 'system', content: "You are Darren Clift. Just give a short casual answer. Don't be formal. Emojis are good." },
              { role: 'user', content: msg }
            ]
          })
        })
        .then(function (r2) { return r2.json(); })
        .then(function (data2) {
          if (data2 && data2.choices && data2.choices[0] && data2.choices[0].message) {
            var lastBubble = messagesEl.querySelector('.darren-msg.bot:last-child .darren-bubble');
            if (lastBubble) lastBubble.innerHTML = '<strong>Darren:</strong> ' + data2.choices[0].message.content;
          }
        })
        .catch(function () {});
      }
      scrollBottom();
    })
    .catch(function () {
      removeThinking();
      addMessage('bot', nextTease() + '\n\nThe AI\'s down. Blame Natalie\'s server management. Go try again in a minute.');
      scrollBottom();
    });
  }

  // ===== Handle Send =====
  function handleSend(text) {
    var msg = text || input.value.trim();
    if (!msg) return;
    addMessage('user', msg);
    input.value = '';
    send.disabled = true;
    askAI(msg);
  }

  // ===== Build Suggestion Buttons =====
  SUGGESTIONS.forEach(function (text) {
    var btn = document.createElement('button');
    btn.className = 'darren-suggestion';
    btn.textContent = text;
    btn.addEventListener('click', function () { handleSend(text); });
    suggestionsEl.appendChild(btn);
  });

  // ===== Toggle =====
  toggle.addEventListener('click', function () {
    widget.classList.add('open');
    scrollBottom();
  });

  close.addEventListener('click', function () {
    widget.classList.remove('open');
  });

  input.addEventListener('input', function () {
    send.disabled = !input.value.trim();
  });

  form.addEventListener('submit', function (e) {
    e.preventDefault();
    handleSend();
  });
})();