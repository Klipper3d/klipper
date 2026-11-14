import importlib.util
from pathlib import Path
import subprocess
import tempfile
import unittest


ROOT = Path(__file__).resolve().parents[2]
MODULE = ROOT / 'scripts/check_public_docs.py'


class PublicDocsTests(unittest.TestCase):
    def checker(self):
        self.assertTrue(MODULE.is_file(),
                        'Public documentation gate is missing')
        spec = importlib.util.spec_from_file_location('public_docs', MODULE)
        module = importlib.util.module_from_spec(spec)
        spec.loader.exec_module(module)
        return module

    def write(self, root, name, text):
        path = root / name
        path.parent.mkdir(parents=True, exist_ok=True)
        path.write_text(text, encoding='utf-8')
        return name

    def pair(self, root, english=None, portuguese=None):
        en = english or (
            '# Guide\n\n[Português brasileiro](pt-BR/Guide.md)\n\n'
            '## Clear path\n\nCheck the path before resuming.\n'
        )
        pt = portuguese or (
            '# Guia\n\n[English](../Guide.md)\n\n'
            '<a id="clear-path"></a>\n## Caminho livre\n\n'
            'Confira o caminho antes de retomar.\n'
        )
        return [self.write(root, 'docs/Guide.md', en),
                self.write(root, 'docs/pt-BR/Guide.md', pt)]

    def test_complete_pair_passes_and_missing_pair_is_a_release_error(self):
        checker = self.checker()
        with tempfile.TemporaryDirectory() as temporary:
            root = Path(temporary)
            files = self.pair(root)
            self.assertEqual([], checker.audit(root, files))
            (root / files[1]).unlink()
            self.assertTrue(any('missing pair' in item for item in
                                checker.audit(root, files[:1])))

    def test_ci_dependencies_are_not_publication_sources(self):
        checker = self.checker()
        with tempfile.TemporaryDirectory() as temporary:
            root = Path(temporary)
            subprocess.run(['git', 'init', '-q', str(root)], check=True)
            self.write(root, '.gitignore', (ROOT / '.gitignore').read_text())
            sources = self.pair(root)
            generated = self.write(
                root, 'ci_build/venv/lib/python3.10/site-packages/'
                'aenum/README.md',
                '# Dependency documentation, not project source\n',
            )
            inventory = subprocess.check_output([
                'git', '-C', str(root), 'ls-files', '--cached', '--others',
                '--exclude-standard', '-z',
            ]).decode().split('\0')
            self.assertNotIn(generated, inventory)
            self.assertTrue(set(sources).issubset(inventory))
            self.assertEqual([], checker.audit(root, inventory))

    def test_links_and_explicit_english_anchor_targets_are_checked(self):
        checker = self.checker()
        with tempfile.TemporaryDirectory() as temporary:
            root = Path(temporary)
            files = self.pair(root)
            path = root / files[1]
            path.write_text(path.read_text().replace('id="clear-path"',
                                                    'id="wrong"')
                            + '\n[Broken](../Guide.md#missing)\n')
            errors = checker.audit(root, files)
            self.assertTrue(any('missing anchor' in item for item in errors))
            self.assertTrue(any('stable anchor' in item for item in errors))

    def test_code_preserves_values_and_allows_translated_comments(self):
        checker = self.checker()
        with tempfile.TemporaryDirectory() as temporary:
            root = Path(temporary)
            files = self.pair(root)
            code = '\n```gcode\n# Check clearance\nG28\n```\n'
            en, pt = (root / name for name in files)
            en.write_text(en.read_text() + code)
            pt.write_text(pt.read_text() + code.replace(
                '# Check clearance', '# Confira a folga'))
            self.assertEqual([], checker.audit(root, files))
            pt.write_text(pt.read_text().replace('G28', 'G0 X50'))
            self.assertTrue(any('executable blocks' in item for item in
                                checker.audit(root, files)))

    def test_english_fallback_is_reported_not_counted_as_translation(self):
        checker = self.checker()
        with tempfile.TemporaryDirectory() as temporary:
            root = Path(temporary)
            files = self.pair(root)
            paragraph = ('Check every filament channel and inspect the shared '
                         'path before attempting to resume this print.\n')
            for name in files:
                path = root / name
                path.write_text(path.read_text() + '\n' + paragraph)
            self.assertTrue(any('English fallback' in item for item in
                                checker.audit(root, files)))

    def test_commented_configuration_examples_are_protected(self):
        checker = self.checker()
        english = [('', '#baud: 250000\n# sensor_pin: PA1\nG28')]
        translated = [('', '#baud: 115200\n# pino_sensor: PA1\nG28')]
        self.assertNotEqual(checker.executable(english),
                            checker.executable(translated))

    def test_inline_comments_can_translate_but_quoted_values_cannot(self):
        checker = self.checker()
        english = [('gcode', 'M118 MSG="Keep; this # literal" ; Check path\n'
                    'G28 # Home all axes')]
        translated = [('gcode', 'M118 MSG="Keep; this # literal" ; Confira\n'
                       'G28 # Referencie todos os eixos')]
        self.assertEqual(checker.executable(english),
                         checker.executable(translated))
        changed = [('gcode', translated[0][1].replace('Keep;', 'Mantenha;'))]
        self.assertNotEqual(checker.executable(english),
                            checker.executable(changed))
        self.assertNotEqual(checker.executable([('c', 'x(); y();')]),
                            checker.executable([('c', 'x(); z();')]))

    def test_html_assets_and_local_links_are_checked(self):
        checker = self.checker()
        with tempfile.TemporaryDirectory() as temporary:
            root = Path(temporary)
            files = self.pair(root)
            path = root / files[1]
            path.write_text(path.read_text()
                            + '\n<img src="missing.png" alt="Teste"/>\n')
            self.assertTrue(any('unpublished target missing.png' in item
                                for item in checker.audit(root, files)))

    def test_inline_identifiers_and_technical_headings_are_not_translated(self):
        checker = self.checker()
        with tempfile.TemporaryDirectory() as temporary:
            root = Path(temporary)
            files = self.pair(root)
            en, pt = (root / name for name in files)
            en.write_text(en.read_text()
                          + '\n### [axis_twist_compensation]\n\n`NAME=en`\n')
            pt.write_text(pt.read_text()
                          + '\n<a id="axis_twist_compensation"></a>\n'
                          '### [compensacao_de_torcao]\n\n`NOME=pt`\n')
            errors = checker.audit(root, files)
            self.assertTrue(any('technical heading' in item for item in errors))
            self.assertTrue(any('inline code' in item for item in errors))

    def test_source_links_cannot_disappear_into_fake_html(self):
        checker = self.checker()
        with tempfile.TemporaryDirectory() as temporary:
            root = Path(temporary)
            files = self.pair(root)
            en, pt = (root / name for name in files)
            en.write_text(en.read_text() + '\n[Self](Guide.md#clear-path)\n')
            pt.write_text(pt.read_text() + '\n<Guide.md>\n')
            self.assertTrue(any('source link omitted' in item for item in
                                checker.audit(root, files)))

    def test_private_links_are_rejected_without_reading_private_documents(self):
        checker = self.checker()
        with tempfile.TemporaryDirectory() as temporary:
            root = Path(temporary)
            files = self.pair(root)
            path = root / files[0]
            path.write_text(path.read_text()
                            + '\n[Private](_privado/note.md)\n')
            private = root / 'docs/_privado/note.md'
            private.parent.mkdir()
            private.write_bytes(b'\xff\xfe must not be decoded')
            self.assertTrue(any('private target' in item for item in
                                checker.audit(root, files)))
            self.assertTrue(any('private file' in item for item in
                                checker.audit(root, files + [
                                    'docs/_privado/note.md'])))

    def test_missing_link_targets_are_not_satisfied_by_unlisted_files(self):
        checker = self.checker()
        with tempfile.TemporaryDirectory() as temporary:
            root = Path(temporary)
            files = self.pair(root)
            path = root / files[0]
            path.write_text(path.read_text() + '\n[Local](hidden.md)\n')
            self.write(root, 'docs/hidden.md', '# Ignored content\n')
            self.assertTrue(any('unpublished target' in item for item in
                                checker.audit(root, files)))

    def test_reference_links_and_fenced_fake_links_are_distinguished(self):
        checker = self.checker()
        with tempfile.TemporaryDirectory() as temporary:
            root = Path(temporary)
            files = self.pair(root)
            en, pt = (root / name for name in files)
            en.write_text(en.read_text() + '\n[Other][target]\n'
                          '[target]: pt-BR/Guide.md#clear-path\n'
                          '\n```text\n[not a link](absent.md)\n```\n')
            pt.write_text(pt.read_text() + '\n[Outro](#clear-path)\n')
            self.assertEqual([], checker.audit(root, files))
            en.write_text(en.read_text().replace('#clear-path', '#absent'))
            self.assertTrue(any('missing anchor' in item for item in
                                checker.audit(root, files)))

    def test_legacy_commands_are_rejected_in_current_guides_not_history(self):
        checker = self.checker()
        with tempfile.TemporaryDirectory() as temporary:
            root = Path(temporary)
            files = self.pair(root)
            for name in files:
                path = root / name
                path.write_text(path.read_text() + '\n`RETIRAR_FILAMENTO`\n')
            errors = checker.audit(root, files,
                                   legacy_commands={'RETIRAR_FILAMENTO'})
            self.assertTrue(any('legacy command' in item for item in errors))
            history = self.write(root, 'docs/releases/old.md',
                                 '# Old release\n`RETIRAR_FILAMENTO`\n')
            errors = checker.audit(root, files + [history],
                                   legacy_commands={'RETIRAR_FILAMENTO'})
            self.assertFalse(any('old.md: legacy command' in item
                                 for item in errors))


if __name__ == '__main__':
    unittest.main()
