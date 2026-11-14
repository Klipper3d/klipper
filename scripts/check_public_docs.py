#!/usr/bin/env python3
"""Check public EN/PT-BR pairs without reading ignored/private documents.

Structural checks are a release gate, not a semantic translation assessment.
Copied prose is rejected, but a human must still review technical accuracy.
"""

import argparse
from collections import Counter
import importlib.util
from pathlib import Path
import re
import subprocess
import unicodedata
from urllib.parse import unquote, urlsplit


def private(path):
    return '_privado' in path.parts


def counterpart(path):
    if path.name == 'README.md':
        return path.with_name('README.pt-BR.md')
    if path.parts[0] == 'docs':
        return Path('docs/pt-BR') / path.relative_to('docs')
    return path.with_name(path.stem + '.pt-BR.md')


def is_english(path):
    return ('pt-BR' not in path.parts
            and not path.name.endswith('.pt-BR.md'))


def markdown(text):
    """Separate fenced examples from headings, links and prose."""
    lines, blocks, current = [], [], []
    fence = language = None
    for line in text.splitlines():
        marker = re.match(r'^ {0,3}(`{3,}|~{3,})(.*)$', line)
        if fence is None and marker:
            fence, language = marker[1], marker[2].strip().lower()
        elif fence is not None:
            if marker and marker[1][0] == fence[0] and (
                    len(marker[1]) >= len(fence)) and not marker[2].strip():
                blocks.append((language, '\n'.join(current)))
                current, fence = [], None
            else:
                current.append(line)
        else:
            lines.append(line)
    return '\n'.join(lines), blocks


def slug(text):
    text = re.sub(r'<[^>]*>', '', text)
    text = re.sub(r'\[([^]]+)\]\([^)]*\)', r'\1', text).strip().lower()
    text = ''.join(char for char in text if char in '_- '
                   or unicodedata.category(char)[0] in 'LN')
    return text.replace(' ', '-')


def anchors(text):
    headings, seen = [], Counter()
    for match in re.finditer(r'(?m)^ {0,3}(#{1,6})\s+(.+?)\s*#*$', text):
        name = slug(match[2])
        index = seen[name]
        seen[name] += 1
        suffix = '-%d' % index if index else ''
        headings.append((len(match[1]), name + suffix))
    explicit = set(re.findall(r'<a\s+[^>]*id=["\x27]([^"\x27]+)', text))
    return headings, explicit | {name for _, name in headings}


def links(text):
    targets = re.findall(r'\]\(\s*(<[^>]+>|[^\s)]+)', text)
    targets += re.findall(r'(?m)^ {0,3}\[[^]]+\]:\s*(<[^>]+>|\S+)', text)
    targets += re.findall(
        r'<(?:img|a)\b[^>]*\b(?:src|href)=["\x27]([^"\x27]+)', text,
        flags=re.IGNORECASE)
    targets += re.findall(r'<((?:https?|mailto):[^<>\s]+)>', text)
    return [target.strip('<>') for target in targets]


def example_line(line, language):
    """Ignore explanatory comments, never quoted values or option examples."""
    stripped = line.lstrip()
    if stripped.startswith(('#', ';', '//')):
        if re.match(r'^[#;] ?[A-Za-z_][\w_]*\s*[:=]', stripped):
            return line.rstrip()
        return ''
    quote, escaped = None, False
    for index, char in enumerate(line):
        if escaped:
            escaped = False
        elif char == '\\':
            escaped = True
        elif quote:
            if char == quote:
                quote = None
        elif char in ('"', "'"):
            quote = char
        elif (char == '#' or (char == ';' and language == 'gcode')) and (
                index == 0 or line[index - 1].isspace()):
            return line[:index].rstrip()
    return line.rstrip()


def executable(blocks):
    examples = []
    for language, block in blocks:
        if language in ('text', 'plaintext', 'console'):
            continue
        code = '\n'.join(value for line in block.splitlines()
                         if (value := example_line(line, language)).strip())
        if code:
            examples.append((language, code))
    return Counter(examples)


def paragraphs(text):
    result = set()
    for block in re.split(r'\n\s*\n', text):
        if block.lstrip().startswith(('#', '<', '|', '[')):
            continue
        prose = re.sub(r'`[^`]+`|\[[^]]*\]\([^)]*\)', '', block)
        words = re.findall(r'[^\W\d_]+', prose, flags=re.UNICODE)
        if len(words) >= 12:
            result.add(' '.join(words).lower())
    return result


def inline_code(text):
    values = []
    for match in re.finditer(r'`+([^`]+)`+', text):
        value = re.sub(r'[ \t]*\n[ \t]*', ' ', match[1])
        parts = re.split(r'("(?:\\.|[^"\\])*"|\x27(?:\\.|[^\x27\\])*\x27)',
                         value)
        # Normalize wrapping outside quoted literals, not the literal contents.
        values.append(''.join(part if index % 2 else ' '.join(part.split())
                              for index, part in enumerate(parts)))
    return Counter(values)


def technical_headings(text):
    return {match[1] for match in re.finditer(
        r'(?m)^#{3,6}\s+(\[[\w ]+\]|[A-Z][A-Z0-9_]+)\s*$', text)}


