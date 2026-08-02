//
// common.js — shared front-end utilities, exposed as window.MemDB.
//
// Mirrors the HubMonitor webui's common.js: theme resolution (?theme= URL param
// > localStorage > prefers-color-scheme > dark) plus the small formatting and
// escaping helpers every view needs.
//
// Copyright (c) 2026, Aleph ONE Software Engineering LLC.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//
(function () {
  'use strict';

  const THEME_KEY = 'memdb.theme';

  function resolveInitialTheme() {
    const fromUrl = new URLSearchParams(location.search).get('theme');
    if (fromUrl === 'light' || fromUrl === 'dark') {
      try { localStorage.setItem(THEME_KEY, fromUrl); } catch {}
      return fromUrl;
    }
    let stored = null;
    try { stored = localStorage.getItem(THEME_KEY); } catch {}
    if (stored === 'light' || stored === 'dark') return stored;
    const prefersLight = window.matchMedia &&
      window.matchMedia('(prefers-color-scheme: light)').matches;
    return prefersLight ? 'light' : 'dark';
  }

  function applyThemeAttr(theme) {
    if (theme === 'light') document.documentElement.setAttribute('data-theme', 'light');
    else                   document.documentElement.removeAttribute('data-theme');
  }

  function escapeHtml(s) {
    return String(s).replace(/[&<>"']/g, c => ({
      '&': '&amp;', '<': '&lt;', '>': '&gt;', '"': '&quot;', "'": '&#39;',
    }[c]));
  }

  const fmtInt = n => Number(n || 0).toLocaleString();

  function fmtBytes(n) {
    if (!n) return '0';
    const units = ['B', 'KiB', 'MiB', 'GiB', 'TiB'];
    let i = 0;
    while (n >= 1024 && i < units.length - 1) { n /= 1024; ++i; }
    return n.toFixed(i === 0 ? 0 : 1) + ' ' + units[i];
  }

  // The API reports timestamps as microseconds since the epoch; 0 means "never"
  // (an unsealed shard's sealedAt, a never-flushed database).
  function fmtTime(usec) {
    if (!usec) return '—';
    const d = new Date(usec / 1000);
    const p = (n, w = 2) => String(n).padStart(w, '0');
    return `${d.getFullYear()}-${p(d.getMonth() + 1)}-${p(d.getDate())} ` +
           `${p(d.getHours())}:${p(d.getMinutes())}:${p(d.getSeconds())}`;
  }

  function fmtAgo(usec) {
    if (!usec) return 'never';
    const s = Math.max(0, (Date.now() - usec / 1000) / 1000);
    if (s < 1)    return 'just now';
    if (s < 60)   return s.toFixed(0) + 's ago';
    if (s < 3600) return Math.floor(s / 60) + 'm ago';
    return Math.floor(s / 3600) + 'h ago';
  }

  // Every endpoint answers with a JSON body carrying `ok`; a non-2xx status
  // still has one, so surface the server's message rather than "HTTP 400".
  async function api(path, body) {
    const init = body === undefined
      ? { method: 'GET' }
      : { method: 'POST', headers: { 'Content-Type': 'application/json' },
          body: JSON.stringify(body) };
    const rsp = await fetch(path, init);
    let json = null;
    try { json = await rsp.json(); } catch {}
    if (!json) throw new Error('HTTP ' + rsp.status);
    return json;
  }

  window.MemDB = {
    THEME_KEY, resolveInitialTheme, applyThemeAttr,
    escapeHtml, fmtInt, fmtBytes, fmtTime, fmtAgo, api,
  };
})();
