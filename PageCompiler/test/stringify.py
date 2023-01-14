#!/usr/bin/env python3
"""Compile generated pages and exercise standalone rendering.

Usage: python3 PageCompiler/test/stringify.py /path/to/cpspc
Requires a C++17 compiler and installed Poco Net/Foundation headers and libraries.
CXX, CPPFLAGS and LDFLAGS can select another compiler or Poco installation.
"""

import itertools
import os
from pathlib import Path
import shlex
import subprocess
import sys
import tempfile


def run(args):
    subprocess.run(args, check=True)


compiler = str(Path(sys.argv[1]).resolve())
cxx = shlex.split(os.environ.get("CXX", "c++"))
cppflags = shlex.split(os.environ.get("CPPFLAGS", ""))
ldflags = shlex.split(os.environ.get("LDFLAGS", ""))

with tempfile.TemporaryDirectory(prefix="cpspc-stringify-") as directory:
    root = Path(directory)
    headers, sources, checks = [], [], []
    cases = itertools.product(
        (False, True),
        (False, True),
        (False, True),
        ("direct", "buffered", "compressed"),
    )

    for index, (stringify, form, escape, mode) in enumerate(cases):
        name = "Page" + str(index)
        attrs = (
            f'class="{name}" form="{str(form).lower()}" escape="{str(escape).lower()}"'
        )

        if stringify:
            attrs += ' stringify="true"'

        if mode != "direct":
            attrs += f' {mode}="true"'

        pre = '<%% response.set("X-Test", request.getMethod()); int beforeBody = 1; (void) beforeBody; %>'

        body = '<b><%= "<&>" %>|<%- "<&>" %></b>'
        if not stringify:
            body = (
                "<% (void) request.getURI(); (void) response.getStatus(); (void) beforeBody; %>"
                + body
            )

        if form:
            body += '<% const std::string value = form.get("value"); %><%= value %>|<%- value %>'

        page = root / (name + ".cpsp")
        page.write_text("<%@ page " + attrs + " %>" + pre + body)

        run([compiler, "--noline", "--output-dir=" + directory, str(page)])

        header = root / (name + ".h")
        source = root / (name + ".cpp")
        h, cpp = header.read_text(), source.read_text()

        assert (
            "void handleRequest(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response);"
            in h
        )
        assert ("class HTMLForm;" in h) == (stringify and form)
        assert "HTMLForm.h" not in h
        assert cpp.index('response.set("X-Test"') < cpp.index("response.send()")

        if form:
            assert cpp.index("HTMLForm form(") < cpp.index('response.set("X-Test"')

        if stringify:
            http, render = cpp.split(f"void {name}::stringify(")

            assert 'response.set("X-Test"' in http
            assert "request" not in render and "response.send" not in render
            assert "_escapeStream(responseStream)" not in http
            assert ("_escapeStream(responseStream)" in render) == escape
            assert http.index('response.set("X-Test"') < http.index(
                "stringify(responseStream"
            )

            expected = "<b>&lt;&amp;&gt;|<&></b>" if escape else "<b><&>|<&></b>"
            if form:
                expected += "&lt;&amp;&gt;|<&>" if escape else "<&>|<&>"

            headers.append(f'#include "{name}.h"')
            setup = (
                'Poco::Net::HTMLForm form; form.set("value", "<&>"); ' if form else ""
            )
            args = "stream, form" if form else "stream"
            checks.append(
                f"{{ {name} handler; std::ostringstream stream; {setup}handler.stringify({args}); "
                f'std::string html = stream.str(); assert(html == "{expected}"); }}'
            )
        else:
            assert "stringify" not in cpp and "stringify" not in h

        run(cxx + ["-std=c++17"] + cppflags + ["-fsyntax-only", str(source)])
        sources.append(str(source))

    # Also cover a page with all defaults, including the implicit form.
    page = root / "Simple.cpsp"
    page.write_text('<%@ page class="Simple" %><p>Hello!</p>')
    run([compiler, "--output-dir=" + directory, str(page)])

    run(cxx + ["-std=c++17"] + cppflags + ["-fsyntax-only", str(root / "Simple.cpp")])

    main = root / "main.cpp"
    main.write_text(
        "\n".join(headers)
        + "\n#include <Poco/Net/HTMLForm.h>\n#include <sstream>\n#include <cassert>\nint main() {\n"
        + "\n".join(checks)
        + "\n}\n"
    )

    executable = root / "render"
    run(
        cxx
        + ["-std=c++17"]
        + cppflags
        + [str(main)]
        + sources
        + ldflags
        + ["-lPocoNet", "-lPocoFoundation", "-o", str(executable)]
    )

    run([str(executable)])
    print("PASS: 25 generated pages compiled; 12 standalone renders checked.")