def canonical_link(root, origin, target):
    url = urlsplit(target)
    if url.scheme or url.netloc:
        return target
    destination = ((root / origin).parent / unquote(url.path)).resolve()
    if not url.path:
        destination = root / origin
    if not destination.is_relative_to(root):
        return target
    relative = destination.relative_to(root)
    if relative.parts[:2] == ('docs', 'pt-BR'):
        relative = Path('docs', *relative.parts[2:])
    elif relative.name.endswith('.pt-BR.md'):
        relative = relative.with_name(relative.name.replace('.pt-BR.md', '.md'))
    return relative.as_posix() + ('#' + unquote(url.fragment)
                                 if url.fragment else '')


def audit(root, files, legacy_commands=()):
    """Return errors using the publication inventory, never a private rglob."""
    root = Path(root).resolve()
    published = {Path(name) for name in files}
    pages, errors = {}, []
    for path in sorted(published):
        if private(path):
            errors.append('%s: private file in publication inventory' % path)
            continue
        if path.suffix != '.md' or not (root / path).is_file():
            continue
        resolved = (root / path).resolve()
        if not resolved.is_relative_to(root) or private(resolved):
            errors.append('%s: private or external document target' % path)
            continue
        text = (root / path).read_text(encoding='utf-8')
        if not {'releases', 'superpowers'}.intersection(path.parts):
            tokens = set(re.findall(r'\b[A-Z][A-Z_0-9]+\b', text))
            for command in sorted(tokens.intersection(legacy_commands)):
                errors.append('%s: legacy command %s' % (path, command))
        prose, blocks = markdown(text)
        headings, ids = anchors(prose)
        pages[path] = (prose, blocks, headings, ids)

    for path, (prose, blocks, headings, ids) in pages.items():
        for target in links(prose):
            url = urlsplit(target)
            if url.scheme or url.netloc:
                continue
            destination = ((root / path).parent / unquote(url.path)).resolve()
            if not url.path:
                destination = root / path
            if private(destination):
                errors.append('%s: private target %s' % (path, target))
                continue
            if not destination.is_relative_to(root):
                errors.append('%s: external local target %s' % (path, target))
                continue
            relative = destination.relative_to(root)
            directory = destination.is_dir() and any(
                item.is_relative_to(relative) for item in published)
            if relative not in published and not directory:
                errors.append('%s: unpublished target %s' % (path, target))
            elif not destination.exists():
                errors.append('%s: missing target %s' % (path, target))
            elif url.fragment and relative in pages and (
                    unquote(url.fragment) not in pages[relative][3]):
                errors.append('%s: missing anchor %s' % (path, target))
        if not is_english(path):
            continue
        other = counterpart(path)
        if other not in pages:
            errors.append('%s: missing pair %s' % (path, other))
            continue
        translated, translated_blocks, translated_headings, translated_ids = (
            pages[other])
        if len(translated_headings) < len(headings):
            errors.append('%s: translated section count is incomplete' % other)
        for level, anchor in headings:
            if level > 1 and anchor not in translated_ids:
                errors.append('%s: missing stable anchor #%s' % (other, anchor))
        if executable(blocks) != executable(translated_blocks):
            errors.append('%s: executable blocks differ from %s'
                          % (other, path))
        if paragraphs(prose) & paragraphs(translated):
            errors.append('%s: English fallback prose copied from %s'
                          % (other, path))
        if inline_code(prose) - inline_code(translated):
            errors.append('%s: source inline code omitted or changed' % other)
        if technical_headings(prose) - technical_headings(translated):
            errors.append('%s: source technical heading translated or omitted'
                          % other)
        en_targets = links(prose)
        pt_targets = links(translated)
        required = {canonical_link(root, path, t) for t in en_targets}
        # Language switches are checked separately; all other source targets
        # must remain navigable, including assets and external attribution.
        switch = canonical_link(root, other, '')
        required.discard(switch)
        available = {canonical_link(root, other, t) for t in pt_targets}
        for omitted in sorted(required - available):
            errors.append('%s: source link omitted %s' % (other, omitted))
        for origin, destination, targets in (
                (path, other, en_targets), (other, path, pt_targets)):
            if not any(((root / origin).parent / unquote(urlsplit(t).path))
                       .resolve() == root / destination for t in targets
                       if not urlsplit(t).scheme and urlsplit(t).path):
                errors.append('%s: missing language switch to %s'
                              % (origin, destination))
    return sorted(set(errors))


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument(
        '--report-only', action='store_true',
        help='Print incomplete work without approving a release')
    args = parser.parse_args()
    root = Path(__file__).resolve().parents[1]
    raw = subprocess.check_output([
        'git', '-C', str(root), 'ls-files', '--cached', '--others',
        '--exclude-standard', '-z',
    ])
    files = {name for name in raw.decode('utf-8').split('\0') if name}
    migration_path = root / 'scripts/ender3-v3-se/migrate_v2.py'
    spec = importlib.util.spec_from_file_location(
        'docs_migration', migration_path)
    migration = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(migration)
    errors = audit(root, files, legacy_commands=migration.COMMANDS)
    for error in errors:
        print(error)
    sources = [Path(name) for name in files if name.endswith('.md')
               and not private(Path(name)) and is_english(Path(name))
               and (root / name).is_file()]
    missing = sum(not (root / counterpart(path)).is_file() for path in sources)
    print('Public English pages: %d; missing PT-BR pairs: %d; errors: %d.'
          % (len(sources), missing, len(errors)))
    print('Structural checks do not replace human translation review.')
    return 1 if errors and not args.report_only else 0


if __name__ == '__main__':
    raise SystemExit(main())
