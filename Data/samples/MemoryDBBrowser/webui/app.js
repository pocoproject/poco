//
// app.js — MemoryDB browser front end.
//
// Two views over four endpoints: Structure renders GET /api/status (shards +
// files) and GET /api/schema (databases, tables, columns, indexes) and drives
// POST /api/action; SQL posts to /api/query and renders the result grid.
//
// Copyright (c) 2026, Aleph ONE Software Engineering LLC.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//
(function () {
  'use strict';
  const { resolveInitialTheme, applyThemeAttr, THEME_KEY,
          escapeHtml, fmtInt, fmtBytes, fmtTime, fmtAgo, api } = window.MemDB;

  let theme = resolveInitialTheme();
  applyThemeAttr(theme);

  const $ = id => document.getElementById(id);

  // ---- Theme + tabs ----

  const themeBtn = $('theme');
  function updateThemeBtn() {
    themeBtn.textContent = theme === 'light' ? '☀' : '🌙';
    themeBtn.title = theme === 'light' ? 'switch to dark theme' : 'switch to light theme';
  }
  themeBtn.addEventListener('click', () => {
    theme = theme === 'light' ? 'dark' : 'light';
    try { localStorage.setItem(THEME_KEY, theme); } catch {}
    applyThemeAttr(theme);
    updateThemeBtn();
  });
  updateThemeBtn();

  // The visible tab lives in location.hash, so #sql is a shareable link and a
  // reload keeps you where you were.
  function showPanel(name) {
    document.querySelectorAll('.tab').forEach(t => {
      const on = t.dataset.panel === name;
      t.classList.toggle('active', on);
      t.setAttribute('aria-selected', on ? 'true' : 'false');
    });
    document.querySelectorAll('.panel').forEach(p => {
      p.classList.toggle('active', p.id === 'panel-' + name);
    });
    if (location.hash !== '#' + name) history.replaceState(null, '', '#' + name);
    if (name === 'sql') $('sql').focus();
  }
  document.querySelectorAll('.tab').forEach(t =>
    t.addEventListener('click', () => showPanel(t.dataset.panel)));
  showPanel(location.hash === '#sql' ? 'sql' : 'structure');

  // ---- Structure ----

  // status.readOnly reflects the inspector's SQL policy: with it set, the console
  // accepts only SELECT and the verbs that would change the store are hidden.
  // This process owns the database either way, so flush/attach/detach stay.
  let readOnly = true;
  function applyMode(s) {
    readOnly = !!s.readOnly;
    $('mode').classList.toggle('hidden', !readOnly);
    document.querySelectorAll('.write-only').forEach(e => e.classList.toggle('hidden', readOnly));
    $('note').textContent = readOnly
      ? 'SQL console is read-only: only statements that parse as SELECT are accepted. '
        + 'Start with --allow-writes to lift that.'
      : '';
  }

  function renderStats(s) {
    const tiles = [
      ['shards', fmtInt(s.shardCount)],
      ['on disk', fmtBytes(s.diskBytes)],
      ['files', fmtInt((s.files || []).length)],
      ['state', s.dirty ? 'dirty' : 'clean', s.dirty ? 'dirty' : 'clean'],
      ['last flush', fmtAgo(s.lastFlush)],
    ];
    $('stats').innerHTML = tiles.map(([k, v, cls]) =>
      `<div class="stat"><span class="k">${escapeHtml(k)}</span>` +
      `<span class="v ${cls || ''}">${escapeHtml(v)}</span></div>`).join('');
  }

  function renderShards(s) {
    const body = $('shards').tBodies[0];
    if (!s.shards.length) {
      body.innerHTML = '<tr><td colspan="8" class="empty">no shards</td></tr>';
      return;
    }
    body.innerHTML = s.shards.map(d => {
      // Sealed shards are immutable, so attach/detach only apply to them; the
      // active shard offers nothing but the toolbar's Seal. drop deletes data, so
      // it follows the write policy.
      const acts = d.sealed
        ? (d.attached
            ? `<button class="btn btn-mini" data-shard="${d.id}" data-act="detach">detach</button> `
            : `<button class="btn btn-mini" data-shard="${d.id}" data-act="attach">attach</button> `) +
          (readOnly ? ''
                    : `<button class="btn btn-mini" data-shard="${d.id}" data-act="drop">drop</button>`)
        : '<span class="muted">—</span>';
      const alias = d.alias || ('arc_' + d.id);
      return `<tr>
        <td class="num">${d.id}</td>
        <td><span class="badge ${d.sealed ? 'sealed' : 'active'}">${d.sealed ? 'sealed' : 'active'}</span></td>
        <td>${escapeHtml(d.filename || '—')}</td>
        <td class="num">${fmtBytes(d.bytes)}</td>
        <td class="muted">${fmtTime(d.createdAt)}</td>
        <td class="muted">${fmtTime(d.sealedAt)}</td>
        <td>${d.attached ? '<span class="badge on">' + escapeHtml(alias) + '</span>' : '<span class="muted">—</span>'}</td>
        <td>${acts}</td>
      </tr>`;
    }).join('');
  }

  function renderFiles(s) {
    const body = $('files').tBodies[0];
    if (!s.files.length) {
      body.innerHTML = '<tr><td colspan="3" class="empty">directory is empty</td></tr>';
      return;
    }
    body.innerHTML = s.files.map(f => `<tr>
      <td>${escapeHtml(f.name)}</td>
      <td class="num">${fmtBytes(f.bytes)}</td>
      <td class="muted">${fmtTime(f.modified)}</td>
    </tr>`).join('');
  }

  function renderSchema(schema) {
    const html = schema.databases.map(db => {
      const objects = db.objects.map(o => {
        const cols = o.columns.map(c => `<tr>
          <td>${escapeHtml(c.name)}</td>
          <td class="muted">${escapeHtml(c.type || '')}</td>
          <td>${c.pk ? '<span class="badge pk">pk</span>' : ''}</td>
          <td class="muted">${c.notNull ? 'not null' : ''}</td>
          <td class="muted">${c.default === null || c.default === undefined ? '' :
                              'default ' + escapeHtml(c.default)}</td>
        </tr>`).join('');
        const idx = o.indexes.length
          ? '<div class="ddl">' + o.indexes.map(i =>
              escapeHtml(i.sql || ('index ' + i.name))).join('\n') + '</div>'
          : '';
        const rows = o.rows === null || o.rows === undefined ? '?' : fmtInt(o.rows);
        return `<details class="tbl">
          <summary>${escapeHtml(o.name)}
            <span class="meta">${o.type} · ${o.columns.length} cols · ${rows} rows</span>
          </summary>
          <div class="tbl-body">
            <button class="btn btn-mini" data-db="${escapeHtml(db.name)}" data-table="${escapeHtml(o.name)}">select rows</button>
            <table class="grid">${cols}</table>
            ${o.sql ? '<div class="ddl">' + escapeHtml(o.sql) + '</div>' : ''}
            ${idx}
          </div>
        </details>`;
      }).join('') || '<div class="hint">no tables</div>';

      return `<details class="db" open>
        <summary>${escapeHtml(db.name)}
          <span class="file">${escapeHtml(db.file || 'in-memory')}</span>
        </summary>
        ${objects}
      </details>`;
    }).join('');
    $('schema').innerHTML = html;
  }

  let refreshing = false;
  async function refresh() {
    if (refreshing) return;
    refreshing = true;
    try {
      const [status, schema] = await Promise.all([api('/api/status'), api('/api/schema')]);
      $('dir').textContent = status.directory;
      $('dir').title = status.directory;
      applyMode(status);
      renderStats(status);
      renderShards(status);
      renderFiles(status);
      renderSchema(schema);
    } catch (e) {
      showActionResult(String(e.message || e), false);
    } finally {
      refreshing = false;
    }
  }

  function showActionResult(text, ok) {
    const el = $('action-result');
    el.textContent = text;
    el.className = 'action-result ' + (ok ? 'ok' : 'err');
  }

  async function runAction(action, shardId) {
    try {
      const r = await api('/api/action', { action, shardId: shardId || 0 });
      showActionResult(r.ok ? r.message : r.error, !!r.ok);
    } catch (e) {
      showActionResult(String(e.message || e), false);
    }
    await refresh();
  }

  $('panel-structure').addEventListener('click', ev => {
    const btn = ev.target.closest('button');
    if (!btn) return;

    if (btn.dataset.action === 'refresh') { refresh(); return; }
    if (btn.dataset.action) { runAction(btn.dataset.action); return; }

    if (btn.dataset.act) {
      const id = Number(btn.dataset.shard);
      // Dropping a sealed shard deletes its file — the one irreversible action
      // on this page, so it asks first.
      if (btn.dataset.act === 'drop' &&
          !confirm('Delete shard ' + id + ' and its file? This cannot be undone.')) return;
      runAction(btn.dataset.act, id);
      return;
    }

    if (btn.dataset.table) {
      const db = btn.dataset.db, table = btn.dataset.table;
      const from = db === 'main' ? `"${table}"` : `"${db}"."${table}"`;
      $('sql').value = `SELECT * FROM ${from} LIMIT 100;`;
      showPanel('sql');
      runQuery();
    }
  });

  let autoTimer = null;
  $('auto').addEventListener('change', ev => {
    clearInterval(autoTimer);
    autoTimer = ev.target.checked ? setInterval(refresh, 3000) : null;
  });

  // ---- SQL ----

  function renderResult(r) {
    const head = $('result').tHead.rows[0];
    const body = $('result').tBodies[0];
    head.innerHTML = r.columns.map(c =>
      `<th title="${escapeHtml(c.type)}">${escapeHtml(c.name)}</th>`).join('');
    body.innerHTML = r.rows.map(row => '<tr>' + row.map(v =>
      v === null || v === undefined
        ? '<td class="null">NULL</td>'
        : `<td>${escapeHtml(v)}</td>`).join('') + '</tr>').join('');
    if (r.columns.length && !r.rows.length)
      body.innerHTML = `<tr><td colspan="${r.columns.length}" class="empty">no rows</td></tr>`;
  }

  function clearResult() {
    $('result').tHead.rows[0].innerHTML = '';
    $('result').tBodies[0].innerHTML = '';
  }

  async function runQuery() {
    const sql = $('sql').value.trim();
    if (!sql) return;

    $('run').disabled = true;
    $('query-error').textContent = '';
    $('query-status').textContent = 'running…';
    try {
      const r = await api('/api/query', { sql });
      if (!r.ok) {
        clearResult();
        $('query-error').textContent = r.error;
        $('query-status').textContent = `failed in ${r.elapsedMs.toFixed(1)} ms`;
        return;
      }
      renderResult(r);
      const parts = [];
      if (r.columns.length) {
        parts.push(fmtInt(r.rowCount) + ' row' + (r.rowCount === 1 ? '' : 's'));
        if (r.truncated) parts.push(`showing first ${fmtInt(r.maxRows)}`);
      } else {
        parts.push(fmtInt(r.affected) + ' row' + (r.affected === 1 ? '' : 's') + ' affected');
      }
      parts.push(r.elapsedMs.toFixed(1) + ' ms');
      if (r.dirty) parts.push('unflushed changes');
      $('query-status').textContent = parts.join(' · ');
      // A write changes the shard model and the schema, so keep Structure honest.
      if (!r.columns.length && !readOnly) refresh();
    } catch (e) {
      $('query-error').textContent = String(e.message || e);
      $('query-status').textContent = '';
    } finally {
      $('run').disabled = false;
    }
  }

  $('run').addEventListener('click', runQuery);
  $('sql').addEventListener('keydown', ev => {
    if ((ev.ctrlKey || ev.metaKey) && ev.key === 'Enter') { ev.preventDefault(); runQuery(); }
  });

  refresh();
})();
